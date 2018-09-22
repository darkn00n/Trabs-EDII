#include "hash.h"

#define M 7 // define o tamanho da tabela

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

int hash(int codigo){

	return codigo % M;
}

void inicializa(FILE* hashFile){
	int menosUm = -1;

	for(int i=0;i<M;i++){
		fwrite(&menosUm, sizeof(int), 1, hashFile);
	}
}

void insere(cliente* cliente,FILE* tabHash,FILE* clientesDat){
	int cursor; // cursor para posicionar na tabHash
	int listFirst; // para verificar se na posicao achada ja tem uma lista ou esta vazia
	int posicao = 0; // contabiliza quantos registros foram lidos para atualizar tabela Hash
	int temp = 0;

	cursor = sizeof(int)*hash(cliente->codCliente);

	fseek(tabHash,cursor,SEEK_SET);

	fread(&listFirst,sizeof(int),1,tabHash);

	rewind(clientesDat);

	if(listFirst == -1){
		temp = le_status(clientesDat);
		printf("fala seu merda:%d\n",temp);
		if(temp == -1){
			adiciona_cliente(cliente,clientesDat);
			puts("kk eai");
		}
		else{
			while(temp == 0){
				temp = le_status(clientesDat);
				printf("%d\n",temp);
				posicao++;
			}
			if(temp == 1){
				fseek(clientesDat,-tamanho_cliente(),SEEK_CUR);
				puts("kk deu merda");
			}

			adiciona_cliente(cliente,clientesDat);
		}
	}
	else{
		cliente->prox = listFirst;

		temp = le_status(clientesDat);

		if(temp == -1){
			adiciona_cliente(cliente,clientesDat);
		}
		else{
			while(temp == 0){
				temp = le_status(clientesDat);
				posicao++;
			}
			if(temp == 1) fseek(clientesDat,-tamanho_cliente(),SEEK_CUR);

			adiciona_cliente(cliente,clientesDat);
		}	
	}

	fseek(tabHash,cursor,SEEK_SET);

	fwrite(&posicao,sizeof(int),1,tabHash);

	//fflush(tabHash);
	//fflush(clientesDat);
}












































/*for(int i=0;i<M;i++){
		fwrite(&menosUm, sizeof(int), 1, hashFile);
		write('a', sizeof(char), sizeof(cliente->nome), hashFile);
		write(&menosUm, sizeof(int), sizeof(cliente*), hashFile);
	}
*/