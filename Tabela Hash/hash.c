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
//insere o cliente na tabela hash resetando todos os valores nescessarios.
void insere(cliente* cliente,FILE* tabHash,FILE* clientesDat){
	int cursor; // cursor para posicionar na tabHash
	int listFirst; // para verificar se na posicao achada ja tem uma lista ou esta vazia
	int posicao = 0; // contabiliza quantos registros foram lidos para atualizar tabela Hash
	int temp = 0;//me diz o status do cliente lido.

	cursor = sizeof(int)*hash(cliente->codCliente);//calcula qual local da hash vai ficar o cliente

	fseek(tabHash,cursor,SEEK_SET);//prepara o ponteiro para ler o que tem na hash

	fread(&listFirst,sizeof(int),1,tabHash);//le a hash

	rewind(clientesDat);//confirma que o ponteiro de clientes esta no começo

	if(listFirst == -1)//se nao houver colisao
	{
		temp = le_status(clientesDat);//pega o codigo do primeiro cliente

		if(temp == -1){//se o arquivo de clientes já esta vazio
			adiciona_cliente(cliente,clientesDat);
		}
		else//se existe cliente
		{
			while(temp == 0)//se o cliente tem status 0 ele ainda existe, logo procura o final do arquivou ou um cliente com 1.
			{
				temp = le_status(clientesDat);//le o status do proximo cliente
				posicao++;//contador para saber o endereço do cliente atual
			}
			if(temp == 1){//se eu achei um cara que pode ser reescrito, preciso voltar para escrever sobre
				fseek(clientesDat,-tamanho_cliente(),SEEK_CUR);
			}

			adiciona_cliente(cliente,clientesDat);//escrevo o cliente no final do arquivou ou em cima de um vazio.
		}
	}
	else{//se ja tem um cara com esse codigo, logo, existe colisao
		cliente->prox = listFirst;//adiciona o ponteiro do novo cliente para o inicio da lista
		//o restante é igual ao caso acima.
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

int busca_cod(int codigo,FILE* tabHash,FILE* clientesDat)
{
	int cursor; // cursor para posicionar na tabHash
	int listFirst; //pega o endereço do primeiro cara.
	cliente* temp;//auxiliar para ter o cliente lido.

	cursor = sizeof(int)*hash(codigo);//calcula qual local da hash vai ficar o cliente

	fseek(tabHash,cursor,SEEK_SET);//prepara o ponteiro para ler o que tem na hash
	fread(&listFirst,sizeof(int),1,tabHash);//le a hash

	if(listFirst == -1) return -1;//o cliente não existe pq a lista esta vazia.
	else
	{
		fseek(clientesDat,listFirst*tamanho_cliente(),SEEK_SET);//vai para o primeiro da lista.
		temp = le_cliente(clientesDat);//le o primeiro cliente da lista.

		if(temp->codCliente == codigo)//o primeiro cliente da lista é o cliente com o codigo
		{
			return listFirst;
		}
		else
		{
			while(temp->prox != -1)
			{
				listFirst = temp->prox;//recebe o proximo da lista.
				fseek(clientesDat,listFirst*tamanho_cliente(),SEEK_SET);//vai para o proximo da lista.
				temp = le_cliente(clientesDat);//le o proximo cliente da lista.
				if(temp->codCliente == codigo) return listFirst;//encontrei no meio da lista.
			}
		}
	}
	return -1;//se tiver um problema eu saio retornando que não existe.
}

void busca(int codigo,FILE* tabHash,FILE* clientesDat)
{
	int onde = busca_cod(codigo,tabHash,clientesDat);

	if(onde != -1)
	{
		fseek(clientesDat,onde*tamanho_cliente(),SEEK_SET);//vai no cliente encontrado
		printf("O cliente econtrado é:\n\n");
		imprime_cliente(le_cliente(clientesDat));//imprime o cliente encontrado
	}
	else printf("\nO cliente procurado não existe\n\n");
}









































/*for(int i=0;i<M;i++){
		fwrite(&menosUm, sizeof(int), 1, hashFile);
		write('a', sizeof(char), sizeof(cliente->nome), hashFile);
		write(&menosUm, sizeof(int), sizeof(cliente*), hashFile);
	}
*/