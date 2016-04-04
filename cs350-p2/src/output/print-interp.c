/* ************************************************************************* *
 * print.c                                                                   *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This code is used to print the interpreter                     *
 * ************************************************************************* */

/* ************************************************************************* *
 * Library includes here.                                                    *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <inttypes.h>           /* allows PRIu8 */
#include <string.h>		/* strcat */
#include <stdlib.h>		/* malloc */

#include "../main/debug.h"      /* DEBUG statements */
#include "print-interp.h"	/* header file */
/* ************************************************************************* *
 * Local function prototypes                                                 *
 * ************************************************************************* */

/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Print the divider                                       *
 * ************************************************************************* */
void print_divider()
{
    printf("------------------------------------\n");
}

/* ************************************************************************* *
 * Purpose: Print the full interpreter		                             *
 * 									     *
 * Parameters:							 	     *
 *     instructions -- the list of instructions to print out		     *
 *     memory -- the array of bytes in memory 				     *
 * ************************************************************************* */
void print_interpreter(cpu_t* pep8)
{
    print_divider();
    print_status_bits(pep8);
    print_accumulator(pep8);
    print_index_register(pep8);
    print_program_counter(pep8);
    print_instruction_register(pep8);
}

/* ************************************************************************* *
 * Purpose: Print the status bits of pep8	 	                     *
 *                                                                           *
 * Parameters:                                                               *
 *     pep8- the cpu to print values from                                    *
 * ************************************************************************* */
void print_status_bits(cpu_t* pep8)
{
    printf("Status bits (NZVC)");
    printf("          ");
    if (pep8->n)
	printf("1 ");
    else
	printf("0 ");
    if (pep8->z)
        printf("1 ");
    else
        printf("0 ");
    if (pep8->v)
        printf("1 ");
    else
        printf("0 ");
    if (pep8->c)
        printf("1 ");
    else
        printf("0 ");

    printf("\n");
}

/* ************************************************************************* *
 * Purpose: Print the accumulator of pep8                                    *
 *                                                                           *
 * Parameters:                                                               *
 *     pep8- the cpu to print values from                                    *
 * ************************************************************************* */
void print_accumulator(cpu_t* pep8)
{
    printf("Accumulator (A)");
    printf("             ");
    printf("0x%04X\n",pep8->accum);
}

/* ************************************************************************* *
 * Purpose: Print the index register of pep8                                 *
 *                                                                           *
 * Parameters:                                                               *
 *     pep8- the cpu to print values from                                    *
 * ************************************************************************* */
void print_index_register(cpu_t* pep8)
{
    printf("Index Register (X)");
    printf("          ");
    printf("0x%04X\n",pep8->x);
}

/* ************************************************************************* *
 * Purpose: Print the program counter of pep8                                *
 *                                                                           *
 * Parameters:                                                               *
 *     pep8- the cpu to print values from                                    *
 * ************************************************************************* */
void print_program_counter(cpu_t* pep8)
{
    printf("Program counter (PC)");
    printf("        ");
    printf("0x%04X\n",pep8->pc);
}

/* ************************************************************************* *
 * Purpose: Print the instruction register of pep8                           *
 *                                                                           *
 * Parameters:                                                               *
 *     pep8- the cpu to print values from                                    *
 * ************************************************************************* */
void print_instruction_register(cpu_t* pep8)
{
    printf("Instruction register (IR)");
    printf("   ");
    printf("0x%06X\n",pep8->inst_reg);
}

/* ************************************************************************* *
 * Purpose: Print an error message corresponding to function title           *
 *                                                                           *
 * Parameters:                                                               *
 *     inst- the instruction corresponding to unsupported attributes         *
 * ************************************************************************* */
void print_unsupported_instruction(instruction_t* inst)
{
    printf("The given instruction \"%s\" is not supported by this"
	   " interpreter.  Exiting program \n",MNEMONICS[inst->mnem]);
    exit(0);
}

/* ************************************************************************* *
 * Purpose: Print an error message corresponding to function title           *
 *                                                                           *
 * Parameters:                                                               *
 *     inst- the instruction corresponding to unsupported attributes         *
 * ************************************************************************* */
void print_unsupported_addr_mode(instruction_t* inst)
{
    char* addr_mode = NULL;
    if (inst->addr_mode == 0x02)
	addr_mode = "Indirect";
    else if (inst->addr_mode == 0x03)
        addr_mode = "Stack-relative";
    else if (inst->addr_mode == 0x04)
        addr_mode = "Stack_relative deferred";
    else if (inst->addr_mode == 0x05)
        addr_mode = "Indexed";
    else if (inst->addr_mode == 0x06)
        addr_mode = "Stack-indexed";
    else if (inst->addr_mode == 0x07)
        addr_mode = "Stack-indexed deferred";
    else
	addr_mode = "Invalid";
    printf("The given instruction %s's addressing mode: %s is not supported by"
	   "this interpreter. Exiting program \n",
	     MNEMONICS[inst->mnem],addr_mode);
    exit(0);
}

/* ************************************************************************* *
 * Purpose: Print an error message corresponding to function title           *
 *                                                                           *
 * Parameters:                                                               *
 *     inst- the instruction corresponding to unsupported attributes         *
 * ************************************************************************* */
void print_invalid_addr_mode(instruction_t* inst)
{
    char* addr_mode = NULL;
    if (inst->addr_mode == 0x00)
        addr_mode = "Immediate";
    else if (inst->addr_mode == 0x01)
        addr_mode = "Direct";
    else if (inst->addr_mode == 0x02)
        addr_mode = "Indirect";
    else if (inst->addr_mode == 0x03)
        addr_mode = "Stack-relative";
    else if (inst->addr_mode == 0x04)
        addr_mode = "Stack_relative deferred";
    else if (inst->addr_mode == 0x05)
        addr_mode = "Indexed";
    else if (inst->addr_mode == 0x06)
        addr_mode = "Stack-indexed";
    else if (inst->addr_mode == 0x07)
        addr_mode = "Stack-indexed deferred";
    printf("The given instruction %s's addressing mode: %s is not valid for"
           "this instruction\n",MNEMONICS[inst->mnem],addr_mode);
    exit(1);
}



