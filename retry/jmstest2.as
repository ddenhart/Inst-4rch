/ Program : jmstest2.as
/ Desc : tests jumping into a subroutine
/-------------------------------------------
*0200          
Main, TAD A
DCA .+3
TAD B
JMS I 30
0
DCA PRDUCT
HLT          / Halt program

PRDUCT, 0000
A, 0051
B, 0027
*30 MULT
HLT          / Halt program

*600          
MULT, 0
CIA
DCA MTALLY
TAD I MULT
ISZ MTALLY
JMP .-2
ISZ MULT
JMP I MULT
MTALLY, 0
$Main           / End of Program; Main is entry point