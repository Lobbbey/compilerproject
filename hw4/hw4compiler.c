// Joshua Bandy

// Includes
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Defines
#define MAX_IDENTIFIERS 11
#define MAX_DIGIT 5
#define MAX_SYMBOL_TABLE_SIZE 500
#define ARRAY_SIZE 500

// Structs
typedef struct assembly assembly;
typedef struct symbol symbol;
typedef struct lexemeInfo lexemeInfo;

struct assembly{
    int op;
    int L;
    int M;
};

struct symbol{
    int kind;      // const = 1, var = 2, proc = 3
    char name[10]; // name up to 11 chars
    int val;       // number (ASCII value)
    int level;     // L level
    int addr;      // M address
    int mark;      // to indicate unavailable or deleted
};

struct lexemeInfo{
    char* tokenName;
    int tokenVal;
    int tableCount;
};

typedef enum{
    oddsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5, multsym = 6,
    slashsym = 7, fisym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,gtrsym = 13,
    geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
    periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
    whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29, procsym = 30,
    writesym = 31, readsym = 32, elsesym = 33
} token_type;

// Global Variables
char *reservedWords[] = {"const", "var", "begin",
                             "end", "if", "fi", "then",
                             "while", "do", "read", "write"};
int tableCount = 0, symbolTableCount = 0, assemblyCount = 0, tokenCount = 1, token;
lexemeInfo lexemeTable[ARRAY_SIZE];
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
assembly assemblyCode[ARRAY_SIZE];


// Function Prototypes
void letterHandler(FILE *source, char input);
void punctHandler(FILE *source, char input);
void numHandler(FILE *source, char input);
int findSymbolVal(char input);
int findTokenVal(char *inputHolder);
void getNextToken();
int symbolTableCheck();
void program();
void block();
void constDeclaration();
int varDeclaration();
void statement();
void condition();
void expression();//Modify it to match the grammar
void term();
void factor();
void emit(int op, int L, int M);
void printInfo();
void printTokenList();
void printError(int errorType);
void printSymbolTable();
void printassembly();

// Functions
void letterHandler(FILE *source, char input){
    char inputHolder[13];
    int index = 0;
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    while (!feof(source) && isalpha(input)){
        if (index > MAX_IDENTIFIERS){ // to many letters, Set error and leave
            printError(2);
            break;
        }
        inputHolder[index] = input;
        index++;
        input = fgetc(source);
    }

    // Store information in struct
    tableCount++;
    lexemeTable[tableCount].tokenVal = findTokenVal(inputHolder);
    lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (strlen(inputHolder) + 1));
    strcpy(lexemeTable[tableCount].tokenName, inputHolder);

    // check current input
    if (isalnum(input) != 0){
        numHandler(source, input);
    }
    else if (ispunct(input) != 0){
        punctHandler(source, input);
    }
}

void numHandler(FILE *source, char input){
    int index = 0;
    char inputHolder[13];
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    while (!feof(source) && isalnum(input)){
        if (index > MAX_DIGIT){ // to many digits, set error leave
            printError(1);
            break;
        }
        inputHolder[index] = input;
        index++;
        input = fgetc(source);
    }

    // Store information in struct
    tableCount++;
    lexemeTable[tableCount].tokenVal = 3;
    lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
    strcpy(lexemeTable[tableCount].tokenName, inputHolder);

    // check current input
    if (isalpha(input) != 0){
        numHandler(source, input);
    }
    else if (ispunct(input) != 0){
        punctHandler(source, input);
    }
}

