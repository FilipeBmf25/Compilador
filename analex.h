/*
Universidade do Estado da Bahia
Alunos: Adriano da Silva Maur�cio
		Filipe Bomfim Santos Furtado
Disciplina: Compiladores
Professora: Maria In�s
*/

typedef enum PalavraReservada{
BOOL,	
CALL,
CHAR,
DISPLAY,
DUP,
ELSE,	
ENDFOR,
ENDFUNC,
ENDIF,
ENDPROC,
ENDPROG,
ENDVAR,
ENDWHILE,
FOR,
FWD,
IF,
INT,
KEYBOARD,
NDPROC,
NOPARAM,
PL,
PROC,
PROG,
REAL,
RETURN,
VAR,
WHILE} PALAVRARESERVADA;


/*
** Vetor de Palavras Reservadas da Linguagem
*/
char PalavrasReservadas[][12]=
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
	"endproc",
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
	"while"	
};

/*
** Enumerador para as Categorias 
*/
typedef enum Categoria{
CARAC_ESPEC,
ID,
INTCON,
REALCON,
PR,
OPREL,
OPARIT,
OPLOG,
CARACCON,
CADEIACON,
COMENTARIO,
DELIMITADOR,
FIM,
ERRO} CATEGORIA;

/*
** Vetor que atribui � Categoria o seu valor em formato String
*/
char Categorias[][12]=
{
	"CARAC_ESPEC",
	"ID",
	"INTCON",
	"REALCON",
	"PR",
	"OPREL",
	"OPARIT",
	"OPLOG",
	"CARACCON",
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
** Enumerador para os Operadores L�gicos 
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
** Vetor que atribui aos Delimitadores o seu valor em formato String
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
** Enumerador para os Operadores Aritm�ticos 
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


/*
** Struct para os tokens da Linguagem PL
*/
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
** Adequa��o da Busca Bin�ria Recursiva para verifica��o se o token � uma palavra reservada da Linguagem PL ou n�o
*/
int isPalavraReservada(char *buffer, int menor, int maior);

/*
** Chama a fun��o de Analisar Tokens para retornar um token v�lido para a linguagem
*/
Token AnalisadorLexico(FILE *fp);

/*
** Vetor de Strings para coment�rios e CADEIACON da Linguagem PL
*/
char stringsPL[100][56];
