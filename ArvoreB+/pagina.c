#include "pagina.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

pagina* cria_pagina(int pai,int quantos, int* indice,int* folhas,int tipo_filho)
{
	pagina* pag = (pagina*)malloc(tamanho_pagina());
	int i;

	pag->pai = pai;

	pag->indice = (int*)malloc(sizeof(int)*ORDEM*2);
	pag->folhas = (int*)malloc(sizeof(int)*(ORDEM*2+1));

	for (i = 0; i < ORDEM*2; i++)
	{
		pag->indice[i]=indice[i];
		pag->folhas[i]=folhas[i];
		//printf("i = %d\n",i);
	}
	//printf("i fora do loop = %d\n",i);
	pag->folhas[i]=folhas[i];
	pag->tipo_filho = tipo_filho;
	pag->quantos=quantos;
	pag->status = 0;

	return pag;
}
pagina* le_pagina(FILE* out)
{

	pagina* pag = (pagina*)malloc(tamanho_pagina());
	if(fread(&pag->pai, sizeof(int), 1, out) > 0)
	{
		pag->indice = (int*)malloc(sizeof(int)*2*ORDEM);
		pag->folhas = (int*)malloc(sizeof(int)*(2*ORDEM+1));
		fread(&pag->quantos, sizeof(int), 1, out);
		for(int i = 0; i < 2*ORDEM; i++)
		{
			fread(&(pag->indice[i]), sizeof(int), 1, out);
		}
		for(int i = 0; i < 2*ORDEM + 1; ++i)
		{
			fread(&(pag->folhas[i]), sizeof(int), 1, out);
		}

	   	fread(&pag->tipo_filho, sizeof(int), 1, out);
	   	fread(&pag->status, sizeof(int), 1, out);
	}
	else
	{
		free(pag);
		pag=NULL;
	}
	return pag;
}
void adiciona_pagina(pagina* pag,FILE* out)
{
	if(!pag) return; //verifica se o page é diferente de NULL

	fwrite(&pag->pai, sizeof(int), 1, out);
	fwrite(&pag->quantos, sizeof(int), 1, out);
	for (int i = 0; i < 2*ORDEM; i++)
	{
		fwrite(&(pag->indice[i]), sizeof(int), 1, out);
	}
	for (int i = 0; i < 2*ORDEM + 1; ++i)
	{
		fwrite(&(pag->folhas[i]), sizeof(int), 1, out);
	}

   	fwrite(&pag->tipo_filho, sizeof(int), 1, out);
   	fwrite(&pag->status, sizeof(int), 1, out);

    fflush(out);
}
void imprime_pagina(pagina* pag)
{
	int i;
	printf("*************************************************");
    printf("\nPai: ");
    printf("%d",pag->pai);
    printf("\nQuantos tem: ");
    printf("%d",pag->quantos);
    printf("\nApontado e Indices: ");
    for(i = 0; i < 2*ORDEM; ++i)
    {
    	printf("P%d ",pag->folhas[i]);
    	printf("%d ",pag->indice[i]);
    }
    printf("P%d ",pag->folhas[i]);
    printf("\nTipo do filho: ");
    printf("%d",pag->tipo_filho);
    printf("\nStatus da pagina: ");
    printf("%d\n",pag->status);
    printf("*************************************************\n\n");
}
void ler_paginas(FILE* in)
{
	fflush(in);
	rewind(in);
	int conta=0;
	pagina* aux = NULL;

	for (int i = 0;i>-1; i++)
	{
		aux = le_pagina(in);
		if(aux != NULL){
			printf("onde essa pagina está no arquivo:%d\n",conta);
			conta++;
			imprime_pagina(aux);
			free(aux);
		}
		else{
			break;
		}
	}
}
int tamanho_pagina(void)
{
	return sizeof(int)
		 + sizeof(int)
		 + sizeof(int)*2*ORDEM
		 + sizeof(int)*(2*ORDEM + 1)
		 + sizeof(int)
		 + sizeof(int);
}
