#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ;
FILE *fp;

void getToken(){
	tk=AnalisadorLexico(fp);
}

void Erro(int e) {
	if(e>0) system("color f4");
	else system("color f2");
	printf("Erro: %s\n", erros[e]);
	exit(e);
}

void prog(){
	getToken();
	if((tk.cat==PR)&&(tk.valor.numInt==PL)){
	
	getToken(fp);
	if(tk.cat==ID){
		getToken();
		if((tk.cat==PR)&&(tk.valor.numInt==VAR)){
			getToken();
			while(tk.valor.numInt!=ENDVAR){
				tipo();
				getToken();
				decl_var();
				getToken();
				while(tk.valor.numInt==VIRG){
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
		Erro(7);
	}
	
	}else {
		Erro(7);
	}
	
	
}
							
void decl_var(){
	if(tk.cat!=ID) Erro(7); 
}		

void tipo(){
	if(tk.cat==PR){
		if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(7);
	}else {
		Erro(7);
	}	
}

void fwd(){
     getToken();
     if((tk.cat==PR)&&(tk.valor.numInt==PROC)){
         getToken();                                                                     
     }else{
         tipo();
         getToken();
     } 
     if(tk.cat==ID){
          getToken();
          if((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT)){
                getToken();
                if((tk.cat==PR)&&(tk.valor.numInt==NOPARAM)){
                      getToken();                                      
                }else{
                      tipo();
                      getToken();
                      while(tk.valor.numInt==VIRG){
                          getToken();
					      tipo();
					      getToken();
				      }
                } 
                if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)) Erro(4);
                                                           
          }else Erro(3);         
     }else{
           Erro(7);
     }                                                                                   
}	

void func(){
	getToken();
	tipo();
	getToken();
	if(tk.cat==ID){
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){
			Erro(3);	
		}else {
			tipos_param();
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDFUNC)&&
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo();
						getToken();
						decl_var();
						getToken();
						while(tk.valor.numInt==VIRG){
							getToken();
							decl_var();
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDFUNC){
				//	cmd();
					getToken();
				}
				
			}
		}
	}else{
		Erro(7);
	}
}	

void proc(){
	getToken();
	if((tk.cat!=PR)||(tk.valor.numInt!=PROC)) Erro(7);
	getToken();
	if(tk.cat==ID){
		getToken();
		if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=A_PARENT)){
			Erro(3);	
		}else {
			tipos_param();
			if((tk.cat!=DELIMITADOR)&&(tk.valor.numInt!=F_PARENT)){
				Erro(4);
			}else{
				getToken();
				while((tk.valor.numInt!=ENDPROC)&&
					 (tk.valor.numInt!=IF) &&
					 (tk.valor.numInt!=WHILE) &&
					 (tk.valor.numInt!=FOR) &&
					 (tk.valor.numInt!=RETURN) &&
					 (tk.valor.numInt!=CALL) &&
					 (tk.valor.numInt!=PT_VIRG) &&
					 (tk.valor.numInt!=KEYBOARD) &&
					 (tk.valor.numInt!=DISPLAY) &&
					 (tk.cat!=ID)) {
					
						tipo();
						getToken();
						decl_var();
						getToken();
						while(tk.valor.numInt==VIRG){
							getToken();
							decl_var();
							getToken();
						}				
				}
				
				while(tk.valor.numInt!=ENDPROC){
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

void tipos_param(){
	getToken();
	if(tk.cat!=PR) Erro(7);
	if(tk.valor.numInt!=NOPARAM){
		tipo();
		getToken();
		if(tk.cat!=ID) Erro(7);
		getToken();
		while(tk.valor.numInt==VIRG){
				getToken();
				tipo();
				getToken();
				if(tk.cat!=ID) Erro(7);
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
