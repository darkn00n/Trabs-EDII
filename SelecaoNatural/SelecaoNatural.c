#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 6 // M eh o tamanho de registros que a mmoria aguenta por vez
#define oi puts("aqui fdp!!");
//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

typedef struct Cliente
{
	int codCliente;
	char nome[50];
	char dataNascimento[11];
}cliente;


int achaMenor(cliente**,int*);
int decisao(cliente*,int,int);
void salva(cliente*, FILE*);
void imprime_cliente(cliente*,FILE*);
cliente* le_cliente(FILE*);
FILE** selecao_natural(FILE**,int*);

int main(){

	int rodando = 1;
	int escolha;
	int contParticoes = 0 ; // contar quantidade de particoes abertas
	FILE** particoes;
	char nome[20];
	strcpy(nome,"ParticaoA.dat"); //inicializando a string nome com o nome da primeira particao
	particoes = (FILE**)malloc(sizeof(FILE*)*250);

	for(int i=0;i<250;i++){
		particoes[i] = NULL;
	}

	while(rodando){
		printf("\n===============================================================================\n                                  Selecao Natural\n");
		printf("\n                 Digite 1 para ordenar o arquivo de registros\n                 Digite 2 para mostrar as particoes ordenadas\n                 Digite 3 para sair\n");
		printf("\n         By: Douglas Custodio, Jose Lucas, Vitor Pinheiro - UFRRJ");
		printf("\n===============================================================================\n");
		scanf("%d",&escolha);
		if(escolha == 1){
			printf("\n      |=================================================================|\n");
			printf("\n      |     Digite o nome do arquivo a ser lido, sem a extensao .dat    |\n");
			printf("\n      |=================================================================|\n\n\n");
			particoes = selecao_natural(particoes,&contParticoes);
			printf("\n                      |======================================|\n");
			printf("\n                      |    Particoes ordenadas criadas !     |\n");
			printf("\n                      |======================================|\n");
		}
		if(escolha == 2){
			strcpy(nome,"ParticaoA.dat");
			FILE* log = fopen("log.txt","a");
			for(int i = 0;i<contParticoes;i++){
				particoes[i] = fopen(nome,"r+b");
				fprintf(log,"\n===============================================================================\n");
				fprintf(log,"                        PRINTANDO CLIENTES DA PARTICAO %d\n",i);
				fprintf(log,"===============================================================================\n\n");
				cliente* aux;
				int keep = 1;
				while(keep){
					keep = 0;
					aux = le_cliente(particoes[i]);
					if(aux != NULL){
						imprime_cliente(aux,log);
						keep = 1;
					}
				}
				fclose(particoes[i]);
				nome[8]++;
			}
			fclose(log);

			printf("\n                    |===========================================|\n");
			printf("\n                    |    Arquivo log.txt criado com sucesso!    |\n");
			printf("\n                    |===========================================|\n");
		}
		if(escolha == 3){
			rodando = 0;
		}
		if(escolha > 3 || escolha < 1){
			printf("\n                       |======================================|\n");
			printf("\n                       |    Opcao Invalida, digite novamente  |\n");
			printf("\n                       |======================================|\n");
		}
	}
	free(particoes);
	return 0;
}
// acha o registro com menor codigo
int achaMenor(cliente** reg,int* menor){
	
	int indice = -1;
	*menor = 999999;
	for(int j=0;j<M;j++){
		if(reg[j] != NULL && reg[j]->codCliente < *menor){
			*menor = reg[j]->codCliente;
			indice = j;
		}
	}
	return indice;
}
// retorna um inteiro para definir qual acao tomar
int decisao(cliente* Ftemp,int menor,int espaco){
	if(Ftemp == NULL){
		return 1;
	}
	else if(Ftemp->codCliente < menor && espaco > 0){// se o cod do registro lido for menor q o menor anterior, joga esse novo registro para o reservatorio
		return 2;
	}
	//se o cod do registro lido for maior que o menor anterior, joga esse novo 
	// registro para o array de registros
	else if(Ftemp->codCliente > menor){
		return 3;
	}
}
// salva um registro no arquivo de saida
void salva(cliente *client, FILE *out) {
    fwrite(&client->codCliente, sizeof(int), 1, out);
	fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
	fwrite(client->dataNascimento, sizeof(char), sizeof(client->dataNascimento), out);
	free(client);
	client = NULL;
}
// imprime um registros de cliente no arquivo txt de entrada [log]
void imprime_cliente(cliente* cliente,FILE* log)
{
	fprintf(log,"**********************************************");
	fprintf(log,"\nCodigo do cliente: ");
	fprintf(log,"%d", cliente->codCliente);
    fprintf(log,"\nNome do cliente: ");
    fprintf(log,"%s", cliente->nome);
    fprintf(log,"\nData de nascimento do cliente: ");
    fprintf(log,"%s", cliente->dataNascimento);
    fprintf(log,"\n**********************************************\n");
}
// le um registro de cliente do arq de entrada
cliente* le_cliente(FILE* in){
	cliente *client = (cliente*)malloc(sizeof(cliente));
	    if (0 >= fread(&client->codCliente, sizeof(int), 1, in)) {
			free(client);
			return NULL;
	    }
	    fread(client->nome, sizeof(char) , sizeof(client->nome) , in);
	    fread(client->dataNascimento, sizeof(char), sizeof(client->dataNascimento), in);
	    return client;
}
// realiza o algoritmo de selecao natural
FILE** selecao_natural(FILE** particao,int* contParticoes){

	FILE* registros; // ponteiro para o arquivo de registros
	FILE* reservatorio;

	cliente* Ftemp; // guardara um registro temporario
 	cliente** reg;
 	reg = (cliente**)malloc(sizeof(cliente*)*M); // guardara M[nesse caso 6] registros do arquivo na memoria
	
	char nome[20];
	char regName[50];
	
	int menor = 999999; // guarda menor valor do array
	int indice; // guarda o indice do menor valor no array
	int espaco; // espaco livre no reservatorio
	int contador = 0; // conta quantas particoes foram abertas
	int running = 1; // Para determinar se o algoritmo acabou
	int set=0;// define quando acaba o while de running
	int indiceAnt;
	scanf("%s",regName); // pega o nome do arquivo de registros a ser lido
	int novo = strlen(regName); // pega o tamanho da string para poder criar uma nova string com +4 posicoes para adicionar o .dat

	char newName[novo+4]; // nova string pra guardar o nome do arquivo com o .dat no final

	strcpy(newName,regName); // copia conteudo de nome para new name
	strcat(newName,".dat"); // adiciona a extensao .dat para o fim da string newName
	strcpy(nome,"ParticaoA.dat"); //inicializando a string nome com o nome da primeira particao
	
	registros = fopen(newName,"r+b"); // abrindo arquivo de registros
	reservatorio = fopen("reservatorio.dat","w+b"); // abre o arquivo reservatorio
	
	cliente ant;

	// lendo 8 registros, admitindo que a maquina so consegue ler 8 registros por vez
	for(int i=0;i<M;i++){
		reg[i] = le_cliente(registros);
	}
	while(running){
		ant.codCliente = -1;
		espaco = M;
		// enquanto ha espaco no reservatorio, realiza operacoes em uma mesma particao
		while(espaco >= 0){
			
			indice = achaMenor(reg,&menor);

			if(particao[contador] == NULL){
				*contParticoes += 1; 
				particao[contador] = fopen(nome,"w+b");
			} // abre arquivo de particao no contador com nome atualizado
			ant = *(reg[indice]);
			if(ant.codCliente != reg[indice]->codCliente)
			{
				salva(reg[indice],particao[contador]); // salvo o registro de menor valor na particao aberta atual
				reg[indice] = NULL;
			}
			if(set==0)
			{
				Ftemp = le_cliente(registros); // carrega o proximo registro para um registro temporario
				int option = decisao(Ftemp,menor,espaco);

				switch(option){
					case 1: 
						free(reg[indice]);
						reg[indice]=NULL; // coloca null no reg[indice]
						set = 1;
						break; 
					
					case 2:
						salva(Ftemp,reservatorio); // salva Ftemp no reservatorio
						Ftemp = NULL;
						espaco--;
						break;
					
					case 3:
						reg[indice] = Ftemp; // insere o registro no array de registros pois o 
						break;
				}
				// se espaco eh zero, entao reservatorio encheu
				if(espaco == 0){
					int controle=1;
					while(controle)
					{
						controle = 0;

						indice = achaMenor(reg,&menor);

						if(indice >= 0){
							salva(reg[indice],particao[contador]);
							reg[indice] = NULL;
							controle = 1;
						} 
					}
					
					fclose(particao[contador]); // fecha a particao pois o reservatorio encheu
					contador++; // incrementa contador de particoes pois uma nova particao foi criada

					rewind(reservatorio);

					// carrega os M registros salvos do reservatorio, na memoria
					for(int i=0;i<M;i++){
						reg[i] = le_cliente(reservatorio);
					}

					reservatorio = fopen("reservatorio.dat","w+b");
					nome[8] += 1; // altera o nome de particao0.dat para particao1.dat para criar um novo arquivo de particao
					if(particao[contador] == NULL){ 
						*contParticoes +=1;
						particao[contador] = fopen(nome,"w+b");
					} // cria uma nova particao com o nome atualizado
					espaco = -1;
				}
			}
			if(set == 1)
			{ 
				int controle=1;
				indice = achaMenor(reg,&menor);
				while(controle)
				{
					controle = 0;

					indice = achaMenor(reg,&menor);

					if(indice >= 0){
						salva(reg[indice],particao[contador]);
						reg[indice] = NULL;
						controle = 1;
					} 
				}
				
				fclose(particao[contador]); // fecha a particao pois o reservatorio encheu
				contador++; // incrementa contador de particoes pois uma nova particao foi criada
				rewind(reservatorio);
				// carrega os M registros salvos do reservatorio, na memoria
				for(int i=0;i<M;i++){
					reg[i] = le_cliente(reservatorio);
				}
				indice = achaMenor(reg,&menor);
				nome[8] += 1;// altera o nome de particao0.dat para particao1.dat para criar um novo arquivo de particao
				if(particao[contador] == NULL && indice>=0){
					*contParticoes += 1; 
					particao[contador] = fopen(nome,"w+b");

					controle = 1;
					while(controle)
					{
						controle = 0;

						indice = achaMenor(reg,&menor);
						if(indice >= 0){
							salva(reg[indice],particao[contador]);
							reg[indice] = NULL;
							controle = 1;
						} 
					}
					printf("contador:%d\n",contador);
					fclose(particao[contador]); // fecha a particao pois o reservatorio encheu
				} // abre arquivo de particao no contador com nome atualizado
				running=0;
				espaco=-1;	
			}
		}
	}
	fclose(reservatorio);
	free(reg);
	return particao;
}
