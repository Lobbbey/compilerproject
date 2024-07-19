//Joshua Bandy

//Includes
#include<stdio.h>
#include<stdlib.h>

//Defines
#define ARRAY_SIZE 500

//Global Declarations
int pas[ARRAY_SIZE];

//Structs
typedef struct Instruction Instruction;

struct Instruction{
    int op;
    int L;
    int M;
};

//Function prototypes
int base(int BP, int L);
void print(Instruction In, int pc, int bp, int sp);

//Functions
int base(int BP, int L){
    int arb = BP;  //arb = activation record base
    while( L > 0){ //find base L levels down
        arb = pas[arb];
        L--;
    }
    return arb;
}

void print(Instruction In, int pc, int bp, int sp){
    switch (In.op){//Print what command was used based off In.op
    case 1://LIT
        printf("  LIT %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 2://RTN
        switch (In.M){// Arithmetic/Logical Instructions
        case 0:
            printf("  RTN %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 1: // ADD
            printf("  ADD %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 2: // SUB
            printf("  SUB %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 3: // MUL
            printf("  MUL %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 4: // DIV
            printf("  DIV %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 5: // EQL
            printf("  EQL %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 6: // NEQ
            printf("  NEQ %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 7: // LSS
            printf("  LSS %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 8: // LEQ
            printf("  LEQ %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 9: // GTR
            printf("  GTR %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        case 10: // GEQ
            printf("  GEQ %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
            break;
        default:
            break;
        }
        break;
    case 3://LOD
        printf("  LOD %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 4://STO
        printf("  STO %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 5://CAL
        printf("  CAL %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 6://INC
        printf("  INC %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 7://JMP
        printf("  JMP %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 8://JPC
        printf("  JPC %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    case 9://SYS
        printf("  SYS %d %d \t %d  %d %d ", In.L, In.M, pc, bp, sp);
        break;
    default:
        break;
    }
    
    //Used to print the stack
    for(int i = ARRAY_SIZE - 1; i >= sp; i--){
        printf("%d ", pas[i]);
        if((i - 1) == bp && bp != ARRAY_SIZE - 1){//Print | for AR but not at start
            printf("| ");
        }
    }
    

    printf("\n");//Final new line
}

int main(int argc, char **argv){
    //Declarations
    int index = 10, pc = 10, bp = 499, sp = 500, Run = 1;
    char input;
    Instruction In;//create Instruction and initalize to 0
    In.op = 0;
    In.L = 0;
    In.M = 0;
    char* fname = argv[1];//take input from cmd line for file
    FILE *file = fopen(fname, "r");//OPen file

    //Initialize the pas array to 0
    for(int i = 0; i < ARRAY_SIZE; i++){
        pas[i] = 0;
    }

    // Read values from file
    while(!feof(file)){
        fscanf(file, "%d", &pas[index]);
        index++;
    }
    
    //Print starting values
    printf("\t\t PC  BP  SP  stack \n");
    printf("Initial values:  %d  %d %d \n\n", pc, bp, sp);

    // Run until we Fetch a stop
    while(Run == 1){

        //Fetch
        In.op = pas[pc];
        In.L = pas[pc + 1];
        In.M = pas[pc + 2];
        pc = pc + 3;

        // Exec
        switch (In.op){
        case 1: // LIT
            sp = sp - 1;
            pas[sp] = In.M;
            break;
        case 2: // RTN
            switch (In.M){//Arithmetic/Logical Instructions
            case 0:// Restore the caller's AR
                sp = bp + 1;
                bp = pas[sp - 2];
                pc = pas[sp - 3];
                break;
            case 1:// ADD
                pas[sp + 1] = pas[sp + 1] + pas[sp];
                sp = sp + 1;
                break;
            case 2:// SUB
                pas[sp + 1] = pas[sp + 1] - pas[sp];
                sp = sp + 1;
                break;
            case 3:// MUL
                pas[sp + 1] = pas[sp + 1] * pas[sp];
                sp = sp + 1;
                break;
            case 4:// DIV
                pas[sp + 1] = pas[sp + 1] / pas[sp];
                sp = sp + 1;
                break;
            case 5:// EQL
                pas[sp + 1] = pas[sp + 1] == pas[sp];
                sp = sp + 1;
                break;
            case 6:// NEQ
                pas[sp + 1] = pas[sp + 1] != pas[sp];
                sp = sp + 1;
                break;
            case 7:// LSS
                pas[sp + 1] = pas[sp + 1] < pas[sp];
                sp = sp + 1;
                break;
            case 8:// LEQ
                pas[sp + 1] = pas[sp + 1] <= pas[sp];
                sp = sp + 1;
                break;
            case 9:// GTR
                pas[sp + 1] = pas[sp + 1] > pas[sp];
                sp = sp + 1;
                break;
            case 10:// GEQ
                pas[sp + 1] = pas[sp + 1] >= pas[sp];
                sp = sp + 1;
                break;
            default:
                break;
            }
            break;
        case 3: // LOD
            sp = sp - 1;
            pas[sp] = pas[base(bp, In.L) - In.M];
            break;
        case 4: // STO
            pas[base(bp, In.L) - In.M] = pas[sp];
            sp = sp + 1;
            break;
        case 5: // Cal
            pas[sp - 1] = base(bp, In.L);
            pas[sp - 2] = bp;
            pas[sp - 3] = pc;
            bp = sp - 1;
            pc = In.M;
            break;
        case 6: // INC
            sp = sp - In.M;
            break;
        case 7: // JMP
            pc = In.M;
            break;
        case 8: // JPC
            if(pas[sp] == 0){
                pc = In.M;
            }
            sp = sp + 1;
            break;
        case 9: // SYS
            switch(In.M){
                case 1:
                    printf("Output result is: ");
                    printf("%d\n", pas[sp]);
                    sp = sp + 1;
                    break;
                case 2://Read an int then push to stack
                    sp = sp - 1;
                    printf("Please enter an integer: ");
                    input = getc(stdin);
                    pas[sp] = input - 48;
                    break;
                case 3://set Run to 0 for EOP
                    Run = 0;
                    break;
            }
            break;
        }
        print(In, pc, bp, sp);
    }

    return 0;
}
