#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"

int indiceString=0;

int isPalavraReservada(char *buffer, int menor, int maior){
   int meio, cmp;
   if (menor > maior) return -1;
   meio = (menor + maior) / 2;
   cmp = strcmp(PalavrasReservadas[meio], buffer);
   if (cmp == 0 ) return meio;
   if (cmp > 0) return isPalavraReservada(buffer, menor, meio - 1);
   else return isPalavraReservada(buffer, meio + 1, maior);
}

Token AnalisadorLexico(FILE *fp){
	Token tk;
	char buffer[256]="";
	char ch;
	int estado = 0;
	int indice;

		while (estado>=0){

			switch(estado){

					case 0 :
						ch=getc(fp);

						if (isalpha(ch)){
							buffer[strlen(buffer)] = ch;
							estado = 1;

						} else if (isdigit(ch)){
							buffer[strlen(buffer)] = ch;
							estado = 4;

						}else if (ch == '\''){
							estado = 9;

						}else if (ch == '\n'){
							estado = 12;

						}else if (ch == '\0'){
							buffer[strlen(buffer)] = ch;
							estado = 13;

						}else if (ch == '"'){
							estado = 14;

						}else if (ch == '.'){
							buffer[strlen(buffer)] = ch;
							estado = 16;

						}else if (ch == ' '){
							estado = 23;

						}else if (ch == '\t'){
							estado = 24;

						}else if (ch == '*'){
							estado = 25;

						}else if (ch == '/'){
							estado = 27;

						}else if (ch == '-'){
							estado = 31;

						}else if (ch == '+'){
							estado = 32;

						}else if (ch == '#'){
							estado = 33;

						}else if (ch == '>'){
							estado = 34;

						}else if (ch == '<'){
							estado = 37;

						}else if (ch == '='){
							estado = 40;

						}else if (ch == '{'){
							estado = 43;

						}else if (ch == '}'){
							estado = 44;

						}else if (ch == '('){
							estado = 45;

						}else if (ch == ')'){
							estado = 46;

						}else if (ch == '['){
							estado = 47;

						}else if (ch == ']'){
							estado = 48;

						}else if (ch == ','){
							estado = 49;

						}else if (ch == ';'){
							estado = 50;

						}else if (ch == EOF){
							tk.cat=FIM;
							return (tk);
						}else{
							tk.cat = ERRO;
							return(tk);
						}

						break;

					case 1 : // RECONHECIMENTO DE ID
						ch=getc(fp);
						if (isalpha(ch)){
							buffer[strlen(buffer)] = ch;

						}else if(isdigit(ch)){
							buffer[strlen(buffer)] = ch;
							estado = 2;

						}else{
							estado = 3;
						}

						break;


					case 2 : // RECONHECIMENTO DE ID
						ch=getc(fp);
						if (isdigit(ch)){
							buffer[strlen(buffer)] = ch;
						}else if (isalpha(ch)){
							buffer[strlen(buffer)] = ch;
							estado = 1;
						}else{
							estado = 3;
						}

						break;

					case 3 : // RECONHECIMENTO DE ID
						ungetc(ch,fp);
						indice = isPalavraReservada(buffer,0,25);
						if(indice==-1){
							tk.cat = ID;
							strcpy(tk.valor.s,buffer);
						}else{
							tk.cat=PR;
							tk.valor.numInt=indice;
						}

						return (tk);

					case 4 : // RECONHECIMENTO DE INTEIRO
						ch=getc(fp);
						if (isdigit(ch)){
							buffer[strlen(buffer)] = ch;
						}else if (ch=='.'){
							buffer[strlen(buffer)] = ch;
							estado = 6;
						}else{
							estado = 5;
						}
						break;

					case 5 : // RECONHECIMENTO DE INTEIRO
						ungetc(ch,fp);
						tk.cat = INTCON;
						tk.valor.numInt = atoll(buffer);
					return (tk);


					case 6 : // RECONHECIMENTO DE FLOAT
						ch=getc(fp);
						if(isdigit(ch)){
							buffer[strlen(buffer)] = ch;
							estado = 7;
						}else { // RECONHECIMENTO DE INTEIRO
							ungetc(ch,fp);
							ch='.';
							estado = 5;
						}
					break;


					case 7:
						ch=getc(fp);
						if(isdigit(ch)){
							buffer[strlen(buffer)] = ch;
						}else {
							estado = 8;
						}
					break;


					case 8:
						ungetc(ch,fp);
						tk.cat = REALCON;
						tk.valor.numFloat = atof(buffer);
					return (tk);

					case 9 :// RECONHECIMENTO DE CARACON
						ch=getc(fp);
						if (ch == '\''){ // leitura do VAZIO ''
							estado = 11;
							
						}else if ((isprint(ch)) && (ch != '\\')){ // leitura de algo printavel diferente de /
							buffer[strlen(buffer)] = ch;
							estado = 10;
							
						}else if(ch == '\\'){// leitura do /
							ch=getc(fp);
							if(ch=='\'') {
								buffer[strlen(buffer)] = '\'';
								estado = 10;	
							}else if(ch=='n'){
								buffer[strlen(buffer)]='\n';	
								estado = 10;
							} 
							else if(ch=='0') {
								buffer[strlen(buffer)]='\0';	
								estado = 10;
							}else if(ch=='\''){				 // RESOLVER O BARRA AQUI				
								buffer[strlen(buffer)]='\\';	
								estado = 10;								
							}else {
								tk.cat = ERRO;
								strcpy(tk.valor.s,"ERRO LEXICO");
								return(tk);
							}
																			
						}else{
									estado = -1;
									tk.cat = ERRO;
									strcpy(tk.valor.s,"ERRO LEXICO");
									return (tk);
						}	
						
					    break;

					case 10 :
						ch=getc(fp);

						if (ch == '\''){ // delimitador final de CARACON
							estado = 11;
						}
						else{
							estado = -1;
							tk.cat = ERRO;
							strcpy(tk.valor.s,"ERRO LEXICO");
							return (tk);
						}
						
					break;

					case 11 : // GERANDO UM TOKEN CARACON
						if(buffer[0] == '\0'){
							tk.cat = CARACON;
							tk.valor.numInt = NUL;
							return(tk);
						}else if (buffer[0] == '\n'){
							tk.cat = CARACON;
							tk.valor.numInt = CR;
							return(tk);
						}else{
							tk.cat = CARACON;
							tk.valor.c = buffer[0];
							return(tk);
						}


					case 12 : // TRATANDO O \N
						tk.cat=CARACON;
						tk.valor.numInt=CR;
						return (tk);

					break;

					case 14 : //RECONHECIMENTO DE CADEIACON
					ch=getc(fp);
						if (ch == '\"'){
							estado = 15;
						}else if(ch=='\\'){
							ch=getc(fp);							
							if(ch=='"') buffer[strlen(buffer)] = '\"';
							else if(ch=='n') estado=14;
							else if(ch=='0') estado=14;
							else {
									buffer[strlen(buffer)] = '\\';	
									buffer[strlen(buffer)] = ch;	
							}
						}else buffer[strlen(buffer)] = ch;
						
					break;

					case 15 : // RECONHECIMENTO DE CADEIACON
							strcpy(stringsPL[indiceString],buffer);
						    tk.cat=CADEIACON;
							tk.valor.numInt=indiceString++;		
						
						return (tk);


					case 16 : // RECONHECIMENTO DOS OPERADORES LÓGICOS
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						if(!(strcmp(buffer,".or"))) {
							estado = 19;
							break;
						}
						ch=getc(fp);
						buffer[strlen(buffer)] = ch;
						if(!(strcmp(buffer,".and"))) {
							estado = 17;
							break;
						}else if (!(strcmp(buffer,".not"))) {
							estado = 21;
							break;
						}else {
							tk.cat=ERRO;
							return (tk);
						}

					break;


					case 17 : // RECONHECIMENTO DO .AND.
						ch=getc(fp);
						if(ch=='.'){
							buffer[strlen(buffer)] = ch;
							estado = 18;
						}else {
							tk.cat=ERRO;
							return (tk);
						}
					break;


					case 18 : // RECONHECIMENTO DO .AND.
						tk.cat = OPLOG;
						tk.valor.numInt = AND;
					return (tk);


					case 19 : // RECONHECIMENTO DO .OR.
						ch=getc(fp);
						if(ch=='.'){
							buffer[strlen(buffer)] = ch;
							estado = 20;
						}else {
							tk.cat=ERRO;
							return (tk);
							//MENSAGEM DE ERRO NA LINHA RESULTANTE
						}
					break;


					case 20 : // RECONHECIMENTO DO .OR.
						tk.cat = OPLOG;
						tk.valor.numInt = OR;
					return (tk);


					case 21 : // RECONHECIMENTO DO .NOT.
						ch=getc(fp);
						if(ch=='.'){
							buffer[strlen(buffer)] = ch;
							estado = 22;
						}else {
							//MENSAGEM DE ERRO NA LINHA RESULTANTE
						}
					break;


					case 22 : // RECONHECIMENTO DO .NOT.
						tk.cat = OPLOG;
						tk.valor.numInt = NOT;
					return (tk);


					case 23 : // TRATANDO O CARACTER BRANCO
						estado = 0;
					break;

					case 24: // TRATANDO O \T (TAB)
						estado = 0;
					break;

					case 25 :
						tk.cat=OPARIT;
						tk.valor.numInt = MULTIPLICACAO;
					return (tk);

					case 27 :
						ch=getc(fp);
						if(ch=='/') estado = 29;
						else estado = 28;
					break;

					case 28 : //RECONHECIMENTO DE DIVISÃO
						ungetc(ch,fp);
						tk.cat=OPARIT;
						tk.valor.numInt = DIVISAO;
					return (tk);

					case 29 : //RECONHECIMENTO DE COMENTARIO
						ch=getc(fp);
						if(ch=='\n'){
							estado = 30;
							ungetc(ch,fp);

						}else if (ch==EOF) estado = 30;
						else buffer[strlen(buffer)] = ch;
					break;

					case 30 :
							strcpy(stringsPL[indiceString],buffer);
							tk.cat=COMENTARIO;
							tk.valor.numInt=indiceString++;		
						
					return (tk);


					case 31 :
						tk.cat=OPARIT;
						tk.valor.numInt = MENOS;
					return (tk);


					case 32 :
						tk.cat=OPARIT;
						tk.valor.numInt = MAIS;
					return (tk);


					case 33 :
						tk.cat=OPREL;
						tk.valor.numInt = DIFERENTE;
					return (tk);


					case 34 :
						ch=getc(fp);
						if(ch=='=') estado = 35;
						else estado = 36;
					break;


					case 35 :
						tk.cat = OPREL;
						tk.valor.numInt = MAIORIGUAL;
					return (tk);


					case 36 :
						ungetc(ch,fp);
						tk.cat = OPREL;
						tk.valor.numInt = MAIOR;
					return (tk);


					case 37 :
						ch=getc(fp);
						if(ch=='=') estado = 38;
						else estado = 39;
					break;


					case 38 :
						tk.cat = OPREL;
						tk.valor.numInt = MENORIGUAL;
					return (tk);


					case 39 :
						ungetc(ch,fp);
						tk.cat = OPREL;
						tk.valor.numInt = MENOR;
					return (tk);


					case 40 :
						ch=getc(fp);
						if(ch=='=') estado = 41;
						else estado = 42;
					break;


					case 41 :
						tk.cat = OPREL;
						tk.valor.numInt = IGUALIGUAL;
					return (tk);

					case 42 :
						ungetc(ch,fp);
						tk.cat = OPREL;
						tk.valor.numInt = IGUAL;
					return (tk);

					case 43 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = A_CHAVES;
					return (tk);

					case 44 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = F_CHAVES;
					return (tk);

					case 45 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = A_PARENT;
					return (tk);

					case 46 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = F_PARENT;
					return (tk);

					case 47 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = A_COLCH;
					return (tk);

					case 48 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = F_COLCH;
					return (tk);

					case 49 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = VIRG;
					return (tk);

					case 50 :
						tk.cat = DELIMITADOR;
						tk.valor.numInt = PT_VIRG;
					return (tk);
				}

		}


}

