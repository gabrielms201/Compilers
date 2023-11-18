// Ricardo Gabriel Marques dos Santos Ruiz - 32134908
// Gabriel Augusto Ribeiro Gomes - 32134762

#include "MiniLexicalAndSintaxRecognizer.h"
#include "LinkedList.h"

// Global
// #define LOG_DEBUG
char* g_input = NULL;
long g_charCount = 0;
long g_currentLine = 1;
unsigned int g_address = 0;
Node* g_symbolTable = NULL;
Token lookahead;
const int MAX_ID_LENGTH = 15;


// Semantic
int g_currentLabel = 0;
int g_varDeclaration = 0;
TokenInfo g_tokenInfo;


char strToken[][30] =
{
	"ERRO LEXICO",
	"IDENTIFICADOR",
	"ALGORITMO",
	";",
	".",
	"FIM DA ANALISE SINTATICA",
	"VARIAVEL",
	":",
	"TIPO",
	"TIPO",
	",",
	"INICIO",
	"FIM",
	"SE",
	"ENQUANTO",
	"LEIA",
	"ESCREVA",
	"(",
	")",
	"<",
	"<=",
	"=",
	"#",
	">",
	">=",
	"+",
	"-",
	"OU",
	"*",
	"DIV",
	"e",
	"VERDADEIRO",
	"FALSO",
	"NUMERO",
	":=",
	"FACA",
	"COMENTARIO",
	"COMENTARIO MULTILINHA",
	"ENTAO",
	"SENAO"
};

// We can use this KeyValue Array in order to simple storage reserved words and they respective 
// Token and Content
ReservedWord reservedWords[][100] = {
	{"algoritmo", ALGORITMO},
	{"variavel", VARIAVEL},
	{"inteiro", INTEIRO},
	{"logico", LOGICO},
	{"inicio", INICIO},
	{"fim", FIM},
	{"se", SE},
	{"enquanto", ENQUANTO},
	{"leia", LEIA},
	{"escreva", ESCREVA},
	{"ou", OU},
	{"div", DIV},
	{"e", E},
	{"verdadeiro", VERDADEIRO},
	{"falso", FALSO},
	{"faca", FACA},
	{"entao", ENTAO},
	{"senao", SENAO},
	{"", ERRO} // To check if we have reached the end of the array
};

// Utils
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
	char* input = (char*)calloc(charCount, sizeof(char));

	int i = 0;
	while (!feof(fp))
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

// Lexical Analyzer

/// <summary>
/// Returns the number of characters to skip
/// </summary>
/// <returns>Number of characters we should skip</returns>
int ShouldSkipOneChar()
{
	int isLineBreak = *g_input == '\n';
	int isEscape = *g_input == '\r';
	int isTab = *g_input == '\t';
	int isSpace = *g_input == ' ';

	if (isLineBreak) g_currentLine++;
	return isLineBreak || isEscape || isTab || isSpace;
}


/// <summary>
/// Goes to the next terminal given certain conditions
/// </summary>
TokenInfo NextTerminal()
{
	TokenInfo info;
	info.Token = ERRO;
	
	int shouldSkip = ShouldSkipOneChar();
	while (shouldSkip)
	{
		g_input++;
		shouldSkip = ShouldSkipOneChar();
	}
	int currentLine = g_currentLine;

	int isMultiCommentary = *g_input == '/' && *(g_input + 1) != '\0' && *(g_input + 1) == '*';
	if (isMultiCommentary)
	{
		int finishedCommentary = (*g_input == '*' && *(g_input + 1) != '\0' && *(g_input + 1) == '/');
		while (!finishedCommentary)
		{
			if (*g_input == '\0')
			{
				info.Token = ERRO;
				return info;
			}
			info.Token = MULTI_COMMENTARY;
			g_input++;
			finishedCommentary = (*g_input == '*' && *(g_input + 1) != '\0' && *(g_input + 1) == '/');
			if (*g_input == '\n') g_currentLine++;
		}
		g_input+=2;
	}
	int isCommentary = *g_input == '/' && *(g_input + 1) != '\0' && *(g_input + 1) == '/';
	if (isCommentary)
	{
		g_input += 2;
		while (*g_input != '\n' && *g_input != '\0')
		{
			g_input++;
		}
		info.Token = COMMENTARY;
	}


	info.Line = currentLine;
	return info;
}

