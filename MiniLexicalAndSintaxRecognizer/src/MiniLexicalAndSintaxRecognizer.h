#pragma once
// main.h
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h> 
#include <string.h>

// Utils
void Error();

// File && String Operations
long GetFileSize(FILE* fp);
char* PutFileContentIntoString(FILE* fp, long charCount);
FILE* OpenFileAndCheck(const char* fileName);


// Conditions to Skip ( \n - \t - \r - \\ - " " )
int GetNumberOfCharactersToSkip();
void NextTerminal();

// Lexical Analyzer
typedef enum
{
    ERRO,
    ID,
    ALGORITHM,
    SEMILICON,
    DOT,
    EOS,
} Token;

typedef struct
{
    Token Token;
    int Line;
    float Number;
    char ID[200];
} TokenInfo;

typedef struct 
{
    char* content;
    Token token;
} ReservedWord;

TokenInfo GetToken();
TokenInfo CheckReservedWord();
TokenInfo CheckID();
TokenInfo CreateTokenInfo(char* initBuffer, Token token, char* input);


// Grammar
void Program();
void Block();
// Sintax
void Consume(Token token);
// ~main.h