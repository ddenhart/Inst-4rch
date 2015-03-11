/ Program : indirect.as
/ Date : March 11, 2015
/
/ Desc : Stores an indirect address
/
/-------------------------------------------
/
/ Code Section
/
*0200			/ start at address 0200
Main, 	cla cll 	/ clear AC and Link
	jmp I 100 		/ indirect jump call
	dca 50 		/ store 50 at 300
	hlt 		/ Halt program
	jmp Main	/ To continue - goto Main
/
/ Data Section
/
*100 			/ place data at address 100
A, 	300 		/ store 300
*300
C, 	0
$Main 			/ End of Program; Main is entry point