TokenInfo GetToken()
{
	TokenInfo info = NextTerminal();

	if (info.Token == COMMENTARY || info.Token == MULTI_COMMENTARY)
	{
		return info;
	}

	// First we need to check if we can fit a word
	if (isalpha(*g_input))
	{
		info = CheckID();
		TokenInfo reservedKeyWordCheck = CheckReservedWord(info.ID);
		if (reservedKeyWordCheck.Token != ERRO)
		{
			info = reservedKeyWordCheck;
		}
	}
	// Then we check if we can fit a number
	else if (isdigit(*g_input))
	{
		info = CheckNumber();
	}
	// otherwise, we check for the other atoms
	else if (*g_input == ';')
	{
		info.Token = SEMILICON;
		g_input++;
	}
	else if (*g_input == '.')
	{
		info.Token = DOT;
		g_input++;
	}
	else if (*g_input == ':')
	{
		info.Token = COLON;
		g_input++;
		if (*g_input == '=')
		{
			info.Token = ASSIGN_SIGN;
			g_input++;
		}
	}
	else if (*g_input == ',')
	{
		info.Token = COMMA;
		g_input++;
	}
	else if (*g_input == '(')
	{
		info.Token = OPEN_PARENTHESES;
		g_input++;
	}
	else if (*g_input == ')')
	{
		info.Token = CLOSE_PARENTHESES;
		g_input++;
	}

	else if (*g_input == '<')
	{
		info.Token = LESSER;
		g_input++;
		if (*g_input == '=')
		{
			info.Token = LESSER_EQUAL;
			g_input++;
		}
	}
	else if (*g_input == '=')
	{
		info.Token = EQUAL;
		g_input++;
	}
	else if (*g_input == '#')
	{
		info.Token = MARK;
		g_input++;
	}
	else if (*g_input == '>')
	{
		info.Token = GREATER;
		g_input++;
		if (*g_input == '=')
		{
			info.Token = GREATER_EQUAL;
			g_input++;
		}
	}
	else if (*g_input == '+')
	{
		info.Token = PLUS_SIGN;
		g_input++;
	}
	else if (*g_input == '-')
	{
		info.Token = MINUS_SIGN;
		g_input++;
	}
	else if (*g_input == '*')
	{
		info.Token = MULT_SIGN;
		g_input++;
	}
	else if (*g_input == '\0')
		info.Token = EOS;

	if (info.Token == ID && g_varDeclaration)
	{
		Node* declaredTokenNode = Find(g_symbolTable, info.ID);
		if (declaredTokenNode != NULL)
		{
			printf("Erro semantico: variavel %s ja declarada anteriormente na linha %d\n", info.ID, declaredTokenNode->data.Line);
			exit(1);
		}
		Add(&g_symbolTable, info);
	}
	return info;

}

