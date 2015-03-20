/ Program : jmstest.as
/ Desc : tests jumping into a subroutine
/-------------------------------------------
/
/ Code Section
/
*0200          
Main, CLA CLL   / clear AC and Link
TAD N
JMS DOUBLE
DCA TWON
HLT          / Halt program

DOUBLE, 0000
CLL RAL
SNL
JMP I DOUBLE
RAR
HLT          / Halt program
/
/ Data Section
/
*0150           / place data at address 0150
N, 4000
TWON, 0          / displacement
$Main           / End of Program; Main is entry point