#ifndef __PEP8_CONST__
#define __PEP8_CONST__

#include <stdint.h>     /* uint32_t */
#include <string.h>     /* strncmp */
#include <stdio.h>	/* printf */

#include "debug.h"	/*DEBUG macros*/
#include "pep8.h"	/*mnemonic constants */

const char * MNEMONICS[] = {
    "STOP", "RETTR", "MOVSPA", "MOVFLGA", "BR", "BRLE", "BRLT", "BREQ", "BRNE",
    "BRGE", "BRGT", "BRV", "BRC", "CALL", "NOTA", "NOTX", "NEGA", "NEGX",
    "ASLA", "ASLX", "ASRA", "ASRX", "ROLA", "ROLX", "RORA", "RORX", "NOP0",
    "NOP1", "NOP2", "NOP3", "NOP", "DECI", "DECO", "STRO", "CHARI", "CHARO",
    "RET0", "RET1", "RET2", "RET3", "RET4", "RET5", "RET6", "RET7", "ADDSP",
    "SUBSP", "ADDA", "ADDX", "SUBA", "SUBX", "ANDA", "ANDX", "ORA", "ORX",
    "CPA", "CPX", "LDA", "LDX", "LDBYTEA", "LDBYTEX", "STA", "STX", "STBYTEA",
    "STBYTEX", ".ASCII", ".BLOCK", ".WORD"
};

/* ************************************************************************* *
 * Purpose: Convert a string code to a mnemonic_t.                           *
 * ************************************************************************* */
mnemonic_t get_mnemonic_by_id (const char *code)
{
    uint32_t index = 0;
    for (index = 0; index < NUMBER_OF_MNEMONICS; index++)
    {
        if (strncmp(code,MNEMONICS[index],8) == 0)
        {
            return (mnemonic_t)index;
        }
    }

    //special case for .ASCII, .BLOCK, and .WORD
    char* new_code = strdup(code);
    char* period = ".";
    strcat(new_code,period);
    for (index = NUMBER_OF_MNEMONICS-4;
         index < NUMBER_OF_MNEMONICS;index++)
    {
        DEBUGx("MNEMONICS[index]: %s\n",MNEMONICS[index]);
        if (strncmp(new_code,MNEMONICS[index],8) == 0)
        {
            return (mnemonic_t)index;
        }
    }
    DEBUGx("invalid mnemonic: %s",new_code);
    return INVALID_MNEMONIC_ID; /*is reached if code != to any mnemonic*/
}


#endif