TokenInfo CreateTokenInfo(char* initBuffer, Token token, char* input)
{
	TokenInfo info;
	info.Token = token;
	info.Line = g_currentLine;
	strncpy(info.ID, initBuffer, input - initBuffer);
	info.ID[input - initBuffer] = '\x0';
	info.Number = atof(info.ID);
	info.Address = g_address++;
	
	return info;
}
TokenInfo CheckNumber()
{
	char* initBuffer = g_input;
	TokenInfo tokenInfo;
	tokenInfo.Token = ERRO;
	tokenInfo.Line = g_currentLine;
	if (isdigit(*g_input))
	{
		g_input++;
		goto q1;
	}
	return tokenInfo;
q1:
	// If we have an "e", then we finish checking the digit and check the expoent
	if (*g_input == 'E' || *g_input == 'e')
	{
		g_input++;
		if (*g_input == '+' || *g_input == '-')
		{
			g_input++;
		}
		
		if (!isdigit(*g_input))
		{
			tokenInfo.Token = ERRO;
			return tokenInfo;
		}
		// since "digito" is a positive clojure, we use while statement to represent the automata
		while (isdigit(*g_input))
		{
			g_input++;
		} 
	}
	// if is a digit, we continue til we have an "E" or empty
	else if (isdigit(*g_input))
	{
		g_input++;
		goto q1;
	}
	tokenInfo = CreateTokenInfo(initBuffer, NUMBER, g_input);
	return tokenInfo;
}
TokenInfo CheckID()
{
	char* initBuffer = g_input;
    TokenInfo tokenInfo;
	tokenInfo.Line = g_currentLine;
	tokenInfo.Token = ERRO;
	int count = 0;
	if (isalpha(*g_input))
	{
		g_input++; count++;
		goto q1;
	}
		
	return tokenInfo;
q1:
	if (isalpha(*g_input) || isdigit(*g_input) || *g_input == '_')
	{
		g_input++; count++;
		goto q1;
	}
	tokenInfo = CreateTokenInfo(initBuffer, ID, g_input);
	if (count > MAX_ID_LENGTH)
		tokenInfo.Token = ERRO;
	return tokenInfo;
}
TokenInfo CheckReservedWord(char* originalId)
{
	for (int i = 0; reservedWords[i]->token != ERRO; i++)
	{
		ReservedWord* word = reservedWords[i];
		// if the id content's len is not the same as the desired reserved word, we are not able to find a 
		// reserved word
		if (strlen(originalId) != strlen(word->content))
			continue;
		// if we have a reserved word with the same value as the id content, 
		// we substitute the token from an id to a reserved keyword
		int result = strncmp(originalId, word->content, strlen(word->content));
		if (result == 0)
		{			
			char* initBuffer = originalId;
			int len = strlen(word->content);
			originalId += len;

			TokenInfo info = CreateTokenInfo(initBuffer, word->token, originalId);
			return info;
		}
	}
	TokenInfo none =
	{
		ERRO,
		0,
		0,
		""
	};
	return none;
}


// Semantic Analyzer
int SearchAddress(TokenInfo token)
{
	Node* node = Find(g_symbolTable, token.ID);
	if (node == NULL)
	{
		printf("ERRO SEMANTICO: Variavel %s nao declarada\n", token.ID);
		exit(1);
	}
	int address = node->data.Address;
	return address;
}
int NextLabel()
{
	return g_currentLabel++;
}
// Syntax Analyzer
void Consume(Token token)
{
	// Caso seja um comentario, vamos apenas para o proximo token
	while (lookahead == COMMENTARY || lookahead == MULTI_COMMENTARY)
	{
		TokenInfo tokenInfo = GetToken();
		lookahead = tokenInfo.Token;
	}
	if (lookahead == token)
	{

		TokenInfo tokenInfo = GetToken();
		lookahead = tokenInfo.Token;
		g_tokenInfo = tokenInfo;
		// Se o proximo lookahead for comentario, apenas pulamos
		while (lookahead == COMMENTARY || lookahead == MULTI_COMMENTARY)
		{
			TokenInfo tokenInfo = GetToken();
			lookahead = tokenInfo.Token;
		}
	}
	else
	{
		printf("#%03ld: Erro sintatico: esperado [%s], encontrado [%s]\n", g_currentLine, strToken[token], strToken[lookahead]);
		exit(1);
	}
}

