#include "banco.h"
//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

//funções para incerção na tabela:
	cliente* cria_cliente()
	{
		int cod=1;
		char nome[100];

		cliente *client = (cliente*) malloc(sizeof(cliente));
		printf("digite o codigo do cliente\n");

		strcpy(nome,"C++ Obrigatorio");
		//fgets(nome,101,stdin);
		scanf("%d",&cod);

		nome[strcspn(nome,"\n")] = 0;

		//coloca os atributos recebidos
		strcpy(client->nome,nome);
		client->codCliente = cod;

		return client;
	}

	void adiciona_cliente(cliente* client,FILE* out)
	{
		if(!client) return; //verifica se o cliente é diferente de NULL
	    
		fwrite(&client->codCliente, sizeof(int), 1, out);
	    fwrite(client->nome, sizeof(char), sizeof(client->nome), out);

	    free(client);
	}

//funções para remoção da tabela:
	void remove_cliente(FILE* out)
	{
		
	}

//funções impresao:
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
				return;
			}
		}
	}

	cliente* le_cliente(FILE* in)
	{

		cliente *client = (cliente*)malloc(sizeof(cliente));

	    if (0 >= fread(&client->codCliente, sizeof(int), 1, in)) {
			free(client);
			return NULL;
	    }
	    fread(client->nome, sizeof(char) , sizeof(client->nome) , in);

	   
	    return client;
	}

	void imprime_cliente(cliente* cliente)
	{
		printf("**********************************************");
	    printf("\nCodigo do cliente: ");
	    printf("%d", cliente->codCliente);
	    printf("\nNome do cliente: ");
	    printf("%s", cliente->nome);
	    printf("\n**********************************************\n");
	}

//pesquisar:
	cliente* pesquisa_cliente();

//auxiliares:
	int tamanho_cliente()
	{
		return sizeof(int)  //cod
	            + sizeof(char) * 100; //nome
	}
	/*
	int comparar_cliente(const void *a,const void *b)
	{
		cliente** a1 = a;
		cliente** a2 = b;

		if ( (*a1)->cod <  (*a2)->cod ) return -1;
		if ( (*a1)->cod == (*a2)->cod ) return 0;
		if ( (*a1)->cod >  (*a2)->cod ) return 1;
	}

	int achar_cod_cliente(char* nomeclient,FILE* index,FILE* out)
	{
		int tam,cod=0;
		cliente* vet;

		rewind(index);
		fread(&tam,sizeof(int),1,index);
		
		if(tam<1) return -1;
		else{
			rewind(out);
			for(int i = 0;i<tam;i++)
			{
				vet = le_cliente(out);
				if(strcmp(vet->nome,nomeclient))
				{
					cod = vet->cod;
					free(vet);
					return cod;
				}
				
				free(vet);
			}
			if(cod == 0)
			{
				printf("cliente não existe\n");
				return 0;
			}
		}
	}*/