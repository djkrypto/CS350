# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);

--------------------------------------
Addr  Code   Symbol  Mnemonic  Operand
--------------------------------------
0000  C10011         LDA       word1,d
0003  710013         ADDA      word2,d
0006  A10015         ORA       word3,d
0009  F10010         STBYTEA   thing,d
000C  510010         CHARO     thing,d
000F  00             STOP      
0010  00     thing:  .BLOCK    1
0011  0005   word1:  .WORD     0x0005
0013  0003   word2:  .WORD     0x0003
0015  0030   word3:  .WORD     0x0030


------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0000
Index Register (X)          0x0000
Program counter (PC)        0x0003
Instruction register (IR)   0xC10011
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0005
Index Register (X)          0x0000
Program counter (PC)        0x0006
Instruction register (IR)   0x710013
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0008
Index Register (X)          0x0000
Program counter (PC)        0x0009
Instruction register (IR)   0xA10015
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0038
Index Register (X)          0x0000
Program counter (PC)        0x000C
Instruction register (IR)   0xF10010
------------------------------------
  Mem[0010] <-- 0x0038
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0038
Index Register (X)          0x0000
Program counter (PC)        0x000F
Instruction register (IR)   0x510010
------------------------------------
  Output '8'
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0038
Index Register (X)          0x0000
Program counter (PC)        0x0010
Instruction register (IR)   0x003800
------------------------------------
EOF
pass;
