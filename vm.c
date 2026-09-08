/*
Assignment:
vm - HW1 PM/0 virtual machine
Author: <Leonel Hernandez>
Language: C only
To Compile:
gcc -Wall -Wextra -std=c11 -O2 vm.c -o vm
To Execute (on Eustis):
./vm <input_file>
where:
<input_file> is the path to a text file holding one PM/0 instruction
per line, as three integers OP L M
Notes:
- Implements the PM/0 virtual machine described in the homework
instructions.
- No heap allocation and no function-like macros. The PAS array is
indexed, not walked with a pointer.
- Does not implement any VM instruction as a separate function; the
fetch-execute cycle runs directly in main.
13
- Defines at most three functions: main, base, and one print function.
- Exits with status 0 after a normal halt and with a non-zero status
after any error.
- Tested on Eustis.
Class: COP 3402 - Systems Software
Instructor: Jie Lin, Ph.D.
Due Date: 9/18/26
*/
#define PAS_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
//Intialize Functions
int base(int bp, int L);
//Initialize Memory and Register Values
int pas[PAS_SIZE];
int pc = 200;
int bp = 999;
int sp = 1000;
FILE* ifp = fopen(argv[1], "r");
if (ifp == NULL)
    return 1;


fclose(ifp);
    return 0;
}

int base(int* pas, int bp, int L)
{
int arb = bp;
while (L > 0) {
arb = pas[arb];
L--;
}
return arb;
}