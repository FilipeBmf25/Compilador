#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;

void getToken(FILE *fp){
	tk=AnalisadorLexico(fp);
}

void Erro(int e) {
	printf("Erro: %s\n", erros[e]);
	exit(e);
}

void prog(FILE *fp){
	getToken(fp);
	if((tk.cat==PR)&&(tk.valor.numInt==PL)){
	
	getToken(fp);
	if(tk.cat==ID){
		getToken(fp);
		if((tk.cat==PR)&&(tk.valor.numInt==VAR)){
			getToken(fp);
			while(tk.valor.numInt!=ENDVAR){
				tipo(fp);
				getToken(fp);
				decl_var(fp);
				getToken(fp);
				while(tk.valor.numInt==VIRG){
					getToken(fp);
					decl_var(fp);
					getToken(fp);
				}
				
			}

			
		}
	}else{
		// ERRO ID - Erro()
	}
	
	}else {
		// ERRO PL - Erro()
	}
	
	Erro(0);
}
							
void decl_var(FILE *fp){
	if(tk.cat!=ID) Erro(4); 
}		

void tipo(FILE *fp){
	if(tk.cat==PR){
		if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(4);
	}else {
		Erro(4);
	}	
}



int analisint(FILE *fp){
	
}

int main(int argc, char *argv[]) {
	FILE *fp;
	int linha=0; 
	
	printf("[ANALEX LINGUAGEM PL]\n\n");
	
	system("color f0");

	if((fp = fopen("Editor Linguagem PL.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	prog(fp);
	printf("\n%d.  ",linha);


	fclose(fp);
	
	return 0;
}
