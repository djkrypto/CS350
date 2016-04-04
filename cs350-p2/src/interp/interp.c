/* ************************************************************************* *
 * interp.c                                                                  *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This file is used to interpret the instructions                *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.  For documentation of standard C library           *
 * functions, see the list at:                                               *
 *   http://pubs.opengroup.org/onlinepubs/009695399/functions/contents.html  *
 * ************************************************************************* */

#include <stdbool.h>                    /* bool types */
#include <stdint.h>                     /* uint32_t, uint8_t, etc. */
#include <stdlib.h>                     /* malloc */
#include <inttypes.h>                   /* declares PRIu8 */
#include <stdio.h>		        /* printf */

#include "interp.h"			/* header file */
#include "bus.h"			/* bus methods */
#include "processor.h"			/* instruction */
#include "../output/print-interp.h"	/* output interpreter */
#include "../main/debug.h"		/* DEBUG macros */
/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Figure out what instruction is in the pep8 inst_reg              *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 *	memory: the bytes to interpret					     *
 *	mem_length: the length of memory				     *
 * ************************************************************************* */
void interpret_memory(uint8_t* memory,cpu_t* pep8,uint16_t mem_length)
{
    preset_cpu(pep8);

    while (pep8->pc < mem_length)
    {
        pep8->inst_reg = fetch(memory,pep8->pc);//fetch
	instruction_t *inst = malloc(sizeof(instruction_t));
	decode(pep8,&inst);//decode
	increment(pep8,inst);//increment
	print_interpreter(pep8); //print out cpu
	execute(pep8,inst,memory); //execute
	free(inst);
    }
    //stylistically since you have reached the last instruction
    print_divider();
}

/* ************************************************************************* *
 * Purpose: Preset all of the values of the cpu before running interpreter   *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * ************************************************************************* */
void preset_cpu(cpu_t* pep8)
{
    pep8->inst_reg = 0;
    pep8->accum = 0;
    pep8->x = 0;
    pep8->pc = 0;
    pep8->n = false;
    pep8->z = false;
    pep8->v = false;
    pep8->c = false;
}

