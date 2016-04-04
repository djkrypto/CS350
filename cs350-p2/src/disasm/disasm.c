/* ************************************************************************* *
 * disasm.c                                                                  *
 * -------                                                                   *
 *  Author:   David Johnson                                                  *
 *  Purpose:  This code is used to disassemble the array of bytes in         *
 *            project 2.                                                     *
 * ************************************************************************* */


/* ************************************************************************* *
 * Library includes here.                                                    *
 * ************************************************************************* */

#include <stdio.h>              /* standard I/O */
#include <ctype.h>              /* declares isprint() */
#include <stdint.h>             /* declares uint32_t */
#include <string.h>             /* declares strcmp() */
#include <inttypes.h>		/* allows PRIu8 */
#include <stdbool.h>		/* allows booleans */
#include <stdlib.h>		/* allows exit() */

#include "disasm.h"		/* header file */
#include "../main/debug.h"	/* DEBUG statements */
#include "../main/pep8.h"	/* mnemonics */
/* ************************************************************************* *
 * Local function prototypes                                                 *
 * ************************************************************************* */

/* ************************************************************************* *
 * Global variable declarations                                              *
 * ************************************************************************* */

/* ************************************************************************* *
 * Purpose: To create an instruction corresponding to the given mnemonic and *
 *            add it to the list of instructions.  			     *
 *									     *
 * Parameters:								     *
 *     addr-the address of the instruction in memory 		             *
 *     mnemonic- the string corresponding to the mnemonic of the instruction *
 *     instruction- the instruction specifier				     *
 *     op-spec- operand specifier. NULL if instruction is unary  	     *
 *     symbol- the symbol table element for the address of this instruction  *
 *     unary- true if instruction is unary, false otherwise		     *
 * 									     *
 * Returns: instruction_t						     *
 * ************************************************************************* */
instruction_t create_instruction(uint16_t addr, const char* mnemonic,
				 uint8_t instruction, uint16_t op_spec,
				 uint8_t registr, uint8_t addr_mode,
				 symtab_t* symbol, bool unary)
{
    instruction_t inst = {addr,symbol,instruction,registr,addr_mode,
			  get_mnemonic_by_id(mnemonic),op_spec,unary};
    return inst;
}

/* ************************************************************************* *
 * Purpose: To determine all of the instructions in memory		     *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     mem_length- the number of bytes in memory			     *
 *     symtable- the symbol table for the disassembler to work with          *
 * ************************************************************************* */
void determine_instructions(instruction_t **instructions,uint8_t* memory,
			    int mem_length,symtab_t** symtable)
{
    uint16_t index = 0;
    uint8_t op = 0;
    bool unary;
    *instructions = malloc(sizeof(instruction_t));
    instruction_t *cur_inst = *instructions; //current intruction
    symtab_t* cur_sym = *symtable; //current symbol
    while (index<mem_length)
    {
	cur_inst->single_digit_addressing = false; //preset
	cur_sym = *symtable; //reset current symbol at start of each loop

	//check if index matches the address of any symbol in symbol table
	//if it does, pass it to the determine instructions
	//if none match the index, pass NULL to determine instructions
	while (cur_sym != NULL)
	{
	    DEBUGx("index: %d\n",index);
	    DEBUGx("offset: %jd\n",(intmax_t)cur_sym->offset);
	    if (cur_sym->offset == index)
	    {
		break; //this is the symbol to pass for this instruction
	    }
	    if (cur_sym->next != NULL)
	    {
		cur_sym = cur_sym->next;
	    }
	    else
	    {
		cur_sym = NULL; //pass NULL to determine instructions
		break;
	    }
	}
	unary = false;
	op = memory[index];
	
	//determine if instruction is ASCII or BLOCK
	//if it is, skip the usual process below
	if ((cur_sym != NULL) && (
	    (cur_sym->type == 1) ||
	    (cur_sym->type == 2) ||
	    (cur_sym->type == 3) ))
	{
	    uint16_t increment = determine_symbol_instruction(memory,op,index,
							      cur_inst,cur_sym);
	    DEBUG("current symbol is ASCII, BLOCK, or WORD\n");
	    DEBUGx("%d\n",cur_sym->type);
	    index+= increment; //increment by the number of bytes for each
	}
	else
	{
	    if ((op >= 0x00 && op <= 0x03) || (op >= 0x18 && op <= 0x27)
				|| (op >= 0x58 && op <= 0x5F)) 
	    {
	        determine_unary_instruction(memory,op,index,cur_inst,cur_sym);
	        unary = true;
	    }
            //check if instruction should have operand specifier but doesn't
            else if (index + 2 >= mem_length)
            {
                printf("One of your non-unary instructions does not contain an"
                  " operand specifier.\nTherefore it is invalid. Exiting\n");
                exit(1);
            }
	    else if (op >= 0x04 && op <= 0x17)
	        determine_branch_call_instruction(memory,op,index,cur_inst,cur_sym);
	    else if (op >= 0x28 && op <= 0x47)
	        determine_trap_instruction(memory,op,index,cur_inst,cur_sym);
            else if (op >= 0x48 && op <= 0x57)
                determine_char_in_out_instruction(memory,op,index,cur_inst,cur_sym);
	    else if (op >= 0x60 && op <= 0x6F)
	        determine_stack_pointer_instruction(memory,op,index,cur_inst,cur_sym);
	    else if (op >= 0x70 && op <= 0xBF)
	        determine_add_sub_comp_instruction(memory,op,index,cur_inst,cur_sym);
	    else if (op >= 0xC0 && op <= 0xFF)
	        determine_load_store_instruction(memory,op,index,cur_inst,cur_sym);	    
	    else
	    {
	        printf("Error in determine_instructions, op is: %" PRIu8 "\n",op);
	        exit(1);
	    }

	    // increment	
            if (unary)
                index++;
            else
                index += 3;
 	}
	//Move to next instruction in the linked list
        if (index <= mem_length)
	{
	    cur_inst->next = malloc(sizeof(instruction_t));
	    cur_inst = cur_inst->next;
	}
    }
    cur_inst->next = NULL; //set the final instruction's next value to NULL

    //workaround to fix the fact that you end up with extra instruction
    //checks if cur_inst->next->next == NULL
    //Then sets cur_inst->next == NULL to get rid of empty instruction
    cur_inst = *instructions;
    while (cur_inst->next->next != NULL)
        cur_inst = cur_inst->next;
    cur_inst->next = NULL;

}

