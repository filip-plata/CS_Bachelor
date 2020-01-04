#ifndef ZADANIE3_COMMUNICATION_H
#define ZADANIE3_COMMUNICATION_H

#include <stdbool.h>
#include "expression.h"
#include "parse.h"
#include "limits.h"

void initialize_values(const init_pair*, const variable_node*, int);
int read_value(FILE*, long*);
int create_process_tree(equation*, const variable_node*, int);
void end_processes(const variable_node*);
void clear_init_values(init_pair*, int);

#endif //ZADANIE3_COMMUNICATION_H
