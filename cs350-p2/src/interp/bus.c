/* ************************************************************************* *
 * bus.c                                                                     *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This file is used by interp.c to simulate the bus              *
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

#include "bus.h"			/* header file */
#include "interp.h"			/* cpu_t */
#include "processor.h"			/* instructions */
/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: fetch the next 3 bytes of memory                                 *
 *									     *
 * Parameters: 								     *
 * 	memory:  the bytes to interpret the instructions		     *
 *	starting-address: the place in memory to start fetching bytes	     * 
 * 									     *
 * Returns: 								     *
 * 	uint32_t - returns the three bytes for the instruction register in   *
 *		   the form 0x00XXXXXX where X are the bytes		     *
 * ************************************************************************* */
uint32_t fetch(uint8_t *memory, uint16_t starting_address)
{
    //big endian, most significant is at smallest address
    uint32_t most_sig = memory[starting_address];
    uint32_t sec_most_sig = memory[starting_address+1];
    uint32_t least_sig = memory[starting_address+2];
    uint32_t next_3_bytes = 0;
    
    next_3_bytes += (most_sig <<16);
    next_3_bytes += (sec_most_sig <<8);
    next_3_bytes += (least_sig);
    return next_3_bytes;     
}
