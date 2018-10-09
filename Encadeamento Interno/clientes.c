#include "clientes.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

cliente* cria_cliente(){
	int cod;
	char nome[100];

	cliente *client = (cliente*) malloc(sizeof(cliente));
	printf("Digite o codigo do cliente\n");

	strcpy(nome,"Tabela Hash");
	scanf("%d",&cod);

	//nome[strcspn(nome,"\n")] = 0;

	//coloca os atributos recebidos
	strcpy(client->nome,nome);
	client->codCliente = cod;
	client->prox = -1;
	client->status = 0;

	return client;
}
void ler_clientes(FILE* in)
{
	rewind(in);

	cliente* aux = NULL;

	for (int i = 0;i>-1; i++)
	{
		aux = le_cliente(in);
		if(aux != NULL){
			imprime_cliente(aux);
			free(aux);
		}
		else{
			break;
		}
	}
}
void imprime_cliente(cliente* client)
{
	printf("**********************************************");
    printf("\nCodigo do cliente: ");
    printf("%d", client->codCliente);
    printf("\nNome do cliente: ");
    printf("%s", client->nome);
    printf("\nProx do cliente: ");
    printf("%d\n", client->prox);
    printf("\nStatus do cliente: ");
    printf("%d\n", client->status);
    printf("**********************************************\n");
}
void adiciona_cliente(cliente* client,FILE* out)
{
	if(!client) return; //verifica se o cliente é diferente de NULL

	fwrite(&client->codCliente, sizeof(int), 1, out);
    fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
    fwrite(&client->prox,sizeof(int),1,out);
    fwrite(&client->status,sizeof(int),1,out);
    free(client);
    fflush(out);
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

	if(fread(&temp->codCliente, sizeof(int), 1, clientesDat) > 0){
		fread(temp->nome, sizeof(char), sizeof(temp->nome), clientesDat);
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
	int cod = 0;

	if(fread(&cod,sizeof(int),1,clientesDat))
	{
	    //printf("o%d cod\n", cod);
		fseek(clientesDat,tamanho_cliente() - 2*sizeof(int),SEEK_CUR);

		fread(&status,sizeof(int),1,clientesDat);
	}
		//if(status == -1) puts("oi");
    //printf("%d Status\n", status);
	return status;
}

int le_status_keep(FILE* clientesDat){
	int status = -1;
	int cod = 0;

	if(fread(&cod,sizeof(int),1,clientesDat))
	{
	    //printf("o%d cod\n", cod);
		fseek(clientesDat,tamanho_cliente() - 2*sizeof(int),SEEK_CUR);

		fread(&status,sizeof(int),1,clientesDat);
	}
	fseek(clientesDat, -tamanho_cliente(), SEEK_CUR);
		//if(status == -1) puts("oi");
    //printf("%d Status\n", status);
	return status;
}

int le_proximo(FILE* clientesDat){
    int prox = 0;
    int cod = 0;
    if(fread(&cod, sizeof(int),1,clientesDat)){
        fseek(clientesDat, tamanho_cliente() - 3*sizeof(int), SEEK_CUR);
        fread(&prox, sizeof(int), 1, clientesDat);
    }
    return prox;
}
int le_proximo_keep(FILE* clientesDat){
    int prox = 0;
    int cod = 0;
    if(fread(&cod, sizeof(int),1,clientesDat)){
        fseek(clientesDat, tamanho_cliente() - 3*sizeof(int), SEEK_CUR);
        fread(&prox, sizeof(int), 1, clientesDat);
    }
    fseek(clientesDat, -tamanho_cliente()+sizeof(int), SEEK_CUR);
    //p(prox,"prox")
    return prox;
}
int le_codigo(FILE* clientesDat)
{
	int codigo = 7;

	if(fread(&codigo,sizeof(int),1,clientesDat))
	{
	    //oi
		fseek(clientesDat,tamanho_cliente() - sizeof(int),SEEK_CUR);
	}
    //printf("Cod = %d\n", codigo);
	return codigo;
}

int le_codigo_keep(FILE* clientesDat)
{
	int codigo = -1;

	if(fread(&codigo,sizeof(int),1,clientesDat))
	{
	    //oi
		fseek(clientesDat,tamanho_cliente() - sizeof(int),SEEK_CUR);
	}
	fseek(clientesDat, -tamanho_cliente(), SEEK_CUR);
    //printf("Cod = %d\n", codigo);
	return codigo;
}
void sobrescreve_prox(FILE* tabHash, int new_prox){
    fseek(tabHash,tamanho_cliente()-2*sizeof(int), SEEK_CUR);
    fwrite(&new_prox, sizeof(int), 1, tabHash);
    fseek(tabHash, -(tamanho_cliente()-sizeof(int)), SEEK_CUR);

}
void sobrescreve_cod(FILE* tabHash, int new_cod){
    fwrite(&new_cod, sizeof(int), 1, tabHash);
    fseek(tabHash, -sizeof(int), SEEK_CUR);
}
void sobrescreve_status(FILE* tabHash, int new_status){
    fseek(tabHash, tamanho_cliente()-sizeof(int), SEEK_CUR);
    fwrite(&new_status, sizeof(int), 1, tabHash);
    fseek(tabHash, -tamanho_cliente(), SEEK_CUR);
}
void remove_cliente(FILE* clientesDat)
{
	int status = 1;//me diz que o status é 1 e o cliente pode ser removido.
	fseek(clientesDat,tamanho_cliente() - sizeof(int),SEEK_CUR);//coloca o ponteiro para escrever no status
	fwrite(&status,sizeof(int),1,clientesDat);//sobrescreve o status com true para dizer que pode escrever nessa posição
}

void ler_hash(FILE* hash)
{
	rewind(hash);

	int aux = -1;

	printf("********************************************\n");
	for (int i = 0;i<7; i++)
	{
		fread(&aux,sizeof(int),1,hash);
		printf("hash posição %d = %d\n",i,aux);
	}
	printf("********************************************\n");
}
