
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
CARACON,
CADEIACON,
COMENTARIO,
DELIMITADOR,
FIM,
ERRO} CATEGORIA;

/*
** Vetor que atribui à Categoria o seu valor em formato String
*/
char Categorias[][12]=
{
	"ID",
	"INTCON",
	"REALCON",
	"PR",
	"OPREL",
	"OPARIT",
	"OPLOG",
	"CARACON",
	"CADEIACON",
	"COMENTARIO",
	"DELIMIT",
	"ERRO"
};

/*
** Enumerador para os Operadores Relacionais 
*/
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

/*
** Enumerador para os Operadores Lógicos 
*/
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

/*
** Enumerador para os Caracteres \n e \0 
*/
typedef enum CARAC_ESPECIAIS{
	CR,
	NUL
}CARAC_ESPECIAIS;

/*
** Vetor que atribui aos Caracteres \n e \0 o seu valor em formato String
*/
char Especiais[][5]=
{
	"CR",
	"NUL"
};

/*
** Enumerador para os Delimitadores da Linguagem PL
*/
typedef enum DELIMITADORES{
	A_PARENT,
	F_PARENT,
	A_COLCH,
	F_COLCH,
	A_CHAVES,
	F_CHAVES,
	VIRG,
	PT_VIRG
}DELIMITADORES;

/*
** Vetor que atribui aos Caracteres \n e \0 o seu valor em formato String
*/
char Delimitadores[][15]=
{
	"A_PARENT",
	"F_PARENT",
	"A_COLCH",
	"F_COLCH",
	"A_CHAVES",
	"F_CHAVES",
	"VIRG",
	"PT_VIRG",
};

/*
** Enumerador para os Operadores Aritméticos 
*/
typedef enum OP_ARIT{
	MAIS,
	MENOS,
	MULTIPLICACAO,
	DIVISAO
}PALAVRASRESERVADAS;

/*
** Vetor que atribui aos Operadores Aritmeticos o seu valor em formato String
*/
char OperadoresAritmeticos[][15]=
{
	"MAIS",
	"MENOS",
	"MULTIPLICACAO",
	"DIVISAO"
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

/*
** Vetor de Strings para comentários e CADEIACON da Linguagem PL
*/
char stringsPL[100][56];
