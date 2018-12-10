#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.c"
#include "analisint.h"


Token tk ; // Token
Token tk_next; // Pr�ximo Token
FILE *fp;
int linha=1;

void getToken(){ // Retorna o pr�ximo Token do ANALEX
	tk = tk_next;
	tk_next=AnalisadorLexico(fp); // Leitura do Token
	while(((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR))||(tk_next.cat==COMENTARIO)){ // La�o para ignorar coment�rios e CR, al�m de incrementar linha
		if((tk_next.cat == CARAC_ESPEC)&&(tk_next.valor.numInt == CR)) linha++;
		tk_next=AnalisadorLexico(fp);
	}
}

void Erro(int e) { // Fun��o de retorno de mensagem de erro
	system("color f4");
	printf("Erro na linha %d: %s\n",linha, erros[e]);
	exit(e);
}

void prog(){ // Fun��o principal PROG
	getToken();
	if(!((tk.cat==PR)&&(tk.valor.numInt==PL))) Erro(8);
	if(!((tk_next.cat==ID)))Erro(10);
	getToken();
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==VAR)){// Verifica��o n�o obrigatoria de declara��o de vari�vel
		getToken();
		while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))|| // La�o de repeti��o para permanecer declarando variavel, com o controle baseado na existencia de tipo
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
			 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
		 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
		 	 	tipo(); // Verifica se atende as exig�ncias da fun��o tipo();
		 	 	decl_var(); // Fun��o para comparar se o token � um ID
		 	 	while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
					getToken();
					decl_var();
				}
		}
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDVAR))) Erro(8);//Verfica o token obrigat�rio para terminar as declara��es de vari�vel
		getToken();
	}
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==PROG))){//La�o de controle para saber se existe FWD,PROC ou FUNC 0 ou mais vezes
	
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==FWD)){
			fwd();
		}else if(((tk_next.cat==PR)&&(tk_next.valor.numInt==PROC))){
			proc();
		}else{
			func();
		}
		
	}
	getToken();// Leitura do PROG obrigat�ria
	cmd();//Verifica se o pr�ximo token atende as especifica��es de CMD(Obrigat�rio)
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDPROG))){// Verifica se n�o � fim de programa, caso n�o seja � inserido mais comandos.
		cmd();
	}
	getToken();
	
	//Verifica��o se existe algo depois do termino de codigo da liguagem PL
	getToken();
	if(!(tk.cat==FIM)) Erro(21);
				
}
// ======================================================FIM DE PROG=====================================================
void decl_var(){ 
	getToken();
	if(!(tk.cat==ID)) Erro(10); // Mensagem de erro caso n�o for ID
}	

void tipo(){ // Fun��o para determinar se o token � uma palavra reservada CHAR, INT, REAL OU BOOL
	getToken();
	if(!(tk.cat==PR)) Erro(8); // Mensagem de erro caso n�o seja nenhuma das 4 palavras reservadas
	if(!((tk.valor.numInt==CHAR) || (tk.valor.numInt==INT) || (tk.valor.numInt==REAL) || (tk.valor.numInt==BOOL))) Erro(9);
	
}

void tipos_param(){ //Fun��o para determinar o tipo de parametro
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {// Caso entre no IF ele tem que ser CHAR,INT,REAL ou BOOL (verificado na fun��o tipo())
		tipo();
		getToken();
		if(!(tk.cat==ID)) Erro(10);// Verifica a exist�ncia obrigat�ria de um ID ap�s o Tipo
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de tipo seguido de ID
			getToken();
			tipo();
			getToken();
			if(!(tk.cat==ID)) Erro(10);
		}
	}else{ //TK � NOPARAM
		getToken();
	}
}

