#include "B+.h"

int main(int argc, char const *argv[])
{
	FILE* metadados = fopen("metadados.dat","r+b");
	FILE* dados = fopen("paginas.dat","r+b");
	FILE* index = fopen("indexador.dat","r+b");

	int entrar=1,acao,cod;

	while(entrar)
	{
		printf("1 para registrar um cliente\n2 para ver paginas\n3 para ver a folhas\n4 para remover um cliente\n5 para pesquisar um cliente\n6 para sair do programa\n");
		scanf("%d",&acao);
		getchar();
		switch(acao)
		{
			case 1:
				insere(metadados,index,dados);
				break;
			case 2:
				ler_paginas(index);
				break;
			case 3:
				ler_folhas(dados);
				break;
			case 4:
				printf("Digite o codigo do cliente a ser removido.\n");
				scanf("%d",&cod);
				remocao(cod,metadados,index,dados);
				break;
			case 5:
				printf("Digite o codigo a ser buscado.\n");
				scanf("%d",&cod);
				busca_aux(cod,metadados,index,dados);
				break;
			case 6:
				entrar=0;
				break;
			default:
				printf("caso invalido tente novamente\n");
				break;
		}
	}

	return 0;
}
