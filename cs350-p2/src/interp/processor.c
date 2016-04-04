/* ************************************************************************* *
 * processor.c                                                               *
 * ------                                                                    *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This file is used by interp.c to simulate the processor        *
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

#include "bus.h"			/* access bus methods */
#include "interp.h"			/* instructions */
#include "processor.h"			/* header file */
#include "../main/debug.h"		/* DEBUG macros */
#include "../main/pep8.h"		/* mnemonics */
#include "proc-helper.h"		/* helper functions */
#include "../output/print-interp.h"	/* print functions */

/* ************************************************************************* *
 * Local function declarations                                               *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: Figure out what instruction is in the pep8 inst_reg	             *
 *                                                                           *
 * Parameters:                                                               *
 *	pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object		     *
 * ************************************************************************* */
void decode(cpu_t* pep8,instruction_t** inst)
{
    uint8_t op = pep8->inst_reg>>16;
    instruction_t* cur_inst = *inst;
    if ((op >= 0x00 && op <= 0x03) || (op >= 0x18 && op <= 0x27)
                                   || (op >= 0x58 && op <= 0x5F))
        decode_unary_instruction(pep8,cur_inst);
    else if (op >= 0x04 && op <= 0x17)
        decode_branch_call_instruction(pep8,cur_inst);
    else if (op >= 0x28 && op <= 0x47)
        decode_trap_instruction(pep8,cur_inst);
    else if (op >= 0x48 && op <= 0x57)
        decode_char_in_out_instruction(pep8,cur_inst);
    else if (op >= 0x60 && op <= 0x6F)
        decode_stack_pointer_instruction(pep8,cur_inst);
    else if (op >= 0x70 && op <= 0xBF)
        decode_add_sub_comp_instruction(pep8,cur_inst);
    else if (op >= 0xC0 && op <= 0xFF)
        decode_load_store_instruction(pep8,cur_inst);
    else
    {
        printf("Error in decode_instructions, op is: %" PRIu8 "\n",op);
        exit(1);
    }
}

/* ************************************************************************* *
 * Purpose: Increment the pep8 program counter		                     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object to increment		                     *
 * 	inst: the instruction to use to determine how much to increment	     *
 * ************************************************************************* */
void increment (cpu_t* pep8,instruction_t* inst)
{
    if (inst->unary)
	pep8->pc += 1;
    else
	pep8->pc += 3;
}

/* ************************************************************************* *
 * Purpose: Execute the instruction inst			             *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to determine the instruction               *
 *	inst: the instruction to execute				     *
 *	memory: the bytes of memory that the instruction may affect	     *
 * ************************************************************************* */
void execute (cpu_t* pep8, instruction_t* inst,uint8_t* memory)
{
    uint8_t op = pep8->inst_reg>>16;
    if (op == 0x00)
	execute_stop();
    else if ((op >= 0x18 && op <= 0x1B) ||
	    (op >= 0x70 && op <= 0xBF))
	execute_arithmetic_and_logic_operators(pep8,inst,memory);
    else if (op >= 0xC0 && op <= 0xFF)
	execute_load_and_store(pep8,inst,memory);
    else if ((op >= 0x38 && op <= 0x3F) ||
	    (op >= 0x50 && op <= 0x57))
	execute_output(pep8,inst,memory);
    else if (op >= 0x04 && op <= 0x11)
	execute_branches(pep8,inst,memory);
    else
	print_unsupported_instruction(inst);
}

/* ************************************************************************* *
 * Purpose: To create an instruction corresponding to the given mnemonic and *
 *            add it to the list of instructions.                            *
 *                                                                           *
 * Parameters:                                                               *
 *     addr-the address of the instruction in memory                         *
 *     mnemonic- the string corresponding to the mnemonic of the instruction *
 *     instruction- the instruction specifier                                *
 *     op-spec- operand specifier. NULL if instruction is unary              *
 *     symbol- the symbol table element for the address of this instruction  *
 *     unary- true if instruction is unary, false otherwise                  *
 *                                                                           *
 * Returns: instruction_t                                                    *
 * ************************************************************************* */
