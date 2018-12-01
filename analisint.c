#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;
FILE *fp;

void getToken(){ // Retorna o próximo Token do ANALEX
	tk=AnalisadorLexico(fp);
}

void Erro(int e) { // Função de retorno de mensagem de erro
	if(e>0) system("color f4");
	else system("color f2");
	printf("Erro: %s\n", erros[e]);
	exit(e);
}

void prog(){ // Inicio da função PROG 
	getToken();
	if((tk.cat==PR)&&(tk.valor.numInt==PL)){ // Compara o token retornado com o token obrigatório PL
	
	getToken(fp);
	if(tk.cat==ID){// Compara o token retornado com o token obrigatório ID
		getToken();
		if((tk.cat==PR)&&(tk.valor.numInt==VAR)){ // Análise do token para verificar a existência de VAR no prog
			getToken();
			while(tk.valor.numInt!=ENDVAR){ // Laço para determinar se existe nenhuma,uma ou mais de uma declaração de variável
				tipo();
				getToken();
				decl_var();
				getToken();
				while(tk.valor.numInt==VIRG){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
					getToken();
					decl_var();
					getToken();
				}				
			}
		}
		/*
		while((tk.cat==PR)&&(tk.valor.numInt!=PROG)){
			if((tk.cat==PR)&&(tk.valor.numInt==FWD) {
				getToken(fp);
				if((tk.cat==PR)&&(tk.valor.numInt==PROC) proc(fp);
			}
			if((tk.cat==PR)&&(tk.valor.numInt==PROC) proc(fp);
		}
		*/		
		
		
	}else{
		Erro(7); // Mensagem de erro caso não exista ID
	}
	
	}else {
		Erro(7); // Mensagem de erro caso não exista PL
	}
	
	
}
							
void decl_var(){ // Função para comparar se o token é um ID
	if(tk.cat!=ID) Erro(7); // Mensagem de erro caso não for ID
}		

void tipo(){ // Função para determinar se o token é uma palavra reservada CHAR, INT, REAL OU BOOL
	if(tk.cat==PR){
		if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(7);
	}else {
		Erro(7); // Mensagem de erro caso não seja nenhuma das 4 palavras reservadas
	}	
}

void fwd(){ // Função FWD ( pode começar com proc ou tipo())
     getToken();
     if((tk.cat==PR)&&(tk.valor.numInt==PROC)){ // Compara o token retornado com PROC
         getToken();                                                                     
     }else{
         tipo(); // Compara o token retornado com as exigências da função Tipo()
         getToken();
     } 
     if(tk.cat==ID){ // Compara se o próximo Token é um ID
          getToken();
          if((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT)){ // Verfica a existência obrigatória de um abre parênteses
                getToken();
                if((tk.cat==PR)&&(tk.valor.numInt==NOPARAM)){ // Verifica se não existe parâmetro
                      getToken();                                      
                }else{
                      tipo(); // Compara o token retornado com as exigências da função Tipo()
                      getToken();
                      while(tk.valor.numInt==VIRG){ // Verifica se é uma VIRGULA para determinar mais tipos
                          getToken();
					      tipo(); // // Compara o token retornado com as exigências da função Tipo() ( NESTE CASO JÁ FOI DECLARADO PELO MENOS UMA VEZ )
					      getToken();
				      }
                } 
                if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)) Erro(4); // Compara de após a definição do parametro, aparece o token obrigatório fecha parênteses
                                                           
          }else Erro(3); // Mensagem de erro caso não insira o Abre parênteses obrigatório         
     }else{
           Erro(7); // Mensagem de erro caso não exista ID
     }                                                                                   
}	

