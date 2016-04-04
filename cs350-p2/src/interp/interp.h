#ifndef __INTERP__
#define __INTERP__

typedef struct cpu {
    uint32_t inst_reg;// instruction register
    uint16_t accum; //accumulator
    uint16_t x; //index register
    uint16_t pc; //program counter
    _Bool n; //n-bit, 1/true if the result is negative
    _Bool z; //z-bit, 1/true if the result is all zeros
    _Bool v; //v-bit, 1/true if a signed integer overflow occurs
    _Bool c; //c bit, 1/true if an unsigned integer overflow occurs
} cpu_t;

/*Prototypes*/
void interpret_memory(uint8_t*,cpu_t*,uint16_t);
void preset_cpu(cpu_t*);


#endif
