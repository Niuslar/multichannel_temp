/*
 * command_parser.c
 *
 *  Created on: Jan 27, 2022
 *      Author: salavat.magazov
 */

#include "command_parser.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* these are command delimiting characters and must not appear anywhere within
 * command body. */
#define COMMAND_SYMBOL  '>'
#define CHECKSUM_SUMBOL ';'

/* private function prototypes */
uint8_t isCommandStart(char character);
uint8_t isCommandBody(char character);
uint8_t isChecksum(char character);
uint8_t isNewLine(char character);
uint8_t storeCharacter(command_parser_t *p_parser, char character);
void calculateChecksum(command_parser_t *p_parser);

void initParser(command_parser_t *p_parser)
{
    p_parser->parser_state = ESTATE_IDLE;
    p_parser->p_head = p_parser->command_buffer;
}

uint8_t newCharacter(command_parser_t *p_parser, char character)
{
    uint8_t new_command_ready = 0;
    switch (p_parser->parser_state)
    {
        case ESTATE_IDLE:
            /* This state can transition only into ESTATE_COMMAND upon receiving
             * correct command start symbol. */
            if (isCommandStart(character) != 0)
            {
                p_parser->parser_state = ESTATE_COMMAND;
            }
            break;
        case ESTATE_COMMAND:
            /* This state can transition into either ESTATE_CHECKSUM if checksum
             * start symbol is received or into ESTATE_IDLE if CRLF is received.
             * The state can also transition into itself during accumulation of
             * command body. */
            if (isCommandBody(character) != 0)
            {
                if (storeCharacter(p_parser, character) == 0)
                {
                    /* If was unable to store the character due to full buffer,
                     * reset parser. This will return state machine to IDLE.*/
                    initParser(p_parser);
                    break;
                }
            }
            else if (isChecksum(character) != 0)
            {
                if (storeCharacter(p_parser, character) == 0)
                {
                    initParser(p_parser);
                    break;
                }
                p_parser->p_checksum = p_parser->p_head;
                p_parser->parser_state = ESTATE_CHECKSUM;
            }
            else if (isNewLine(character) != 0)
            {
                if (storeCharacter(p_parser, 0) == 0)
                {
                    initParser(p_parser);
                    break;
                }
                /* at this point a new command without a checksum is ready. */
                strncpy(p_parser->command,
                        p_parser->command_buffer,
                        MAX_COMMAND_SIZE);
                new_command_ready = 1;
            }
            else
            {
                /* This is perhaps somewhat drastic, but if something unexpected
                 * arrives that is recognised as neither special characters nor
                 * a command body, then reset is in order. */
                initParser(p_parser);
            }
            break;
        case ESTATE_CHECKSUM:
            if (isxdigit(character))
            {
                if (storeCharacter(p_parser, character) == 0)
                {
                    initParser(p_parser);
                    break;
                }
            }
            else if (isNewLine(character) != 0)
            {
                /* Here we have a command with a checksum. Need to calculate
                 * checksum and compare it with a checksum that was transmitted
                 * as part of the command message.*/
                calculateChecksum(p_parser);
                unsigned int received_checksum;
                if (sscanf(p_parser->p_checksum, ";%X", &received_checksum) !=
                    1)
                {
                    initParser(p_parser);
                    break;
                }
                if (received_checksum == p_parser->checksum)
                {
                    strncpy(p_parser->command,
                            p_parser->command_buffer,
                            MAX_COMMAND_SIZE);
                    new_command_ready = 1;
                }
            }
            break;
    }

    return new_command_ready;
}

uint8_t isCommandStart(char character)
{
    if (character == COMMAND_SYMBOL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t isCommandBody(char character)
{
    /* Arbitrarily complex conditional statement can be created here to limit
     * what is considered command and what is not a command.
     * For now for simplicity sake anything that is a printable character, i.e.
     * anything that is not a control character is considered valid. */
    return isprint(character);
}

uint8_t isChecksum(char character)
{
    unsigned int result = 0;
    if (character == CHECKSUM_SUMBOL)
    {
        result = 1;
    }
    return result;
}

uint8_t isNewLine(char character)
{
    unsigned int result = 0;
    if ((character == '\n') || (character == '\r'))
    {
        result = 1;
    }
    return result;
}

uint8_t storeCharacter(command_parser_t *p_parser, char character)
{
    /* Memory safety checking due to direct operation with pointer. If pointer
     * has grown beyond the size of the array, do no allow pointer operation.*/
    if (((p_parser->p_head) - (p_parser->command_buffer)) > MAX_COMMAND_SIZE)
    {
        return 0;
    }
    else
    {
        *(p_parser->p_head) = character;
        p_parser->p_head++;
        return 1;
    }
}

void calculateChecksum(command_parser_t *p_parser)
{
    char *p_runner = p_parser->command_buffer;
    uint32_t char_counter = 0;
    p_parser->checksum = 0;

    while ((*p_runner != CHECKSUM_SUMBOL) && (char_counter <= MAX_COMMAND_SIZE))
    {
        p_parser->checksum += *p_runner;
        p_runner++;
        char_counter++;
    }
    p_parser->checksum &= CHECKSUM_MASK;
}