void fwd(){//Fun��o FWD
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==FWD)) Erro(9);// Verifica a exist�ncia obrigat�ria da palavra reservada FWD
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==PROC)){ // Verifica a exist�ncia da palavra reservada PROC
		getToken();
	}else{
		tipo(); //Verifica a exist�ncia obrigat�ria caso o token n�o seja PROC, das exig�ncias da fun��o tipo()
	}
	getToken();
	if(!(tk.cat==ID)) Erro(10);//Verifica a exist�ncia obrigat�ria de um ID
	getToken();	
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT))) Erro(3);//Verifica a exist�ncia obrigat�ria de um abre parenteses
	if(!(tk_next.cat==PR)) Erro(8);
	if(!(tk_next.valor.numInt==NOPARAM)) {
		tipo();
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de tipo
			getToken();
			tipo();
		}
	}else{
		getToken();// Leitura do NOPARAM
	}
	getToken();
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria de um fecha parenteses
}

void func(){ // Fun��o func()
	tipo();
	getToken();
	if(!(tk.cat==ID)) Erro(10);
	getToken();	
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==A_PARENT))) Erro(3);
	tipos_param();
	getToken();
	if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);
	while(((tk_next.cat==PR)&&(tk_next.valor.numInt==CHAR))||//La�o para saber se existem 0 ou mais declara��es de vari�veis baseado no tipo
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==INT))||
		 ((tk_next.cat==PR)&&(tk_next.valor.numInt==REAL))||
	 	 ((tk_next.cat==PR)&&(tk_next.valor.numInt==BOOL))){
			 
			  tipo();
			  decl_var();	
		  	  while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
				  getToken();
				  decl_var();
		  }
	}
	
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDFUNC))){//La�o para saber se chegou ao fim da fun��o, caso n�o chegue, � obrigat�rio a exist�ncia de um comando
		cmd();
	}
	
	getToken();	//Leitura do ENDFUNC
}

void proc(){ // Fun��o Proc ( Similar a fun��o func(), mudando a leitura inicial
	getToken();
	if(!(tk.cat==PR)) Erro(8);
	if(!(tk.valor.numInt==PROC)) Erro(9);//Leitura obrigat�ria da palavra reservada PROC
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
		  	  while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de variavel
				  getToken();
				  decl_var();
		  	  }
	}
	
	while(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDPROC))){
		cmd();
	}
	
	getToken();	
}

void cmd(){// Fun��o comando
// ======================================================INICIO DO IF=====================================================  
	if((tk_next.cat==PR)&&(tk_next.valor.numInt==IF)){//Verifica se o pr�ximo token � IF
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a exist�ncia obrigat�ria do abre parenteses
		getToken();
		expr();//Exist�ncia obrigat�ria de uma express�o
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria do fecha parenteses
		getToken();
		cmd();//Comando obrigat�rio
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))|| //La�o para saber se o pr�ximo token atende as exig�ncias de uma fun��o cmd() n�o obrigat�ria
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
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==ELSE)){//Verifica a exist�ncia n�o obrigat�ria de um ELSE
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDIF))) Erro(8);//Verifica se o token � um ENDIF (Obrigat�rio)
		getToken();// Leitura do ENDIF
		
 // ======================================================FIM DO IF=====================================================  
 
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==WHILE)){//Verifica se o pr�ximo token � WHILE
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a exist�ncia obrigat�ria do abre parenteses
		getToken();
		expr();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria do fecha parenteses
		getToken();
		cmd();
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||//La�o para saber se o pr�ximo token atende as exig�ncias de uma fun��o cmd() n�o obrigat�ria
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDWHILE))) Erro(8);//Leitura para saber se o token obrigat�rio ENDWHILE
		getToken();//leitura do ENDWHILE
