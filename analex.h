/*
** Enumerador para as Categorias 
*/
typedef enum Categoria{
ID,
INTCON,
REALCON,
PR,
OPREL,
OPARIT,
OPLOG,
CR,
CARACON,
ERRO} CATEGORIA;

/*
** Vetor que atribui à Categoria o seu valor em formato String
*/
char Categorias[][10]=
{
	"ID",
	"INTCON",
	"REALCON",
	"PR",
	"OPREL",
	"OPARIT",
	"OPLOG",
	"CR",
	"CARACON",
	"ERRO"
};

typedef enum OP_REL{
MAIOR,
MENOR,
IGUAL,
IGUALIGUAL,
MAIORIGUAL,
MENORIGUAL,
DIFERENTE
} OPERADORESRELACIONAIS;

/*
** Vetor que atribui aos Operadores Relacionais o seu valor em formato String
*/
char OperadoresRelacionais[][12]=
{
	"MAIOR",
	"MENOR",
	"IGUAL",
	"IGUALIGUAL",
	"MAIORIGUAL",
	"MENORIGUAL",
	"DIFERENTE"
};

typedef enum OP_LOG{
	AND,
	OR,
	NOT
}OPERADORESLOGICOS;

/*
** Vetor que atribui aos Operadores Logicos o seu valor em formato String
*/
char OperadoresLogicos[][6]=
{
	"AND",
	"OR",
	"NOT"
};

typedef enum OP_ARIT{
	MAIS,
	MENOS,
	MULTIPLICACAO,
	DIVISAO
}PALAVRASRESERVADAS;

/*
** Vetor que atribui aos Operadores Aritmeticos o seu valor em formato String
*/
char OperadoresAritmeticos[][13]=
{
	"MAIS",
	"MENOS",
	"MULTIPLICACAO",
	"DIVISAO"
};

/*
** Vetor de Palavras Reservadas da Linguagem
*/
char PalavrasReservadas[][10]=
{
	"bool",	
	"call",
	"char",
	"display",
	"dup",
	"else",	
	"endfor",
	"endfunc",
	"endif",
	"endprog",
	"endvar",
	"endwhile",
	"for",
	"fwd",
	"if",
	"int",
	"keyboard",
	"ndproc",
	"noparam",
	"pl",
	"proc",
	"prog",
	"real",
	"return",
	"var",
	"while",
	
};

typedef struct token {
	CATEGORIA cat;
	union valor {
		int numInt;
		float numFloat;
		char c;
		char s[10];
	}valor;
}Token;


/*
** Retorna um inteiro, onde (1) é uma Palavra Reservada e (0) não é
*/
int isPalavraReservada(char *buffer, int menor, int maior);

/*
** Chama a função de Analisar Tokens para retornar um token válido para a linguagem
*/
Token AnalisadorLexico(FILE *fp);


