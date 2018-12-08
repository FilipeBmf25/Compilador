#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;
Token tk_next;
FILE *fp;
int linha=1;

void getToken(){ // Retorna o próximo Token do ANALEX
	tk = tk_next;
	tk_next=AnalisadorLexico(fp);
	while((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR)){
		linha++;
		tk_next=AnalisadorLexico(fp);
		
	}
}

void Erro(int e) { // Função de retorno de mensagem de erro
	if(e>0) system("color f4");
	else system("color f2");
	printf("Erro na linha %d: %s\n",linha, erros[e]);
	exit(e);
}

void prog(){
	getToken();
	if(!((tk.cat==PR)&&(tk.valor.numInt==PL))) Erro(8);
	if(!((tk_next.cat==ID)))Erro(10);
	getToken();
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==VAR)){
		getToken();
		while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))||
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
		 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
		 	 	tipo();
		 	 	decl_var();
		 	 	while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
					getToken();
					decl_var();
				}
		}
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDVAR))) Erro(8);
		getToken();
	}
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==PROG))){
	
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==FWD)){
			fwd();
		}else if(((tk_next.cat==PR)&&(tk_next.valor.numInt==PROC))){
			proc();
		}else{
			func();
		}
		
	}
	getToken();
	cmd();
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDPROG))){
		cmd();
	}
	getToken();
				
}
// ======================================================FIM DE PROG=====================================================
void decl_var(){ // Função para comparar se o token é um ID
	getToken();
	if(tk.cat!=ID) Erro(10); // Mensagem de erro caso não for ID
}	

void tipo(){ // Função para determinar se o token é uma palavra reservada CHAR, INT, REAL OU BOOL
	getToken();
	if(!(tk.cat==PR)) Erro(8); // Mensagem de erro caso não seja nenhuma das 4 palavras reservadas
	if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(9);
	
}

void tipos_param(){
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {
		tipo();
		getToken();
		if(!(tk.cat==ID)) Erro(10);
		while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
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
		while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
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
		  	  while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
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
		  	  while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
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
// ======================================================INICIO DO IF=====================================================  
	if(tk_next.valor.numInt==IF){
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);
		getToken();
		expr();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);
		getToken();
		cmd();
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN))||
		   	  (tk_next.cat==ID)||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL))||
		   	  ((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY))){
		   	
		   		cmd();	
		   }
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==ELSE)){
			getToken();
			cmd();
			while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE))||
		  	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN))||
		   	  (tk_next.cat==ID)||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL))||
		   	  ((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY))){
		   	
		   		cmd();	
		   }	
		}
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDIF))) Erro(8);
		getToken();
		
 // ======================================================FIM DO IF=====================================================  
 
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE)){
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);
		getToken();
		expr();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);
		getToken();
		cmd();
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN))||
		   	  (tk_next.cat==ID)||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL))||
		   	  ((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY))){
		   	
		   		cmd();	
		   }
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDWHILE))) Erro(8);
		getToken();
// ======================================================FIM DO WHILE=====================================================	
	}else if(tk_next.valor.numInt==FOR){
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);
		getToken();
		if(tk_next.cat==ID){
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
			}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);
		getToken();
		if(tk_next.cat==ID){
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);
		getToken();
		cmd();
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN))||
		   	  (tk_next.cat==ID)||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL))||
		   	  ((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD))||
		   	  ((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY))){
		   	
		   		cmd();	
		}
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDFOR))) Erro(8);
		getToken();
// ======================================================FIM DO FOR=====================================================			
	
	}else if(tk_next.valor.numInt==RETURN){
		getToken();
		if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){
			getToken();
			expr();
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))	Erro(4);	
			getToken();		
		}
		
// ======================================================FIM DO RETURN====================================================	
	}else if(tk_next.cat==ID) {
		atrib();
		
// ======================================================FIM DO ATRIB====================================================	
	}else if(tk_next.valor.numInt==CALL){
		getToken();
		if(!(tk_next.cat==ID)) Erro(10);
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)))Erro(3);
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
				  getToken();
				  expr();
		  	  }
				
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))Erro(4);
		getToken();
// ======================================================FIM DO CALL====================================================		
	}else if(tk_next.valor.numInt==PT_VIRG){
		getToken();
		
//======================================================FIM DO PT_VIRG====================================================	
	}else if(tk_next.valor.numInt==KEYBOARD){
		getToken();
		if(!(tk_next.cat==ID))Erro(10);
		while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
			getToken();
			if(!(tk_next.cat==ID))Erro(10);
			getToken();
		}

//======================================================FIM DO KEYBOARD====================================================		  		
	}else if (tk_next.valor.numInt==DISPLAY){
		getToken();
		if	(!((tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)||
			(tk_next.cat==CADEIACON))) Erro(19);
		
		getToken();
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);
			getToken();
		}
		while(tk_next.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
			getToken();
			if	(!((tk_next.cat==ID)||
				(tk_next.cat==INTCON)||
				(tk_next.cat==REALCON)||
				(tk_next.cat==CARACCON)||
				(tk_next.cat==CADEIACON))) Erro(19);
			getToken();
			if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);
			getToken();
			}	
		}
		
//======================================================FIM DO DISPLAY====================================================			
	}else Erro(19);
}

void atrib(){
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();
	if(!(tk.cat==OPREL)) Erro(16);
	if(!(tk.valor.numInt==IGUAL)) Erro(17);
	expr();
}

void expr(){
	expr_simp();
	if(tk_next.cat==OPREL){
		op_rel();
		expr_simp();
	}	
}

void expr_simp(){
	if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))){
		getToken();
	}
	termo();
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==OR))){
			getToken();
			termo();
	}
}

void termo(){
	fator();
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MULTIPLICACAO))||
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==DIVISAO))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==AND))){
			getToken();
			fator();
	}
}

void fator(){
	if(tk_next.cat==ID){
		getToken();
		if((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==A_PARENT)){
			getToken();
			if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
						getToken();
						expr();
				}		
			}
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);
			getToken();
		}
	}else if(tk_next.cat==INTCON) getToken();
		else if(tk_next.cat==REALCON) getToken();
		else if(tk_next.cat==CARACCON) getToken();
		else if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){
			getToken();
			expr();
			getToken();
			if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);	
		}else if((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT)){
			getToken();
			fator();
		}else Erro(18);		
}

void op_rel(){
	getToken();
	if(!(tk.cat==OPREL)) Erro(15);
	if(tk.valor.numInt==IGUAL) Erro(16);
}

int main(int argc, char *argv[]) {
	int linha=0; 
	printf("[ANALEX LINGUAGEM PL]\n\n");
	system("color f0");
	if((fp = fopen("teste.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	tk_next=AnalisadorLexico(fp);
	
	
	cmd();
	printf("\nPrograma Compilado sem Erros");
	fclose(fp);
	
	return 0;
}
