#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ; // Token
Token tk_next; // Próximo Token
FILE *fp;
int linha=1;

void getToken(){ // Retorna o próximo Token do ANALEX
	tk = tk_next;
	tk_next=AnalisadorLexico(fp); // Leitura do Token
	while(((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR))||(tk_next.cat==COMENTARIO)){ // Laço para ignorar comentários e CR, além de incrementar linha
		if((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR)) linha++;
		tk_next=AnalisadorLexico(fp);
	}
}

void Erro(int e) { // Função de retorno de mensagem de erro
	system("color f4");
	printf("Erro na linha %d: %s\n",linha, erros[e]);
	exit(e);
}

void prog(){ // Função principal PROG
	getToken();
	if(!((tk.cat==PR)&&(tk.valor.numInt==PL))) Erro(8);
	if(!((tk_next.cat==ID)))Erro(10);
	getToken();
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==VAR)){// Verificação não obrigatoria de declaração de variável
		getToken();
		while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))|| // Laço de repetição para permanecer declarando variavel, com o controle baseado na existencia de tipo
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
		 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
		 	 	tipo(); // Verifica se atende as exigências da função tipo();
		 	 	decl_var(); // Função para comparar se o token é um ID
		 	 	while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
					getToken();
					decl_var();
				}
		}
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDVAR))) Erro(8);//Verfica o token obrigatório para terminar as declarações de variável
		getToken();
	}
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==PROG))){//Laço de controle para saber se existe FWD,PROC ou FUNC 0 ou mais vezes
	
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==FWD)){
			fwd();
		}else if(((tk_next.cat==PR)&&(tk_next.valor.numInt==PROC))){
			proc();
		}else{
			func();
		}
		
	}
	getToken();// Leitura do PROG obrigatória
	cmd();//Verifica se o próximo token atende as especificações de CMD(Obrigatório)
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDPROG))){// Verifica se não é fim de programa, caso não seja é inserido mais comandos.
		cmd();
	}
	getToken();
	
	//Verificação se existe algo depois do termino de codigo da liguagem PL
	getToken();
	if(!(tk.cat==FIM)) Erro(21);
				
}
// ======================================================FIM DE PROG=====================================================
void decl_var(){ 
	getToken();
	if(!(tk.cat==ID)) Erro(10); // Mensagem de erro caso não for ID
}	

void tipo(){ // Função para determinar se o token é uma palavra reservada CHAR, INT, REAL OU BOOL
	getToken();
	if(!(tk.cat==PR)) Erro(8); // Mensagem de erro caso não seja nenhuma das 4 palavras reservadas
	if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(9);
	
}

void tipos_param(){ //Função para determinar o tipo de parametro
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {// Caso entre no IF ele tem que ser CHAR,INT,REAL ou BOOL (verificado na função tipo())
		tipo();
		getToken();
		if(!(tk.cat==ID)) Erro(10);// Verifica a existência obrigatória de um ID após o Tipo
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de tipo seguido de ID
			getToken();
			tipo();
			getToken();
			if(!(tk.cat==ID)) Erro(10);
		}
	}else{ //TK é NOPARAM
		getToken();
	}
}

void fwd(){//Função FWD
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==FWD)) Erro(9);// Verifica a existência obrigatória da palavra reservada FWD
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==PROC)){ // Verifica a existência da palavra reservada PROC
		getToken();
	}else{
		tipo(); //Verifica a existência obrigatória caso o token não seja PROC, das exigências da função tipo()
	}
	getToken();
	if(!(tk.cat==ID)) Erro(10);//Verifica a existência obrigatória de um ID
	getToken();	
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT))) Erro(3);//Verifica a existência obrigatória de um abre parenteses
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {
		tipo();
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de tipo
			getToken();
			tipo();
		}
	}else{
		getToken();// Leitura do NOPARAM
	}
	getToken();
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória de um fecha parenteses
}

void func(){ // Função func()
	tipo();
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT))) Erro(3);
	tipos_param();
	getToken();
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);
	while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))||//Laço para saber se existem 0 ou mais declarações de variáveis baseado no tipo
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
	 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
			 
			  tipo();
			  decl_var();	
		  	  while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
				  getToken();
				  decl_var();
		  }
	}
	
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDFUNC))){//Laço para saber se chegou ao fim da função, caso não chegue, é obrigatório a existência de um comando
		cmd();
	}
	
	getToken();	//Leitura do ENDFUNC
}

