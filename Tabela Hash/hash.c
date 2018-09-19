#include "hash.c"
#include "clientes.c"

#define M 7 // define o tamanho da tabela

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

int hash(int codigo){
	return cod % 7;
}

void inicializa(FILE* hashFile){
	int menosUm = -1;

	for(int i=0;i<M;i++){
		fwrite(&menosUm, sizeof(int), 1, hashFile);
	}
}














































/*for(int i=0;i<M;i++){
		fwrite(&menosUm, sizeof(int), 1, hashFile);
		write('a', sizeof(char), sizeof(cliente->nome), hashFile);
		write(&menosUm, sizeof(int), sizeof(cliente*), hashFile);
	}
*/