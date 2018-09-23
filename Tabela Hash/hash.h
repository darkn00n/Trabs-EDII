#pragma once

#include "clientes.h"
//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes



// retorna o indice onde sera inserido o registro
// de acordo com o seu codigo
int hash(int);

// inicializa o arquivo hash de ponteiros pra listas
void inicializa(FILE*);

void insere(cliente*,FILE*,FILE*);

//busca o codigo na tabela hash, retorna -1 se não existe e retorna um inteiro com o endereço dele na tabela.
int busca_cod(int ,FILE*,FILE*);
void busca(int,FILE*,FILE*);