instruction_t create_new_instruction(uint16_t addr, const char* mnemonic,
                                 uint8_t instruction, uint16_t op_spec,
                                 uint8_t registr, uint8_t addr_mode,
                                 bool unary)
{
    instruction_t inst = {addr,instruction,registr,addr_mode,
                          get_mnemonic_by_id(mnemonic),op_spec,unary};
    return inst;
}

/* ************************************************************************* *
 * Purpose: To determine the given stack pointer instruction                 *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_stack_pointer_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t addr_mode = inst_spec & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;

    // determine Accumulator versus Index Register
    switch(inst_spec & 0xF8)
    {
        case /*ADDSP*/ 0x60:
            mnemonic = "ADDSP";
            break;
        case /*SUBSP*/ 0x68:
            mnemonic = "SUBSP";
            break;
    }

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,DNE,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given unary instruction                         *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_unary_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t registr = inst_spec & 0x01;
    const char* mnemonic = NULL;
    uint16_t address = pep8->pc;
    uint8_t n_bytes; // used for NOPn and RETn
    // determine Accumulator versus Index Register
    switch(inst_spec)
    {
        case /*STOP*/ 0x00:
            mnemonic = "STOP";
            break;
        case /*RETTR*/ 0x01:
            mnemonic = "RETTR";
            break;
        case /*MOVSPA*/ 0x02:
            mnemonic = "MOVSPA";
            break;
        case /*MOVFLGA*/ 0x03:
            mnemonic = "MOVFLGA";
    }
    if ((inst_spec & 0xF8) == 0x18) // NOTr to ASRr instructions
    {
        switch (inst_spec & 0x06)
        {
            case /*NOTr*/ 0x00:
                mnemonic = registr ? "NOTX" : "NOTA";
                break;
            case /*NEGr*/ 0x02:
                mnemonic = registr ? "NEGX" : "NEGA";
                break;
            case /*ASLr*/ 0x04:
                mnemonic = registr ? "ASLX" : "ASLA";
                break;
            case /*ASRr*/ 0x06:
                mnemonic = registr ? "ASRX" : "ASRA";
                break;
        }
    }
    if ((inst_spec & 0xF4) == 0x20) //ROLr and RORr
    {
        switch (inst_spec & 0x06)
        {
            case /*ROLr*/ 0x00:
                mnemonic = registr ? "ROLX" : "ROLA";
                break;
            case /*RORr*/ 0x02:
                mnemonic = registr ? "RORX" : "RORA";
                break;
        }
    }
    if ((inst_spec & 0xF8) == 0x58) //RETn instruction
    {
        n_bytes = inst_spec & 0x07;
        if (n_bytes == 0) mnemonic = "RET0";
        else if (n_bytes == 1) mnemonic = "RET1";
        else if (n_bytes == 2) mnemonic = "RET2";
        else if (n_bytes == 3) mnemonic = "RET3";
        else if (n_bytes == 4) mnemonic = "RET4";
        else if (n_bytes == 5) mnemonic = "RET5";
        else if (n_bytes == 6) mnemonic = "RET6";
        else if (n_bytes == 7) mnemonic = "RET7";
    }
    else if ((inst_spec & 0xFC) == 0x24) // NOPn instruction
    {
        n_bytes = inst_spec & 0x03;
        if (n_bytes == 0) mnemonic = "NOP0";
        else if (n_bytes == 1) mnemonic = "NOP1";
        else if (n_bytes == 2) mnemonic = "NOP2";
        else if (n_bytes == 3) mnemonic = "NOP3";
    }

    DEBUG("after switch in unary\n");

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,DNE,registr,
                                       DNE,true);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given branch instruction                        *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_branch_call_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;    
    uint8_t addr_mode = inst_spec & 0x01;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;

    // determine Accumulator versus Index Register
    switch(inst_spec & 0xFE)
    {
        case /*BR*/ 0x04:
            mnemonic = "BR";
            break;
        case /*BRLE*/ 0x06:
            mnemonic = "BRLE";
            break;
        case /*BRLT*/ 0x08:
            mnemonic = "BRLT";
            break;
        case /*BREQ*/ 0x0A:
            mnemonic = "BREQ";
            break;
        case /*BRNE*/ 0x0C:
            mnemonic = "BRNE";
            break;
        case /*BRGE*/ 0x0E:
            mnemonic = "BRGE";
            break;
        case /*BRGT*/ 0x10:
            mnemonic = "BRGT";
            break;
        case /*BRV*/ 0x12:
            mnemonic = "BRV";
            break;
        case /*BRC*/ 0x14:
            mnemonic = "BRC";
            break;
        case /*CALL*/ 0x16:
            mnemonic = "CALL";
            break;
    }

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,DNE,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given trap instruction                          *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_trap_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t addr_mode = inst_spec & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;

    // determine Accumulator versus Index Register
    switch(inst_spec & 0xF8) /* NOP to STRO */
    {
        case /*NOP*/ 0x28:
            mnemonic = "NOP";
            break;
        case /*DECI*/ 0x30:
            mnemonic = "DECI";
            break;
        case /*DECO*/ 0x38:
            mnemonic = "DECO";
            break;
        case /*STRO*/ 0x40:
            mnemonic = "STRO";
            break;
    }

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,DNE,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given add, subtract, or compare instruction     *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_add_sub_comp_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t addr_mode = inst_spec & 0x07;
    uint8_t registr = (inst_spec & 0x08) >> 3;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;

    // determine Accumulator versus Index Register
    switch(inst_spec & 0xF0)
    {
        DEBUG("Entered switch in sub comp\n");
        case /*ADDr*/ 0x70:
            mnemonic = registr ? "ADDX" : "ADDA";
            break;
        case /*SUBr*/ 0x80:
            mnemonic = registr ? "SUBX" : "SUBA";
            break;
        case /*ANDr*/ 0x90:
            mnemonic = registr ? "ANDX" : "ANDA";
            break;
        case /*ORr*/ 0xA0:
            mnemonic = registr ? "ORX" : "ORA";
            break;
        case /*CPr*/ 0xB0:
            mnemonic = registr ? "CPX" : "CPA";
            break;
    }
    DEBUG("after switch in sub comp\n");

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,registr,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given load or store instruction                 *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_load_store_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t addr_mode = inst_spec & 0x07;
    uint8_t registr = (inst_spec & 0x08) >> 3;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;
    
    // determine Accumulator versus Index Register
    switch(inst_spec & 0xF0)
    {
        case /*LDr*/ 0xC0:
            mnemonic = registr ? "LDX" : "LDA";
            break;
        case /*LDBYTEr*/ 0xD0:
            mnemonic = registr ? "LDBYTEX" : "LDBYTEA";
            break;
        case /*STr*/ 0xE0:
            mnemonic = registr ? "STX" : "STA";
            break;
        case /*STBYTEr*/ 0xF0:
            mnemonic = registr ? "STBYTEX" : "STBYTEA";
            break;
    }

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,registr,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine between remaining instructions                      *
 *                                                                           *
 * Parameters:                                                               *
 *      pep8: the cpu object used to decode the instruction                  *
 * Returns:                                                                  *
 *      instruction_t: A pointer to an instruction object                    *
 * ************************************************************************* */
void decode_char_in_out_instruction(cpu_t* pep8,instruction_t* inst)
{
    uint8_t inst_spec = pep8->inst_reg>>16;
    uint8_t addr_mode = inst_spec & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = pep8->inst_reg & 0xFFFF;
    uint16_t address = pep8->pc;

    // determine Accumulator versus Index Register
    switch(inst_spec & 0xF0)
    {
        case /*CHARI*/ 0x40:
            mnemonic = "CHARI";
            break;
        case /*CHARO*/ 0x50:
            mnemonic = "CHARO";
            break;
    }

    //get the instruction_t object
    *inst = create_new_instruction(address,mnemonic,inst_spec,op_spec,DNE,
                                       addr_mode,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

