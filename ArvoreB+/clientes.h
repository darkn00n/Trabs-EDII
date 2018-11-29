#ifndef CLIENTES_H
#define CLIENTES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ORDEM 3

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

typedef struct Cliente
{
	int codCliente;
	char nome[100];
}cliente;

//funções para inserção na tabela:
	cliente* cria_cliente();
	void adiciona_cliente(cliente*,FILE*);
//funções impressao:
	cliente* le_cliente(FILE*);
	void imprime_cliente(cliente*);
	void ler_clientes(FILE*);
//auxiliares:
	int tamanho_cliente();

#endif //fim do ifdef CLIENTES_H
