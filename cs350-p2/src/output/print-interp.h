#ifndef __INTERP_PRINTER__
#define __INTERP_PRINTER__

/* ************************************************************************* *
 * print-interp.h                                                            *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  Header file for print-interp.c                                 *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here. If none needed, delete this comment.               *
 * ************************************************************************* */
#include "../main/pep8.h"		/* mnemonics */
#include "../interp/interp.h"		/* cpu_t */
#include "../interp/processor.h"	/* instruction_t */ 

/* ************************************************************************* *
 * Function prototypes here. Note that variable names are often omitted.     *
 * ************************************************************************* */
void print_divider();
void print_interpreter(cpu_t*);
void print_status_bits(cpu_t*);
void print_accumulator(cpu_t*);
void print_index_register(cpu_t*);
void print_program_counter(cpu_t*);
void print_instruction_register(cpu_t*);

void print_unsupported_instruction(instruction_t*);
void print_unsupported_addr_mode(instruction_t*);
void print_invalid_addr_mode(instruction_t*);
#endif
