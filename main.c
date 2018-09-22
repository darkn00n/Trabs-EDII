#include "clientes.c"
#include "hash.c"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

int main()
{
	int entrar=1,acao=0;

	FILE* hashFile = fopen("tabHash.dat","r+b");
	FILE* clientes = fopen("clientes.dat","r+b");

	while(entrar)
	{
		printf("1 para registrar um cliente\n2 para ver clientes\n3 para pesquisar um cliente\n4 para sair do programa\n");
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
				//remove_cliente(clientes);
				break;
			case 4:
				entrar=0;
				break;
			case 5:
				inicializa(hashFile);
				break;
			default:
				printf("caso invalido tente novamente\n");
				break;
		}
	}
	fclose(clientes);
	return 0;
}