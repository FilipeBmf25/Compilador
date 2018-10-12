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

typedef enum OP_REL{
MAIOR,
MENOR,
IGUAL,
IGUALIGUAL,
MAIORIGUAL,
MENORIGUAL,
DIFERENTE
} OPERADORESRELACIONAIS;

typedef enum OP_LOG{
	AND,
	OR,
	NOT
}OPERADORESLOGICOS;

typedef enum OP_ARIT{
	MAIS,
	MENOS,
	MULTIPLICACAO,
	DIVISAO
}PALAVRASRESERVADAS;


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
** Retorna a string correspondente ao enum de Categorias setado no token como inteiro
*/
char* getCategoria (int indice);

/*
** Retorna a string correspondente ao enum de Palavras Reservadas setado no token como inteiro
*/
char* getPalavraReservada(int indice);

/*
** Retorna a string correspondente ao enum de Operadores Aritmeticos setado no token como inteiro
*/
char* getOperadorAritmetico(int indice);

/*
** Retorna a string correspondente ao enum de Operadores Relacionais setado no token como inteiro
*/
char* getOperadorRelacional(int indice);

/*
** Retorna a string correspondente ao enum de Operadores Lógicos setado no token como inteiro
*/
char* getOperadorLogico(int indice);

/*
** Retorna um inteiro, onde (1) é uma Palavra Reservada e (0) não é
*/
int isPalavraReservada(char *buffer, int menor, int maior);

/*
** Chama a função de Analisar Tokens para retornar um token válido para a linguagem
*/
Token AnalisadorLexico(FILE *fp);


