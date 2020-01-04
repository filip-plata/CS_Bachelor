#include <stdio.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/wait.h>
#include "expression.h"
#include "parse.h"
#include "communication.h"
#include "graph.h"
#include "limits.h"


int zero_var_desc;
char buf[BUF_SIZE];
variable_node variable_nodes[MAX_VARIABLES];
equation* equations[MAX_VARIABLES];
init_pair initialization[MAX_VARIABLES];
unsigned int N, K, V;

void cleanup() {
    for (unsigned int i=0; i<MAX_VARIABLES; i++) {
        remove_equation(equations[i]);
    }
}

int main() {
    scanf("%u %u %u", &N, &K, &V); getchar();

    for (unsigned int i=0; i<K; i++) {
        bool error = false;
        fgets(buf, BUF_SIZE, stdin);
        equation *eq = parse_equation(buf, variable_nodes);

        if (eq == NULL || equations[eq->left_variable] != NULL) {
            error = true;
        } else {
            equations[eq->left_variable] = eq;
            if (is_cyclic(equations, eq)) {
                error = true;
            }
        }

        if (error) {
            printf("%d F\n", i+1);
            cleanup();
            return 1;
        } else {
            printf("%d P\n", i+1);
        }
    }

    for (int i=0; i<MAX_VARIABLES; i++) {
        variable_nodes[i].is_lefthandside = equations[i] != NULL;
    }

    fflush(stdout);

    for (int i=0; i<MAX_VARIABLES; i++) {
        if (equations[i] != NULL) {
            int desc = create_process_tree(equations[i],
                                           variable_nodes,
                                            N - K);
            if (i == 0) {
                zero_var_desc = desc;
            }
        }
    }
    int round = 0;
    FILE* fp = fdopen(zero_var_desc, "r");

    for (unsigned int i=K; i<N; i++) {
        clear_init_values(initialization, MAX_VARIABLES);
        round++;
        fgets(buf, BUF_SIZE, stdin);
        parse_initialization_list(initialization, buf);

        if (equations[0] == NULL) {
            printf("%d F\n", i + 1);
            continue;
        }

        initialize_values(initialization, variable_nodes, round);
        long res = initialization[0].value;
        int ret = read_value(fp, &res);

        if (ret == -1 && initialization[0].variable_index == -1) {
            printf("%d F\n", i + 1);
            continue;
        }

        if (initialization[0].variable_index != -1) {
            res = initialization[0].value;
        }

        printf("%d P %ld\n", i + 1, res);
    }


    fflush(stdout);
    fclose(fp);
    cleanup();
    end_processes(variable_nodes);

    pid_t wpid;
    int status;
    while ((wpid = wait(&status)) > 0)

    return 0;
}