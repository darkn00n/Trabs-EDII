#pragma once

#include "clientes.c"

#define ORDEM 3

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

typedef struct Pagina{
	int pai; // ponteiro para a pagina pai
	cliente clientes[2*ORDEM]; // vetor para guardar os registros
	int quantos; // conta quantos registros ha
	int prox; // ponteiro para a proxima pagina folha
}folha;

typedef struct PaginaI{
	int pai; // ponteiro para a pagina pai
	int indice[2*ORDEM]; // vetor para guardar os indices
	int folhas[2*ORDEM+1]; // vetor para guardar os ponteiros para as folhas
	int quantos; // conta quantos indices ha no vetor
}pagina;