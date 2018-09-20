#pragma once

#include <stdio.h>
#include <string.h>
#include "clientes.c"
//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes



// retorna o indice onde sera inserido o registro
// de acordo com o seu codigo
int hash(int);

// inicializa o arquivo hash de ponteiros pra listas
void inicializa(FILE*);

void insere(cliente*,FILE*,FILE*);