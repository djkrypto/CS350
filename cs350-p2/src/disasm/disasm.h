#ifndef __DISASM__
#define __DISASM__

#include "../main/pep8.h" /* mnemonic */

typedef enum { LINE, ASCII, BLOCK, WORD } symtype_t;

#define NUMBER_OF_SYMTYPES (WORD + 1)
#define INVALID_SYMTYPE_ID -1

typedef struct symtab {
    char *label;
    symtype_t type;
    off_t offset;
    size_t block_length;
    struct symtab *next;
} symtab_t;

//define "Does not exist" variable for registr and addr_mode
#define DNE 255

typedef struct instruction_t {
    uint16_t addr;
    struct symtab* symb;
    uint8_t inst_spec; //instruction specifier
    uint8_t registr; //DNE if does not apply to this instruction
    uint8_t addr_mode; //DNE if does not apply to this instruction
    mnemonic_t mnem;
    uint16_t op_spec; //if unary is true, op_spec arbitrarily set to DNE
    _Bool unary; //unary means no op-spec
    uint16_t ascii_bytes; //number of bytes in ascii string
    _Bool single_digit_addressing;
	//for branch and call instructions for "i,x"; preset to false
    struct instruction_t *next;
} instruction_t;

/* Global constants defined in disasm-const.c */
extern const char *MNEMONICS[];

/*Prototypes*/
void determine_instructions(instruction_t**,uint8_t*,int, symtab_t**);
uint16_t determine_symbol_instruction(uint8_t*,uint8_t,uint16_t,
                                      instruction_t*, symtab_t*);
void determine_unary_instruction(uint8_t*,uint8_t,uint16_t,instruction_t*,
				 symtab_t*);
void determine_branch_call_instruction(uint8_t*,uint8_t,uint16_t,
				       instruction_t*, symtab_t*);
void determine_trap_instruction(uint8_t*,uint8_t,uint16_t,
				instruction_t*, symtab_t*);
void determine_stack_pointer_instruction(uint8_t*,uint8_t,uint16_t,
					 instruction_t*, symtab_t*);
void determine_add_sub_comp_instruction(uint8_t*,uint8_t,uint16_t,
					instruction_t*, symtab_t*);
void determine_load_store_instruction(uint8_t*,uint8_t,uint16_t,
				      instruction_t*, symtab_t*);
void determine_char_in_out_instruction(uint8_t*,uint8_t,uint16_t,
				       instruction_t*, symtab_t*);

mnemonic_t get_mnemonic_by_id (const char*);
instruction_t create_instruction(uint16_t, const char*, uint8_t,
				 uint16_t, uint8_t, uint8_t,
				 symtab_t*,_Bool);

#endif
