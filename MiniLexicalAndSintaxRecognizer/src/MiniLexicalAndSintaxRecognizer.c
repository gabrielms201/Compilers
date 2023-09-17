// <programa> ::= <declaracoes> <comandos>
// <declaracoes> ::= <declaracao> | <declaracao> <declaracoes>
// <declaracao> ::= 'int' <variavel>';'
// <comandos> ::= <atribuicao> {';' <atribuicao>}
// <atribuicao> ::= <variavel> '=' <expr>
// <expr> ::= ['+' | '-'] <termo> {('+' | '-' ) <termo>}
// <termo> ::= <fator> {('*'|'/') <fator>}
// <fator> ::= <numero> | '('<expr>')'
// <variavel> ::= <letra> <variavel> | <letra>
// <numero> ::= <digito> <numero> | <digito>
// <letra> ::= 'a' | 'b' | 'c' | 'd' | 'e' | 'f' | 'g' | 'h' | 'i' | 'j' | 'k' | 'l' |
//  'm' | 'n' | 'o' | 'p' | 'q' | 'r' | 's' | 't' | 'u' | 'v' | 'w' | 'x' |
//  'y' | 'z'
// <digito> ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'

#include "GrammarRecognizer.h"

//#define LOG_DEBUG
char* g_input = NULL;
long g_charCount = 0;
long g_currentLine = 1;


void Error(const char* message)
{
    fprintf(stderr, "Falha na derivacao.\n Erro na linha %ld: ", g_currentLine);
    fprintf(stderr, "%s\n", message);
    exit(1);
}

// File && String Operations
long GetFileSize(FILE* fp)
{
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp) + 1; // numberOfBytes + '\0'
    fseek(fp, 0, SEEK_SET);
    return size;
}

char* PutFileContentIntoString(FILE* fp, long charCount)
{
    char* input = (char*) calloc(charCount, sizeof(char));
    
    int i = 0;
    while(!feof(fp))
    {   
        int result = fscanf(fp, "%c", &input[i]);
        if (!result)
        {
            fprintf(stderr, "ERRO AO LER ARQUIVO!\n");
            exit(1);
        }
        i++;
    }
    return input;
}

FILE* OpenFileAndCheck(const char* fileName)
{
    FILE* fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }
    return fp;
}

// Grammar Functions

/// <summary>
/// Returns the number of characters to skip
/// </summary>
/// <returns>Number of characters we should skip</returns>
int GetNumberOfCharactersToSkip()
{
    int isLineBreak = *g_input == '\n';
    int isEscape = *g_input == '\r';
    int isTab = *g_input == '\t';
    int isSpace = *g_input == ' ';
    int isComment = *g_input == '/' && *(g_input + 1) != '\0' && *(g_input + 1) == '/';
    
    if (isLineBreak) g_currentLine++;
    if (isComment) return 2;
    return isLineBreak || isEscape || isTab || isSpace || isComment;

}

/// <summary>
/// Goes to the next terminal given certain conditions
/// </summary>
void NextTerminal()
{
    int numberOfCharactersToSkip = GetNumberOfCharactersToSkip();
    while (numberOfCharactersToSkip)
    {
        #ifdef LOG_DEBUG
            printf("~> [DEBUG] JUMPING CHARACTER: [%c]\n", *g_input);
            printf("\t\t Current Value: {%s}\n", g_input);
        #endif
        g_input += numberOfCharactersToSkip;
        if (numberOfCharactersToSkip == 2)
        {
            while (*g_input != '\n' && *g_input != '\0')
            {
                g_input++;
            }
        }
        numberOfCharactersToSkip = GetNumberOfCharactersToSkip();
    }
}


// Derivacoes
void Variavel()
{
    if (isalpha(*g_input))
    {
        g_input++;
        NextTerminal();
    }
    else
    {
        Error("Esperava variavel");
    }
    if (isalpha(*g_input))
    {
        Variavel();
    }
}
void Declaracao()
{
    switch (*g_input)
    {
    case 'i':
        g_input++;
        NextTerminal();
        if (*g_input == 'n')
        {
            g_input++;
            NextTerminal();
            if (*g_input == 't')
            {
                g_input++;
                NextTerminal();
                Variavel();
                if (*g_input == ';')
                {
                    g_input++;
                    NextTerminal();
                    break;
                }
                else Error("Esperava um ponto e virgula apos a variavel");
            }
            else Error("Esperava a palavra 'int'");
        }
        else Error("Esperava a palavra 'int'");
    default:
        Error("Comando invalido");
    }
}
void Declaracoes()
{
    // While the 3 first digits are equal to "int", we have a declaration
    if (strncmp(g_input, "int", 3) == 0)
    {
        Declaracao();
    }
    else
    {
        Error("Esperava declaracao");
    }
    while (strncmp(g_input, "int", 3) == 0)
        Declaracoes();
}


void Digito()
{
    if (isdigit(*g_input))
    {
        g_input++;
        NextTerminal();
    }
    else
    {
        Error("Esperava digito");
    }
}

void Numero()
{
    Digito();
    if (isdigit(*g_input))
    {
        Numero();
    }
}
void Fator()
{
    if (*g_input == '(')
    {
        g_input++;
        NextTerminal();
        Expressao();
        if (*g_input == ')')
        {
            g_input++;
            NextTerminal();
        }
        else
        {
            Error("Esperava um ')' apos a abertura de um '('");
        }
    }
    else
    {
        Numero();
    }
}
void Termo()
{
    Fator();
    while (*g_input == '*' || *g_input == '/')
    {
        g_input++;
        NextTerminal();
        Fator();
    }
}
void Expressao()
{
    if (*g_input == '+' || *g_input == '-')
    {
        g_input++;
        NextTerminal();
    }
    Termo();
    while (*g_input == '+' || *g_input == '-')
    {
        g_input++;
        NextTerminal();
        Termo();
    }
}
void Atribuicao()
{
    Variavel();
    if (*g_input == '=')
    {
        g_input++;
        NextTerminal();
        Expressao();
    }
    else
    {
        Error("Esperava expressao apos declaracao");
    }
}
void Comandos()
{
    Atribuicao();
    while (*g_input == ';')
    {
        g_input++;
        NextTerminal();
        Atribuicao();
    }
}
void Programa()
{
    Declaracoes();
    Comandos();
}
// Fim Derivacoes

void StartRecognizer()
{
    if (g_input == NULL)
    {
        fprintf(stderr, "ERRO AO ABRIR ARQUIVO!\n");
        exit(1);
    }
    // Start Analyzing
    NextTerminal();
    Programa();
    
    // Finishes
    if (g_input != NULL && *g_input == '\0')
    {
        printf("Palavra reconhecida.\n ~> %ld Linha(s) Analizadas. \n", g_currentLine);
    }
    else
    {
        fprintf(stderr, "Falha na derivacao.\n Erro na linha %ld", g_currentLine);
    }
}



int main(int argc, char** argv)
{
    const char* fileName = argc == 2 ? argv[1] : "input.txt";
    FILE* fp = OpenFileAndCheck(fileName);
    
    g_charCount = GetFileSize(fp);
    g_input = PutFileContentIntoString(fp, g_charCount);
    printf("Numero de Bytes: %ld\n", g_charCount);
    printf("INPUT: \n```\n%s\n```\n", g_input);
    // Since we're going to move the input pointer to check if the grammar is valid, we MUST
    // storage a reference to the beggining of the input, otherwise, we can't free this memory block
    char* originalInputPointer = g_input;

    // Start Recognizing
    StartRecognizer();

    
    free(originalInputPointer);
    fclose(fp);
}
