/ Program : or.as
/ Desc : This program computes C = A | B
/        where B is indirect for D
/-------------------------------------------
/
/ Code Section
/
*0200           / start at address 0200
Main, cla cll   / clear AC and Link
   TAD A        / load A to the alu
   CMA          / complement alu
   DCA C        / store result to C
   TAD I B      / load indirect B (D) to alu
   CMA          / complement it
   AND C        / and it with C
   CMA          / complement result
   DCA C        / store result to C
   HLT          / Halt program
   JMP Main     / To continue - goto Main
/
/ Data Section
/
*0310           / place data at address 0310
A, 25           / A equals 25
B, 313          / B equals the indirect address for D
C, 0            / C will hold the saved result
D, 45           / D equals 45
$Main           / End of Program; Main is entry point
