#ifndef PAGINA_H
#define PAGINA_H

#include "clientes.h"

typedef struct PaginaI{
	int pai; // ponteiro para a pagina pai
	int quantos; //conta quantos indices ha no vetor
	int *indice; //vetor para guardar os indices
	int *folhas; //vetor para guardar os ponteiros para as folhas
	int tipo_filho;//me diz se aponta pra folha ou pagina | 0 para pagina 1 para folha
	int status; //me diz se pode escrever ou não
}pagina;

//pagina
	pagina* cria_pagina(int,int,int*,int*,int);
	pagina* le_pagina(FILE*);
	void adiciona_pagina(pagina*,FILE*);
	void imprime_pagina(pagina*);
	void ler_paginas(FILE*);
	int tamanho_pagina(void);

#endif// final do ifdef PAGINA_H
