#ifndef __PEP8_PRINTER__
#define __PEP8_PRINTER__

/* ************************************************************************* *
 * print.h                                                                   *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  Header file for print.c                                        *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here. If none needed, delete this comment.               *
 * ************************************************************************* */
#include "../disasm/disasm.h"          /* disasm structs and types */


/* ************************************************************************* *
 * Function prototypes here. Note that variable names are often omitted.     *
 * ************************************************************************* */
void print_first_line();
void print_disassembler(instruction_t*,uint8_t*,symtab_t**);
void print_address(instruction_t*);
uint8_t print_code(instruction_t*);
void print_symbol(instruction_t*);
void print_mnemonic(instruction_t*);
void print_operand(instruction_t*,uint8_t*,symtab_t**);
uint8_t print_pseudo_operand(instruction_t*);
void print_excess_bytes(instruction_t*,uint8_t*);
#endif