// ======================================================FIM DO WHILE=====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==FOR)){//Verifica se o pr�ximo token � FOR
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))) Erro(3);//Verifica a exist�ncia obrigat�ria do abre parenteses
		getToken();
		if(tk_next.cat==ID){//Verifica a exist�ncia n�o obrigat�ria de uma atribui��o
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);//Verifica a exist�ncia da v�rgula obrigat�ria
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a exist�cia n�o obrigat�ria de uma express�o
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
			}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG))) Erro(20);//Verifica a exist�ncia obrigat�ria da v�rgula
		getToken();
		if(tk_next.cat==ID){//Verifica a exist�ncia n�o obrigat�ria de uma atribui��o
			atrib();
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria do fecha parenteses
		getToken();
		cmd();//Comando obrigat�rio
		while (((tk_next.cat==PR)&&(tk_next.valor.numInt==IF))||//La�o para saber se o pr�ximo token atende as exig�ncias de uma fun��o cmd() n�o obrigat�ria
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
		if(!((tk_next.cat==PR)&&(tk_next.valor.numInt==ENDFOR))) Erro(8);//Verifica se o token � ENDFOR
		getToken();//Leitura do ENDFOR
// ======================================================FIM DO FOR=====================================================			
	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==RETURN)){//Verifica se o pr�ximo token � RETURN
		getToken();
		if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){
			getToken();
			expr();//Verifica a exist�ncia obrigat�ria de uma express�o
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))	Erro(4);	
			getToken();		
		}
		
// ======================================================FIM DO RETURN====================================================	
	}else if(tk_next.cat==ID) {//Verifica se o pr�ximo token � uma atribui��o
		atrib();
		
// ======================================================FIM DO ATRIB====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==CALL)){//Verifica se o pr�ximo token � CALL
		getToken();
		if(!(tk_next.cat==ID)) Erro(10);//Verifica a exist�ncia obrigat�ria de um ID
		getToken();
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)))Erro(3);
		getToken();
		if	(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a exist�cia n�o obrigat�ria de uma express�o
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma express�o
				  getToken();
				  expr();
		  	  }
				
		}
		if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT)))Erro(4);
		getToken();
// ======================================================FIM DO CALL====================================================		
	}else if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==PT_VIRG)){//Verifica se o pr�ximo token � ponto e v�rgula
		getToken();
		
//======================================================FIM DO PT_VIRG====================================================	
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==KEYBOARD)){//Verifica se o pr�ximo token � KEYBOARD
		getToken();
		if(!(tk_next.cat==ID))Erro(10);
		getToken();
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de um ID
			getToken();
			if(!(tk_next.cat==ID))Erro(10);
			getToken();
		}

//======================================================FIM DO KEYBOARD====================================================		  		
	}else if((tk_next.cat==PR)&&(tk_next.valor.numInt==DISPLAY)){//Verifica se o pr�ximo token � DISPLAY
		getToken();
		if	(!((tk_next.cat==ID)|| //Verfica a exist�ncia obrigat�ria de ID ou INTCON ou REALCON ou CARACCON ou CADEIACON
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)||
			(tk_next.cat==CADEIACON))) Erro(19);
		
		getToken();
		if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){//Leitura n�o obrigat�ria da palavra reservada DUP
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);
			getToken();
		}
		while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula
			getToken();
			if	(!((tk_next.cat==ID)|| //Verfica a exist�ncia obrigat�ria de ID ou INTCON ou REALCON ou CARACCON ou CADEIACON a ser lido ap�s a v�rgula
				(tk_next.cat==INTCON)||
				(tk_next.cat==REALCON)||
				(tk_next.cat==CARACCON)||
				(tk_next.cat==CADEIACON))) Erro(19);
			getToken();
			if((tk_next.cat==PR)&&(tk_next.valor.numInt==DUP)){//Verifica se existe a palavra reservada DUP
			getToken();
			if(!((tk_next.cat==ID)||(tk_next.cat==INTCON)))Erro(19);//caso DUP exista, a leitura de INTCON ou ID � obrigat�ria
			getToken();
			}	
		}
		
//======================================================FIM DO DISPLAY====================================================			
	}else Erro(19);//Erro se n�o atender a nenhuma exig�ncia de Cmd()
}