/* ************************************************************************* *
 * Purpose: To determine the given WORD, ASCII, BLOCK instruction            *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
uint16_t determine_symbol_instruction(uint8_t* memory,uint8_t inst,uint16_t
                                         address, instruction_t* instructions,
                                         symtab_t* symbol)
{
    uint16_t increment = 0;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    //if WORD, return 2 (for 2 bytes)
    //if BLOCK, return block_length
    //if ASCII, look for NULL byte and return that many plus 1
    
    if (symbol->type == ASCII)
    {
	mnemonic = ".ASCII";
	uint16_t index = address;
	//look through memory for NULL byte
	while (memory[index] != 0x00) //current value not null byte
	    index++;
	increment = (index+1)-address; //account for null byte and start value	
    }
    else if (symbol->type == BLOCK)
    {
	mnemonic = ".BLOCK";
	increment = symbol->block_length;
    }
    else if (symbol->type == WORD)
    {
	mnemonic = ".WORD";
	increment = 2;
    }
    else
    {
        printf("Error in determine_symbol_instructions, op is: %" PRIu8 "\n",
		inst);
        exit(1);
    }

    //get the instruction_t object
    *instructions = create_instruction(address,mnemonic,inst,op_spec,DNE,
                                       DNE,symbol,false);

    //record total number of bytes for ascii string if symbol type was ASCII
    if (symbol->type == ASCII)
	instructions->ascii_bytes = increment;

    //set this particular instruction's symb to the correct symtab
    instructions->symb = symbol;

    DEBUGx("Mnemonic: %s\n",mnemonic);

    return increment;
}

/* ************************************************************************* *
 * Purpose: To determine the given stack pointer instruction`                 *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_stack_pointer_instruction(uint8_t* memory,uint8_t inst,uint16_t
					 address, instruction_t* instructions,
					 symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    // determine Accumulator versus Index Register
    switch(inst & 0xF8)
    {
        case /*ADDSP*/ 0x60:
            mnemonic = "ADDSP";
            break;
        case /*SUBSP*/ 0x68:
            mnemonic = "SUBSP";
            break;
    }

    //get the instruction_t object
    *instructions = create_instruction(address,mnemonic,inst,op_spec,DNE,
                                       addr_mode,symbol,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given unary instruction                         *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_unary_instruction(uint8_t* memory,uint8_t inst,uint16_t address,
				 instruction_t* instructions, symtab_t* symbol)
{
    uint8_t registr = inst & 0x01;
    const char* mnemonic = NULL;
    uint8_t n_bytes; // used for NOPn and RETn
    // determine Accumulator versus Index Register
    switch(inst)
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
    if ((inst & 0xF8) == 0x18) // NOTr to ASRr instructions
    {
	switch (inst & 0x06)
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
    if ((inst & 0xF4) == 0x20) //ROLr and RORr
    {
	switch (inst & 0x06)
	{
	    case /*ROLr*/ 0x00:
		mnemonic = registr ? "ROLX" : "ROLA";
		break;
	    case /*RORr*/ 0x02:
		mnemonic = registr ? "RORX" : "RORA";
		break;
	}
    }
    if ((inst & 0xF8) == 0x58) //RETn instruction
    {
	n_bytes = inst & 0x07;
	if (n_bytes == 0) mnemonic = "RET0";
	else if (n_bytes == 1) mnemonic = "RET1";
        else if (n_bytes == 2) mnemonic = "RET2";
        else if (n_bytes == 3) mnemonic = "RET3";
        else if (n_bytes == 4) mnemonic = "RET4";
        else if (n_bytes == 5) mnemonic = "RET5";
        else if (n_bytes == 6) mnemonic = "RET6";
        else if (n_bytes == 7) mnemonic = "RET7";
    }
    else if ((inst & 0xFC) == 0x24) // NOPn instruction
    {
	n_bytes = inst & 0x03;
        if (n_bytes == 0) mnemonic = "NOP0";
        else if (n_bytes == 1) mnemonic = "NOP1";
        else if (n_bytes == 2) mnemonic = "NOP2";
        else if (n_bytes == 3) mnemonic = "NOP3";
    }
	
    DEBUG("after switch in unary\n");

    //get the instruction_t object
    *instructions = create_instruction(address,mnemonic,inst,DNE,registr,
				       DNE,symbol,true);
    DEBUGx("Mnemonic: %s\n",mnemonic);   
}

/* ************************************************************************* *
 * Purpose: To determine the given branch instruction                        *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_branch_call_instruction(uint8_t* memory,uint8_t inst, uint16_t
				       address, instruction_t* instructions,
				       symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x01;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    DEBUGx("0x%04" PRIx16, op_spec);

    // determine Accumulator versus Index Register
    switch(inst & 0xFE)
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
    *instructions = create_instruction(address,mnemonic,inst,op_spec,DNE,
                                       addr_mode,symbol,false);
    instructions->single_digit_addressing = true;
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given trap instruction	                     *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_trap_instruction(uint8_t* memory,uint8_t inst,uint16_t address,
				instruction_t* instructions, symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    DEBUGx("0x%04" PRIx16, op_spec);

    // determine Accumulator versus Index Register
    switch(inst & 0xF8) /* NOP to STRO */
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
    *instructions = create_instruction(address,mnemonic,inst,op_spec,DNE,
                                       addr_mode,symbol,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given add, subtract, or compare instruction     *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_add_sub_comp_instruction(uint8_t* memory,uint8_t inst, uint16_t
					address, instruction_t* instructions,
					symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x07;
    uint8_t registr = (inst & 0x08) >> 3; 
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    DEBUGx("0x%04" PRIx16, op_spec);
        
    // determine Accumulator versus Index Register
    switch(inst & 0xF0)
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
    *instructions = create_instruction(address,mnemonic,inst,op_spec,registr,
                                       addr_mode,symbol,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine the given load or store instruction                 *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier                                       *
 *     address- the address in memory of the op                              *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_load_store_instruction(uint8_t* memory, uint8_t inst,uint16_t
				      address, instruction_t* instructions,
				      symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x07;
    uint8_t registr = (inst & 0x08) >> 3;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];
    
    DEBUGx("0x%04" PRIx16, op_spec);
    // determine Accumulator versus Index Register
    switch(inst & 0xF0)
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
    *instructions = create_instruction(address,mnemonic,inst,op_spec,registr,
				       addr_mode,symbol,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

/* ************************************************************************* *
 * Purpose: To determine between remaining instructions                      *
 *                                                                           *
 * Parameters:                                                               *
 *     memory- the array of bytes read from file                             *
 *     instructions - the pointer to the instruction linked list             *
 *     inst- the Instruction specifier					     *
 *     address- the address in memory of the op				     *
 *     symbol- the symbol table element for the address of this instruction  *
 * ************************************************************************* */
void determine_char_in_out_instruction(uint8_t* memory,uint8_t inst,uint16_t
				       address, instruction_t* instructions,
				       symtab_t* symbol)
{
    uint8_t addr_mode = inst & 0x07;
    const char* mnemonic = NULL;
    uint16_t op_spec = memory[address+1];
    op_spec = (op_spec << 8) + memory[address+2];

    DEBUGx("0x%04" PRIx16, op_spec);

    // determine Accumulator versus Index Register
    switch(inst & 0xF0)
    {
        case /*CHARI*/ 0x40:
            mnemonic = "CHARI";
            break;
	case /*CHARO*/ 0x50:
	    mnemonic = "CHARO";
	    break;
    }

    //get the instruction_t object
    *instructions = create_instruction(address,mnemonic,inst,op_spec,DNE,
                                       addr_mode,symbol,false);
    DEBUGx("Mnemonic: %s\n",mnemonic);
}

