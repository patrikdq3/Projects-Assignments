// Alexander Lokhanov
// Patrik De Quevedo
#define charLimit 1500
#define NUMSYM 12
#define NORW 14
#define charMAX 12
#define digMAX 5
#define MAXCLENGTH 500
#define MSYMSIZE 100
#define MAX_SYMBOL_TABLE_SIZE 100

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
//#include <malloc.h>

typedef enum {
oddsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, $callsym, constsym, varsym, writesym, readsym, fisym } token_type;

typedef struct specialSymbols{
    char symbol;
    int token;
} specialSymbols;

typedef struct reservedWords{
    char word[25];
    int token;
} reservedWords;

typedef struct lexemeTable{
    char lexeme[charMAX];
    int numLex;
    int tokenType;
} lexemeTable;

typedef struct
{
    int kind;        // const = 1, var = 2, proc = 3
    char name[charMAX]; // name up to 11 chars
    int val;         // number <ASCII value>
    int level;       // L level
    int addr;        // M address
    int mark;        // to indicate unavailable or delete d
} SYMBOL_TABLE;

typedef struct
{
    char value[charMAX]; // Value of identifier
    char type[digMAX];  // Token type
    int numType;      // converted to int
} TOKEN;

typedef enum
{
    constant = 1,
    variable = 2,
    procedure = 3,
} symbol_kind;

typedef enum
{
    lit = 1,
    opr = 2,
    lod = 3,
    sto = 4,
    cal = 5,
    inc = 6,
    jmp = 7,
    jpc = 8,
    sio1 = 9,
    sio2 = 10,
    sio3 = 11,
} OP_codes;

typedef enum
{
    opr_ret = 0,
    opr_neg = 1,
    opr_add = 2,
    opr_sub = 3,
    opr_mul = 4,
    opr_div = 5,
    opr_odd = 11,
    opr_mod = 7,
    opr_eql = 8,
    opr_neq = 9,
    opr_lss = 6,
    opr_leq = 11,
    opr_gtr = 12,
    opr_geq = 13
} OPR_M_codes;

// Global Variables
char lexTemp[charLimit] = "";
char lexOutput[charLimit*3] = "";
FILE *inputfile;
FILE *outputfile;
SYMBOL_TABLE symbol_table[MAX_SYMBOL_TABLE_SIZE];
int tabAdd;
int table_size;
TOKEN token;
char parseOut[10000] = "";
char parseTemp[MAXCLENGTH] = "";


void parse();
void errorMessage(FILE *inFile, FILE *outFile, int errorCode);
int isComment(char src[], int idx, int inx);
void displayLexeme(lexemeTable lexemes[], int lexCnt, FILE *outFile);

void syntaxError(int errorCode);
void symboltable_insert(int k, char *name, char *val, int level, int addr, int mark);
int rel_op();
void getToken();
char *prog1();

void program();
void block(int level);
void const_dec(int level, int *identNum);
void var_dec(int level, int *identNum);
//void proc_dec(int level, int *identNum);
void statement(int level);
void condition(int level);
void expression(int level);
void term(int level);
void factor(int level);
void printSymbols();

