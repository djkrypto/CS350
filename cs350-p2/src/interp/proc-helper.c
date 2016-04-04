/* ************************************************************************* *
 * proc-helper.c                                                             *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This file is used by the processor for execute methods         *
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
#include <stdio.h>			/* printf */
#include <string.h>			/* strncmp */
#include <ctype.h>			/* isprint */

#include "proc-helper.h"		/* header file */
#include "bus.h"			/* access bus methods */
#include "interp.h"			/* instructions */
#include "../main/debug.h"		/* DEBUG macros */
#include "../output/print-interp.h"	/* output */
/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Execute the instruction inst                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_arithmetic_and_logic_operators(cpu_t* pep8,instruction_t* inst,
					    uint8_t* memory)
{
    uint8_t mnem = inst->mnem;

    if (mnem == 46 || mnem == 47) //ADDA and ADDX
	execute_addr(pep8,inst,memory);
    else if (mnem == 48 || mnem == 49) //SUBA and SUBX
        execute_subr(pep8,inst,memory);
    else if (mnem == 50 || mnem == 51) //ANDA and ANDX
        execute_andr(pep8,inst,memory);
    else if (mnem == 52 || mnem == 53) //ORA and ORX
        execute_orr(pep8,inst,memory);
    else if (mnem == 54 || mnem == 55) //CPA and CPX
        execute_cpr(pep8,inst,memory);
    else if (mnem == 14 || mnem == 15) //NOTA and NOTX
        execute_notr(pep8,inst,memory);
    else if (mnem == 16 || mnem == 17) //NEGA and NEGX
        execute_negr(pep8,inst,memory);
    else
	printf("execute_arithmetic error\n");

}

/* ************************************************************************* *
 * Purpose: Execute the instruction ADDr                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_addr(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
	temp = inst->op_spec;
    else if (inst->addr_mode == 1) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec + 1];
	temp = ((most_sig_byte <<8) + least_sig_byte);
    }
    else
	print_unsupported_addr_mode(inst);

    
    if (inst->mnem == 46) //ADDA
    {
	pep8->accum += temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //ADDX
    {
	pep8->x += temp;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }

}
/* ************************************************************************* *
 * Purpose: Execute the instruction SUBr                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_subr(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
        temp = inst->op_spec;
    else if (inst->addr_mode == 1) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec + 1];
        temp = ((most_sig_byte <<8) + least_sig_byte);
    }
    else
        print_unsupported_addr_mode(inst);


    if (inst->mnem == 48) //SUBA
    {
        pep8->accum -= temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //SUBX
    {
        pep8->x -= temp;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction ANDr                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_andr(cpu_t* pep8,instruction_t* inst, uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
	temp = inst->op_spec;
    else if (inst->addr_mode == 1) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec + 1];
	temp = ((most_sig_byte <<8) + least_sig_byte);
    }
    else
        print_unsupported_addr_mode(inst);

    if (inst->mnem == 50) //ANDA
    {
        pep8->accum &= temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //ANDX
    {
        pep8->x &= temp;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction ORr                                      *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_orr(cpu_t* pep8,instruction_t* inst, uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
        temp = inst->op_spec;
    else if (inst->addr_mode == 1) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec + 1];
        temp = ((most_sig_byte <<8) + least_sig_byte);
    }
    else
        print_unsupported_addr_mode(inst);

    pep8->z = temp == 0 ? true:false; //set z bit
    pep8->n = ((temp>>15) & 1) == 1 ? true:false; //set n bit

    if (inst->mnem == 52) //ORA
    {
        pep8->accum |= temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //ORX
    {
        pep8->x |= inst->op_spec;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction CPr                                      *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_cpr(cpu_t* pep8,instruction_t* inst, uint8_t* memory)
{
    int16_t temp;
    if (inst->mnem == 54) //CPA
    {
	temp = pep8->accum - inst->op_spec;
    }
    else //CPX
    {
	temp = pep8->x - inst->op_spec;
    }
    
    pep8->z = temp == 0 ? true:false; //set z bit
    pep8->n = temp < 0 ? true:false; //set n bit
   
}

/* ************************************************************************* *
 * Purpose: Execute the instruction NOTr                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_notr(cpu_t* pep8,instruction_t* inst, uint8_t* memory)
{
    if (inst->mnem == 14) //NOTA
        pep8->accum = flip_bits(pep8->accum);
    else //NOTX
        pep8->x = flip_bits(pep8->x);
}

/* ************************************************************************* *
 * Purpose: Execute the instruction NEGr                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_negr(cpu_t* pep8,instruction_t* inst, uint8_t* memory)
{
    if (inst->mnem == 16) //NEGA
        pep8->accum = flip_bits(pep8->accum) + 1;
    else //NEGX
        pep8->x = flip_bits(pep8->x) + 1;
}

/* ************************************************************************* *
 * Purpose: "NOT" or "flip" all bits of num                                  *
 *                                                                           *
 * Parameters:                                                               *
 *      num: the number to flip		                                     *
 * 									     *
 * Returns:								     *
 *	uint16_t: num with all of it's bits flipped			     *
 * ************************************************************************* */
