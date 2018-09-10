#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define M 6 // M eh o tamanho de registros que a mmoria aguenta por vez
#define F 4 // Define quantos arquivos podem ser abertos para a intercalação otima
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

typedef struct List_file
{
	FILE* info;
	struct List_file* prox; 
}list_file;

typedef struct fila_Particoes
{
	 list_file* start;
	 list_file* end;
}fila;
//inicio do TAD de fila
	fila* fila_cria(void);
	void fila_insere(fila* f,FILE* v);
	FILE* fila_retira(fila* f);
	int fila_vazia(fila* f);
	void fila_libera(fila* f);
//Fim do TAD de fila

//inicio do TAD de arquivo binario
	void imprime_cliente(cliente*,FILE*);
	cliente* le_cliente(FILE*);
//fim do TAD de arquivo binario

//funcoes para selecao natural
	int achaMenor(cliente**,int*);
	int decisao(cliente*,int,int);
	void salva(cliente*, FILE*);
	fila* selecao_natural(fila*,int*);
//fim das funcoes para selecao natural

//funcoes para intercalacao otima
	void preenche_entrada(FILE**,FILE**,int*);//essa
	FILE** inter_otima(FILE**,int*);//essa
	void arvore_campeao(FILE** entrada,FILE** particoes,int* qual_estou)//em processo de criacao
//fim das funcoes para intercalacao otima

int main(){

	int rodando = 1;
	int escolha;
	int contParticoes = 0 ; // contar quantidade de particoes abertas
	fila* particoes;
	char nome[20];
	strcpy(nome,"ParticaoA.dat"); //inicializando a string nome com o nome da primeira particao
	
	particoes = fila_cria();

	while(rodando){
		printf("\n===============================================================================\n                                  Selecao Natural\n");
		printf("\n                 Digite 1 para ordenar o arquivo de registros\n                 Digite 2 para mostrar as particoes ordenadas\n                 Digite 3 para sair\n");
		printf("\n         By: Douglas Custodio, Jose Lucas, Vitor Pinheiro - UFRRJ");
		printf("\n===============================================================================\n");
		//scanf("%d",&escolha);
		escolha=1;//tirar depois e coocar a linha de cima de novo.
		if(escolha == 1){
			printf("\n      |=================================================================|\n");
			printf("\n      |     Digite o nome do arquivo a ser lido, sem a extensao .dat    |\n");
			printf("\n      |=================================================================|\n\n\n");
			particoes = selecao_natural(particoes,&contParticoes);
			printf("\n                      |======================================|\n");
			printf("\n                      |    Particoes ordenadas criadas !     |\n");
			printf("\n                      |======================================|\n");
			particoes=inter_otima(particoes,&contParticoes);//realiza a intercalação das partes criadas na selecao natural
			escolha = 3;//tirar depois
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
	fila_libera(particoes);
	return 0;
}
//TAD de lista
	fila* fila_cria(void){
		fila* f = (fila*)malloc(sizeof(fila));
		f->ini = f->fim = NULL;
		return f;
	}
	void fila_insere(fila* f,FILE* v){
		lista* n = (lista*)malloc(sizeof(lista));
		
		n->info = v;
		n->prox=NULL;
		if(f->fim != NULL) f->fim->prox = n;
		else f->ini = n;
		f->fim = n;
	}
	FILE* fila_retira(fila* f){
		lista* t;
		FILE* v;
		if(fila_vazia(f)){
			printf("fila vazia FDP!!!\n");
			exit(1);
		}
		t = f->ini;
		v = t->info;
		f->ini = t->prox;
		if(f->ini == NULL) f->fim=NULL;
		free(t);
		return v;
	}
	void fila_libera(fila* f){
		lista* q = f->ini;
		while(q!=NULL){
			lista* t = q->prox;
			free(q);
			q = t;
		}
		free(f);
	}
	int fila_vazia(fila* f){
		if(f->ini == NULL) return 1;
		return 0;
	}
//Fim do TAD de lista
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
fila* selecao_natural(fila* particao,int* contParticoes){

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
	//scanf("%s",regName); // pega o nome do arquivo de registros a ser lido
	strcpy(regName,"clientes");//tirar isso aqui depois e por a linha de cima de novo.
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

			*contParticoes += 1; 
			fila_insere(particoes,fopen(nome,"w+b"));//abre arquivo de particao no contador com nome atualizado
			//ALTERAR DAQUI PRA BAIXO pq precisa terminar de trocar o particoes pra fila!!!!!!!!!!!!!
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
//realiza o algoritmo de intercalação otima
FILE** inter_otima(FILE** particoes,int* nameFixer)
{
	
	FILE** entrada;
	FILE* saida;
	char nome[14];
	cliente* teste;

	int qual_estou = 0;
	int num_particoes = *nameFixer;
	int i;
	
	entrada = (FILE**)malloc(sizeof(FILE*)*(F-1));
	strcpy(nome,"ParticaoA.dat");

	//Abrindo as particoes de novo
	for(i = 0; i < num_particoes; i++){
		if((particoes[i] = fopen(nome, "a+b")) == NULL) perror("Erro no arquivo\n");
		nome[8] ++;
	}
	preenche_entrada(entrada,particoes,&qual_estou);//pega 3 caras do vetor partições e coloca no vetro entrada

	arvore_campeao(entrada,particoes,&qual_estou);

	//fechando tudo
	fclose(log);
	for(i = 0; i < num_particoes; i ++){
		fclose(particoes[i]);
	}
	return particoes;
}
//pega os 3 primeiros do vetor de particoes e coloca no vetor de entrada.
void preenche_entrada(FILE** entrada,FILE** particoes,int* qual_estou)
{
	for(int i=(*qual_estou);i<F-1; i++,(*qual_estou)++) {
		entrada[i] = particoes[(*qual_estou)];
		if(entrada[i] == NULL) perror("entrada nula");
	}
}
//faz uma interação da arvore de campeões
//essa aqui ainda tá sendo feita !!!!!!
void arvore_campeao(FILE** entrada,FILE** particoes,int* qual_estou)
{

}