int main(int argc, char *argv[]) {
    printf("\n");
    
    specialSymbols ssym[NUMSYM] = {
        {'+', plussym},
        {'-', minussym},
        {'*', multsym},
        {'/', slashsym},
        {'(', lparentsym},
        {')', rparentsym},
        {'=', eqsym},
        {',', commasym},
        {'.', periodsym},
        {'<', lessym},
        {'>', gtrsym},
        {';', semicolonsym},
    };

    reservedWords rwords[NORW] = {
        {"const", constsym},
        {"var", varsym},
        //{"procedure", procsym},
        //{"call", callsym},
        {"if", ifsym},
        {"then", thensym},
        //{"else", elsesym},
        {"while", whilesym},
        {"do", dosym},
        {"read", readsym},
        {"write", writesym},
        {"odd", oddsym},
        {"begin", beginsym},
        {"end", endsym}
    };

    lexemeTable lexemes[MAXCLENGTH]; 


    // DEV MODE
    int devMode = 0;
    if (argc == 3){
        if (!strcmp(argv[2], "-dev")){
            devMode = 1;
            printf("~~~~~DEV MODE ENABLED~~~~~\n");
        }}
    
    //open input file
    FILE* inFile;
    inFile = fopen(argv[1], "r");
    if (inFile == NULL) {
            printf("file not found.");
            return 0;
        }
    //open output file
    FILE* outFile;
    outFile = fopen("outProto.txt", "w+");
    
    token_type token_type;
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    char src[charLimit];
    char temp;
    int inx = 0;
    
    printf("\n");
    printf("Source Program:\n");
    printf("\n");
    
    // Source Program input for loop
    for(int i=0; i < charLimit; i++){
        src[i] = fgetc(inFile);
        inx++;
        if ( feof(inFile))
            break;
        // Print out Source Program
        printf("%c", src[i]);
    }
    // Ending array where it should end
    inx--;
    src[inx]='\0';
    
    /*
    if (devMode == 1){
        printf("**Entering src** Press K for next\n");
        printf("inx = %d\n", inx);
        for (int i=0; i<inx; i++) {
            printf("%c", src[i]);
        }
    }
    
    
     
     For later me:
     
     ok, so basically, we can seperate everything by delimeters (including whitespaces)
     dont forget about enumators and all that shit
     and dont forget about the trauma you endured on mar 7.
     */
    
    // ~~~~~Lexical Analysis~~~~~
    
    int idx = 0;
    int commentIdx = 0;
    char inString[charLimit] = "";
    int valSym = 0;
    int symbolFound = 0;
    int lexCnt = 0;
    char digitTemp[charLimit] = "";
    char currentWord[charLimit] = "";
   
    
    do {
        
        if( isspace(src[idx]) ){
            idx++;
            if (devMode == 1)
                printf("WS detected at idx %d;\n", idx-1);
        } else if ( ispunct(src[idx]) ){
            commentIdx = isComment(src, idx, inx);
            if (devMode == 1 && commentIdx > 0)
                printf("Comment detected at idx %d;\n", idx);
        if (commentIdx != -1){
            idx = commentIdx+1;
        }
        for(int i=0; i < NUMSYM; i++){
            if (src[idx] == ssym[i].symbol || src[idx] == ':' || src[idx] == '!'){
                symbolFound = 1;
                lexemes[lexCnt].lexeme[0] = src[idx];

                if ((idx + 1) < inx && ispunct(src[idx+1])){
                    switch(src[idx]){
                    case '<':
                        if(src[idx+1]== '>'){
                            lexemes[lexCnt].tokenType = neqsym;
                            lexemes[lexCnt].lexeme[1] = src[idx++];
                        } else if (src[idx+1] == '='){
                            lexemes[lexCnt].tokenType = leqsym;
                            lexemes[lexCnt].lexeme[1] = src[idx++];
                        } else
                            lexemes[lexCnt].tokenType = ssym[i].token;
                            break;
                        case '>': // looking for >=
                            if(src[idx+1] == '='){
                                lexemes[lexCnt].tokenType = becomessym;
                                lexemes[lexCnt].lexeme[1] = src[idx++];
                            } else 
                                lexemes[lexCnt].tokenType = ssym[i].token;
                                break;
                            case ':': //looking for :=
                                if(src[idx+1]== '='){
                                    lexemes[lexCnt].tokenType = becomessym;
                                    lexemes[lexCnt].lexeme[1] = src[idx++];
                                } else
                                    errorMessage(inFile, outFile, 101);
                                    break;
                                default:
                                    lexemes[lexCnt].tokenType = ssym[i].token;
                                    break;
                    }
                }
                else{
                    if(src[idx] == ':' || src[idx] == '!')
                        errorMessage(inFile, outFile, 101);

                    lexemes[lexCnt].tokenType = ssym[i].token;
                }
                idx++;
                lexCnt++;
                break;
            }
        }
        
        if(!symbolFound)
            errorMessage( inFile, outFile, 101);
    }
    else if (isalpha(src[idx])){
        int letterCount = 0;
        while (isalnum(src[idx])){
            currentWord[letterCount] = src[idx];
            idx++;
            letterCount++;
        }

        currentWord[letterCount++] = '\0';

        if(letterCount > charMAX)
            errorMessage(inFile, outFile, 202);
        else{
            strcpy(lexemes[lexCnt].lexeme, currentWord);

            for(int i = 0; i < NORW; i++) //checking for keyword
            {
                if(strcmp(rwords[i].word, currentWord) == 0){
                    lexemes[lexCnt].tokenType = rwords[i].token;
                    break;
                } else
                    lexemes[lexCnt].tokenType = identsym;
            }
            lexCnt++;
        }
    } else if (isdigit(src[idx])){
        int digiCnt = 0;

        while(isdigit(src[idx])){
            digitTemp[digiCnt] = src[idx];
            idx++;
            digiCnt++;
        }

        digitTemp[digiCnt++] = '\0';

        if (isalpha(src[idx]))
            errorMessage(inFile, outFile, 303);
        else if(digiCnt <= digMAX)
        {
            strcpy(lexemes[lexCnt].lexeme, digitTemp);
            lexemes[lexCnt].numLex = atoi(digitTemp);
            lexemes[lexCnt].tokenType = numbersym;
            lexCnt++;
        }
        else
            errorMessage(inFile, outFile, 404);
    }

    if(lexemes[lexCnt-1].tokenType == numbersym){
        sprintf(lexTemp, "%d", lexemes[lexCnt - 1].numLex);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%d", lexemes[lexCnt - 1].numLex);
    } else {
        sprintf(lexTemp, "%s", lexemes[lexCnt - 1].lexeme);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%s", lexemes[lexCnt - 1].lexeme);
    }

    sprintf(lexTemp, "\t\t%d\n", lexemes[lexCnt - 1].tokenType);
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "\t\t%d\n", lexemes[lexCnt - 1].tokenType);


        
    } while (idx < inx);

    displayLexeme(lexemes, lexCnt, outFile);
    fclose(inFile);
    fclose(outFile);
    printf("\n");
    prog1();
    return 0;
}


