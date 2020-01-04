#ifndef ZADANIE3_EXPRESSION_H
#define ZADANIE3_EXPRESSION_H


#include <stdbool.h>

typedef struct minus_expression_node minus_expression_node;
typedef struct add_expression_node add_expression_node;
typedef struct mult_expression_node mult_expression_node;
typedef struct var_expression_node var_expression_node;
typedef struct expression expression;
typedef struct variable_node variable_node;
typedef struct equation equation;
typedef struct init_pair init_pair;

struct equation {
    unsigned int left_variable;
    expression *right_expression;
};

struct init_pair {
    int variable_index;
    long value;
};

struct variable_node {
    int desc[1024][2];
    int desc_number;
    bool is_lefthandside;
};

void remove_equation(equation* eq);

enum Type {uexpr, mexpr, aexpr, var, num};
typedef enum Type Type;

struct minus_expression_node {
    expression *child;
    int pipe_r;
    int pipe_w;
};


struct add_expression_node {
    expression *left;
    expression *right;
    int pipe_w;
};


struct mult_expression_node {
    expression *left;
    expression *right;
    int pipe_w;
};


struct var_expression_node {
    int index;
    int fd[2];
};


struct expression {
    Type type;
    union {
        long value;
        var_expression_node variable;
        minus_expression_node u_expr;
        add_expression_node a_expr;
        mult_expression_node m_expr;
    };
};

void remove_expression(expression* expr);
expression* new_minus_expr(expression* expr);
expression* new_number_expr(int number);
expression* new_variable_expr(int index);
expression* new_binary_expr(expression*, expression*, char);

#endif //ZADANIE3_EXPRESSION_H
