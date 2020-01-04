#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <wait.h>
#include "communication.h"
#include "limits.h"


enum Status_ {NO_VALUE, END, VALUE, INIT, INIT_NO_VALUE, NONE};
typedef enum Status_ Status_;

typedef struct Status Status;
struct Status {
    Status_ status;
    long value;
    int round;
};

Status end_status() {
    Status result = { .status = END};
    return result;
}

Status init_status(int round, long value) {
    Status result = { .status = INIT, .value = value, .round = round};
    return result;
}

Status no_value_init_status(int round) {
    Status result = { .status = INIT_NO_VALUE, .round = round};
    return result;
}

Status value_status(int round, long value) {
    Status result = { .status = VALUE, .value = value, .round = round};
    return result;
}

Status no_value_status(int round) {
    Status result = { .status = NO_VALUE, .round = round};
    return result;
}

Status parse_status(FILE *fp) {
    static char buf[100];
    Status result;

    fgets(buf, 100, fp);
    //printf("%s", buf);

    if (strcmp(buf, "END\n") == 0) {
        result = end_status();
    } else {
        char *pch = strtok(buf, " ");
        char *second = strtok(NULL, " ");

        int round = (int) strtol(pch, (char **)NULL, 10);


        if (strcmp(second, "NONE\n") == 0) {
            result = no_value_status(round);
        } else {
            char *third = strtok(NULL, " ");
            if (strcmp(third, "NONE\n") == 0) {
                result = no_value_init_status(round);
            } else {
                long value = strtol(third, (char **) NULL, 10);
                if (strcmp(second, "VALUE") == 0) {
                    result = value_status(round, value);
                } else {
                    result = init_status(round, value);
                }
            }
        }
    }

    return result;
}

void write_status(int fd, Status status) {
    static char buf[100];
    int len = 0;

    switch (status.status) {
        case END:
            len = sprintf(buf, "END\n");
            break;
        case VALUE:
            len = sprintf(buf, "%d VALUE %ld\n", status.round, status.value);
            break;
        case NO_VALUE:
            len = sprintf(buf, "%d NONE\n", status.round);
            break;
        case INIT:
            len = sprintf(buf, "%d INIT %ld\n", status.round, status.value);
            break;
        case INIT_NO_VALUE:
            len = sprintf(buf, "%d INIT NONE\n", status.round);
            break;
        default:
            return;
    }
    write(fd, buf, len);
}

void clear_init_values(init_pair* pairs, int size) {
    for (int i=0; i<size; i++) {
        pairs[i].variable_index = -1;
    }
}

int create_process_tree_(expression* expr, int K, const variable_node* nodes);


void initialize_values(const init_pair* pair,
                       const variable_node* nodes,
                       int round) {

    for (int i=0; i< MAX_VARIABLES; i++) {
        for (int j=0; j<nodes[i].desc_number; j++) {
            int desc = nodes[i].desc[j][1];
            if (pair[i].variable_index != -1) {
                write_status(desc, init_status(round, pair[i].value));
            } else {
                write_status(desc, no_value_init_status(round));
            }
        }

    }

}

int read_value(FILE* fp, long* result) {
    Status st = parse_status(fp);

    if (st.status == NO_VALUE) {
        return -1;
    }
    if (st.status == VALUE) {
        *result = st.value;
    }
    return 0;
}

int create_process_u(minus_expression_node node,
                    int K, int write_d, const variable_node* nodes) {
    int pipe_r = create_process_tree_(node.child, K, nodes);
    FILE* fp = fdopen(pipe_r, "r");
    int round = 0;

    while (true) {
        round++;
        Status st = parse_status(fp);

        if (st.status == END) {
            write_status(write_d, end_status());
            fclose(fp); close(write_d);
            break;
        }

        if (st.status == NO_VALUE) {
            write_status(write_d, no_value_status(round));
        }

        if (st.status == VALUE) {
            write_status(write_d, value_status(round, -st.value));
        }

    }

    wait(0);
    exit(0);
}

