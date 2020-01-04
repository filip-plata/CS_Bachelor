#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINE_SIZE 101
#define MAX_NAME_LENGTH 15
#define MAX_NUMBER 2147483647

static int command_count = 0;
static int init_count = 0;
static struct def_command first_init;

typedef struct def_command {
    char name[16];
    int data[7];
} command;

bool validate_command_name(command *data) {
    int success;

    success = strcmp(data->name, "INIT") *
              strcmp(data->name, "MOVE") *
              strcmp(data->name, "END_TURN") *
              strcmp(data->name, "PRODUCE_PEASANT") *
              strcmp(data->name, "PRODUCE_KNIGHT");
    return (success == 0) ? true : false;
}

bool invalid_coordinate(int x) {
    if (x <= 0 || x > first_init.data[0]) {
        return true;
    }
    else {
        return false;
    }
}

int validate_two_points(int x1, int y1, int x2, int y2) {
    if (y2 < 0) {
        return 1;
    }

    if (abs(x1 - x2) > 1 || abs(y1 - y2) > 1) {
        return 2;
    }

    if (invalid_coordinate(x1) || invalid_coordinate(y1) ||
        invalid_coordinate(x2) || invalid_coordinate(y2)) {
        return 3;
    }

    return 0;
}

bool validate_integer(char *string) {
    if (strlen(string) > 10)
        return false;

    if (atol(string) < 1 || atol(string) > MAX_NUMBER)
        return false;

    return true;
}

bool validate_king_column(int* tab) {
    if (tab[3] + 3 > tab[0] || tab[5] + 3 > tab[0]) {
        return false;
    }

    return true;
}

int validate_init_data(command *data) {
    if (data->data[0] <= 8) {
        return 1;
    }

    if (data->data[6] < 0) {
        return 2;
    }

    if (init_count > 2) {
        return 3;
    }

    if (command_count > 2) {
        return 4;
    }

    if (data->data[2] != 1 && data->data[2] != 2) {
        return 4;
    }

    if (!validate_king_column(data->data)) {
        return 4;
    }

    //Copying data for later comparison with
    //second init
    if (init_count == 1) {
        if (abs(data->data[3] - data->data[5]) < 8
            && abs(data->data[4] - data->data[6]) < 8) {
            return 5;
        }

        int j = 0;
        while (j <= 6 && data->data[j] != -1) {
            if (j >= 3 &&
                (data->data[j] < 1 || data->data[j] > data->data[0])) {
                return 7;
            }
            first_init.data[j] = data->data[j];
            j++;
        }
    }

    if (init_count == 2) {
        int j = 0;

        if (data->data[2] == first_init.data[2]) {
            return 9;
        }

        data->data[2] = first_init.data[2];
        while (j <= 6) {
            if (data->data[j] != first_init.data[j]) {
                return 9;
            }
            j++;
        }
        data->data[2] = 3 - first_init.data[2];

    }

    return 0;
}

int validate_move_data(int *data) {
    if (data[4] != -1) { return 1;}
    return validate_two_points(data[0], data[1], data[2], data[3]);
}

int validate_produce_knight_data(int *data) {
    return validate_two_points(data[0], data[1], data[2], data[3]);;
}

int validate_produce_peasant_data(int *data) {
    return validate_two_points(data[0], data[1], data[2], data[3]);
}

int validate_end_turn_data(int *data) {
    if (data[0] >= 0) {
        return 1;
    }

    return 0;
}

int parse_data_into_struct(command *comm, char *buffor) {

    if (strstr(buffor, "  ") != NULL) {
        return 1;
    }

    if (buffor[0] == ' ') {
        return 1;
    }

    char *helper = strtok(buffor, " ");

    if (helper == NULL) {
        return 1;
    }

    int name_length = (int) strlen(helper);

    if (helper[name_length - 1] == '\n' || name_length > MAX_NAME_LENGTH) {
        helper[name_length - 1] = '\0';
    }
    strcpy(comm->name, helper);

    int i = 0;
    while ((helper = strtok(NULL, " ")) != NULL) {
        if (validate_integer(helper) == 0 || i == 7) {
            return 1;
        }
        comm->data[i] = atoi(helper);
        i++;
    }

    return 0;
}

command *command_constructor() {
    command *data = malloc(sizeof(command));

    for (int i = 0; i < 7; i++) {
        data->data[i] = -1;
    }

    return data;
}

command* parse_command() {
    static char buffor[MAX_LINE_SIZE + 7];
    buffor[0] = '\0';
    long position_in_buffer = ftell(stdin);
    command *data = command_constructor();


    fgets(buffor, MAX_LINE_SIZE + 5, stdin);

    if (ftell(stdin) - position_in_buffer > MAX_LINE_SIZE) {
        return NULL;
    }

    if (parse_data_into_struct(data, buffor)) {
        return NULL;
    }

    if (!validate_command_name(data)) {
        return NULL;
    }

    command_count++;

    if (strcmp(data->name, "INIT") == 0) {
        init_count++;
        return (validate_init_data(data) == 0) ? data : NULL;
    }

    //There must be two consecutive init's at the beginning
    if (command_count < 1) {
        return NULL;
    }

    if (strcmp(data->name, "MOVE") == 0) {
        return (validate_move_data(data->data) == 0) ? data : NULL;
    }

    if (strcmp(data->name, "PRODUCE_KNIGHT") == 0) {
        return (validate_produce_knight_data(data->data) == 0) ? data : NULL;
    }

    if (strcmp(data->name, "PRODUCE_PEASANT") == 0) {
        return (validate_produce_peasant_data(data->data) == 0) ? data : NULL;
    }

    if (strcmp(data->name, "END_TURN") == 0) {
        return (validate_end_turn_data(data->data) == 0) ? data : NULL;
    }

    return NULL;
}