void func(){ // Inicio da função func ()
	getToken();
	tipo();// Compara o token retornado com as exigências da função Tipo()
	getToken();
	if(tk.cat==ID){ // Compara se o próximo Token é um ID
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){ // Verfica a existência obrigatória de um abre parênteses
			Erro(3);	
		}else {
			tipos_param(); // Compara o token retornado com as exigências da função tipos_param()
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){ // Verfica a existência obrigatória de um fecha parênteses
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDFUNC)&& // Compara o token retornado com as exigências do while() ( ELES CONTENPLAM O INICIO DE UM CMD )
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo();// Compara o token retornado com as exigências da função Tipo()
						getToken();
						decl_var(); // Função para comparar se o token é um ID
						getToken();
						while(tk.valor.numInt==VIRG){ // Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel do mesmo tipo
							getToken();
							decl_var(); // Função para comparar se o token é um ID
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDFUNC){ // Verifica a existência obrigátorio do token ENDFUNC para fim de função
				//	cmd();
					getToken();
				}
				
			}
		}
	}else{
		Erro(7); // Mensagem de erro caso não exista ID
	}
}	

void proc(){ // Inicio da função proc ()
	getToken();
	if((tk.cat!=PR)||(tk.valor.numInt!=PROC)) Erro(7); // Compara o token retornado com proc
	getToken();
	if(tk.cat==ID){// Compara se o próximo Token é um ID
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){// Verfica a existência obrigatória de um abre parênteses
			Erro(3);	
		}else {
			tipos_param();// Compara o token retornado com as exigências da função tipos_param()
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){// Verfica a existência obrigatória de um fecha parênteses
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDPROC)&& // Compara o token retornado com as exigências do while() ( ELES CONTENPLAM O INICIO DE UM CMD )
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo(); // Compara o token retornado com as exigências da função Tipo()
						getToken();
						decl_var(); // Função para comparar se o token é um ID
						getToken();
						while(tk.valor.numInt==VIRG){ // Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel do mesmo tipo
							getToken();
							decl_var();// Função para comparar se o token é um ID
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDPROC){ // Verifica a existência obrigátorio do token ENDPROC para fim do procedimento
				//	cmd();
					getToken();
				}
				
			}
		}
	}else{
		Erro(7);
	}
}

void atrib(){
	getToken();
	if(!(tk.cat==ID))Erro(7);
	getToken();
	if(!((tk.cat==OPREL)&&(tk.valor.numInt==IGUAL)))Erro(7);
	//expr(fp);
}

void oprel(){
	getToken();
	if(!(tk.cat==OPREL)) Erro(7);
	if(!((tk.valor.numInt==IGUALIGUAL)||(tk.valor.numInt==DIFERENTE)||(tk.valor.numInt==MENORIGUAL)||(tk.valor.numInt==MAIORIGUAL)||
	(tk.valor.numInt==MENORIGUAL)||(tk.valor.numInt==MENOR)||(tk.valor.numInt==MAIOR))) Erro(7);
}

void tipos_param(){ // Inicio da função tipo param
	getToken();
	if(tk.cat!=PR) Erro(7); // Verifica se o token é uma palavra reservada
	if(tk.valor.numInt!=NOPARAM){ // Verifica se a palavra reservada é noparam
		tipo(); // Caso não seja noparam, verifica se contempla as exigências da função tipo()
		getToken();
		if(tk.cat!=ID) Erro(7); // Verifica a existência obrigatória de um ID
		getToken();
		while(tk.valor.numInt==VIRG){  // laço para verificar a existência de mais de uma declaração de tipo
				getToken();
				tipo(); // Compara o token retornado com as exigências da função Tipo()
				getToken();
				if(tk.cat!=ID) Erro(7); // Verifica a existência obrigatória de um ID após o tipo
				getToken();
		}
		
	}
}

int main(int argc, char *argv[]) {
	
	int linha=0; 
	
	printf("[ANALEX LINGUAGEM PL]\n\n");
	
	system("color f0");

	if((fp = fopen("Editor Linguagem PL.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	proc();
	Erro(0);
	

	fclose(fp);
	
	return 0;
}