void punctHandler(FILE *source, char input){
    char inputHolder[13];
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    if (input == '+' || input == '-' || input == '*' || input == '(' ||
        input == ')' || input == ',' || input == '.' || input == ';' || input == '='){
        // Store information in struct

        inputHolder[0] = input;
        tableCount++;
        lexemeTable[tableCount].tokenVal = findSymbolVal(input);
        lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
        strcpy(lexemeTable[tableCount].tokenName, inputHolder);
    }
    else if (input == '<'){
        inputHolder[0] = input;
        char tmpInput = inputHolder[0];
        input = fgetc(source);
        inputHolder[1] = input;

        // check next input
        if (strcmp(inputHolder, "<>") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 10;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
        else if (strcmp(inputHolder, "<=") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 12;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
        else
        {
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 11;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
    }
    else if (input == '>'){
        inputHolder[0] = input;
        input = fgetc(source);
        inputHolder[1] = input;

        if (strcmp(inputHolder, ">=") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 14;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
        else{
            // Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 13;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
    }
    else if (input == ':'){
        inputHolder[0] = input; // Store initial value
        input = fgetc(source);
        inputHolder[1] = input;

        if (strcmp(inputHolder, ":=") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 20;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
        else{ // Invalid symbol
            // Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 0;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
    }
    else if (input == '/'){
        inputHolder[0] = input; // Store initial value
        input = fgetc(source);
        inputHolder[1] = input;

        if (strcmp(inputHolder, "/*") == 0){ // Comments
            memset(inputHolder, 0, sizeof(inputHolder));

            while (!feof(source)){
                input = fgetc(source);

                if (input == '*'){
                    inputHolder[0] = input;
                    input = fgetc(source);
                    inputHolder[1] = input;

                    if (strcmp(inputHolder, "*/") == 0){
                        break;
                    }
                    memset(inputHolder, 0, sizeof(inputHolder));
                }
            }
        }
        else{ // slashsym
            // Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 7;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        }
    }
    else{
        // Store information in struct
        inputHolder[0] = input;
        tableCount++;
        lexemeTable[tableCount].tokenVal = 0;
        lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
        strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        printError(3);
    }

    // Check current input
    if (isalpha(input) != 0)
    { // current input is alpa
        letterHandler(source, input);
    }
    else if (isalnum(input) != 0)
    { // current input is num
        numHandler(source, input);
    }
}

int findSymbolVal(char input){
    if (input == '+'){
        return 4;
    }
    else if (input == '-'){
        return 5;
    }
    else if (input == '*'){
        return 6;
    }
    else if (input == '('){
        return 15;
    }
    else if (input == ')'){
        return 16;
    }
    else if (input == ','){
        return 17;
    }
    else if (input == '.'){
        return 19;
    }
    else if (input == ';'){
        return 18;
    }
    else if (input == '='){
        return 9;
    }
    else{
        return 0;
    }
}

int findTokenVal(char *inputHolder){
    int index = -1;

    // check reservered words
    for (int i = 0; i < 11; i++){
        if (strcmp(inputHolder, reservedWords[i]) == 0){
            index = i;
            break;
        }
    }

    // Return a value based on lexical
    if (index == -1){// Identsym
        return 2;
    }
    else if (strcmp(reservedWords[index], "const") == 0){
        return 28;
    }
    else if (strcmp(reservedWords[index], "var") == 0){
        return 29;
    }
    else if (strcmp(reservedWords[index], "begin") == 0){
        return 21;
    }
    else if (strcmp(reservedWords[index], "end") == 0){
        return 22;
    }
    else if (strcmp(reservedWords[index], "if") == 0){
        return 23;
    }
    else if (strcmp(reservedWords[index], "fi") == 0){
        return 8;
    }
    else if (strcmp(reservedWords[index], "then") == 0){
        return 24;
    }
    else if (strcmp(reservedWords[index], "while") == 0){
        return 25;
    }
    else if (strcmp(reservedWords[index], "read") == 0){
        return 32;
    }
    else if (strcmp(reservedWords[index], "write") == 0){
        return 31;
    }
}

void getNextToken(){
    // Ensure we don't get past the table count
   if(tokenCount > tableCount){
        token = -1;
    }
    else{
        // Set next value then increment
        token = lexemeTable[tokenCount].tokenVal;
        tokenCount++;
    }
}

int symbolTableCheck(char *name){
    for(int i = 0; i < symbolTableCount; i++){
        if(strcmp(name, symbolTable[i].name) == 0){
            return i;
        }
    }
    return -1;
}

void program(){
    // begen parsercodegen
    getNextToken();
    block();
    if (token != periodsym){
        printError(4);
    }
    emit(9, 0, 3); // Halt
}


void block(){
    constDeclaration();// Check for constant declaration
    int numVars = varDeclaration();// CHeck for variables declarations
    emit(6 , 0, 3 + numVars);//Store number of Variables
    /* while (token == procsym){
        getNextToken();
        if (token != identsym)
            printError();
        getNextToken();
        if (token != semicolonsym)
            printError();
        getNextToken();
        block();
        if (token != semicolonsym)
            printError();
    } */
    statement();// Begin checking statements
}

void constDeclaration(){// Handle Constant declarations
    if(token == constsym){
        do{
            getNextToken();
            if(token != identsym){
                printError(7);
            }
            if(symbolTableCheck(lexemeTable[tokenCount].tokenName) != -1){
                printError(6);
            }
            strcpy(symbolTable[symbolTableCount].name, lexemeTable[tokenCount - 1].tokenName); // Save variable name
            getNextToken();
            if (token != eqsym){
                printError(7);
            }
            getNextToken();
            if (token != numbersym){
                printError(8);
            }
            // Store Symbol in Symbol table
            symbolTable[symbolTableCount].kind = 1;
            symbolTable[symbolTableCount].val = token;
            symbolTable[symbolTableCount].addr = 0;
            symbolTable[symbolTableCount].mark = 0;
            symbolTableCount++;
            getNextToken();
        }while(token == commasym);
        if(token != semicolonsym){
            printError(9);// Const must end in a semicolon
        }
        getNextToken();
    }
}

int varDeclaration(){// Handles A variables declaration
    int numVars = 0;// Return number of declarations
    if(token == varsym){
        do{
            numVars++;// Increment variable declarations
            getNextToken();
            if(token != identsym){
                printError(7);
            }
            if(symbolTableCheck(lexemeTable[tableCount - 1].tokenName) != -1){
                printError(6);
            }
            // Store info in symbol table
            symbolTable[symbolTableCount].kind = 2;// Save Kind
            strcpy(symbolTable[symbolTableCount].name, lexemeTable[tokenCount - 1].tokenName); // Save variable name
            symbolTable[symbolTableCount].val =  0;
            symbolTable[symbolTableCount].addr = 2 + numVars;
            symbolTable[symbolTableCount].mark = 0;
            symbolTableCount++;
            getNextToken();
        }while(token == commasym);
        
        if(token != semicolonsym){
            printError(9);
        }
        getNextToken();
    }
    return numVars;
}

void statement(){
    if(token == identsym){
        int symIdx = symbolTableCheck(lexemeTable[tokenCount - 1].tokenName);
        if(symIdx == -1){
            printError(10);
        }
        if(symbolTable[symIdx].kind != 2){// Not a var
            printError(11);
        }
        getNextToken();
        if(token != becomessym){
            printError(12);
        }
        getNextToken();
        expression();
        emit(4, 0, symbolTable[symIdx].addr);// STO
        return;
    }
    if(token != callsym){
        getNextToken();
        if(token != identsym) printError(2);
        getNextToken();
    }
    if(token == beginsym){
        do{
            getNextToken();
            statement();
        }while(token == semicolonsym);
        
        if(token != endsym){
            printf("%s\n", lexemeTable[tokenCount].tokenName);
            printError(13);
        }
        getNextToken();
        return;
    }
    if(token == ifsym){
        getNextToken();
        condition();
        int jpcIdx = assemblyCount;
        emit(7, 0, jpcIdx); 
        if(token != thensym){
            printError(16);
        }
        statement();
        assemblyCode[assemblyCount].M = assemblyCount;
        return;
    }
    if(token == whilesym){
        getNextToken();
        int loopIdx = tokenCount;
        condition();
        if(token != dosym){
            printError(15);// While must be followed by do
        }
        getNextToken();
        int jpcIdx = tokenCount;
        emit(8,0,loopIdx);// JPC
        statement();
        emit(7, 0, loopIdx);// JMP
        assemblyCode[jpcIdx].M = tokenCount;
        return;
    }
    if(token == readsym){
        getNextToken();
        if(token != identsym){
            printError(5);
        }
        int symIdx = symbolTableCheck(lexemeTable[tokenCount - 1].tokenName);
        if (symIdx == -1){

            printError(10);
        }
        if (symbolTable[symIdx].kind != 2){ // Not a var
            printError(11);
        }
        getNextToken();
        expression();
        emit(9, 0, 2);// READ
        emit(4, 0, symbolTable[symIdx].addr);//STO
        return;
    }
    if(token == writesym){
        getNextToken();
        expression();
        emit(9, 0, 1); // WRITE
        return;
    }
}

void condition(){
    int relop;
    if(token == oddsym){
        getNextToken();
        expression();
        emit(10, 0, 1);// ODD
    }
    else{
        expression();
        if((token != eqsym) && (token != neqsym) && (token != lessym) && 
           (token != leqsym) && (token != gtrsym) && (token != geqsym)){
            // If symbol isn't a recognized symbol send error
            printError(3);
        }
        else{
            expression();
            relop = token;
            getNextToken();
            expression();
            switch (relop){
            case 9:
                emit(2, 0, 8);// equal
                break;
            case 10:
                emit(2, 0, 9); //Not equal
                break;
            case 11:
                emit(2, 0, 10); // <
                break;
            case 12:
                emit(2, 0, 11); // <=
                break;
            case 13:
                emit(2, 0, 12); // >
                break;
            case 14:
                emit(2, 0, 13); // >=
                break;
            default:
                break;
            }
        }
    }
}

void expression(){
    if(token == minussym){
        getNextToken();
        term();
        emit(2, 0, 2);// NEG
        while(token == plussym || token == minussym){
            if(token == plussym){
                getNextToken();
                term();
                emit(2,0,1);// ADD
            }
            else{
                getNextToken();
                term();
                emit(2,0,2);// SUB
            }
        }
    }
    else{
        if(token == plussym){
            getNextToken();
        }
        term();
        while(token == plussym || token == minussym){
            if(token == plussym){
                getNextToken();
                term();
                emit(2,0,1);// ADD
            }
            else{
                getNextToken();
                term();
                emit(2,0,2);// SUB
            }
        }
    }
}

void term(){//Check for mult & div
    factor();

    while(token == multsym || token == slashsym){//Keep storing until condition is met
        if(token == multsym){
            getNextToken();
            factor();
            emit(2, 0, 3); // multiplication
        }
        else if(token == slashsym){
            getNextToken();
            factor();
            emit(2, 0, 4);// Division
        }
    }
}

void factor(){
    if(token == identsym){//Token is an ident
        //Check if symbol is already in table
        int symIdx = symbolTableCheck(lexemeTable[tokenCount - 1].tokenName);
        if(symIdx == -1){
            printError(10);
        }
        if(symbolTable[symIdx].kind == 1){//Const
            emit(1, 0, symbolTable[symIdx].val);//LIT
        }
        else{
            emit(3, 0, symbolTable[symIdx].addr);//LOD
        }
        getNextToken();
    }
    else if(token == numbersym){// Token is a num
        emit(1, 0, token - 1);// LIT
        getNextToken();
    }
    else if(token == lparentsym){// Token == (
        getNextToken();
        expression();
        if(token != rparentsym){
            printError(17);            
        }
        getNextToken();
    }
    else{//Error found
        printError(18);
    }
}

void emit(int op, int L, int M){//Store the assembly code
    //Store passed in information
    assemblyCode[assemblyCount].op = op; // opcode
    assemblyCode[assemblyCount].L = L;   // lexicographical level
    assemblyCode[assemblyCount].M = M;   // modifier
    assemblyCount++;
}

void printInfo(){
    // Print the Lexeme Table
    printf("\n\nLexeme Table:\n\n");
    printf("lexeme token type\n");
    for (int i = 1; i <= tableCount; i++){
        printf("%-12s %d\n", lexemeTable[i].tokenName, lexemeTable[i].tokenVal); // Print information
    }
}

void printAssembly(){
    FILE *fptr = fopen("elf.txt", "w");
    // Print assembly header
    printf("Line\t OP\t L\t M\n");

    for(int i = 0; i < assemblyCount; i++){
        printf("%d\t", i);//Print line number

        switch (assemblyCode[i].op)
        {       // Print what command was used based off assemblyCode[i].op
        case 1: // LIT
            printf("LIT\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 2: // RTN
            switch (assemblyCode[i].M)
            { // Arithmetic/Logical Instructions
            case 0:
                printf("RTN\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 1: // ADD
                printf("ADD\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 2: // SUB
                printf("SUB\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 3: // MUL
                printf("MUL\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 4: // DIV
                printf("DIV\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 5: // EQL
                printf("EQL\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 6: // NEQ
                printf("NEQ\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 7: // LSS
                printf("LSS\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 8: // LEQ
                printf("LEQ\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 9: // GTR
                printf("GTR\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 10: // GEQ
                printf("GEQ\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            case 12:
                printf("NEG\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
                break;
            default:
                break;
            }
            break;
        case 3: // LOD
            printf("LOD\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 4: // STO
            printf("STO\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 5: // CAL
            printf("CAL\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 6: // INC
            printf("INC\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 7: // JMP
            printf("JMP\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 8: // JPC
            printf("JPC\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 9: // SYS
            printf("SYS\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        case 10:
            printf("ODD\t %d\t %d \n", assemblyCode[i].L, assemblyCode[i].M);
            break;
        default:
            break;
        }
        fprintf(fptr, "%d %d %d\n", assemblyCode[i].op, assemblyCode[i].L, assemblyCode[i].M);
    }
    printf("\n"); // Final new line
}

void printTokenList(){
    printf("\nToken List:\n");

    for (int i = 1; i <= tableCount; i++){
        if (lexemeTable[i].tokenVal == 2){
            printf("2 %s ", lexemeTable[i].tokenName);
        }
        else if (lexemeTable[i].tokenVal == 3){
            printf("3 %s ", lexemeTable[i].tokenName);
        }
        else{
            printf("%d ", lexemeTable[i].tokenVal);
        }
    }
    printf("\n");
}

void printError(int errorType){
    // Print error message based on number passed
    switch (errorType){
    case 1: // Number too long
        printf("Error: Number too long\n");
        break;
    case 2: // Name too long
        printf("Error: Name too long\n");
        break;
    case 3: // Invalid Symbols
        printf("Error: Invalid Symbols\n");
        break;
    case 4:
        printf("Error: program must end with period\n");
        break;
    case 5:
        printf("Error: cont, var, and read keywords must be followed by identifier\n");
        break;
    case 6:
        printf("Error: Symbol name has already been declared\n");
        break;
    case 7:
        printf("Error: constants must be assigned with =\n");
        break;
    case 8:
        printf("Error: constants must be assigned an integer value\n");
        break;
    case 9:
        printf("Error: constant and variable declarations must be followed by a semicolon\n");
        break;
    case 10:
        printf("Error: undeclared identifier\n");
        break;
    case 11:
        printf("Error: only variable values may be altered\n");
        break;
    case 12:
        printf("Error: assignment statements must use :=\n");
        break;
    case 13:
        printf("Error: begin must be followed by end\n");
        break;
    case 14:
        printf("Error: if must be followed by then\n");
        break;
    case 15:
        printf("Error: while must be followed by do\n");
        break;
    case 16:
        printf("Error: condition must contain comparison operator\n");
        break;
    case 17:
        printf("Error: right parenthesis must follow left parenthesis\n");
        break;
    case 18:
        printf("Error: arithmetic equations must contain operands, parentheses, numbers, or symbols\n");
        break;
    default:
        break;
    }
    exit(1);//Exit program due to error
}

void printSymbolTable(){

    // Print Symbol table header
    printf("Kind | Name        | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");

    // Print Symbol table info
    for (int i = 0; i < symbolTableCount; i++){
        symbolTable[i].mark = 1;
        printf(" %-4d|", symbolTable[i].kind);
        printf(" %-12s|", symbolTable[i].name);
        printf("  %-5d|", symbolTable[i].val);
        printf("  %-5d|", symbolTable[i].level);
        printf("  %-5d|", symbolTable[i].addr);
        printf("  %-5d\n", symbolTable[i].mark);
    }
}

int main(int argc, char **argv)
{
    // Declarations
    char input;
    char *fname = argv[1];
    FILE *source = fopen(fname, "r");

    //Set Initial Assembly Code
    emit(7, 0, 3);

    if (source == 0){ // Make sure file is open
        printf("Could not open file\n");
        return 1;
    }

    while (!feof(source)){
        input = fgetc(source); // Read from file

        if (isalpha(input) != 0)
        { // A letter is found
            letterHandler(source, input);
        }
        else if (isalnum(input) != 0)
        { // A num is found
            numHandler(source, input);
        }
        else if (ispunct(input) != 0)
        { // Symbol is found
            punctHandler(source, input);
        }
    }

    
    program();//Start parser

    //Print result
    printAssembly();
    // printSymbolTable();

    // Free the memory used
    for (int i = 1; i <= tableCount; i++){
        free(lexemeTable[i].tokenName);
    }

    fclose(source); // Close file
    return 0;
}