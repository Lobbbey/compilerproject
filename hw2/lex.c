//Joshua Bandy

//Includes
#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include<string.h>

//Defines
#define MAX_IDENTIFIERS 11
#define MAX_DIGIT 5
#define ARRAY_SIZE 500

//Global Variables
char *reservedWords[] = {"const", "var", "procedure", "call", "begin", 
                         "end", "if", "fi", "then", "else",
                         "while", "do", "read", "write"};
int tableCount = 0;

//Structs
typedef struct lexemeInfo lexemeInfo;

struct lexemeInfo{
    char* tokenName;
    int tokenVal;
    int errorCode;
    int tableCount;
};


// Function Prototypes
void letterHandler(FILE *source, char input, lexemeInfo *lexemeTable);
void numHandler(FILE *source, char input, lexemeInfo *lexemeTable);
void punctHandler(FILE *source, char input, lexemeInfo *lexemeTable);
int findSymbolVal(char input);
int findTokenVal(char *inputHolder);
void printInfo(lexemeInfo *lexemeTable);
void printTokenList(lexemeInfo *lexemeTable);
void printError(int errorType);

//Functions
void letterHandler(FILE *source, char input, lexemeInfo* lexemeTable){
    char inputHolder[13];
    int index = 0;
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    while (!feof(source) && isalpha(input)){
        if(index > MAX_IDENTIFIERS){// to many letters, Set error and leave
            lexemeTable[tableCount].errorCode = 2;
            break;
        }
        inputHolder[index] = input;
        index++;
        input = fgetc(source);
    }

    // Store information in struct
    tableCount++;
    lexemeTable[tableCount].tokenVal = findTokenVal(inputHolder);
    lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
    strcpy(lexemeTable[tableCount].tokenName, inputHolder);

    //check current input
    if (isalnum(input) != 0)
    {
        numHandler(source, input, lexemeTable);
    }
    else if (ispunct(input) != 0)
    {
        punctHandler(source, input, lexemeTable);
    }
}

