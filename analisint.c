#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;
FILE *fp;

void getToken(){ // Retorna o pr�ximo Token do ANALEX
	tk=AnalisadorLexico(fp);
}

void Erro(int e) { // Fun��o de retorno de mensagem de erro
	if(e>0) system("color f4");
	else system("color f2");
	printf("Erro: %s\n", erros[e]);
	exit(e);
}

void prog(){ // Inicio da fun��o PROG 
	getToken();
	if((tk.cat==PR)&&(tk.valor.numInt==PL)){ // Compara o token retornado com o token obrigat�rio PL
	
	getToken(fp);
	if(tk.cat==ID){// Compara o token retornado com o token obrigat�rio ID
		getToken();
		if((tk.cat==PR)&&(tk.valor.numInt==VAR)){ // An�lise do token para verificar a exist�ncia de VAR no prog
			getToken();
			while(tk.valor.numInt!=ENDVAR){ // La�o para determinar se existe nenhuma,uma ou mais de uma declara��o de vari�vel
				tipo();
				getToken();
				decl_var();
				getToken();
				while(tk.valor.numInt==VIRG){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
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
		Erro(7); // Mensagem de erro caso n�o exista ID
	}
	
	}else {
		Erro(7); // Mensagem de erro caso n�o exista PL
	}
	
	
}
							
void decl_var(){ // Fun��o para comparar se o token � um ID
	if(tk.cat!=ID) Erro(7); // Mensagem de erro caso n�o for ID
}		

void tipo(){ // Fun��o para determinar se o token � uma palavra reservada CHAR, INT, REAL OU BOOL
	if(tk.cat==PR){
		if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(7);
	}else {
		Erro(7); // Mensagem de erro caso n�o seja nenhuma das 4 palavras reservadas
	}	
}

void fwd(){ // Fun��o FWD ( pode come�ar com proc ou tipo())
     getToken();
     if((tk.cat==PR)&&(tk.valor.numInt==PROC)){ // Compara o token retornado com PROC
         getToken();                                                                     
     }else{
         tipo(); // Compara o token retornado com as exig�ncias da fun��o Tipo()
         getToken();
     } 
     if(tk.cat==ID){ // Compara se o pr�ximo Token � um ID
          getToken();
          if((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT)){ // Verfica a exist�ncia obrigat�ria de um abre par�nteses
                getToken();
                if((tk.cat==PR)&&(tk.valor.numInt==NOPARAM)){ // Verifica se n�o existe par�metro
                      getToken();                                      
                }else{
                      tipo(); // Compara o token retornado com as exig�ncias da fun��o Tipo()
                      getToken();
                      while(tk.valor.numInt==VIRG){ // Verifica se � uma VIRGULA para determinar mais tipos
                          getToken();
					      tipo(); // // Compara o token retornado com as exig�ncias da fun��o Tipo() ( NESTE CASO J� FOI DECLARADO PELO MENOS UMA VEZ )
					      getToken();
				      }
                } 
                if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)) Erro(4); // Compara de ap�s a defini��o do parametro, aparece o token obrigat�rio fecha par�nteses
                                                           
          }else Erro(3); // Mensagem de erro caso n�o insira o Abre par�nteses obrigat�rio         
     }else{
           Erro(7); // Mensagem de erro caso n�o exista ID
     }                                                                                   
}	

void func(){ // Inicio da fun��o func ()
	getToken();
	tipo();// Compara o token retornado com as exig�ncias da fun��o Tipo()
	getToken();
	if(tk.cat==ID){ // Compara se o pr�ximo Token � um ID
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){ // Verfica a exist�ncia obrigat�ria de um abre par�nteses
			Erro(3);	
		}else {
			tipos_param(); // Compara o token retornado com as exig�ncias da fun��o tipos_param()
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){ // Verfica a exist�ncia obrigat�ria de um fecha par�nteses
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDFUNC)&& // Compara o token retornado com as exig�ncias do while() ( ELES CONTENPLAM O INICIO DE UM CMD )
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo();// Compara o token retornado com as exig�ncias da fun��o Tipo()
						getToken();
						decl_var(); // Fun��o para comparar se o token � um ID
						getToken();
						while(tk.valor.numInt==VIRG){ // La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel do mesmo tipo
							getToken();
							decl_var(); // Fun��o para comparar se o token � um ID
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDFUNC){ // Verifica a exist�ncia obrig�torio do token ENDFUNC para fim de fun��o
				//	cmd();
					getToken();
				}
				
			}
		}
	}else{
		Erro(7); // Mensagem de erro caso n�o exista ID
	}
}	

void proc(){ // Inicio da fun��o proc ()
	getToken();
	if((tk.cat!=PR)||(tk.valor.numInt!=PROC)) Erro(7); // Compara o token retornado com proc
	getToken();
	if(tk.cat==ID){// Compara se o pr�ximo Token � um ID
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){// Verfica a exist�ncia obrigat�ria de um abre par�nteses
			Erro(3);	
		}else {
			tipos_param();// Compara o token retornado com as exig�ncias da fun��o tipos_param()
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){// Verfica a exist�ncia obrigat�ria de um fecha par�nteses
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDPROC)&& // Compara o token retornado com as exig�ncias do while() ( ELES CONTENPLAM O INICIO DE UM CMD )
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo(); // Compara o token retornado com as exig�ncias da fun��o Tipo()
						getToken();
						decl_var(); // Fun��o para comparar se o token � um ID
						getToken();
						while(tk.valor.numInt==VIRG){ // La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel do mesmo tipo
							getToken();
							decl_var();// Fun��o para comparar se o token � um ID
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDPROC){ // Verifica a exist�ncia obrig�torio do token ENDPROC para fim do procedimento
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

void tipos_param(){ // Inicio da fun��o tipo param
	getToken();
	if(tk.cat!=PR) Erro(7); // Verifica se o token � uma palavra reservada
	if(tk.valor.numInt!=NOPARAM){ // Verifica se a palavra reservada � noparam
		tipo(); // Caso n�o seja noparam, verifica se contempla as exig�ncias da fun��o tipo()
		getToken();
		if(tk.cat!=ID) Erro(7); // Verifica a exist�ncia obrigat�ria de um ID
		getToken();
		while(tk.valor.numInt==VIRG){  // la�o para verificar a exist�ncia de mais de uma declara��o de tipo
				getToken();
				tipo(); // Compara o token retornado com as exig�ncias da fun��o Tipo()
				getToken();
				if(tk.cat!=ID) Erro(7); // Verifica a exist�ncia obrigat�ria de um ID ap�s o tipo
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
