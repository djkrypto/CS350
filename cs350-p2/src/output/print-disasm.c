/* ************************************************************************* *
 * print.c                                                                   *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This code is used to print the disassembler                    *
 * ************************************************************************* */

/* ************************************************************************* *
 * Library includes here.                                                    *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <inttypes.h>           /* allows PRIu8 */
#include <string.h>		/* strcat */
#include <stdlib.h>		/* malloc */

#include "../main/debug.h"      /* DEBUG statements */
#include "print-disasm.h"	/* header file */
/* ************************************************************************* *
 * Local function prototypes                                                 *
 * ************************************************************************* */

/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Print Introductory line of disassembler                          *
 * ************************************************************************* */
void print_first_line()
{
    printf("\n");
    printf("--------------------------------------\n");
    printf("Addr  Code   Symbol  Mnemonic  Operand\n");
    printf("--------------------------------------\n");
}

/* ************************************************************************* *
 * Purpose: Print the full disassembler		                             *
 * 									     *
 * Parameters:							 	     *
 *     instructions -- the list of instructions to print out		     *
 *     memory -- the array of bytes in memory 				     *
 * ************************************************************************* */
void print_disassembler(instruction_t* instructions, uint8_t* memory,
			symtab_t** symtab)
{
    print_first_line();
    instruction_t* cur_inst = instructions;
    uint8_t more_than_three_bytes = 0; //set to 1 if code is more than 3 bytes

    if (cur_inst != NULL)
    {
        while (cur_inst != NULL)
        {
	    print_address(cur_inst);
	    more_than_three_bytes = print_code(cur_inst);
	    print_symbol(cur_inst);
	    print_mnemonic(cur_inst);
	    print_operand(cur_inst,memory,symtab);
	    printf("\n");
	    if (more_than_three_bytes)
		print_excess_bytes(cur_inst,memory);
	    if (cur_inst->next != NULL)
                cur_inst = cur_inst->next;
	    else
		break;
        }
    }

    printf("\n\n"); //looks cleaner this way
}

/* ************************************************************************* *
 * Purpose: Print the address of the current instruction                     *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions		                     *
 * ************************************************************************* */
void print_address(instruction_t* instructions)
{
    printf("%04X  ",instructions->addr);
}

/* ************************************************************************* *
 * Purpose: Print the code of the current instruction                        *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions		                     *
 *                                                                           *
 * Returns:                                                                  *
 *     0 - If operation does not need to print more than 3 bytes             *
 *     1 - If operation needs to print more than 3 bytes-due to ASCII        *
 * ************************************************************************* */
uint8_t print_code(instruction_t* instructions)
{
    if ((instructions->symb != NULL) && (
        (instructions->symb->type == 1) || //ASCII
        (instructions->symb->type == 2) || //BLOCK
        (instructions->symb->type == 3) )) //WORD
    {
	return print_pseudo_operand(instructions);
    }
    else
    {
    	//if unary, print just the inst_spec, otherwise both
    	if (instructions->unary)
            printf("%02X     ",instructions->inst_spec);
    	else
            printf("%02X%04X ",instructions->inst_spec,instructions->op_spec);
	return 0;
    }
}

/* ************************************************************************* *
 * Purpose: Print the code of the current instruction that has a symbol      *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions                              *
 *									     *
 * Returns:								     *
 *     0 - If operation does not need to print more than 3 bytes	     *
  *    1 - If operation needs to print more than 3 bytes		     *
 * ************************************************************************* */
uint8_t print_pseudo_operand(instruction_t* instructions)
{
    if (instructions->symb->type == 1) //ASCII
    {
	printf("%02X%04X ",instructions->inst_spec,instructions->op_spec);	
	if (instructions->ascii_bytes > 3)
	    return 1;
	else
	    return 0;
    }
    else if (instructions->symb->type == 2) //BLOCK
    {
	if (instructions->symb->block_length == 1)
	    printf("%02X     ",instructions->inst_spec);	
	else if (instructions->symb->block_length == 2)
            printf("%02X%02X   ",instructions->inst_spec,instructions->inst_spec);
        else if (instructions->symb->block_length == 3)
            printf("%02X%04X ",instructions->inst_spec,instructions->op_spec);        
	//multiple line block
	if (instructions->symb->block_length > 3)
	{
            printf("%02X%04X ",instructions->inst_spec,instructions->op_spec);
	    return 1;
	}
	else
	    return 0;
    }
    else if (instructions->symb->type == 3) //WORD
    {
        uint8_t byte_1 = instructions->inst_spec;
        uint8_t byte_2 = (uint8_t)(instructions->op_spec >> 8);
        printf("%02X%02X   ",byte_1,byte_2);
	return 0;
    }
    else
    {
        printf("Error in print_pseudo_operand, symbol type is: %d\n",
		instructions->symb->type);
        exit(1);
    }

}

/* ************************************************************************* *
 * Purpose: Special case function to print out excess bytes when the         *
 *          mnemonic is ASCII with greater than or equal to 3 bytes   	     *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions                              *
 * ************************************************************************* */