int isComment(char src[], int idx, int inx) {
    // Check if the curr character is the start of a comment
    if (idx + 1 < inx) {
        // For multi-line comments
        if (src[idx] == '/' && src[idx + 1] == '*') {
            idx += 2; // Skip the initial "/*"
            while (idx + 1 < inx) {
                if (src[idx] == '*' && src[idx + 1] == '/') {
                    return idx + 2; // Return the index after "*/"
                }
                idx++;
            }
        }
         else if (src[idx] == '/' && src[idx + 1] == '/') {
             return inx; // Assume rest of line is comment
        }
    }
    return -1; // -1 = not a comment / end of comment not found
}

void errorMessage(FILE *inFile, FILE *outFile, int errorCode)
{
    char *message = "";
    switch(errorCode){
        case 101:
            message = "Symbol does not exist";
            break;
        case 202:
            message = "Identifier is over 11 characters long";
            break;
        case 303:
            message = "Identifiers cannot start with a number";
            break;
        case 404:
            message = "Number is over 5 digits long";
            break;
        case 1:
            printf("\nERROR 1: Symbol name has already been declared");
            fprintf(outputfile, "\nERROR 1: Duplicate identifiers are not allowed");
            sprintf(parseTemp, "\nERROR 1: Duplicate identifiers are not allowed");
            strcat(parseOut, parseTemp);

            break;
        case 2:
            printf("\nERROR 2: Constants must be assigned an integer value");
            fprintf(outputfile, "\nERROR 2: \"=\" must be followed by a positive number");
            sprintf(parseTemp, "\nERROR 2: \"=\" must be followed by a positive number");
            strcat(parseOut, parseTemp);

            break;
        case 3:
            printf("\nERROR 3: Constants must be assigned with \"=\"");
            fprintf(outputfile, "\nERROR 3: Constant identifiers must be followed by a \"=\"");
            sprintf(parseTemp, "\nERROR 3: Constant identifiers must be followed by a \"=\"");
            strcat(parseOut, parseTemp);

            break;
        case 4:
            printf("\nERROR 4: Const, var, and read keywords must be followed by identifier");
            fprintf(outputfile, "\nERROR 4: Expected an identifier");
            sprintf(parseTemp, "\nERROR 4: Expected an identifier");
            strcat(parseOut, parseTemp);

            break;
        case 5:
            printf("\nERROR 5: Constant and variable declarations must be followed by a semicolon");
            fprintf(outputfile, "\nERROR 5: Semicolon or comma missing");
            sprintf(parseTemp, "\nERROR 5: Semicolon or comma missing");
            strcat(parseOut, parseTemp);

            break;
        case 6:
            printf("\nERROR 6: Symbol table full");
            fprintf(outputfile, "\nERROR 6: Symbol table full");
            sprintf(parseTemp, "\nERROR 6: Symbol table full");
            strcat(parseOut, parseTemp);

            break;
        case 7:
            printf("\nERROR 7: Assignment statements must use \":=\"");
            fprintf(outputfile, "\nERROR 7: Variable identifiers must be followed by a \":=\"");
            sprintf(parseTemp, "\nERROR 7: Variable identifiers must be followed by a \":=\"");
            strcat(parseOut, parseTemp);
            break;
        case 8:
            printf("\nERROR 8: Program must end with period");
            fprintf(outputfile, "\nERROR 8: Period expected");
            sprintf(parseTemp, "\nERROR 8: Period expected");
            strcat(parseOut, parseTemp);

            break;
        case 9:
            printf("\nERROR 9: Undeclared identifier");
            fprintf(outputfile, "\nERROR 9: Undeclared identifier");
            sprintf(parseTemp, "\nERROR 9: Undeclared identifier");
            strcat(parseOut, parseTemp);

            break;
        case 10:
            printf("\nERROR 10: Semicolon between statements missing");
            fprintf(outputfile, "\nERROR 10: Semicolon between statements missing");
            sprintf(parseTemp, "\nERROR 10: Semicolon between statements missing");
            strcat(parseOut, parseTemp);

            break;
        case 11:
            printf("\nERROR 11: Only variable values may be altered");
            fprintf(outputfile, "\nERROR 11: Assignment to constant or procedure is not allowed");
            sprintf(parseTemp, "\nERROR 11: Assignment to constant or procedure is not allowed");
            strcat(parseOut, parseTemp);

            break;
        case 12:
            printf("\nERROR 12: Arithmetic equations must contain operands, parentheses, numbers, or symbols");
            fprintf(outputfile, "\nERROR 12: Factor expected");
            sprintf(parseTemp, "\nERROR 12: Factor expected");
            strcat(parseOut, parseTemp);

            break;
        case 13:
            printf("\nERROR 13: Right parenthesis must follow left parenthesis");
            fprintf(outputfile, "\nERROR 13: Right parenthesis missing");
            sprintf(parseTemp, "\nERROR 13: Right parenthesis missing");
            strcat(parseOut, parseTemp);
            break;
        case 14:
            printf("\nERROR 14: Condition must contain comparison operator");
            fprintf(outputfile, "\nERROR 14: Relational operator expected");
            sprintf(parseTemp, "\nERROR 14: Relational operator expected");
            strcat(parseOut, parseTemp);

            break;
        case 15:
            printf("\nERROR 15: \"while\" must be followed by \"do\"");
            fprintf(outputfile, "\nERROR 15: \"do\" expected");
            sprintf(parseTemp, "\nERROR 15: \"do\" expected");
            strcat(parseOut, parseTemp);

            break;
        case 16:
            printf("\nERROR 16: \"if\" must be followed by \"then\"");
            fprintf(outputfile, "\nERROR 16: \"then\" expected");
            sprintf(parseTemp, "\nERROR 16: \"then\" expected");
            strcat(parseOut, parseTemp);

            break;
        case 17:
            printf("\nERROR 17: Begin must be followed by \"end\"");
            fprintf(outputfile, "\nERROR 17: \"end\" expected");
            sprintf(parseTemp, "\nERROR 17: \"end\" expected");
            strcat(parseOut, parseTemp);

            break;
        case 18:
            printf("\nERROR 18: Unexpected parenthesis after \"while\" or \"if\"");
            fprintf(outputfile, "\nERROR 18: Unexpected parenthesis after \"while\" or \"if\"");
            sprintf(parseTemp, "\nERROR 18: Unexpected parenthesis after \"while\" or \"if\"");
            strcat(parseOut, parseTemp);
            break;
        case 19:
            printf("\nERROR 19: Call must be followed by a procedure identifier");
            fprintf(outputfile, "\nERROR 19: Call must be followed by a procedure identifier");
            sprintf(parseTemp, "\nERROR 19: Call must be followed by a procudure identifier");
            strcat(parseOut, parseTemp);
            break;
        case 20:
            printf("\nERROR 20: Expected end of program");
            fprintf(outputfile, "\nERROR 20: Expected end of program");
            sprintf(parseTemp, "\nERROR 20: Expected end of program");
            strcat(parseOut, parseTemp);
            break;
        case 21:
            printf("\nERROR 21: Cannot read a non-variable");
            fprintf(outputfile, "\nERROR 21: Cannot read a non-variable");
            sprintf(parseTemp, "\nERROR 21: Cannot read a non-variable");
            strcat(parseOut, parseTemp);
            break;
        default:
            printf("\nERROR UNKNOWN: Generated code contains unidentified error");
            fprintf(outputfile, "\nERROR UNKNOWN: Generated code contains unidentified error");
            sprintf(parseTemp, "\nERROR UNKNOWN: Generated code contains unidentified error");
            strcat(parseOut, parseTemp);
            break;
    }
    sprintf(lexTemp, "ERROR: %s\n", message);
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "ERROR: %s\n", message);
    fclose(inFile);
    fclose(outFile);
    exit(0);
    
    
}

