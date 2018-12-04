#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;
Token tk_next;
FILE *fp;

void getToken(){ // Retorna o pr�ximo Token do ANALEX
	tk = tk_next;
	tk_next=AnalisadorLexico(fp);
}

void Erro(int e) { // Fun��o de retorno de mensagem de erro
	if(e>0) system("color f4");
	else system("color f2");
	printf("Erro: %s\n", erros[e]);
	exit(e);
}

void prog(){
	getToken();
	if(!(tk.cat==PR))Erro(8);
	if(!(tk.valor.numInt==PL)) Erro(9);// Compara o token retornado com o token obrigat�rio PL
		getToken();
		if(tk.cat!=ID) Erro(10);// Compara o token retornado com o token obrigat�rio ID
			getToken();
			if((tk.cat==PR)&&(tk.valor.numInt==VAR)){ // An�lise do token para verificar a exist�ncia de VAR no prog
				while(tk_next.valor.numInt!=ENDVAR){ // La�o para determinar se existe nenhuma,uma ou mais de uma declara��o de vari�vel
						tipo();
						decl_var();	
						while(tk_next.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
							getToken();
							decl_var();
						}
				}
									
			}
}

void decl_var(){ // Fun��o para comparar se o token � um ID
	getToken();
	if(tk.cat!=ID) Erro(10); // Mensagem de erro caso n�o for ID
}	

void tipo(){ // Fun��o para determinar se o token � uma palavra reservada CHAR, INT, REAL OU BOOL
	getToken();
	if(!(tk.cat==PR)) Erro(8); // Mensagem de erro caso n�o seja nenhuma das 4 palavras reservadas
	if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(9);
	
}

void tipos_param(){
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {
		tipo();
		getToken();
		if(!(tk.cat==ID)) Erro(10);
		while(tk_next.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
			getToken();
			tipo();
			getToken();
			if(!(tk.cat==ID)) Erro(10);
		}
	}else{
		getToken();
	}
}

void fwd(){
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==FWD)) Erro(9);
	if(tk_next.valor.numInt==PROC){
		getToken();
	}else{
		tipo();
	}
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!(tk.valor.numInt==A_PARENT)) Erro(3);
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {
		tipo();
		while(tk_next.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
			getToken();
			tipo();
		}
	}else{
		getToken();
	}
	getToken();
	if(!(tk.valor.numInt==F_PARENT)) Erro(4);
}

void func(){
	tipo();
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!(tk.valor.numInt==A_PARENT)) Erro(3);
	tipos_param();
	getToken();
	if(!(tk.valor.numInt==F_PARENT)) Erro(4);
	while((tk_next.valor.numInt==CHAR)||
		 (tk_next.valor.numInt==INT)||
		 (tk_next.valor.numInt==REAL)||
		 (tk_next.valor.numInt==BOOL)){
			 
			  tipo();
			  decl_var();	
		  	  while(tk_next.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
				  getToken();
				  decl_var();
		  }
	}
	
	while(tk_next.valor.numInt!=ENDFUNC){
		cmd();
	}
	
	getToken();	
}

void proc(){
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==PROC)) Erro(9);
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!(tk.valor.numInt==A_PARENT)) Erro(3);
	tipos_param();
	getToken();
	if(!(tk.valor.numInt==F_PARENT)) Erro(4);
	while((tk_next.valor.numInt==CHAR)||
		 (tk_next.valor.numInt==INT)||
		 (tk_next.valor.numInt==REAL)||
		 (tk_next.valor.numInt==BOOL)){
			 
			  tipo();
			  decl_var();	
		  	  while(tk_next.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
				  getToken();
				  decl_var();
		  }
	}
	
	while(tk_next.valor.numInt!=ENDPROC){
		cmd();
	}
	
	getToken();	
}

void cmd(){
	
}

int main(int argc, char *argv[]) {
	int linha=0; 
	printf("[ANALEX LINGUAGEM PL]\n\n");
	system("color f0");
	if((fp = fopen("Editor Linguagem PL.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	tk_next=AnalisadorLexico(fp);
	
	func();
	Erro(0);
	fclose(fp);
	
	return 0;
}