void numHandler(FILE *source, char input, lexemeInfo *lexemeTable){
    int index = 0;
    char inputHolder[13];
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    while (!feof(source) && isalnum(input)){
        if (index > MAX_DIGIT)
        { // to many digits, set error leave
            lexemeTable[tableCount].errorCode = 1;
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

    //check current input
    if(isalpha(input) != 0){
        numHandler(source, input, lexemeTable);
    }
    else if(ispunct(input) != 0){
        punctHandler(source, input, lexemeTable);
    }
}

void punctHandler(FILE *source, char input, lexemeInfo *lexemeTable){
    char inputHolder[13];
    memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder

    if (input == '+' || input == '-' || input == '*' || input == '(' ||
        input == ')' || input == ',' || input == '.' || input == ';' || input == '=')
    {
        // Store information in struct

        inputHolder[0] = input;
        tableCount++;
        lexemeTable[tableCount].tokenVal = findSymbolVal(input);
        lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
        strcpy(lexemeTable[tableCount].tokenName, inputHolder);
        lexemeTable[tableCount].errorCode = 0;
    }
    else if (input == '<'){
        inputHolder[0] = input;
        char tmpInput = inputHolder[0];
        input = fgetc(source);
        inputHolder[1] = input;
        
        //check next input
        if(strcmp(inputHolder, "<>") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 10;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
        }
        else if(strcmp(inputHolder, "<=") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 12;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
        }
        else{
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 11;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
        }

    }
    else if (input == '>'){
        inputHolder[0] = input;
        input = fgetc(source);
        inputHolder[1] = input;

        if(strcmp(inputHolder, ">=") == 0){
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 14;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
        }
        else{
            //Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 13;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
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
            lexemeTable[tableCount].errorCode = 0;
        }
        else{//Invalid symbol
            // Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder));
            inputHolder[0] = tmpInput;
            
            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 0;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 3;
        }
    }
    else if(input == '/'){
        inputHolder[0] = input; // Store initial value
        input = fgetc(source);
        inputHolder[1] = input;

        if(strcmp(inputHolder, "/*") == 0){//Comments
            memset(inputHolder, 0, sizeof(inputHolder));

            while(!feof(source)){
                input = fgetc(source);
                
                if(input == '*'){
                    inputHolder[0] = input;
                    input = fgetc(source);
                    inputHolder[1] = input;

                    if(strcmp(inputHolder, "*/") == 0){
                        break;
                    }
                    memset(inputHolder, 0, sizeof(inputHolder));
                }
            }
        }
        else{//slashsym
            // Reset inputHolder then store tmpInput
            char tmpInput = inputHolder[0];
            memset(inputHolder, 0, sizeof(inputHolder)); // reset input holder
            inputHolder[0] = tmpInput;

            // Store information in struct
            tableCount++;
            lexemeTable[tableCount].tokenVal = 7;
            lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
            strcpy(lexemeTable[tableCount].tokenName, inputHolder);
            lexemeTable[tableCount].errorCode = 0;
        }
    }
    else{
        // Store information in struct
         inputHolder[0] = input;
         tableCount++;
         lexemeTable[tableCount].tokenVal = 0;
         lexemeTable[tableCount].tokenName = malloc(sizeof(char) * (tableCount + 1));
         strcpy(lexemeTable[tableCount].tokenName, inputHolder);
         lexemeTable[tableCount].errorCode = 3;
    }

    //Check current input
    if (isalpha(input) != 0){//current input is alpa
        letterHandler(source, input, lexemeTable);
    }
    else if (isalnum(input) != 0){//current input is num
        numHandler(source, input, lexemeTable);
    }
}

int findSymbolVal(char input){
    if(input == '+'){
        return  4;
    }
    else if(input == '-'){
        return 5;
    }
    else if(input == '*'){
        return 6;
    }
    else if(input == '('){
        return 15;
    }
    else if(input == ')'){
        return 16;
    }
    else if(input == ','){
        return 17;
    }
    else if(input == '.'){
        return 19;
    }
    else if(input == ';'){
        return 18;
    }
    else if(input == '='){
        return 9;
    }
    else{
        return 0;
    }
}

int findTokenVal(char *inputHolder){
    int index = -1;

    // check reservered words
    for(int i = 0; i < 14; i++){
        if(strcmp(inputHolder, reservedWords[i]) == 0){
            index = i;
            break;
        }
    }

    //Return a value based on lexical 
    if(index == -1){//Identsym
        return 2;
    }
    else if(strcmp(reservedWords[index], "const") == 0){
        return 28;
    }
    else if(strcmp(reservedWords[index], "var") == 0){
        return 29;
    }
    else if(strcmp(reservedWords[index], "procedure") == 0){
        return 30;
    }
    else if (strcmp(reservedWords[index], "call") == 0){
        return 27;
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
    else if (strcmp(reservedWords[index], "else") == 0){
        return 33;
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

void printInfo(lexemeInfo *lexemeTable){
    //Print the Lexeme Table
    printf("\n\nLexeme Table:\n\n");
    printf("lexeme token type\n");
    for(int i = 1; i <= tableCount; i++){
        if(lexemeTable[i].errorCode > 0){//Check for errors
            printError(lexemeTable[i].errorCode);//Print relevant error
        }
        printf("%-12s %d\n", lexemeTable[i].tokenName, lexemeTable[i].tokenVal);//Print information
    }
}

void printTokenList(lexemeInfo *lexemeTable){
    
    printf("\nToken List:\n");

    for(int i = 1; i <= tableCount; i++){
        if(lexemeTable[i].tokenVal == 2){
            printf("2 %s ", lexemeTable[i].tokenName);
        }
        else if(lexemeTable[i].tokenVal == 3){
            printf("3 %s ", lexemeTable[i].tokenName);
        }
        else{
            printf("%d ", lexemeTable[i].tokenVal);
        }
    }
        printf("\n");
}

void printError(int errorType){
    //Print error message based on number passed
    switch (errorType){
    case 1://Number too long
        printf("error: Number too long\n");
        break;
    case 2://Name too long
        printf("error: Name too long\n");
        break;
    case 3://Invalid Symbols
        printf("error: Invalid Symbols\n");
        break;
    
    default:
        break;
    }
}

int main(int argc, char **argv){
    //Declarations
    char input;
    lexemeInfo lexemeTable[ARRAY_SIZE];
    char* fname = argv[1];
    FILE *printSource = fopen(fname, "r");

    if(printSource == 0){ // Make sure file is open
        printf("Could not open file\n");
        return 1;
    }

    // Print out the source program
    printf("Source Program:\n");
    while(!feof(printSource)){
        input = fgetc(printSource);
        printf("%c", input);
        if(input == '.'){
            break;
        }
    }

    //Close file after print then reopen to parse
    fclose(printSource);
    FILE *source = fopen(fname, "r");

    while(!feof(source)){
        input = fgetc(source); // Read from file

        if(isalpha(input) != 0){//A letter is found
            letterHandler(source, input, lexemeTable);
        }
        else if(isalnum(input) != 0){//A num is found
          numHandler(source, input, lexemeTable);
        }
        else if(ispunct(input) != 0){//Symbol is found
            punctHandler(source, input, lexemeTable);
        }
    }

    // print the lexeme table
    printInfo(lexemeTable);
    printTokenList(lexemeTable);

    //Free the memory used
    for(int i = 1; i <= tableCount; i++){
        free(lexemeTable[i].tokenName);
    }

    fclose(source);//Close file
    return 0;
}
