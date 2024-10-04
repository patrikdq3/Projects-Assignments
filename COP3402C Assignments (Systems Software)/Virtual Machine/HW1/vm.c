/*=============================================================================
| Assignment: vm.c - Implemented Virtual P-Machine
|
| Authors: 
| Alexander Lokhanov
| Patrik De Quevedo
|
| Language: ANSI standard C
|
| To Compile: gcc -o vm vm.c -Wall
|
| To Execute: c -> ./vm input.txt
| where input.txt is the input file name.
|
|
| Class: COP3402 - System Security - Spring 2024
| Instructor: Montagne
| Due Date: 2/2/2024
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>

typedef struct instruction{
        int op;
        int l;
        int m;
    } instruction; 

instruction IR;
//AR HELP
int bars[10] = {0};


int pas[500];
int EOP = 1;
int base( int BP, int L);
int num = 0; //User Input

// Initial CPU register values (taken from example in Appendix C)
int SP = 500;
int BP = 499;
int PC = 0;
int halt = 0;


int main(int argc, char *argv[]) {
    // Open the input file (with check)
    FILE *inputfile = fopen(argv[1], "r");
    if (inputfile == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    int OP, l, m;
    int l1count = 0; //loop 1 count

    // Fetching instructions from input file and placing them in the pas (linear) array
    while(fscanf(inputfile, "%d %d %d", &OP, &l, &m) == 3) {
        pas[l1count*3] = OP;
        pas[l1count*3+1] = l;
        pas[l1count*3+2] = m;
        l1count++;
    }
    fclose(inputfile);

    // Print out instruction set from PAS (test)
    // for (int i = 0; i<l1count; i++){
    //     printf("\n%d %d %d", pas[i*3], pas[i*3+1], pas[i*3+2]);
    // }
    // printf("\n");

    // Fetch cycle:
    // instruction pointed by PC is fetched from the “text” segment of pas, 
    // placed in the instruction register (IR) and the PC is incremented to point to the next instruction in the code list. 
    // In the second step the instruction in the IR is executed using the “stack” segment. 
    // (This does not mean that the instruction is stored in the “stack segment.”)
    // Fetch cycle (from pas[text] to IR struct)

    int ARCOUNTER = 0;
    int ARINDEX = 0;
    int i = 0;

    // Printing Initial Values
    printf("\t\tPC\tBP\tSP\tStack\n");
    printf("Initial values:\t%d\t%d\t%d\n\n", PC, BP, SP);

    while(EOP != 0){
        i++;
        IR.op = pas[PC];
        IR.l = pas[PC+1];
        IR.m = pas[PC+2];
        PC += 3;

        switch(IR.op) {
            case 1: // LIT
            printf("  LIT");
            SP = SP - 1;
            pas[SP] = IR.m; //swapped the lines from pseudo code
            break;

            case 2:
            switch(IR.m){
                case 0: // RTN
                    printf("  RTN");
                    SP = BP + 1;
                    BP = pas[SP-2];
                    PC = pas[SP-3];
                    if(ARCOUNTER > 0){ // Decrement pointer of AR
                        ARCOUNTER--;
                        bars[ARCOUNTER] = 0;
                    }
                break;

                case 1: //ADD
                    printf("  ADD");
                    pas[SP+1] = (pas[SP] + pas[SP+1]);
                    SP=SP+1;
                break;

                case 2: // SUB
                    printf("  SUB");
                    pas[SP+1] = (pas[SP+1] - pas[SP]);
                    SP=SP+1;
                break;

                case 3: // MUL
                    printf("  MUL");
                    pas[SP+1] = (pas[SP+1] * pas[SP]);
                    SP=SP+1;
                break;

                case 4: // DIV
                    printf("  DIV");
                    pas[SP+1] = (pas[SP+1] / pas[SP]);
                    SP=SP+1;
                break;

                case 5: // EQL
                    printf("  EQL");
                    pas[SP+1] = (pas[SP+1] == pas[SP]);
                    SP=SP+1;
                break;

                case 6: // NEQ
                    printf("  NEQ");
                    pas[SP+1] = (pas[SP+1]!=pas[SP]);
                    SP = SP+1;
                break;

                case 7: // LSS
                    printf("  LSS");
                    pas[SP+1] = (pas[SP+1]<pas[SP]);
                    SP=SP+1;
                break;

                case 8: // LEQ
                    printf("  LEQ");
                    pas[SP+1] = (pas[SP+1]<= pas[SP]);
                    SP=SP+1;
                break;

                case 9: // GTR
                    printf("  GTR");
                    pas[SP+1] = (pas[SP+1]>pas[SP]);
                    SP=SP+1;
                break;

                case 10: // GEQ
                    printf("  GEQ");
                    pas[SP+1] = (pas[SP+1]>=pas[SP]);
                    SP=SP+1;
                break;

                case 11: // ODD
                    printf("  ODD");
                    pas[SP] = (pas[SP]%2);
                break;
            
            }//switch IR.m
            break;
           
            case 3: // LOD
                printf("  LOD");
                SP = SP - 1;
                pas[SP] = pas[base(BP,IR.l)-IR.m];
                break;

            case 4: // STO
                printf("  STO");
                pas[base(BP,IR.l)-IR.m] = pas[SP];
                SP = SP+1;
                break;

            case 5: // CAL
                printf("  CAL");
                pas[SP-1] = base(BP,IR.l); // SL
                pas[SP-2] = BP; // BP
                pas[SP-3] = PC; // RA
                BP = SP -1;
                PC = IR.m;
                //AR STUFF
                bars[ARCOUNTER] = SP;
                ARCOUNTER++;
                //printf("\nARCOUNTER = %d\n", ARCOUNTER);
                break;

            case 6: // INC
                printf("  INC");
                SP = SP - IR.m;
                break;

            case 7: // JMP
                printf("  JMP");
                PC = IR.m;
                break;
            
            case 8: // JPC
                printf("  JPC");
                if (pas[SP]== 0){
                    PC = IR.m;
                    SP = SP + 1;
                    break;
                }
                SP = SP +1;
                break;

            case 9: // SYS
                        switch(IR.m){
                            case 1: // SYS 1 : Write the top stack element to the screen
                                printf("Output is: %d\n",pas[SP]);
                                SP = SP +1;
                                printf("  SOU");
                            break;

                            case 2: // SYS 2 : Read in input from the user and store it on top of the stack    
                                printf("Please enter an Integer\n");
                                scanf("%d", &num);
                                SP = SP -1;
                                pas[SP] = num;
                                printf("  SIN");
                            break;

                            case 3: // SYS 3 : End of program (Set “eop” flag to zero)
                                EOP = 0;
                                printf("  EOP");
                            break;

                        }// case 9 switch IR.l
  
        }

        // doing IDX>=SP bc it works and IDX>SP doesnt..
        // PRINTS EVERY LINE
        printf(" %d %d \t%d\t%d\t%d\t", IR.l, IR.m, PC, BP, SP);
        for(int idx = 499; idx>=SP; idx--){
            printf("%d ", pas[idx]);
            for(int j = 0; j<=ARINDEX; j++){
                if((bars[j]== idx)&&(idx!=SP)){
                    printf("| ");
                    ARINDEX++;
                }
            }
        }
        
        printf("\n");
        
    }

    return 0;
}//MAIN

/**********************************************/
/* Find base L levels down  */
/*                          */
/**********************************************/

int base( int BP, int L) {
    int arb = BP; // arb = activation record base 
    while ( L > 0) //find base L levels down
    {
    arb = pas[arb];
    L--; }
    return arb; 
}