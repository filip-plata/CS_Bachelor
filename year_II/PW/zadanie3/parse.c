#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <unistd.h>
#include "parse.h"


int parse_number(const char *str, int *iter) {
    int number = 0;
    while (isdigit(str[*iter])) {
        number *= 10;
        number += (str[*iter] - '0');
        (*iter)++;
    }
    return number;
}

int parse_variable(const char *str, int *iter) {
    *iter += 2;
    int result = parse_number(str, iter);

    while (str[*iter] != ']') {
        (*iter)++;
    }

    return result;
}


expression* parse_expression(const char* str, int* iter,
                             variable_node* var_data) {
    expression *expr = NULL;

    while (str[*iter] != '\n') {
        if (str[*iter] == '(' && str[(*iter) + 1] == '-') {
            *iter += 2;
            expression *sub_expr = parse_expression(str, iter, var_data);
            expr = new_minus_expr(sub_expr);

            while (str[*iter] != ')') {
                (*iter)++;
            }
        }

        if (isdigit(str[*iter])) {
            int number = parse_number(str, iter);
            expr = new_number_expr(number);
        }

        if (str[*iter] == 'x') {
            int index = parse_variable(str, iter);

            expr = new_variable_expr(index);
            int h = var_data[index].desc_number;
            var_data[index].desc[h][0] = expr->variable.fd[0];
            var_data[index].desc[h][1] = expr->variable.fd[1];
            var_data[index].desc_number++;
        }

        if (str[*iter] == '(') {
            char operand;
            (*iter)++;
            expression *left_expr = parse_expression(str, iter, var_data);

            while (str[*iter] != '*' && str[*iter] != '+') {
                (*iter)++;
            }

            operand = str[*iter];
            (*iter)++;

            expression *right_expr = parse_expression(str, iter, var_data);

            while (str[*iter] != ')') {
                (*iter)++;
            }

            expr = new_binary_expr(left_expr, right_expr, operand);
        }

        if (expr != NULL) {
            return expr;
        } else {
            (*iter)++;
        }
    }
    return NULL;
}

equation* parse_equation(const char* str, variable_node* var_data) {
    static char buffer[10000];
    unsigned int number, variable_ind;
    int iter = 0;

    if (sscanf(str, "%u x[%u] = %[^\\t\\n]", &number,
               &variable_ind, buffer) != 3) {
        return NULL;
    }

    expression* expr = parse_expression(buffer, &iter, var_data);

    if (expr == NULL) {
        return NULL;
    }

    equation* ptr = (equation*) malloc(sizeof(equation));
    ptr->left_variable = variable_ind;
    ptr->right_expression = expr;

    return ptr;
}

void parse_initialization_list(init_pair* init, const char *str) {
    static char buffer[10000];
    unsigned int line;
    int iter = 0;
    int value = 0;

    if (sscanf(str, "%u %[^\\t\\n]", &line, buffer) != 2) {
        return ;
    }

    while (buffer[iter] != '\000') {
        int variable_index = parse_variable(buffer, &iter);
        iter++;
        while (buffer[iter] == ' ') {
            iter++;
        }

        if (buffer[iter] == '-') {
            iter++;
            value = (-1) * parse_number(buffer, &iter);
        } else {
            value = parse_number(buffer, &iter);
        }

        init[variable_index].variable_index = variable_index;
        init[variable_index].value = value;
        iter++;
    }
}