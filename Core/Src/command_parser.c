/*
 * command_parser.c
 *
 *  Created on: Jan 27, 2022
 *      Author: salavat.magazov
 */

#include "command_parser.h"
#include <ctype.h>

/* these are command delimiting characters and must not appear anywhere within
 * command body. */
#define COMMAND_SYMBOL '>'
#define CHECKSUM_SUMBOL ';'

/* private function prototypes */
uint8_t isCommandStart(char character);
uint8_t isCommandBody(char character);
uint8_t isChecksum(char character);
uint8_t storeCharacter(command_parser_t *p_parser, char character);


void initParser(command_parser_t *p_parser)
{
	p_parser->parser_state = ESTATE_IDLE;
	p_parser->p_head = p_parser->buffer;
}


void newCharacter(command_parser_t *p_parser, char character)
{
	static enum parser_state_t parser_state = ESTATE_IDLE;

	switch (parser_state)
	{
	case ESTATE_IDLE:
		/* This state can transition only into ESTATE_COMMAND upon receiving
		 * correct command start symbol. */
		if(isCommandStart(character) != 0)
		{
			storeCharacter(p_parser, character);
			p_parser->parser_state = ESTATE_COMMAND;
		}
		break;
	case ESTATE_COMMAND:
		/* This state can transition into either ESTATE_CHECKSUM if checksum
		 * start symbol is received or into ESTATE_IDLE if CRLF is received.
		 * The state can also transition into itself during accumulation of
		 * command body. */
		if(isCommandBody(character) != 0){
			if(storeCharacter(p_parser, character) == 0)
			{
				/* If was unable to store the character due to full buffer,
				 * reset parser. This will return state machine to IDLE.*/
				initParser(p_parser);
			}
		}
		else if(is)
		break;
	case ESTATE_CHECKSUM:
		break;

	}
}

uint8_t isCommandStart(char character)
{
	if(character == COMMAND_SYMBOL){
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
	if(character == CHECKSUM_SUMBOL)
	{
		result = 1;
	}
	return result;
}


uint8_t isNewLine(char character)
{
	unsigned int result = 0;
	if((character == '\n') || (character == '\r'))
	{
		result = 1;
	}
	return result;
}


uint8_t storeCharacter(command_parser_t *p_parser, char character)
{
	/* Memory safety checking due to direct operation with pointer. If pointer
	 * has grown beyond the size of the array, do no allow pointer operation.*/
	if (((p_parser->p_head) - (p_parser->buffer)) > MAX_COMMAND_SIZE)
	{
		return 0;
	}
	else
	{
		*(p_parser->p_head) = character;
		p_parser->p_head++;
	}
	return 1;
}