void atrib(){//fun��o atrib()
	getToken();
	if(!(tk.cat==ID)) Erro(10);//Leitura obrig�toria de um ID
	getToken();
	if(!(tk.cat==OPREL)) Erro(16);
	if(!(tk.valor.numInt==IGUAL)) Erro(17);//Leitura obrigat�ria de um operador relacional que n�o seja IGUAL
	expr();//Verifica a exist�ncia obrigat�ria de uma express�o ap�s o operador relacional
}

void expr(){// Fun��o express�o
	expr_simp();//Verifica a exist�ncia obrigat�ria de uma express�o simples
	if(tk_next.cat==OPREL){//Verifica a exist�ncia n�o obrigat�ria de um operador relacional
		op_rel();//caso exista um operador, verificar as exig�ncias da fun��o op_rel()
		expr_simp();//Verifica a exist�ncia obrigat�ria de uma express�o simples ap�s o operador relacional
	}	
}

void expr_simp(){//Fun��o express�o simples
	if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))){//Verifica a exist�ncia n�o obrigat�ria de + ou -
		getToken();
	}
	termo();
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||// La�o de controle baseado em +,- ou OR para saber a exist�ncia de 0 ou mais operadores seguidos de termo
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==OR))){
			getToken();
			termo();
	}
}

void termo(){//Fun��o termo
	fator();// Verifica a exist�ncia de um fator obrigat�rio
	while(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MULTIPLICACAO))||// La�o de controle baseado em *,/ ou AND para saber a exist�ncia de 0 ou mais operadores seguidos de fator
		 ((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==DIVISAO))||
		 ((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==AND))){
			getToken();
			fator();
	}
}

void fator(){//Fun��o fator
	if(tk_next.cat==ID){//Verifica se o token � ID
		getToken();
		if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){//Verifica ap�s o token ter sido ID se � seguido de abre parenteses
			getToken();
			if(((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MAIS))||//Verifica a exist�ncia n�o obrigat�ria de uma express�o
			((tk_next.cat==OPARIT)&&(tk_next.valor.numInt==MENOS))||
			((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT))||
			((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT))||
			(tk_next.cat==ID)||
			(tk_next.cat==INTCON)||
			(tk_next.cat==REALCON)||
			(tk_next.cat==CARACCON)){
				expr();
				while((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==VIRG)){// La�o de controle baseado na virgula, caso exista mais de uma declara��o de express�o
						getToken();
						expr();
				}		
			}
			if(!((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria de um fecha parenteses se ap�s ID foi seguindo de abre parenteses
			getToken();//Leitura do fecha parenteses
		}
	}else if(tk_next.cat==INTCON) getToken();//Verifica se o pr�ximo token � INTCON
	else if(tk_next.cat==REALCON) getToken();//Verifica se o pr�ximo token � REALCON
	else if(tk_next.cat==CARACCON) getToken();//Verifica se o pr�ximo token � CARACCON
	else if((tk_next.cat==DELIMITADOR)&&(tk_next.valor.numInt==A_PARENT)){//Verifica se o pr�ximo token � abre parenteses
			getToken();
			expr();//Verifica a exist�ncia obrigat�ria de uma express�o ap�s o abre parenteses
			getToken();
			if(!((tk.cat==DELIMITADOR)&&(tk.valor.numInt==F_PARENT))) Erro(4);//Verifica a exist�ncia obrigat�ria do fecha parenteses
	}else if((tk_next.cat==OPLOG)&&(tk_next.valor.numInt==NOT)){//Verifica se o pr�ximo token � NOT
			getToken();
			fator();//Verifica a exist�ncia obrigat�ria de fator se o token anterior foi um NOT
	}else Erro(18);	//Erro por n�o atender as especifica��es de fator	
}

void op_rel(){//fun��o operador relacional
	getToken();
	if(!(tk.cat==OPREL)) Erro(15);
	if(tk.valor.numInt==IGUAL) Erro(16);//Verifica se o operador relacional n�o � um IGUAL
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
