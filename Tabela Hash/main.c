#include "clientes.c"
#include "hash.c"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

int main()
{
	int entrar=1,acao=0,cod;

	FILE* hashFile = fopen("tabHash.dat","r+b");
	FILE* clientes = fopen("clientes.dat","r+b");

	while(entrar)
	{
		printf("1 para registrar um cliente\n2 para ver clientes\n3 para ver a tabela hash\n4 para remover um cliente\n5 para pesquisar um cliente\n6 para sair do programa\n7 para iniciar a tabela hash\n");
		scanf("%d",&acao);
		getchar();
		switch(acao)
		{
			case 1:
				insere(cria_cliente(),hashFile,clientes);
				break;
			case 2:
				ler_clientes(clientes);
				break;
			case 3:
				ler_hash(hashFile);
				break;
			case 4:
				printf("Digite o codigo do cliente a ser removido.\n");
				scanf("%d",&cod);
				Remove(cod,hashFile,clientes);
				break;
			case 5:
				printf("Digite o codigo a ser buscado.\n");
				scanf("%d",&cod);
				busca(cod,hashFile,clientes);
				break;	
			case 6:
				entrar=0;	
				break;
			case 7:
				printf("Tem certeza que deseja reinicializar a tabela hash?\n");
				scanf("%d",&cod);
				if(cod==1) inicializa(hashFile);
				break;
			default:
				printf("caso invalido tente novamente\n");
				break;
		}
	}
	fclose(clientes);
	return 0;
}