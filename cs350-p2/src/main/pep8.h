#ifndef __PEP8__
#define __PEP8__

typedef enum mnemonic {
    STOP, RETTR, MOVSPA, MOVFLGA, BR,
    BRLE, BRLT, BREQ, BRNE, BRGE, //10
    BRGT, BRV, BRC, CALL, NOTA,
    NOTX, NEGA, NEGX, ASLA, ASLX, //20
    ASRA, ASRX, ROLA, ROLX, RORA,
    RORX, NOP0, NOP1, NOP2, NOP3, //30
    NOP, DECI, DECO, STRO, CHARI,
    CHARO, RET0, RET1, RET2, RET3, //40
    RET4, RET5, RET6, RET7, ADDSP,
    SUBSP, ADDA, ADDX, SUBA, SUBX, //50
    ANDA, ANDX, ORA, ORX, CPA,
    CPX, LDA, LDX, LDBYTEA, LDBYTEX, //60
    STA, STX, STBYTEA, STBYTEX, ASCII_mnem,
    BLOCK_mnem, WORD_mnem //67
} mnemonic_t;

/* Helpful constants indicating the number of mnemonics and a special ID for
 * invalid mnemonics.
 */
#define NUMBER_OF_MNEMONICS (WORD_mnem + 1)
#define INVALID_MNEMONIC_ID -1

/* Global constants defined in pep8-const.c */
extern const char *MNEMONICS[];

mnemonic_t get_mnemonic_by_id(const char*);
#endif