// Derivations
void Program()
{
	Consume(ALGORITMO);
	Consume(ID);
	Consume(SEMILICON);
	printf("L%d:\tNADA\n", NextLabel());
	Scope();
	Consume(DOT);
}
void Scope()
{
	if (lookahead == VARIAVEL)
		VarDeclaration();
	CompoundCommand();

}
void VarDeclaration()
{
	g_varDeclaration = 1;
	Consume(VARIAVEL);
	do 
	{
		VarList();
		Consume(COLON);
		Type();
		Consume(SEMILICON);

	} while (lookahead == ID);
	g_varDeclaration = 0;
}
void CompoundCommand()
{
	Consume(INICIO);
	Command();
	while (lookahead == SEMILICON)
	{
		Consume(SEMILICON);
		Command();
	}
	Consume(FIM);
}
// <comando>
void Command()
{
	// First:
	//  <comando_atribuicao>		=> ID
	//	<comando_se>				=> SE
	//	<comando_enquanto>			=> ENQUANTO
	//	<comando_entrada>			=> LEIA
	//	<comando_saida>				=> ESCREVA
	//	<comando_composto>			=> INICIO
	
	switch (lookahead)
	{
	case ID:
		Assignment();
		break;
	case SE:
		If();
		break;
	case ENQUANTO:
		// TODO: TESTAR
		While();
		break;
	case LEIA:
		Input();
		break;
	case ESCREVA:
		Print();
		break;
	case INICIO:
		CompoundCommand();
		break;
	default:
		printf("#%03ld: Erro: esperado comando, encontrado [%s]\n", g_currentLine, strToken[lookahead]);
		exit(1);
		break;
	}
}
// <comando_se>
void If()
{
	Consume(SE);
	Consume(OPEN_PARENTHESES);
	Expression();
	Consume(CLOSE_PARENTHESES);
	Consume(ENTAO);
	Command();
	if (lookahead == SENAO)
	{
		Consume(SENAO);
		Command();
	}
}
// <comando_atribuicao>
void While()
{
	int l1 = NextLabel();
	int l2 = NextLabel();
	Consume(ENQUANTO);
	printf("L%d:\tNADA\n", l1);
	Consume(OPEN_PARENTHESES);
	Expression();
	Consume(CLOSE_PARENTHESES);
	printf("\tDSVF L%d\n", l2);
	Consume(FACA);
	Command();
	printf("\tDSVS L%d\n", l1);
	printf("L%d:\tNADA\n", l2);
}
// <comando_entrada>
void Input()
{
	Consume(LEIA);
	Consume(OPEN_PARENTHESES);
	VarList();
	Consume(CLOSE_PARENTHESES);
}
// <comando_atribuicao>
void Assignment()
{
	Consume(ID);
	Consume(ASSIGN_SIGN);
	Expression();
}
// <comando_saida>
void Print() 
{
	Consume(ESCREVA);
	Consume(OPEN_PARENTHESES);
	Expression();
	while (lookahead == COMMA)
	{
		Consume(COMMA);
		Expression();
	}
	Consume(CLOSE_PARENTHESES);
}
// <expressao>
void Expression() 
{
	SimpleExpression();
	if (lookahead == LESSER || lookahead == LESSER_EQUAL ||
		lookahead == EQUAL || lookahead == MARK || 
		lookahead == GREATER || lookahead == GREATER_EQUAL)
	{
		Relational();
		SimpleExpression();
	}
}
// <relacional>
void Relational()
{
	if (lookahead == LESSER)
		Consume(LESSER);
	else if (lookahead == LESSER_EQUAL)
		Consume(LESSER_EQUAL);
	else if (lookahead == MARK)
		Consume(MARK);
	else if (lookahead == EQUAL)
		Consume(EQUAL);
	else if (lookahead == GREATER)
		Consume(GREATER);
	else if (lookahead == GREATER_EQUAL)
		Consume(GREATER_EQUAL);
	else
	{
		printf("#%03ld: Erro: esperado relacional, encontrado [%s]\n", g_currentLine, strToken[lookahead]);
		exit(1);
	}
}
// <expressao_simples>
void SimpleExpression()
{
	// todo: confirmar que ta tudo ok
	if (lookahead == PLUS_SIGN)
		Consume(PLUS_SIGN);
	else if (lookahead == MINUS_SIGN)
		Consume(MINUS_SIGN);

	Term();
	while (lookahead == PLUS_SIGN || lookahead == MINUS_SIGN || lookahead == OU)
	{
		Token op = lookahead;
		if (lookahead == PLUS_SIGN)
			Consume(PLUS_SIGN);
		else if (lookahead == MINUS_SIGN)
			Consume(MINUS_SIGN);
		else if (lookahead == OU)
			Consume(OU);
		else
		{
			printf("#%03ld: Erro: esperado +,-, ou , encontrado [%s]\n", g_currentLine, strToken[lookahead]);
			exit(1);
		}
		Term();
		if (op == PLUS_SIGN)
		{
			printf("\tSOMA\n");
		}
		else if (op == MINUS_SIGN)
		{
			printf("\tSUBT\n");
		}
	}
}
// <termo>
void Term()
{
	Factor();
	while (lookahead == MULT_SIGN || lookahead == DIV || lookahead == E)
	{
		Token op = lookahead;
		if (lookahead == MULT_SIGN)
		{
			Consume(MULT_SIGN);
		}
		else if (lookahead == DIV)
		{
			Consume(DIV);
		}
		else if (lookahead == E)
		{
			Consume(E);
		}
		else
		{
			printf("#%03ld: Erro: esperado *,div, e, encontrado [%s]\n", g_currentLine, strToken[lookahead]);
			exit(1);
		}
		Factor();
		if (op == MULT_SIGN)
		{
			printf("\tMULT\n");
		}
		else if (op == DIV)
		{
			printf("\tDIVI\n");
		}
	}
}
// <fator>
void Factor()
{
	if (lookahead == ID)
	{
		int address = SearchAddress(g_tokenInfo);
		printf("\tCRVL %d\n", address);
		Consume(ID);
	}
	else if (lookahead == NUMBER)
	{
		printf("\tCRCT %d\n", g_tokenInfo.Number);
		Consume(NUMBER);
	}
	else
	{
		Consume(OPEN_PARENTHESES);
		Expression();
		Consume(CLOSE_PARENTHESES);
	}

}
// <lista_variavel>
void VarList() 
{
	Consume(ID);

	while (lookahead == COMMA)
	{
		Consume(COMMA);
		Consume(ID);
	}
}
// <tipo>
void Type()
{
	if (lookahead == INTEIRO)
		Consume(INTEIRO);
	else
		Consume(LOGICO);
}

