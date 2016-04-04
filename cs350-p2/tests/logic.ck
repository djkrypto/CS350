# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);

--------------------------------------
Addr  Code   Symbol  Mnemonic  Operand
--------------------------------------
0000  710005         ADDA      0x0005,d
0003  700005         ADDA      0x0005,i
0006  790005         ADDX      0x0005,d
0009  780005         ADDX      0x0005,i
000C  810005         SUBA      0x0005,d
000F  810005         SUBA      0x0005,d
0012  890005         SUBX      0x0005,d
0015  890005         SUBX      0x0005,d
0018  910005         ANDA      0x0005,d
001B  910005         ANDA      0x0005,d
001E  990005         ANDX      0x0005,d
0021  990005         ANDX      0x0005,d
0024  A10005         ORA       0x0005,d
0027  A10005         ORA       0x0005,d
002A  A90005         ORX       0x0005,d
002D  A90005         ORX       0x0005,d
0030  B10005         CPA       0x0005,d
0033  B10005         CPA       0x0005,d
0036  B90005         CPX       0x0005,d
0039  B90005         CPX       0x0005,d
003C  18             NOTA      
003D  19             NOTX      
003E  1A             NEGA      
003F  1B             NEGX      
0040  00             STOP      


------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0000
Index Register (X)          0x0000
Program counter (PC)        0x0003
Instruction register (IR)   0x710005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x0000
Program counter (PC)        0x0006
Instruction register (IR)   0x700005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x057E
Index Register (X)          0x0000
Program counter (PC)        0x0009
Instruction register (IR)   0x790005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x057E
Index Register (X)          0x0579
Program counter (PC)        0x000C
Instruction register (IR)   0x780005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x057E
Index Register (X)          0x057E
Program counter (PC)        0x000F
Instruction register (IR)   0x810005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0005
Index Register (X)          0x057E
Program counter (PC)        0x0012
Instruction register (IR)   0x810005
------------------------------------
Status bits (NZVC)          1 0 0 0 
Accumulator (A)             0xFA8C
Index Register (X)          0x057E
Program counter (PC)        0x0015
Instruction register (IR)   0x890005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0xFA8C
Index Register (X)          0x0005
Program counter (PC)        0x0018
Instruction register (IR)   0x890005
------------------------------------
Status bits (NZVC)          1 0 0 0 
Accumulator (A)             0xFA8C
Index Register (X)          0xFA8C
Program counter (PC)        0x001B
Instruction register (IR)   0x910005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0008
Index Register (X)          0xFA8C
Program counter (PC)        0x001E
Instruction register (IR)   0x910005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0008
Index Register (X)          0xFA8C
Program counter (PC)        0x0021
Instruction register (IR)   0x990005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0008
Index Register (X)          0x0008
Program counter (PC)        0x0024
Instruction register (IR)   0x990005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0008
Index Register (X)          0x0008
Program counter (PC)        0x0027
Instruction register (IR)   0xA10005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x0008
Program counter (PC)        0x002A
Instruction register (IR)   0xA10005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x0008
Program counter (PC)        0x002D
Instruction register (IR)   0xA90005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x0030
Instruction register (IR)   0xA90005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x0033
Instruction register (IR)   0xB10005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x0036
Instruction register (IR)   0xB10005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x0039
Instruction register (IR)   0xB90005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x003C
Instruction register (IR)   0xB90005
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x003D
Instruction register (IR)   0x18191A
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0xFA86
Index Register (X)          0x000D
Program counter (PC)        0x003E
Instruction register (IR)   0x191A1B
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0xFA86
Index Register (X)          0xFFF2
Program counter (PC)        0x003F
Instruction register (IR)   0x1A1B00
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0xFFF2
Program counter (PC)        0x0040
Instruction register (IR)   0x1B0000
------------------------------------
Status bits (NZVC)          0 0 0 0 
Accumulator (A)             0x0579
Index Register (X)          0x000D
Program counter (PC)        0x0041
Instruction register (IR)   0x000000
------------------------------------
EOF
pass;