void displayLexeme(lexemeTable lexemes[], int lexCnt, FILE *outFile)
{
    FILE *out = fopen("tokens.txt", "w");
    if (out == NULL)
    {
        sprintf(lexTemp, "Unable to write tokens to file.");
        strcat(lexOutput, lexTemp);
    }
    sprintf(lexTemp, "Lexeme List:\n");
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "Lexeme List:\n");

    for (int i = 0; i < lexCnt; i++)
    {
        sprintf(lexTemp, "%d ", lexemes[i].tokenType);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%d ", lexemes[i].tokenType);
        fprintf(out, "%d ", lexemes[i].tokenType);

        if (lexemes[i].tokenType == identsym) 
        {
            sprintf(lexTemp, "%s ", lexemes[i].lexeme);
            strcat(lexOutput, lexTemp);
            fprintf(outFile, "%s ", lexemes[i].lexeme);
            fprintf(out, "%s ", lexemes[i].lexeme);
        }
        if (lexemes[i].tokenType == numbersym) 
        {
            sprintf(lexTemp, "%d ", lexemes[i].numLex);
            strcat(lexOutput, lexTemp);
            fprintf(outFile, "%d ", lexemes[i].numLex);
            fprintf(out, "%d ", lexemes[i].numLex);
        }
    }

    sprintf(lexTemp, "\n");
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "\n");
    fprintf(out, "\n");
    fclose(out);
}