uint16_t flip_bits(uint16_t num)
{
    uint16_t num_of_bits = sizeof(num) * 8; //better be 16
    uint16_t reverse_num = 0;
    int i = 0;
    for (i = 0; i < num_of_bits; i++)
    {
        if ((num & (1 << (num_of_bits-i)))) //check if 1 is in the current column
    	    continue;
	else //if 0 is in the current column
	    reverse_num += (1 << (num_of_bits-i));  //set the reverse_num
    }
    
    return reverse_num;
}

/* ************************************************************************* *
 * Purpose: Execute the instruction inst                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_load_and_store(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint8_t mnem = inst->mnem;
    DEBUGx("mnemonic in load and store: %" PRIu8 "\n",mnem);

    if (mnem == 56 || mnem == 57) //LDA and LDX
        execute_ldr(pep8,inst,memory);
    else if (mnem == 58 || mnem == 59) //LDBYTEA and LDBYTEX
        execute_ldbyter(pep8,inst,memory);
    else if (mnem == 60 || mnem == 61) //STA and STX
        execute_str(pep8,inst,memory);
    else if (mnem == 62 || mnem == 63) //STBYTEA and STBYTEX
        execute_stbyter(pep8,inst,memory);
    else
        printf("execute_load_store error\n");
}

/* ************************************************************************* *
 * Purpose: Execute the instruction LDr                                      *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_ldr(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
        temp = inst->op_spec;
    else if (inst->addr_mode == 1) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec + 1];
        temp = ((most_sig_byte <<8) + least_sig_byte);
    }
    else
        print_unsupported_addr_mode(inst);


    if (inst->mnem == 56) //LDA
    {
        pep8->accum = temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //LDX
    {
        pep8->x = temp;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction LDBYTEr                                  *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_ldbyter(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint16_t temp = 0;
    if (inst->addr_mode == 0) //immediate
        temp = (pep8->accum & 0xFF00) + (uint8_t)inst->op_spec;
    else if (inst->addr_mode == 1) //direct
        temp = (pep8->x & 0xFF00) + (uint8_t)inst->op_spec;
    else
        print_unsupported_addr_mode(inst);


    if (inst->mnem == 58) //LDBYTEA
    {
        pep8->accum = temp;
        pep8->z = pep8->accum == 0 ? true:false; //set z bit
        pep8->n = ((pep8->accum>>15) & 1) == 1 ? true:false; //set n bit
    }
    else //LDBYTEX
    {
        pep8->x = temp;
        pep8->z = pep8->x == 0 ? true:false; //set z bit
        pep8->n = ((pep8->x>>15) & 1) == 1 ? true:false; //set n bit
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction STr                                      *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_str(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    print_divider(); //cleanlines of output

    if (inst->addr_mode == 1) //direct
    {
	uint16_t most_sig_byte;
	uint16_t least_sig_byte;
        if (inst->mnem == 60) //STA
	{
            most_sig_byte = (uint8_t)(pep8->accum >> 8);
            least_sig_byte = (uint8_t)(pep8->accum);
	}
        else //STX
	{
	    most_sig_byte = (uint8_t)(pep8->x >> 8);
            least_sig_byte = (uint8_t)(pep8->x);
	}
	memory[inst->op_spec] = most_sig_byte;
        memory[inst->op_spec+1] = least_sig_byte;
        printf("  Mem[%04X] <-- 0x%04X\n",inst->op_spec,most_sig_byte);
        printf("  MEM[%04X] <-- 0x%04X\n",inst->op_spec+1,least_sig_byte);
    }
    else if (inst->addr_mode == 0) //immediate
	print_invalid_addr_mode(inst);
    else
        print_unsupported_addr_mode(inst);
}

/* ************************************************************************* *
 * Purpose: Execute the instruction STBYTEr                                  *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_stbyter(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    print_divider(); //this function prints output.  This looks cleaner

    if (inst->addr_mode == 1) //direct
    {
        if (inst->mnem == 62) //STBYTEA
	{
            memory[inst->op_spec] = (uint8_t)pep8->accum;
    	    printf("  Mem[%04X] <-- 0x%04X\n",inst->op_spec,
					      (uint8_t)pep8->accum);
	}
        else //STBYTEX
	{
            memory[inst->op_spec] = (uint8_t)pep8->x;
   	    printf("  Mem[%04X] <-- 0x%04X\n",inst->op_spec,
					      (uint8_t)pep8->x);
	}
    }
    else if (inst->addr_mode == 0) //immediate
        print_invalid_addr_mode(inst);
    else
        print_unsupported_addr_mode(inst);
}

/* ************************************************************************* *
 * Purpose: Execute the instruction inst                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_output(cpu_t* pep8, instruction_t* inst, uint8_t* memory)
{
    uint8_t op = pep8->inst_reg>>16;
    if (op >= 0x38 && op <= 0x3F)
	execute_deco(pep8,inst,memory);
    else if (op >= 0x50 && op <= 0x57)
	execute_charo(pep8,inst,memory);
}

/* ************************************************************************* *
 * Purpose: Execute the instruction DECO                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_deco(cpu_t* pep8, instruction_t* inst, uint8_t* memory)
{
    print_divider();
    int16_t result;
    if (inst->addr_mode == 0x00) //immediate
    {
	result = (int)(inst->op_spec);
	printf("  Output: %d\n",result);
    }
    else if (inst->addr_mode == 0x01) //direct
    {
        uint16_t most_sig_byte = memory[inst->op_spec];
        uint16_t least_sig_byte = memory[inst->op_spec+1];
        result = (most_sig_byte <<8) + least_sig_byte;	
	printf("  Output: %d\n",result);
    }
    else
        print_unsupported_addr_mode(inst);	
}

/* ************************************************************************* *
 * Purpose: Execute the instruction CHARO                                    *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_charo(cpu_t* pep8, instruction_t* inst, uint8_t* memory)
{
    print_divider();
    if(inst->addr_mode == 0x00) //immediate
    {
	if (isprint((uint8_t)inst->op_spec))
	    printf("  Output '%c'\n",(uint8_t)inst->op_spec);
	else
	    printf("  Output '\\x%02X'\n",(uint8_t)inst->op_spec);
    }
    else if (inst->addr_mode == 0x01) //direct
    {
        if (isprint(memory[inst->op_spec]))
            printf("  Output '%c'\n",memory[inst->op_spec]);
        else
            printf("  Output '\\x%02X'\n",memory[inst->op_spec]);
    }	
    else
	print_unsupported_addr_mode(inst);
}

/* ************************************************************************* *
 * Purpose: Execute the instruction inst                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_branches(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    uint8_t mnem = inst->mnem;
    DEBUGx("mnemonic in load and store: %" PRIu8 "\n",mnem);

    if (mnem == 4) //BR
        execute_br(pep8,inst,memory);
    else if (mnem == 5) //BRLE
        execute_brle(pep8,inst,memory);
    else if (mnem == 6) //BRLT
        execute_brlt(pep8,inst,memory);
    else if (mnem == 7) //BREQ
        execute_breq(pep8,inst,memory);
    else if (mnem == 8) //BRNE
        execute_brne(pep8,inst,memory);
    else if (mnem == 9) //BRGE
        execute_brge(pep8,inst,memory);
    else if (mnem == 10) //BRGT
        execute_brgt(pep8,inst,memory);
    else
        printf("execute_load_store error\n");
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BR                                       *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_br(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (inst->addr_mode == 0x00) //immediate
        pep8->pc = inst->op_spec;
    else if (inst->addr_mode == 0x01) //indexed
        pep8->pc = inst->op_spec;
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BRLE                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_brle(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (pep8->n || pep8->z)
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BRLT                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_brlt(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (pep8->n)
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BREQ                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_breq(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (pep8->z)
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BRNE                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_brne(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (!(pep8->z))
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BRGE                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_brge(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (!(pep8->n))
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction BRGT                                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *      inst: the instruction to execute                                     *
 *      memory: the bytes of memory that the instruction may use/affect      *
 * ************************************************************************* */
void execute_brgt(cpu_t* pep8,instruction_t* inst,uint8_t* memory)
{
    if (!(pep8->n) && !(pep8->z))
    {
        if (inst->addr_mode == 0x00) //immediate
            pep8->pc = inst->op_spec;
        else if (inst->addr_mode == 0x01) //indexed
            pep8->pc = inst->op_spec;
    }
}

/* ************************************************************************* *
 * Purpose: Execute the instruction STOP                                     *
 * ************************************************************************* */
void execute_stop()
{
    print_divider();
    exit(0);
}

