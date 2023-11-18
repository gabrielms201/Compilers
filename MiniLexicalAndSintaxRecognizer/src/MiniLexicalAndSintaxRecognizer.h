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
    NUMBER,
    ASSIGN_SIGN,
    FACA,
    COMMENTARY,
    MULTI_COMMENTARY,
    ENTAO,
    SENAO
} Token;

typedef struct
{
    Token Token;
    int Line;
    int Number;
    char ID[20];
    int Address;
} TokenInfo;


typedef struct 
{
    char* content;
    Token token;
} ReservedWord;


// Conditions to Skip ( \n - \t - \r - \\ - " " )
int ShouldSkipOneChar();
TokenInfo NextTerminal();

TokenInfo GetToken();
TokenInfo CheckReservedWord(char* originalId);
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
void Input();           // <comando_entrada>
void Assignment();      // <comando_atribuicao>
void While();           // <comando_enquanto>
void If();              // <comando_se>
// Syntax
void Consume(Token token);
int SearchAddress(TokenInfo token);


// Semantic
int NextLabel();
// ~main.h