// End Derivations


void Compile()
{
	if (g_input == NULL)
	{
		fprintf(stderr, "ERRO AO ABRIR ARQUIVO!\n");
		exit(1);
	}
	// Start Analyzing
	TokenInfo info = GetToken();
	lookahead = info.Token;
	Program();

	// Finishes
	Consume(EOS);
	if (g_input != NULL && *g_input == '\0')
	{
		printf("\n\n%ld linhas analisadas, programa sintaticamente correto!\n", g_currentLine);
	}
}

int main(int argc, char** argv)
{
	// We can take the file from the command line arguments
	// If we don't have the value, we can just use a default file name ("input.txt")
	const char* fileName = argc == 2 ? argv[1] : "input.txt";
	FILE* fp = OpenFileAndCheck(fileName);

	g_charCount = GetFileSize(fp);
	g_input = PutFileContentIntoString(fp, g_charCount);
	printf("Numero de Bytes: %ld\n", g_charCount);
	printf("INPUT: \n```\n%s\n```\n", g_input);
	// Since we're going to move the input pointer to check if the grammar is valid, we MUST
	// storage a reference to the beggining of the input, otherwise, we can't free this memory block
	char* originalInputPointer = g_input;


	printf("Conteudo da compilacao: \n");
	// Start Recognizing
	Compile();

	// Print Symbol Table
	printf("\n///\nLista de simbolos\n\n");
	PrintList(&g_symbolTable);


	free(originalInputPointer);
	fclose(fp);
}
