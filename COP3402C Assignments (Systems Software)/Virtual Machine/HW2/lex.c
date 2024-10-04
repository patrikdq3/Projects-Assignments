#define charLimit 1500
#define NUMSYM 12
#define NORW 14
#define CMAX 12
#define DMAX 5
#define MAXCLENGTH 500
#define MSYMSIZE 100

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
//#include <malloc.h>

typedef enum {
skipsym = 1, identsym, numbersym, plussym, minussym, multsym, slashsym, oddsym, eqsym, neqsym, lessym, leqsym, gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym, periodsym, becomessym, beginsym, endsym, ifsym, thensym, whilesym, dosym, callsym, constsym, varsym, procsym, writesym, readsym , elsesym } token_type;

typedef struct specialSymbols{
    char symbol;
    int token;
} specialSymbols;

typedef struct reservedWords{
    char word[25];
    int token;
} reservedWords;

typedef struct lexemeTable{
    char lexeme[CMAX];
    int numLex;
    int tokenType;
} lexemeTable;

// Global Variables
char lexTemp[charLimit] = "";
char lexOutput[charLimit*3] = ""; 


void errorMessage(FILE *inFile, FILE *outFile, int errorCode);
int isComment(char src[], int idx, int inx);
void displayLexeme(lexemeTable lexemes[], int lexemeCount, FILE *outFile);

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
        {"procedure", procsym},
        {"call", callsym},
        {"if", ifsym},
        {"then", thensym},
        {"else", elsesym},
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
    outFile = fopen("outParse.txt", "w");
    
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
    printf("contained in file outParse.txt\n");
    printf("\n");
    printf("Lexeme Table:\n");
    printf("lexeme\ttokentype\n");
    printf("\n");
    
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
     and dont forget about the trauma you endured on making this.
     */
    
    // ~~~~~Lexical Analysis~~~~~
    
    int idx = 0;
    int commentIdx = 0;
    char inString[charLimit] = "";
    int valSym = 0;
    int symbolFound = 0;
    int lexemeCount = 0;
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
                lexemes[lexemeCount].lexeme[0] = src[idx];

                if ((idx + 1) < inx && ispunct(src[idx+1])){
                    switch(src[idx]){
                    case '<':
                        if(src[idx+1]== '>'){
                            lexemes[lexemeCount].tokenType = neqsym;
                            lexemes[lexemeCount].lexeme[1] = src[idx++];
                        } else if (src[idx+1] == '='){
                            lexemes[lexemeCount].tokenType = leqsym;
                            lexemes[lexemeCount].lexeme[1] = src[idx++];
                        } else
                            lexemes[lexemeCount].tokenType = ssym[i].token;
                            break;
                        case '>': // looking for >=
                            if(src[idx+1] == '='){
                                lexemes[lexemeCount].tokenType = becomessym;
                                lexemes[lexemeCount].lexeme[1] = src[idx++];
                            } else 
                                lexemes[lexemeCount].tokenType = ssym[i].token;
                                break;
                            case ':': //looking for :=
                                if(src[idx+1]== '='){
                                    lexemes[lexemeCount].tokenType = becomessym;
                                    lexemes[lexemeCount].lexeme[1] = src[idx++];
                                } else
                                    errorMessage(inFile, outFile, 1);
                                    break;
                                default:
                                    lexemes[lexemeCount].tokenType = ssym[i].token;
                                    break;
                    }
                }
                else{
                    if(src[idx] == ':' || src[idx] == '!')
                        errorMessage(inFile, outFile, 1);

                    lexemes[lexemeCount].tokenType = ssym[i].token;
                }
                idx++;
                lexemeCount++;
                break;
            }
        }
        
        if(!symbolFound)
            errorMessage( inFile, outFile, 1);
    }
    else if (isalpha(src[idx])){
        int letterCount = 0;
        while (isalnum(src[idx])){
            currentWord[letterCount] = src[idx];
            idx++;
            letterCount++;
        }

        currentWord[letterCount++] = '\0';

        if(letterCount > CMAX)
            errorMessage(inFile, outFile, 2);
        else{
            strcpy(lexemes[lexemeCount].lexeme, currentWord);

            for(int i = 0; i < NORW; i++) //checking for keyword
            {
                if(strcmp(rwords[i].word, currentWord) == 0){
                    lexemes[lexemeCount].tokenType = rwords[i].token;
                    break;
                } else
                    lexemes[lexemeCount].tokenType = identsym;
            }
            lexemeCount++;
            
        }
    } else if (isdigit(src[idx])){
        int digitCount = 0;

        while(isdigit(src[idx])){
            digitTemp[digitCount] = src[idx];
            idx++;
            digitCount++;
        }

        digitTemp[digitCount++] = '\0';

        if (isalpha(src[idx]))
            errorMessage(inFile, outFile, 3);
        else if(digitCount <= DMAX)
        {
            strcpy(lexemes[lexemeCount].lexeme, digitTemp);
            lexemes[lexemeCount].numLex = atoi(digitTemp);
            lexemes[lexemeCount].tokenType = numbersym;
            lexemeCount++;
        }
        else
            errorMessage(inFile, outFile, 4);
    }

    if(lexemes[lexemeCount-1].tokenType == numbersym){
        sprintf(lexTemp, "%d", lexemes[lexemeCount - 1].numLex);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%d", lexemes[lexemeCount - 1].numLex);
        printf("%d", lexemes[lexemeCount - 1].numLex);
    } else {
        sprintf(lexTemp, "%s", lexemes[lexemeCount - 1].lexeme);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%s", lexemes[lexemeCount - 1].lexeme);
        printf("%s", lexemes[lexemeCount - 1].lexeme);
    }

    sprintf(lexTemp, "\t\t%d\n", lexemes[lexemeCount - 1].tokenType);
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "\t\t%d\n", lexemes[lexemeCount - 1].tokenType);
    printf("\t\t%d\n", lexemes[lexemeCount - 1].tokenType);


        
    } while (idx < inx);

    displayLexeme(lexemes, lexemeCount, outFile);
    fclose(inFile);
    fclose(outFile);
    printf("\n");
    return 0;
}



