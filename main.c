#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "analex.h"


int linha = 0;

int main(int argc, char *argv[]) {
	
	FILE *fp;
	Token tk;
	char ch;
	int n;
	
	if((fp = fopen("Teste.txt","r"))==NULL) printf("Arquivo não pode ser aberto\n");
	
	do{
	
	tk = AnalisadorLexico(fp);
	if(tk.cat==PR) printf("<%s, %s> ", getCategoria(tk.cat),getPalavraReservada(tk.valor.numInt));
	else if(tk.cat==OPREL) printf("<%s, %s> ",getCategoria(tk.cat),getOperadorRelacional(tk.valor.numInt));
	else if(tk.cat==OPARIT) printf("<%s, %s> ",getCategoria(tk.cat),getOperadorAritmetico(tk.valor.numInt));
	else if(tk.cat==OPLOG) printf("<%s, %s> ",getCategoria(tk.cat),getOperadorLogico(tk.valor.numInt));
	else if(tk.cat==INTCON) printf("<%s, %d> ",getCategoria(tk.cat),tk.valor.numInt);
	else if(tk.cat==REALCON) printf("<%s, %.2f> ",getCategoria(tk.cat),tk.valor.numFloat);
	else printf("<%s, %s> ",getCategoria(tk.cat), tk.valor.s);
	}while(tk.cat!= ERRO);
	
	fclose(fp);
	
	return 0;
}