int create_process_var(var_expression_node variable,
                     int write_d, const variable_node* nodes) {
    FILE* fp = fdopen(variable.fd[0], "r");
    Status init[MAX_VARIABLES];
    Status intermediate[MAX_VARIABLES];
    int round = 0, init_ = 0, inter_ = 0;
    bool is_left = nodes[variable.index].is_lefthandside;

    for (int i=0; i<MAX_VARIABLES; i++) {
        init[i].status = NONE;
        intermediate[i].status = NONE;

    }

    while (true) {
        Status st = parse_status(fp);

        if (st.status == INIT || st.status == INIT_NO_VALUE) {
            init[init_++] = st;
        }

        if (st.status == VALUE || st.status == NO_VALUE) {
            intermediate[inter_++] = st;
        }

        if (st.status == END) {
            write_status(write_d, end_status());
            close(write_d);
            break;
        }

        if (is_left) {
            if (init_ > round) {
                if (inter_ <= round) {
                    continue;
                }
                if (init[round].status == INIT) {
                    round++;
                    write_status(write_d,
                                 value_status(round, init[round-1].value));
                } else {
                    if (intermediate[round].status == NO_VALUE) {
                        round++;
                        write_status(write_d, no_value_status(round));
                    }
                    if (intermediate[round].status == VALUE) {
                        round++;
                        write_status(write_d, value_status(round,
                                                           intermediate[round - 1].value));
                    }
                }
            }
        }

        if (!is_left) {
            round++;

            if (st.status == INIT) {
                write_status(write_d,
                             value_status(round, st.value));
            }
            if (st.status == INIT_NO_VALUE) {
                write_status(write_d, no_value_status(round));
            }
        }

    }

    exit(0);
}

int create_process_num(long value, int K, int write_d) {
    int round = 0;

    for (int i=0; i<K; i++) {
        round++;
        write_status(write_d, value_status(round, value));
    }

    write_status(write_d, end_status());
    exit(0);
}


int create_process_bin(add_expression_node node,
                     int K, int write_d, char op, const variable_node* nodes) {
    int pipe_r_l = create_process_tree_(node.left, K, nodes);
    int pipe_r_r = create_process_tree_(node.right, K, nodes);
    FILE* fp_l = fdopen(pipe_r_l, "r");
    FILE* fp_r = fdopen(pipe_r_r, "r");
    long temp = 0;
    int round = 0;

    while (true) {
        round++;
        Status st_l = parse_status(fp_l);
        Status st_r = parse_status(fp_r);

        if (st_l.status == END || st_r.status == END) {
            write_status(write_d, end_status());
            fclose(fp_l); fclose(fp_r); close(write_d);
            break;
        }

        if (st_l.status == NO_VALUE || st_r.status == NO_VALUE) {
            write_status(write_d, no_value_status(round));
        }

        if (op == '+') {
            temp = st_l.value + st_r.value;
        }

        if (op == '*') {
            temp = st_l.value * st_r.value;
        }

        if (st_l.status == VALUE && st_r.status == VALUE) {
            write_status(write_d, value_status(round, temp));
        }

    }

    wait(0); wait(0);
    exit(0);
}

int create_process_tree_(expression* expr,
                          int K, const variable_node* nodes) {
    int fd[2];
    pid_t childpid;
    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if (childpid == 0) {
        switch (expr->type) {
            case uexpr:
                return create_process_u(expr->u_expr, K, fd[1], nodes);
            case num:
                return create_process_num(expr->value, K, fd[1]);
            case aexpr:
                return create_process_bin(expr->a_expr, K, fd[1],
                                          '+', nodes);
            case mexpr:
                return create_process_bin(expr->a_expr, K, fd[1],
                                          '*', nodes);
            case var:
                return create_process_var(expr->variable, fd[1], nodes);
            default:
                return -1;
        }
    } else {
        close(fd[1]);
        return fd[0];
    }
}

void create_process_pass(int var_index, const variable_node* nodes, int fd,
                         int K) {
    char buf[100];
    FILE* fp = fdopen(fd, "r");
    int number = nodes[var_index].desc_number;

    switch(fork()) {
        case -1:
            break;
        case 0:
            for (int j=0; j<K; j++) {
                fgets(buf, 100, fp);
                for (int i=0; i < number; i++) {
                    write(nodes[var_index].desc[i][1], buf, strlen(buf));
                }
                if (strcmp(buf, "END\n") == 0) {
                    break;
                }
            }

            fclose(fp);
            close(fd);
            exit(0);
        default:
            break;
    }
}

int create_process_tree(equation* eq, const variable_node* nodes, int K) {
    int var_index = eq->left_variable;
    int out_desc = create_process_tree_(eq->right_expression, K, nodes);

    if (var_index != 0) {
        create_process_pass(var_index, nodes, out_desc, K);
    }

    return out_desc;
}

void end_processes(const variable_node* nodes) {
    for (int i=0; i< MAX_VARIABLES; i++) {
        for (int j=0; j<nodes[i].desc_number; j++) {
            int desc = nodes[i].desc[j][1];
            write_status(desc, end_status());
        }
    }
}