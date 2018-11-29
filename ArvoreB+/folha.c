#include "folha.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

folha* cria_folha(int pai,int quantos,cliente** clientes,int prox)
{

	folha* fol = (folha*)malloc(tamanho_folha());
	int i;

	fol->pai = pai;

	fol->clientes = (cliente**)malloc(sizeof(cliente*)*2*ORDEM);

	for (i = 0; i < ORDEM*2; ++i)
	{
		fol->clientes[i] = clientes[i];
	}
	free(clientes);

	fol->quantos=quantos;
	fol->prox = prox;
	fol->status = 0;

	return fol;
}
folha* le_folha(FILE* out)
{

	folha* fol = (folha*)malloc(tamanho_folha());

	if(fread(&fol->pai, sizeof(int), 1, out) > 0)
	{
		fol->clientes = (cliente**)malloc(sizeof(cliente*)*2*ORDEM);
		fread(&fol->quantos, sizeof(int), 1, out);
		for (int i = 0; i < 2*ORDEM; ++i)
		{
			fol->clientes[i] = le_cliente(out);
		}
	   	fread(&fol->prox, sizeof(int), 1, out);
	   	fread(&fol->status, sizeof(int), 1, out);
	}
	else{
		free(fol);
		fol=NULL;
	}
	return fol;
}
void adiciona_folha(folha* folha,FILE* out)
{
	if(!folha) return; //verifica se o folhae é diferente de NULL

	fwrite(&folha->pai, sizeof(int), 1, out);
	fwrite(&folha->quantos, sizeof(int), 1, out);
	for (int i = 0; i < 2*ORDEM; ++i)
	{
		adiciona_cliente(folha->clientes[i],out);
	}
 	fwrite(&folha->prox, sizeof(int), 1, out);
 	fwrite(&folha->status, sizeof(int), 1, out);

  fflush(out);
}
void imprime_folha(folha* folha)
{
	int i;
	printf("*************************************************");
    printf("\nPai: ");
    printf("%d",folha->pai);
    printf("\nQuantos tem: ");
    printf("%d",folha->quantos);
    printf("\nclientes: \n");
    for(i = 0; i < folha->quantos; ++i)
    {
    	imprime_cliente(folha->clientes[i]);
    }
    printf("\nProximo: ");
    printf("%d",folha->prox);
    printf("\nStatus da folha: ");
    printf("%d\n",folha->status);
    printf("*************************************************\n\n");
}
void ler_folhas(FILE* in)
{
	fflush(in);
	rewind(in);
	int conta=0;

	folha* aux = NULL;
	for (int i = 0;i>-1; i++)
	{
		aux = le_folha(in);
		if(aux != NULL){
			printf("onde essa folha está no arquivo:%d\n",conta);
			conta++;
			imprime_folha(aux);
			free(aux);
		}
		else{
			break;
		}
	}
}
int tamanho_folha(void)
{
	return sizeof(int)//pai
		 + tamanho_cliente()*2*ORDEM//clientes
		 + sizeof(int)//quantos
		 + sizeof(int)//prox
		 + sizeof(int);//status
}
