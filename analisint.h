char erros[][70] = {
	"0 - Programa sem erro", //Erro 0
	"Caracter invalido!", //Erro 1
 	"Formato de numero invalido!", //Erro 2
	"Esperado abre parenteses!", //Erro 3
	"Esperado fecha parenteses!", //Erro 4
	"Esperado abre colchetes!", //Erro 5
	"Esperado fecha colchetes!", //Erro 6
	"Operando invalido!", //Erro 7
	"Token esperado invalido - Esperado Palavra Reservada", //Erro 8
	"Token invalido - Palavra Reservada esperada incorreta", //Erro 9
	"Token esperado invalido - Esperado ID", //Erro 10
	"Token esperado invalido - Esperado Operador Aritmetico", //Erro 11
	"Token invalido - Operador Aritmetico esperado incorreto", //Erro 12
	"Token esperado invalido - Esperado Operador Logico", //Erro 13
	"Token invalido - Operador Logico esperado incorreto", //Erro 14
	"Token esperado invalido - Esperado Operador Relacional", //Erro 15
	"Token invalido - Operador Relacional esperado incorreto", //Erro 16
	"Token esperado invalido - Esperado Delimitador", //Erro 17
	"Token esperado nao atende as especificacoes de fator", //Erro 18
	"Token Invalido",//Erro 19
	"Token Invalido - esperando virgula",//Erro 20
	"Token Invalido apos programa"//Erro 21
	}; 
/*	
typedef enum Escopo_S{
LOCAL,
GLOBAL
} ESCOPO_S;	

typedef enum Tipo_S{
INT,
REAL,
BOOL,
CHAR
} TIPO_S;

typedef enum Categoria_S{
FUNCAO,
PROCEDIMENTO,
PARAMETRO,
VARIAVEL
} CATEGORIA_S;
	
typedef struct simbolo {
	char lexema[10];
	TIPO_S tipo;
	CATEGORIA_S categoria;
	ESCOPO_S escopo;
}Simbolo;

Simbolo tabela_simbolos[200];

void inserirTabela_S(char *s, ESCOPO_S escopo, TIPO_S tipo, CATEGORIA_S cat);

*/
	

void decl_var();
void tipo();
void getToken();
void proc();
void fwd();
void atrib();
void oprel();
void tipos_param();
void cmd();
void expr();
void expr_simp();
void termo();
void fator();
void op_rel();
void func();
void prog();