/*_______________________________________________________
 
 PARSER
_______________________________________________________*/

char *prog1()
{
    tabAdd = 4;

    inputfile = fopen("tokens.txt", "r");
    outputfile = fopen("parseOut.txt", "w");
    rewind(inputfile);

    parse();
    printf("\n");
    printf("\nLine\tOP\tL\tM");
    printf("\n0\tJMP\t0\t3");
    printf("\n");
    sprintf(parseTemp, "No errors. Program is syntactically correct!\n\n");
    strcat(parseOut, parseTemp);
    fprintf(outputfile, "No errors. Program is syntactically correct!\n\n");
    printSymbols();
    fclose(inputfile);
    fclose(outputfile);

    return parseOut;
}


void parse(){
    
    fopen("tokens.txt", "r");
    rewind(inputfile);
    
    int level = 0;
    getToken();
    block(level);
    
    if(token.numType != periodsym)
        errorMessage(inputfile, outputfile, 8);
    
    getToken();
    if(token.numType != -1)
        errorMessage(inputfile, outputfile, 20);
}

void printSymbols()
{
    int kind;
    char name[charMAX];
    int val;
    int level;
    int addr;
    int mark;
    printf("\nSymbol Table:");
    printf("\n");
    printf("\nKIND\t|NAME\t|VAL\t\t|LVL\t\t|ADDR\t|MARK\n");
    printf("--------------------------------------------------------------\n");
    for (int i = 0; i < table_size; i++)
    {
//kind name val lvl add mark
        printf("%d\t|%s\t|%d\t\t|%d\t\t|%d\t|%d\n", symbol_table[i].kind, symbol_table[i].name, symbol_table[i].val, symbol_table[i].level, symbol_table[i].addr, symbol_table[i].mark);
    
    }
}

