 /** @file
    Interface of parser.

 */

#ifndef PARSE_H
#define PARSE_H

 /**
 * @typedef command
  * @brief Name shortening.
 */

 /**
 * @struct def_command
 * @brief Wrapper for command.
 */
typedef struct def_command {
    char name[16]; /**< Name of the command.*/
    int data[7]; /**< Data passed in a command.*/
} command; /**< Handle. */


/** Reads a command.
  Returns command with data points using 'command' structure.
  */
command* parse_command();

#endif /* PARSE_H */