int isComment(char src[], int idx, int inx) {
    // Check for multi-line comment start
    if (src[idx] == '/' && idx + 1 < inx && src[idx + 1] == '*') {
        // Move past the "/*"
        idx += 2;
        // Search for the end of the comment "*/"
        for (; idx < inx - 1; idx++) {
            if (src[idx] == '*' && src[idx + 1] == '/') {
                return idx + 2; // Return position after "*/"
            }
        }
        return -1; // Comment not properly closed
    }
    // Check for single-line comment start
    else if (src[idx] == '/' && idx + 1 < inx && src[idx + 1] == '/') {
        // Move to the next line or end of the string
        for (idx += 2; idx < inx; idx++) {
            if (src[idx] == '\n') {
                return idx + 1; // Return position after "\n"
            }
        }
        return inx; // Reached end without newline; return end index
    }

    return -1; // Not a comment start
}

void errorMessage(FILE *inFile, FILE *outFile, int errorCode)
{
    char *message = "";
    switch(errorCode){
    case 1:
        message = "Symbol does not exist";
    case 2: 
        message = "Identifier is over 11 characters long";
        break;
    case 3:
        message = "Identifiers cannot start with a number";
        break;
    case 4:
        message = "Number is over 5 digits long";
        break;
    default:
        break;
    }
    sprintf(lexTemp, "ERROR: %s\n", message);
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "ERROR: %s\n", message);
    printf("ERROR: %s\n", message);
    fclose(inFile);
    fclose(outFile);
    exit(0);
}

void displayLexeme(lexemeTable lexemes[], int lexemeCount, FILE *outFile)
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
    printf("Lexeme List:\n");

    for (int i = 0; i < lexemeCount; i++)
    {
        int alrPrint = 0;
        sprintf(lexTemp, "%d ", lexemes[i].tokenType);
        strcat(lexOutput, lexTemp);
        fprintf(outFile, "%d ", lexemes[i].tokenType);
        printf("%d ", lexemes[i].tokenType);
        alrPrint = 1;
        fprintf(out, "%d ", lexemes[i].tokenType);
        //printf("%d ", lexemes[i].tokenType);

        if (lexemes[i].tokenType == identsym) 
        {
            sprintf(lexTemp, "%s ", lexemes[i].lexeme);
            strcat(lexOutput, lexTemp);
            fprintf(outFile, "%s ", lexemes[i].lexeme);
            printf("%s ", lexemes[i].lexeme);
            fprintf(out, "%s ", lexemes[i].lexeme);
            //printf("%s ", lexemes[i].lexeme);
        }
        if (lexemes[i].tokenType == numbersym)
        {
            sprintf(lexTemp, "%d ", lexemes[i].numLex);
            strcat(lexOutput, lexTemp);
            fprintf(outFile, "%d ", lexemes[i].numLex);
            printf("%d ", lexemes[i].numLex);
            fprintf(out, "%d ", lexemes[i].numLex);
            //printf("%d ", lexemes[i].numLex);
        }
    }

    sprintf(lexTemp, "\n");
    strcat(lexOutput, lexTemp);
    fprintf(outFile, "\n");
    printf("\n");
    fprintf(out, "\n");
    fclose(out);
}

