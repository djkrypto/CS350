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


EOF
pass;
