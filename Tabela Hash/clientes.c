#include "clientes.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

cliente* cria_cliente(){
	int cod;
	char nome[100];

	cliente *client = (cliente*) malloc(sizeof(cliente));
	printf("digite o codigo do cliente\n");

	strcpy(nome,"Tabela Hash");
	scanf("%d",&cod);

	nome[strcspn(nome,"\n")] = 0;

	//coloca os atributos recebidos
	strcpy(client->nome,nome);
	client->codCliente = cod;
	cliente->prox = NULL;
	cliente->status = 1;

	return client;
}
void adiciona_cliente(cliente* client,FILE* out)
{
	if(!client) return; //verifica se o cliente é diferente de NULL
    
    
    fseek(out,hash(cliente->codCliente),SEEK_SET);
    
	fwrite(&client->codCliente, sizeof(int), 1, out);
    fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
    fwrite(&client->prox,sizeof(cliente*),1,out);
    fwrite(&client->status,sizeof(int),1,out);

    free(client);
}