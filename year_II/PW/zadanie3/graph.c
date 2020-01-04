#include <stddef.h>
#include <stdio.h>
#include "graph.h"
#include "limits.h"


enum STATUS {NOT_VISITED, VISITED, DONE};
typedef enum STATUS STATUS;

static STATUS status[MAX_VARIABLES];
static int neighbours[MAX_VARIABLES][MAX_VARIABLES];


void get_them(expression *expr, int *number, int var_index) {
    switch (expr->type) {
        case num:
            break;
        case aexpr:
            get_them(expr->a_expr.left, number, var_index);
            get_them(expr->a_expr.right, number, var_index);
            break;
        case uexpr:
            get_them(expr->u_expr.child, number, var_index);
            break;
        case mexpr:
            get_them(expr->m_expr.left, number, var_index);
            get_them(expr->m_expr.right, number, var_index);
            break;
        case var:
            neighbours[var_index][(*number)++] = expr->variable.index;
            break;
    }
}

int get_neighbours(equation **equations, int var_index) {
    int number = 0;
    if (equations[var_index] != NULL) {
        get_them(equations[var_index]->right_expression, &number, var_index);
    }
    return number;
}


void visit(equation **equations, int var_index, bool *res) {
    int h = 0;
    status[var_index] = VISITED;

    h = get_neighbours(equations, var_index);

    for (int i = 0; i < h; i++) {
        int neighbour = neighbours[var_index][i];
        if (status[neighbour] == VISITED) {
            *res = true;
        }
        if (status[neighbour] == NOT_VISITED) {
            visit(equations, neighbour, res);
        }
        neighbours[var_index][i] = 0;
    }

    status[var_index] = DONE;
}

bool is_cyclic(equation **equations, equation *eq) {
    for (size_t i = 0; i < MAX_VARIABLES; i++) {
        status[i] = NOT_VISITED;
    }
    bool result = false;
    visit(equations, eq->left_variable, &result);
    return result;
}