void print_excess_bytes(instruction_t* instructions,uint8_t* memory)
{
    uint16_t index = instructions->addr + 3;
    uint16_t starting_index = index;
    uint16_t bytes_left_to_print;
    
    symtype_t instruction_type = instructions->symb->type;
    if (instruction_type == BLOCK)
	bytes_left_to_print = instructions->symb->block_length - 3;
    else if (instruction_type == ASCII)
	bytes_left_to_print = instructions->ascii_bytes - 3;
    uint8_t number_of_blanks = 6; //used to make the output look pretty
    int i = 0;
    for (i = 0; i < number_of_blanks; i++)
	printf(" ");
    while (bytes_left_to_print)
    {
	if (instruction_type == BLOCK)
	    printf("00");
	else if (instruction_type == ASCII)
	    printf("%02X",memory[index]);
	index++;
	bytes_left_to_print--;
	//go to new line
	if (index - starting_index == 3 && bytes_left_to_print != 0)
	{
	    printf("\n");
	    i = 0;
	    for(i = 0; i < number_of_blanks; i++)
		printf(" ");
	}
    }
    printf("\n");
}

/* ************************************************************************* *
 * Purpose: Print the symbol of the current instruction if there is one      *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions                              *
 * ************************************************************************* */
void print_symbol(instruction_t* instructions)
{
    if (instructions->symb != NULL)
    {
	//get the : in the symbol label
	char* label = strdup(instructions->symb->label);
	if (label != NULL)
	{
	    char* colon = ":";
	    strcat(label,colon);
	    printf("%-8s",label);
	    free(label);
	}
	else
        printf("        "); //default to other option if heap allocation fails
    }
    else
	printf("        ");
}

/* ************************************************************************* *
 * Purpose: Print the mnemonic of the current instruction                    *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions                              *
 * ************************************************************************* */
void print_mnemonic(instruction_t* instructions)
{
    printf("%-10s",MNEMONICS[instructions->mnem]);
}

/* ************************************************************************* *
 * Purpose: Print the operand of the current instruction                     *
 *                                                                           *
 * Parameters:                                                               *
 *     instructions -- the list of instructions                              *
 * ************************************************************************* */
void print_operand(instruction_t* instructions, uint8_t* memory,
		   symtab_t** symtab)
{
    //is the instruction a pseudo_op?
    if (instructions->symb != NULL && instructions->symb->type != 0)
    {
	if (instructions->symb->type == 1) //ASCII
	{
	    printf("\"");
	    uint16_t bytes_left_to_print = instructions->ascii_bytes;
	    uint16_t index = 0;
	    char next_byte;
	    while (bytes_left_to_print)
	    {
		next_byte = (char)memory[instructions->addr+index];
		printf("%c",next_byte);
		index++;
		bytes_left_to_print--;
	    }
	    printf("\\x");
	    printf("00\"");
	}
	else if (instructions->symb->type == 2) //BLOCK
	{
	    printf("%zu",instructions->symb->block_length);
	}
	else if (instructions->symb->type == 3) //WORD
	{
	    uint8_t byte_1 = instructions->inst_spec;
	    uint8_t byte_2 = (uint8_t)(instructions->op_spec >> 8);
	    printf("0x%02X%02X",byte_1,byte_2);
	}
    }
    else
    {
	char* addr_mode = NULL;
	
        //check if operand specifier corresponds to a symbol
	//if instruction is unary,jump out of loop 
        symtab_t* cur_sym = *symtab; //current symbol
        while (cur_sym != NULL && instructions->op_spec != DNE)
        {
            if (cur_sym->offset == instructions->op_spec)
                break; //we have found the symbol
            if (cur_sym->next != NULL)
                cur_sym = cur_sym->next;
	    else
	        cur_sym = NULL; //no symbol matches the operand specifier
        }

        //is there an addressing mode for this instruction? is it "a" or "aaa"?
        if (instructions->addr_mode != DNE &&
	    instructions->single_digit_addressing)
        {
	    if (instructions->addr_mode == 0x00)
	        addr_mode = ",i";
	    else if (instructions->addr_mode == 0x01)
	        addr_mode = ",x";
        }
        else if (instructions->addr_mode != DNE &&
            !instructions->single_digit_addressing)
	{
           if (instructions->addr_mode == 0x00)
                addr_mode = ",i";
           else if (instructions->addr_mode == 0x01)
                addr_mode = ",d";
           else if (instructions->addr_mode == 0x02)
                addr_mode = ",n";
           else if (instructions->addr_mode == 0x03)
                addr_mode = ",s";
           else if (instructions->addr_mode == 0x04)
                addr_mode = ",sf";
           else if (instructions->addr_mode == 0x05)
                addr_mode = ",x";
           else if (instructions->addr_mode == 0x06)
                addr_mode = ",sx";
           else if (instructions->addr_mode == 0x07)
                addr_mode = ",sxf";

	}

    	//print out Operand
	//if a symbol label corresponds and the instruction is not unary
	if (cur_sym != NULL && !instructions->unary)
	    printf("%s",cur_sym->label);
	else
	{
	    //if instruction is unary, no operand specifier
	    if (!instructions->unary)
	    	printf ("0x%04X", instructions->op_spec);
	}
	if (addr_mode != NULL)
	    printf("%s",addr_mode);    
    }
}


