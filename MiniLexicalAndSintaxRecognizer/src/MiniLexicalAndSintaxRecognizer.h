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
    ALGORITMO,
    SEMILICON,
    DOT,
    EOS,
    VARIAVEL,
    COLON,
    INTEIRO,
    LOGICO,
    COMMA,
    INICIO,
    FIM,
    SE,
    ENQUANTO,
    LEIA,
    ESCREVA,
    OPEN_PARENTHESES,
    CLOSE_PARENTHESES,
    LESSER,
    LESSER_EQUAL,
    EQUAL,
    MARK,
    GREATER,
    GREATER_EQUAL,
    PLUS_SIGN,
    MINUS_SIGN,
    OU,
    MULT_SIGN,
    DIV,
    E,
    VERDADEIRO,
    FALSO,
    NUMBER
} Token;

typedef struct
{
    Token Token;
    int Line;
    float Number;
    char ID[20];
} TokenInfo;

typedef struct 
{
    char* content;
    Token token;
} ReservedWord;

TokenInfo GetToken();
TokenInfo CheckReservedWord();
TokenInfo CheckID();
TokenInfo CheckNumber();
TokenInfo CreateTokenInfo(char* initBuffer, Token token, char* input);


// Grammar
void Program();         // <programa>
void Scope();           // <bloco>
void VarDeclaration();  // <declaracao_de_variaveis>
void CompoundCommand(); // <comando_composto>
void VarList();         // <lista_variavel>
void Type();            // <tipo>
void Command();         // <comando>
void Print();           // <escreva>
void Expression();      // <expressao>
void SimpleExpression(); // <expressao_simples>
void Relational();      // <relacional>
void Term();            // <termo>
void Factor();           // <fator>
void Number();           // <numero>
// Syntax
void Consume(Token token);
// ~main.h