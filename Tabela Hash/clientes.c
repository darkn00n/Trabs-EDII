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

	fwrite(&client->codCliente, sizeof(int), 1, out);
    fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
    fwrite(&client->prox,sizeof(int),1,out);
    fwrite(&client->status,sizeof(int),1,out);
    free(client);
}
void altera_status(FILE* file)
{
	int status = 1;
	fseek(file,tamanho_cliente() - sizeof(int),SEEK_CUR);//coloca o ponteiro para escrever no status
	fwrite(&status,sizeof(int),1,file);//sobrescreve o status com true para dizer que pode escrever nessa posição
}
int tamanho_cliente()
{
	return sizeof(int)  //cod
         + sizeof(char) * 100//nome
         + sizeof(int) //ponteiro prox
         + sizeof(int);//status
}
cliente* le_cliente(FILE* clientesDat){
	cliente* temp = (cliente*)malloc(sizeof(cliente));


	if(0 >= fread(&temp->codCliente, sizeof(int), 1, clientesDat)){
		fread(temp->nome, sizeof(char), sizeof(client->nome), clientesDat);
	    fread(&temp->prox,sizeof(int),1,clientesDat);
	    fread(&temp->status,sizeof(int),1,clientesDat);
	}
	else{
		temp = NULL;
	}

    return temp;
}
int le_status(FILE* clientesDat){
	int status = -1;

	fseek(file,tamanho_cliente() - sizeof(int),SEEK_CUR);

	fread(&status,sizeof(int),1,clientesDat);

	return status;
}