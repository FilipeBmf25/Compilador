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
	"Token esperado nao atende as especificações de fator", //Erro 18
	"Token Invalido"
	}; 
	

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
