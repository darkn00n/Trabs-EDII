#include "banco.c"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

int main(int argc, char const *argv[])
{
	int entrar=1,acao=0;

	FILE* hashFile = fopen("tabHash.dat","w+b");
	FILE* clientes = fopen("clientes.dat","w+b");

	while(entrar)
	{
		printf("1 para registrar um cliente\n2 para remover um cliente\n3 para pesquisar um cliente\n");
		scanf("%d",&acao);
		getchar();
		switch(acao)
		{
			case 1:
				adiciona_cliente(cria_cliente(),clientes);
				break;
			case 2:
				ler_clientes(clientes);
				break;
			case 3:
				remove_cliente(clientes);
				break;
			case 4:
				entrar=0;
				break;
			default:
				printf("caso invalido tente novamente\n");
				break;
		}
	}
	fclose(clientes);
	return 0;
}