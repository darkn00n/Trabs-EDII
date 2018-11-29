#ifndef FOLHA_H
#define FOLHA_H

#include "clientes.h"

typedef struct Pagina{
	int pai; // ponteiro para a pagina pai
	int quantos; // conta quantos registros ha
	cliente **clientes;// vetor para guardar os registros
	int prox; // ponteiro para a proxima pagina folha
	int status; // me diz se pode escrever ou não
}folha;

//folha
	folha* cria_folha(int ,int ,cliente** ,int );
	folha* le_folha(FILE* );
	void adiciona_folha(folha*,FILE*);
	void imprime_folha(folha*);
	void ler_folhas(FILE*);
	int tamanho_folha(void);

#endif // final do ifdef FOLHA_H
