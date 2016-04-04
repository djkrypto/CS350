#ifndef __PROC_HELP__
#define __PROC_HELP__

#include "../main/pep8.h"	/*mnemonics */
#include "processor.h"		/*processor */
#include "interp.h"		/*cpu_t */

/*Prototypes*/
uint16_t flip_bits(uint16_t num);
void execute_stop();

void execute_arithmetic_and_logic_operators(cpu_t*,instruction_t*,uint8_t*);
  void execute_addr(cpu_t*,instruction_t*,uint8_t*);
  void execute_subr(cpu_t*,instruction_t*,uint8_t*);
  void execute_andr(cpu_t*,instruction_t*,uint8_t*);
  void execute_orr(cpu_t*,instruction_t*,uint8_t*);
  void execute_cpr(cpu_t*,instruction_t*,uint8_t*);
  void execute_notr(cpu_t*,instruction_t*,uint8_t*);
  void execute_negr(cpu_t*,instruction_t*,uint8_t*);
  
void execute_load_and_store(cpu_t*,instruction_t*,uint8_t*);
  void execute_ldr(cpu_t*,instruction_t*,uint8_t*);
  void execute_ldbyter(cpu_t*,instruction_t*,uint8_t*);
  void execute_str(cpu_t*,instruction_t*,uint8_t*);
  void execute_stbyter(cpu_t*,instruction_t*,uint8_t*);

void execute_output(cpu_t*,instruction_t*,uint8_t*);
  void execute_deco(cpu_t*,instruction_t*,uint8_t*);
  void execute_charo(cpu_t*,instruction_t*,uint8_t*);


void execute_branches(cpu_t*,instruction_t*,uint8_t*);
  void execute_br(cpu_t*,instruction_t*,uint8_t*);
  void execute_brle(cpu_t*,instruction_t*,uint8_t*);
  void execute_brlt(cpu_t*,instruction_t*,uint8_t*);
  void execute_breq(cpu_t*,instruction_t*,uint8_t*);
  void execute_brne(cpu_t*,instruction_t*,uint8_t*);
  void execute_brge(cpu_t*,instruction_t*,uint8_t*);
  void execute_brgt(cpu_t*,instruction_t*,uint8_t*);

#if 0
//if you have time:

void execute_shift_or_rotate();
  void execute_aslr();
  void execute_asrr();
  void execute_rolr();
  void execute_rorr();

#endif

#endif
