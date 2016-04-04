#ifndef __PROCESS__
#define __PROCESS__

#include "../main/pep8.h"	/* mnemonics */
#include "interp.h"		/* cpu_t */
//define "Does not exist" variable for registr and addr_mode
#define DNE 255

typedef struct instruction_t {
    uint16_t addr;
    uint8_t inst_spec; //instruction specifier
    uint8_t registr; //DNE if does not apply to this instruction
    uint8_t addr_mode; //DNE if does not apply to this instruction
    mnemonic_t mnem;
    uint16_t op_spec; //if unary is true, op_spec arbitrarily set to DNE
    _Bool unary; //unary means no op-spec
} instruction_t;

/* Global constants defined in pep8-const.c */
extern const char *MNEMONICS[];

/*Prototypes*/
void decode(cpu_t*,instruction_t**);
void increment(cpu_t*,instruction_t*);
void execute(cpu_t*,instruction_t*,uint8_t*);
void decode_unary_instruction(cpu_t*,instruction_t*);
void decode_branch_call_instruction(cpu_t*,instruction_t*);
void decode_trap_instruction(cpu_t*,instruction_t*);
void decode_stack_pointer_instruction(cpu_t*,instruction_t*);
void decode_add_sub_comp_instruction(cpu_t*,instruction_t*);
void decode_load_store_instruction(cpu_t*,instruction_t*);
void decode_char_in_out_instruction(cpu_t*,instruction_t*);

instruction_t create_new_instruction(uint16_t addr, const char* mnemonic,
				     uint8_t inst_spec,uint16_t op_spec,
				     uint8_t registr, uint8_t addr_mode,
				     _Bool unary);

#endif
