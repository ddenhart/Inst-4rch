/ Program : autoi.as
/ Desc : tests auto indexing
/-------------------------------------------
*0200        
Main, CLA
TAD CONST
DCA COUNT
TAD TTABLE
DCA INDEX
DCA I INDEX
ISZ COUNT
JMP .-2
HLT          / Halt program

*10     
INDEX, 0
*300
CONST, 7000
COUNT, 0
TTABLE, TABLE-1
*2000
TABLE, 0
$Main           / End of Program; Main is entry point