void proc(){ // Função Proc ( Similar a função func(), mudando a leitura inicial
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==PROC)) Erro(9);//Leitura obrigatória da palavra reservada PROC
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT))) Erro(3);
	tipos_param();
	getToken();
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);
	while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))||
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
	 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
			 
			  tipo();
			  decl_var();	
		  	  while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de variavel
				  getToken();
				  decl_var();
		  	  }
	}
	
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDPROC))){
		cmd();
	}
	
	getToken();	
}

void cmd(){// Função comando
// ======================================================INICIO DO IF=====================================================  
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==IF)){//Verifica se o próximo token é IF
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a existência obrigatória do abre parenteses
		getToken();
		expr();//Existência obrigatória de uma expressão
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória do fecha parenteses
		getToken();
		cmd();//Comando obrigatório
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))|| //Laço para saber se o próximo token atende as exigências de uma função cmd() não obrigatória
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
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==ELSE)){//Verifica a existência não obrigatória de um ELSE
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDIF))) Erro(8);//Verifica se o token é um ENDIF (Obrigatório)
		getToken();// Leitura do ENDIF
		
 // ======================================================FIM DO IF=====================================================  
 
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE)){//Verifica se o próximo token é WHILE
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a existência obrigatória do abre parenteses
		getToken();
		expr();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória do fecha parenteses
		getToken();
		cmd();
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||//Laço para saber se o próximo token atende as exigências de uma função cmd() não obrigatória
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDWHILE))) Erro(8);//Leitura para saber se o token obrigatório ENDWHILE
		getToken();//leitura do ENDWHILE
// ======================================================FIM DO WHILE=====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR)){//Verifica se o próximo token é FOR
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a existência obrigatória do abre parenteses
		getToken();
		if(tk_next.cat==ID){//Verifica a existência não obrigatória de uma atribuição
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);//Verifica a existência da vírgula obrigatória
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a existêcia não obrigatória de uma expressão
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
			}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);//Verifica a existência obrigatória da vírgula
		getToken();
		if(tk_next.cat==ID){//Verifica a existência não obrigatória de uma atribuição
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória do fecha parenteses
		getToken();
		cmd();//Comando obrigatório
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||//Laço para saber se o próximo token atende as exigências de uma função cmd() não obrigatória
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDFOR))) Erro(8);//Verifica se o token é ENDFOR
		getToken();//Leitura do ENDFOR
// ======================================================FIM DO FOR=====================================================			
	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN)){//Verifica se o próximo token é RETURN
		getToken();
		if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){
			getToken();
			expr();//Verifica a existência obrigatória de uma expressão
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))	Erro(4);	
			getToken();		
		}
		
// ======================================================FIM DO RETURN====================================================	
	}else if(tk_next.cat==ID) {//Verifica se o próximo token é uma atribuição
		atrib();
		
// ======================================================FIM DO ATRIB====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL)){//Verifica se o próximo token é CALL
		getToken();
		if(!(tk_next.cat==ID)) Erro(10);//Verifica a existência obrigatória de um ID
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)))Erro(3);
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a existêcia não obrigatória de uma expressão
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma expressão
				  getToken();
				  expr();
		  	  }
				
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))Erro(4);
		getToken();
// ======================================================FIM DO CALL====================================================		
	}else if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG)){//Verifica se o próximo token é ponto e vírgula
		getToken();
		
//======================================================FIM DO PT_VIRG====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD)){//Verifica se o próximo token é KEYBOARD
		getToken();
		if(!(tk_next.cat==ID))Erro(10);
		getToken();
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de um ID
			getToken();
			if(!(tk_next.cat==ID))Erro(10);
			getToken();
		}

