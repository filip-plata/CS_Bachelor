#include <stddef.h>
#include <malloc.h>
#include <unistd.h>
#include "expression.h"

void remove_expression(expression* expr) {
    if (expr != NULL) {
        switch (expr->type) {
            case var:
                free(expr);
                break;
            case num:
                free(expr);
                break;
            case uexpr:
                remove_expression(expr->u_expr.child);
                free(expr);
                break;
            case aexpr:
                remove_expression(expr->a_expr.left);
                remove_expression(expr->a_expr.right);
                free(expr);
                break;
            case mexpr:
                remove_expression(expr->m_expr.left);
                remove_expression(expr->m_expr.right);
                free(expr);
                break;
        }
    }
}

void remove_equation(equation* eq) {
    if (eq != NULL) {
        remove_expression(eq->right_expression);
        free(eq);
    }
}

expression* new_minus_expr(expression* expr) {
    expression *result = (expression*) malloc(sizeof(expression));
    result->type = uexpr;
    result->u_expr.child = expr;

    return result;
}

expression* new_number_expr(int number) {
    expression *result = (expression*) malloc(sizeof(expression));
    result->type = num;
    result->value = number;

    return result;
}

expression* new_variable_expr(int index) {
    expression *result = (expression*) malloc(sizeof(expression));
    result->type = var;
    result->variable.index = index;
    pipe(result->variable.fd);

    return result;
}

expression* new_binary_expr(expression* left, expression* right, char op) {
    expression* result = NULL;

    if (op == '+' || op == '*') {
        result = (expression*) malloc(sizeof(expression));
    }

    if (op == '+') {
        result->type = aexpr;
        result->a_expr.left = left;
        result->a_expr.right = right;
    }

    if (op == '*') {
        result->type = mexpr;
        result->m_expr.left = left;
        result->m_expr.right = right;
    }

    return result;
}