int searchTable(char *name, int level, int mark)
{
    if (mark == -1)
    {
        for (int i = table_size - 1; i >= 0; i--)
        {
            if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].level == level)
                return i;
        }
    }
    else if (mark == 0)
    {
        for (int i = table_size - 1; i >= 0; i--)
        {
            if (strcmp(symbol_table[i].name, name) == 0 && symbol_table[i].level == level && symbol_table[i].mark == mark)
                return i;
        }
    }
    return -1;
}

int findClosest(char *name, int level, int kind)
{
    if (kind != 0)
    {
        level = level + 1;
        for (int i = table_size - 1; i >= 0; i--)
        {

            if (symbol_table[i].level > level || symbol_table[i].mark != 0 || symbol_table[i].kind != procedure)
                continue;

            if (strcmp(symbol_table[i].name, name) == 0)
                return i;
        }
    }
    else
    {
        for (int i = table_size - 1; i >= 0; i--)
        {
            if (symbol_table[i].level > level || symbol_table[i].mark != 0 || symbol_table[i].kind == procedure)
                continue;

            if (strcmp(symbol_table[i].name, name) == 0)
                return i;
        }
    }

    return -1;
}

void symboltable_insert(int k, char *name, char *val, int level, int addr, int mark)
{
    if (table_size >= MAX_SYMBOL_TABLE_SIZE)
        errorMessage(inputfile, outputfile, 6);

    if (searchTable(name, level, 0) != -1)
        errorMessage(inputfile, outputfile, 1);

    int intVal = atoi(val);
    symbol_table[table_size].kind = k;
    strcpy(symbol_table[table_size].name, name);
    symbol_table[table_size].level = level;
    symbol_table[table_size].val = intVal;
    symbol_table[table_size].addr = addr-1;
    symbol_table[table_size].mark = mark;
    table_size++;
}

int rel_op()
{
    if (token.numType == eqsym) //eqsym
        return opr_eql;
    else if (token.numType == neqsym) //neqsym
        return opr_neq;
    else if (token.numType == lessym) //lessym
        return opr_lss;
    else if (token.numType == leqsym) //leqsym
        return opr_leq;
    else if (token.numType == gtrsym) //gtrsym
        return opr_gtr;
    else if (token.numType == geqsym) //geqsym
        return opr_geq;
    else
        return -1;
}

void getToken(){
    token.numType = -1;
    strcpy(token.value, "");
    strcpy(token.type, "");
    char t;
    
    if(fscanf(inputfile, "%s", token.type) != EOF){
        token.numType = atoi(token.type);
        if (token.numType == identsym || token.numType == numbersym)
            fscanf(inputfile, "%s", token.value);
        else
            token.value[0] = '\0';
    }
}

void block(int level){
    int identNum = 0;
    char token_name[charMAX];
    
    const_dec(level, &identNum);
    var_dec(level, &identNum);
    //proc_dec(level + 1, &identNum);
    int startMarks = table_size;
    statement(level);
    
    int marked = 0;
    for(int i = startMarks - 1; marked < identNum; i--){
        if(!symbol_table[i].mark){
            symbol_table[i].mark=1;
            marked++;
        }}}
void const_dec(int level, int *identNum)
{
    char token_name[charMAX];
    if (token.numType == constsym) //constsym
    {
        do
        {
            getToken();
            if (token.numType != identsym) //identsym
                errorMessage(inputfile, outputfile, 4);
            
            strcpy(token_name, token.value);
            
            getToken();
            if (token.numType != eqsym) //eqsym
                errorMessage(inputfile, outputfile, 3);
            
            getToken();
            if (token.numType != numbersym) //numbersym
                errorMessage(inputfile, outputfile, 2);
            
            symboltable_insert(constant, token_name, token.value, level, 0, 0);
            (*identNum)++;
            
            getToken();
            
            if (token.numType == identsym)
                errorMessage(inputfile, outputfile, 5);
            
        } while (token.numType == commasym); //comsym
        
        if (token.numType != semicolonsym) //semicolomsym
            errorMessage(inputfile, outputfile, 5);
        
        getToken();
    }}


void var_dec(int level, int *identNum)
{
    if (token.numType == varsym) //varsym
    {
        int numVars = 0;
        do
        {
            numVars++;
            getToken();

            if (token.numType != identsym) //identsym
                errorMessage(inputfile, outputfile, 4);

            symboltable_insert(variable, token.value, "0", level, numVars + 3, 0);
            tabAdd++;
            (*identNum)++;

            getToken();

            if (token.numType == identsym)
                errorMessage(inputfile, outputfile, 5);

        } while (token.numType == commasym); //commasym

        if (token.numType != semicolonsym) //semicolomsym
            syntaxError(5);

        getToken();
    }
}