//======================================================FIM DO KEYBOARD====================================================		  		
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY)){//Verifica se o próximo token é DISPLAY
		getToken();
		if	(!((tk_next.cat==ID)|| //Verfica a existência obrigatória de ID ou INTCON ou REALCON ou CARACCON ou CADEIACON
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)||
			(tk_next.cat==CADEIACON))) Erro(19);
		
		getToken();
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){//Leitura não obrigatória da palavra reservada DUP
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);
			getToken();
		}
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula
			getToken();
			if	(!((tk_next.cat==ID)|| //Verfica a existência obrigatória de ID ou INTCON ou REALCON ou CARACCON ou CADEIACON a ser lido após a vírgula
				(tk_next.cat==INTCON)||
				(tk_next.cat==REALCON)||
				(tk_next.cat==CARACCON)||
				(tk_next.cat==CADEIACON))) Erro(19);
			getToken();
			if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){//Verifica se existe a palavra reservada DUP
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);//caso DUP exista, a leitura de INTCON ou ID é obrigatória
			getToken();
			}	
		}
		
//======================================================FIM DO DISPLAY====================================================			
	}else Erro(19);//Erro se não atender a nenhuma exigência de Cmd()
}

void atrib(){//função atrib()
	getToken();
	if(!(tk.cat==ID)) Erro(10);//Leitura obrigátoria de um ID
	getToken();
	if(!(tk.cat==OPREL)) Erro(16);
	if(!(tk.valor.numInt==IGUAL)) Erro(17);//Leitura obrigatória de um operador relacional que não seja IGUAL
	expr();//Verifica a existência obrigatória de uma expressão após o operador relacional
}

void expr(){// Função expressão
	expr_simp();//Verifica a existência obrigatória de uma expressão simples
	if(tk_next.cat==OPREL){//Verifica a existência não obrigatória de um operador relacional
		op_rel();//caso exista um operador, verificar as exigências da função op_rel()
		expr_simp();//Verifica a existência obrigatória de uma expressão simples após o operador relacional
	}	
}

void expr_simp(){//Função expressão simples
	if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))){//Verifica a existência não obrigatória de + ou -
		getToken();
	}
	termo();
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||// Laço de controle baseado em +,- ou OR para saber a existência de 0 ou mais operadores seguidos de termo
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==OR))){
			getToken();
			termo();
	}
}

void termo(){//Função termo
	fator();// Verifica a existência de um fator obrigatório
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MULTIPLICACAO))||// Laço de controle baseado em *,/ ou AND para saber a existência de 0 ou mais operadores seguidos de fator
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==DIVISAO))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==AND))){
			getToken();
			fator();
	}
}

void fator(){//Função fator
	if(tk_next.cat==ID){//Verifica se o token é ID
		getToken();
		if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){//Verifica após o token ter sido ID se é seguido de abre parenteses
			getToken();
			if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a existência não obrigatória de uma expressão
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// Laço de controle baseado na virgula, caso exista mais de uma declaração de expressão
						getToken();
						expr();
				}		
			}
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória de um fecha parenteses se após ID foi seguindo de abre parenteses
			getToken();//Leitura do fecha parenteses
		}
	}else if(tk_next.cat==INTCON) getToken();//Verifica se o próximo token é INTCON
	else if(tk_next.cat==REALCON) getToken();//Verifica se o próximo token é REALCON
	else if(tk_next.cat==CARACCON) getToken();//Verifica se o próximo token é CARACCON
	else if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){//Verifica se o próximo token é abre parenteses
			getToken();
			expr();//Verifica a existência obrigatória de uma expressão após o abre parenteses
			getToken();
			if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);//Verifica a existência obrigatória do fecha parenteses
	}else if((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT)){//Verifica se o próximo token é NOT
			getToken();
			fator();//Verifica a existência obrigatória de fator se o token anterior foi um NOT
	}else Erro(18);	//Erro por não atender as especificações de fator	
}

void op_rel(){//função operador relacional
	getToken();
	if(!(tk.cat==OPREL)) Erro(15);
	if(tk.valor.numInt==IGUAL) Erro(16);//Verifica se o operador relacional não é um IGUAL
}


int main(int argc, char *argv[]) {
	printf("[ANALEX LINGUAGEM PL]\n\n");
	system("color f0");
	if((fp = fopen("Editor Linguagem PL.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	//if((fp = fopen("teste3.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n"); // VALIDANDO A ABERTURA DO ARQUIVO 
	tk_next=AnalisadorLexico(fp);
	while(((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR))||(tk_next.cat==COMENTARIO)){
		if((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR)) linha++;
		tk_next=AnalisadorLexico(fp);
	}
	
	prog();
	system("color f2");
	printf("\nPrograma Compilado sem Erros");
	fclose(fp);
	
	return 0;
}
