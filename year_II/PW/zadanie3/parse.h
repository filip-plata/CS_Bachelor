#ifndef ZADANIE3_PARSE_H
#define ZADANIE3_PARSE_H

#include <stddef.h>
#include <stdlib.h>
#include "expression.h"


equation* parse_equation(const char* str, variable_node*);
void parse_initialization_list(init_pair* init, const char *str);

void remove_equation(equation* eq);

#endif //ZADANIE3_PARSE_H
