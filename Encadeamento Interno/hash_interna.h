#pragma once
#include "clientes.h"

int hash(int);

FILE* inicializa_hash();

void insere_interna(cliente *, FILE*);

//utilizado para interagir com usurio na busca.
void busca(int, FILE*, int*, int *);

//utilizado para interagir com usuario na remocao.
void remove_interna(int, FILE*);
