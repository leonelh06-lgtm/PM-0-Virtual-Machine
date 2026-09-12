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
int base(int pas[], int bp, int L);
int main(int argc, char* argv[]){
    //Intialize Functions
    //Initialize Memory and pc
    int pas[PAS_SIZE];
    int instCount = 0;
    int pc = 200;
    int halt = 0;

    FILE* ifp = fopen(argv[1], "r");
    if (ifp == NULL){
        return 1;
    }

    //Loads input file into memory
    while(fscanf(ifp, "%d %d %d", &pas[pc], &pas[pc + 1], &pas[pc + 2]) == 3){
        pc+= 3;
        instCount++;
    }

    //Maximum adress that the stack can reach (Due to limitations from the text segment)
    int stackSpaceMax = 200 + (instCount * 3);

    //Checks if the program is too large for the text segment
    if(instCount > 266){
        printf("\nError: program too large for the text segment\n");
        return 1;
    }

    //Intialize register values
    pc = 200;
    int bp = 999;
    int sp = 1000;

    while(halt == 0){


        //Take initial instructions from file
        int op = pas[pc];
        int l = pas[pc + 1];
        int m = pas[pc + 2];

        //Conditional statements for each instruction

        if(op >9 || op < 1){
            printf("\nError: unknown opcode\n");
            return 1;
        }
        //Increment Program Counter
        pc += 3;

        //LIT(Literal) (Push M onto the stack)
        if(op == 1){
            sp--;
            if(sp < stackSpaceMax){
                printf("\nError: stack overflow\n");
                return 1;
            }
            pas[sp] = m;
        }

        //OPR(Operation) (Performs sub operation depending on M)
        //For arithmetic operations, the top two elements of the stack are popped and the result is pushed back onto the stack(a = pas[sp], b = pas[sp + 1])
        if(op == 2){

            if(m > 9 || m < 0){
                printf("\nError: unknown OPR sub-operation\n");
                return 1;
            }

            //RTN(Return) Returns procedure and restores callers record
            if(m == 0){
                sp = bp +1;
                bp = pas[sp-2];
                pc = pas[sp-3];
            }


            //ADD(Addition) Adds the top two elements of the stack and pushes the result back onto the stack
            if(m == 1){
                pas[sp+1] = pas[sp+1] + pas[sp];
                sp++;
            }

            //SUB(Subtraction) Subtracts the top two elements of the stack and pushes the result back onto the stack
            if(m == 2){
                pas[sp+1] = pas[sp+1] - pas[sp];
                sp++;
            }
            
            //MUL(Multiplication) Multiplies the top two elements of the stack and pushes the result back onto the stack
            if(m == 3){
                pas[sp+1] = pas[sp+1] * pas[sp];
                sp++;
            }

            //DIV(Division) Divides the top two elements of the stack and pushes the result back onto the stack
            if(m == 4){
                //Division by zero check
                if(pas[sp] == 0){
                    printf("\nError: division by zero\n");
                    return 1;
                }
                pas[sp+1] = pas[sp+1] / pas[sp];
                sp++;
            }

            //EQL(Equality) Compares the top two elements of the stack and pushes 1 if they are equal, 0 otherwise
            if(m == 5){
                if(pas[sp+1] == pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }

            //NEQ(Not Equal) Compares the top two elements of the stack and pushes 1 if they are not equal, 0 otherwise
            if(m ==6){
                if(pas[sp+1] != pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }

            //LSS(Less Than) Compares the top two elements of the stack and pushes 1 if the second element is less than the first, 0 otherwise
            if(m == 7){
                if(pas[sp+1] < pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }

            //LEQ(Less Than or Equal) Compares the top two elements of the stack and pushes 1 if the second element is less than or equal to the first, 0 otherwise
            if(m ==8){
                if(pas[sp+1] <= pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }
                
            //GTR(Greater Than) Compares the top two elements of the stack and pushes 1 if the second element is greater than the first, 0 otherwise
            if(m == 9){
                if(pas[sp+1] > pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }

            //GEQ(Greater Than or Equal) Compares the top two elements of the stack and pushes 1 if the second element is greater than or equal to the first, 0 othwerwise
            if(m == 10){
                if(pas[sp+1] >= pas[sp]){
                    pas[sp+1] = 1;
                } else {
                    pas[sp+1] = 0;
                }
                sp++;
            }


        }

        //LOD(Load) Pushes the value stored at base(BP, L) - M onto the stack
        if(op == 3){
        if(sp < stackSpaceMax){
            printf("\nError: stack overflow\n");
            return 1;
            }
        
        int target = base(pas, bp, l) - m;

        if(target < sp || target > 999){
            printf("\nError: data address out of range\n");
            return 1;
        }
        sp--;
        pas[sp] = pas[target];
        }

        //STO(Store) Pops value stored at the top of the stack and stores it in the variable at base(BP, L) - M
        if(op == 4){
            int target = base(pas, bp, l) - m;
            if(target < sp || target > 999){
                printf("\nError: data address out of range\n");
                return 1;
        }
            pas[target] = pas[sp];
            sp++;
        }

        //CAL(Call) Builds activation record and call the procedure at adress M
        if(op == 5){
            pas[sp-1] = base(pas, bp, l); //(Static Link) Always resolves lexiographic level
            pas[sp-2] = bp; //(Dynamic Link) Follows the current activation record
            pas[sp-3] = pc; //(Return Address) Stores where to return after the function is done running
            bp = sp - 1; //(Activation Record) Sets the base pointer to the top of the stack to create a new frame for the function
            pc = m; //(Jump) Goes to the beggining of the function to start executing it
        }

        //INC (Increment) Allocates M words on the stack
        if(op == 6){
            sp = sp - m;
            if(sp < stackSpaceMax){
                printf("\nError: stack overflow\n");
                return 1;
            }
        }

        //JMP(Jump) Jumps to the address M
        if(op == 7){
            pc = m; //Sets next instruction to adress M
        }

        //JPC(Jump Conditional) Pop the top of the stack and if 0 jump to the address M
        if(op == 8){
            if(pas[sp] == 0){
                pc = m; //Sets next instruction to adress M
            }
            sp++;
        }

        //SYS(System) Performs the system operation selected by M
        if(op == 9){
            if(m > 3 || m < 1){
                printf("\nError: unknown SYS operation\n");
                return 1;
            }

            //Write pop and print the top of the stack
            if(m == 1){
                printf("%d\n", pas[sp]);
                sp++;
            }
            
            //Read pushes prompted user input to top of the stack
            if(m == 2){
                int input;
                printf("Enter an Integer: ");
                scanf("%d", &input);
                sp--;
                if(sp < stackSpaceMax){
                    printf("\nError: stack overflow\n");
                    return 1;
            }
                pas[sp] = input;
            }

            //Halt stops the program and exits with status 0
            if(m == 3){
                halt = 1;
            }
        }

    }  

    fclose(ifp);
    return 0;
}

//Scope Resolver that returns base of the Lth lexiographic Level
int base(int pas[], int bp, int L){
int arb = bp;
while (L > 0) {
    arb = pas[arb];
    L--;
}
return arb;
}