int main(int argc, char *argv[]) {

	FILE *fp;
	Token tk;
	int linha=0;
	
	printf("Teste\" ");
	
	system("color f0");

	if((fp = fopen("Teste Filipe.txt","r"))==NULL) printf("Arquivo nao pode ser aberto\n");
	printf("\n%d.  ",linha);
	do{

		tk = AnalisadorLexico(fp);

		if(tk.cat==PR) printf("<%s, %s> ", Categorias[tk.cat],(toupper(PalavrasReservadas[tk.valor.numInt])));
		else if(tk.cat==ID) printf("<%s, %s> ",Categorias[tk.cat],tk.valor.s);
		else if(tk.cat==OPREL) printf("<%s, %s> ",Categorias[tk.cat],OperadoresRelacionais[tk.valor.numInt]);
		else if(tk.cat==OPARIT) printf("<%s, %s> ",Categorias[tk.cat],OperadoresAritmeticos[tk.valor.numInt]);
		else if(tk.cat==OPLOG) printf("<%s, %s> ",Categorias[tk.cat],OperadoresLogicos[tk.valor.numInt]);
		else if(tk.cat==INTCON) printf("<%s, %d> ",Categorias[tk.cat],tk.valor.numInt);
		else if(tk.cat==REALCON) printf("<%s, %.2f> ",Categorias[tk.cat],tk.valor.numFloat);
		else if(tk.cat==CARACON){
			if(tk.valor.numInt==CR){
				printf("<%s, %s> ",Categorias[tk.cat],Especiais[tk.valor.numInt]);
				printf("\n%d.  ",++linha);
			} 
			else if(tk.valor.numInt==NUL) printf("<%s, %s> ",Categorias[tk.cat],Especiais[tk.valor.numInt]);
			else printf("<%s, %c> ",Categorias[tk.cat],tk.valor.c);
		}else if(tk.cat==CADEIACON) printf("<%s, %s> ",Categorias[tk.cat],stringsPL[tk.valor.numInt]);
		else if(tk.cat==COMENTARIO) printf("<%s, %s> ",Categorias[tk.cat],stringsPL[tk.valor.numInt]);
		else if(tk.cat==DELIMITADOR) printf("<%s, %s> ",Categorias[tk.cat],Delimitadores[tk.valor.numInt]);
		else if(tk.cat==CARAC_ESPEC) printf("<%s, %s> ",Categorias[tk.cat],Especiais[tk.valor.numInt]);
		else if(tk.cat==ERRO) {
			system("color f4");
			printf("\n\n\n--------------------------------");
			printf("\n[!]ERRO: TOKEN INVALIDO NA LINHA %d",linha);
		}
	}while((tk.cat!=FIM)&&(tk.cat!=ERRO));

	fclose(fp);
	
	return 0;
}


