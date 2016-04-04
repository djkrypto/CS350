# -*- perl -*-
use strict;
use warnings;
use tests::tests;
check_expected ([<<'EOF']);
Symbol file "../symlist_fail2.txt" is illegally formatted
EOF
pass;