/*void proc_dec(int level, int *identNum)
{
    if (token.numType == procsym)
    {
        do
        {
            getToken();

            if (token.numType != identsym)
                syntaxError(4);

            
            symboltable_insert(procedure, token.value, "0", level, 0, 0);
            (*identNum)++;

            getToken();

            if (token.numType != semicolonsym)
                syntaxError(5);

            getToken();
            block(level);

            if (token.numType != semicolonsym)
                syntaxError(5);

            getToken();
        } while (token.numType == procsym);
    }
}*/

void statement(int level)
{
    if (token.numType == identsym) //identsym
    {
        int index = findClosest(token.value, level, 0);
        if (index == -1)
            syntaxError(9); 

        if (symbol_table[index].kind != variable)
            syntaxError(11);

        getToken();

        if (token.numType != becomessym) //becommessym
            syntaxError(7);

        getToken();

        expression(level);
    }
   /* else if (token.numType == callsym)
    {
        getToken();

        if (token.numType != identsym)
            syntaxError(4);

        int index = findClosest(token.value, level, procedure);

        if (index == -1)
            syntaxError(19);

        getToken();
    }*/
    else if (token.numType == beginsym) //beginsym
    {

        getToken();
        statement(level);

        //if (token.numType != semicolonsym) -------------------------------------------------_REMEMBER
          //  syntaxError(10);
        while (token.numType == semicolonsym) //semicolonsym
        {
            getToken();
            statement(level);
        }
        if (token.numType != endsym) //endsym
            syntaxError(17);

        getToken();
    }
    else if (token.numType == ifsym) //ifsym
    {
        getToken();
        if (token.numType == lparentsym)
            syntaxError(18);
        condition(level);

        if (token.numType != thensym) //thensym
            syntaxError(16);

        getToken();

        statement(level);
        if (token.numType != fisym) 
            printf("Error: no \"fi\" after \"then\"");
        if (token.numType == semicolonsym)//----------------------Check for semicolon, if no semicolon throw fi error (else passed as identifier)
        {
            getToken();
            statement(level);
        }
        else{
            printf("Error: no \"fi\" after \"then\"");
        }
        
    }
    else if (token.numType == whilesym) //whilesym
    {
        getToken();
        if (token.numType == lparentsym)
            syntaxError(18);
        condition(level);

        if (token.numType != dosym) //dosym
            syntaxError(15);

        getToken();
        statement(level);
    }
    else if (token.numType == readsym)
    {
        getToken();

        if (token.numType != identsym)
            syntaxError(4);

        int index = findClosest(token.value, level, 0);

        if (index == -1)
            syntaxError(9);

        if (symbol_table[index].kind != variable)
            syntaxError(21);

        getToken();
    }
    else if (token.numType == writesym)
    {
        getToken();
        expression(level);
    }
}

void condition(int level)
{
    if (token.numType == oddsym) //oddsym
    {
        getToken();
        expression(level);
    }
    else
    {
        expression(level);
        int operator= rel_op();

        if (operator== - 1)
            syntaxError(14);

        getToken();
        expression(level);
    }
}

void expression(int level)
{
    if (token.numType == plussym || token.numType == minussym) // plussym / minnusym
        getToken();

    term(level);

    while (token.numType == plussym || token.numType == minussym) // plussym / minnusym
    {
        getToken();
        term(level);
    }
}

void term(int level)
{
    factor(level);

    while (token.numType == multsym || token.numType == slashsym) // multsym / slashsym
    {
        getToken();
        factor(level);
    }
}

void factor(int level)
{
    if (token.numType == identsym)
    {
        int index = findClosest(token.value, level, 0);
        if (index == -1)
            syntaxError(9);

        getToken();
    }
    else if (token.numType == numbersym)
    {
        getToken();
    }
    else if (token.numType == lparentsym)
    {
        getToken();
        expression(level);

        if (token.numType != rparentsym)
            syntaxError(13);

        getToken();
    }
    else
    {
        syntaxError(12);
    }
}


void syntaxError(int errorCode)
{
    errorMessage(inputfile, outputfile, errorCode);
    exit(0);
}