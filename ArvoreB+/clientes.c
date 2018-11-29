#include "clientes.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

cliente* cria_cliente()
{
	int cod;
	char nome[100];

	cliente *client = (cliente*) malloc(sizeof(cliente));
	printf("digite o codigo do cliente\n");

	strcpy(nome,"B+");
	scanf("%d",&cod);

	//nome[strcspn(nome,"\n")] = 0;

	//coloca os atributos recebidos
	strcpy(client->nome,nome);
	client->codCliente = cod;

	return client;
}
void adiciona_cliente(cliente* client,FILE* out)
{
	if(client == NULL){
		fseek(out,tamanho_cliente(),SEEK_CUR);//verifica se o cliente é diferente de NULL
	}
	else{
		fwrite(&client->codCliente, sizeof(int), 1, out);
    	fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
	}

    fflush(out);
}
cliente* le_cliente(FILE* clientesDat)
{
	cliente* temp = (cliente*)malloc(sizeof(cliente));
	if(fread(&temp->codCliente, sizeof(int), 1, clientesDat) > 0){
		if(temp->codCliente == 0){
			fseek(clientesDat,tamanho_cliente()-sizeof(int),SEEK_CUR);
			free(temp);
			temp = NULL;
	    }
	    else{
	    	fread(temp->nome, sizeof(char), sizeof(temp->nome), clientesDat);
	    }
	}
	else{
		free(temp);
		temp = NULL;
	}
    return temp;
}
void imprime_cliente(cliente* client)
{
	printf("   **********************************************");
    printf("\n    Codigo do cliente: ");
    printf("%d", client->codCliente);
    printf("\n    Nome do cliente: ");
    printf("%s", client->nome);
    printf("\n   **********************************************\n");
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
int tamanho_cliente()
{
	return sizeof(int)  //cod
         + sizeof(char)*100;
}
