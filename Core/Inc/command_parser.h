/*
 * command_parser.h
 *
 *  Created on: Jan 27, 2022
 *      Author: salavat.magazov
 */

#ifndef INC_COMMAND_PARSER_H_
#define INC_COMMAND_PARSER_H_

#include <stdint.h>

#define MAX_COMMAND_SIZE 50

enum parser_state_t
{
    ESTATE_IDLE = 0,
    ESTATE_COMMAND,
    ESTATE_CHECKSUM
};

typedef struct COMMAND_PARSER_T
{
    char buffer[MAX_COMMAND_SIZE];   // container for storing incoming data
                                     // stream.
    char command[MAX_COMMAND_SIZE];  // container for storing latest reassembled
                                     // command.
    unsigned int checksum;           // container for storing decoded checksum.
    char *p_head;
    enum parser_state_t parser_state;
} command_parser_t;

void initParser(command_parser_t *p_parser);

#endif /* INC_COMMAND_PARSER_H_ */
