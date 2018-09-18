#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define M 6 // M eh o tamanho de registros que a mmoria aguenta por vez
#define N 6 // N eh o tamanho do reservatorio
#define F 4 // Define quantos arquivos podem ser abertos para a intercalação otima
#define INF 999999 //numero magico pra representar infinito
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
	char nome[20];
	struct List_file* prox; 
}lista;

typedef struct fila_Particoes
{
	 lista* ini;
	 lista* fim;
}fila;

typedef struct ArvoreCliente
{
	struct ArvoreCliente* father;
	struct ArvoreCliente* ls;
	struct ArvoreCliente* rs;
	cliente* info;
	int qual_arquivo;
}arvore;
//inicio do TAD de fila
	fila* fila_cria(void);
	void fila_insere(fila* f,char*);
	FILE* fila_retira(fila* f);
	int fila_vazia(fila* f);
	void fila_libera(fila* f);
	FILE* fila_abre_ultimo(fila*);
	int quant_part(fila* );
	char* fila_nome_ini(fila*);
//Fim do TAD de fila

//inicio do TAD de arquivo binario
	void imprime_cliente(cliente*,FILE*);
	cliente* le_cliente(FILE*);
//fim do TAD de arquivo binario

//inicio do TAD de arvore
	arvore* arv_criavazia(void);
	arvore* arv_cria(cliente* cliente,int qual_arquivo,arvore* e,arvore* d,arvore* father);
	int pega_cod(arvore* quem);
	cliente* pega_cliente(arvore* quem);
	void set_father(arvore* quem);
	int arv_vazia(arvore* a);
	void arv_imprime_sim(arvore* a);
//Fim do TAD de arvore

//funcoes para selecao natural
	int achaMenor(cliente**,int*);
	int decisao(cliente*,int,int);
	void salva(cliente*, FILE*);
	fila* selecao_natural(fila*,int*);
//fim das funcoes para selecao natural

//funcoes para intercalacao otima
	void preenche_entrada(FILE**,fila*);//essa
	fila* inter_otima(fila*,int*);//essa
	arvore* cria_arvore_campeao(FILE** entrada);
	arvore* altera_arvore_campeao(arvore* arvore_campeoes,FILE** entrada);
//fim das funcoes para intercalacao otima

//main
	int main()
	{
		int rodando = 1;
		int escolha;
		int contParticoes = 0 ; // contar quantidade de particoes abertas
		fila* particoes;
		char nome[20];
		strcpy(nome,"ParticaoA.dat"); //inicializando a string nome com o nome da primeira particao
		
		particoes = fila_cria();

		while(rodando){
			printf("\n===============================================================================\n                                  Selecao Natural\n");
			printf("\n                 Digite 1 para ordenar o arquivo de registros\n                 Digite 2 para mostrar as particoes ordenadas\n                 Digite 3 para imprimir clientes\n                 Digite 4 para imprimir saida\n                 Digite 5 para sair\n");
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
				particoes = inter_otima(particoes,&contParticoes);//realiza a intercalação das partes criadas na selecao natural
			}
			if(escolha == 2){
				strcpy(nome,"ParticaoA.dat");
				FILE* log = fopen("log.txt","a");
				FILE* part;
				for(int i = 0;i<contParticoes;i++){
					part = fila_retira(particoes);
					//part = fopen(nome,"r+b");
					fprintf(log,"\n===============================================================================\n");
					fprintf(log,"                        PRINTANDO CLIENTES DA PARTICAO %d\n",i);
					fprintf(log,"===============================================================================\n\n");
					cliente* aux;
					int keep = 1;
					while(keep){
						keep = 0;
						aux = le_cliente(part);
						if(aux != NULL){
							imprime_cliente(aux,log);
							keep = 1;
						}
					}
					fclose(part);
					nome[8]++;
				}
				fclose(log);

				printf("\n                    |===========================================|\n");
				printf("\n                    |    Arquivo log.txt criado com sucesso!    |\n");
				printf("\n                    |===========================================|\n");
			}
			if(escolha == 3){
				FILE* part;
				FILE* log = fopen("logcli.txt","w");
				part = fopen("clientes.dat","r+b");
				cliente* aux;
				int keep = 1;
				fprintf(log,"\n===============================================================================\n");
				fprintf(log,"                                    PRINTANDO CLIENTES\n");
				fprintf(log,"===============================================================================\n\n");
				while(keep){
					keep = 0;
					aux = le_cliente(part);
					if(aux != NULL){
						imprime_cliente(aux,log);
						keep = 1;
					}
				}
				fclose(log);
				fclose(part);
			}
			if(escolha == 4){
				FILE* log = fopen("logsaida.txt","w");
				FILE* part = fopen(fila_nome_ini(particoes),"r+b");
					fprintf(log,"\n===============================================================================\n");
					fprintf(log,"                        PRINTANDO CLIENTES DA SAIDA \n");
					fprintf(log,"===============================================================================\n\n");
					cliente* aux;
					int keep = 1;
					while(keep){
						keep = 0;
						aux = le_cliente(part);
						if(aux != NULL){
							imprime_cliente(aux,log);
							keep = 1;
						}
					}
					fclose(part);
				fclose(log);

				printf("\n                    |===========================================|\n");
				printf("\n                    |    Arquivo logsaida.txt criado com sucesso!    |\n");
				printf("\n                    |===========================================|\n");
			}
			if(escolha == 5){
				rodando = 0;
			}
			if(escolha > 5 || escolha < 1){
				printf("\n                       |======================================|\n");
				printf("\n                       |    Opcao Invalida, digite novamente  |\n");
				printf("\n                       |======================================|\n");
			}
		}
		fila_libera(particoes);
		return 0;
	}
//fim main

//TAD de fila
	fila* fila_cria(void){
		fila* f = (fila*)malloc(sizeof(fila));
		f->ini = f->fim = NULL;
		return f;
	}
	void fila_insere(fila* f,char* nome){

		lista* n = (lista*)malloc(sizeof(lista));
		strcpy(n->nome,nome);
		//printf("%s\n",n->nome);
		n->prox=NULL;

		if(f->fim != NULL) f->fim->prox = n;
		else f->ini = n;
		f->fim = n;
	}
	FILE* fila_retira(fila* f){
		lista* t;
		FILE* v = NULL;
		if(fila_vazia(f)){
			printf("fila vazia FDP!!!\n");
			return NULL;
		}
		t = f->ini;
		v = fopen(t->nome,"r+b");
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
	FILE* fila_abre_ultimo(fila* f){
		if(f!= NULL && f->fim != NULL)	return fopen(f->fim->nome,"w+b"); 
		else return NULL;
	}
	int quant_part(fila* f)
	{
		lista* temp;
		int quant = 0;
		if(f == NULL) return -1;
		temp = f->ini;
		while(temp)
		{
			quant++;
			temp = temp->prox;
		}
		return quant;
	}
	char* fila_nome_ini(fila* f)
	{
		if(f->ini) return f->ini->nome;
		return "\n";
	}
//Fim do TAD de fila

//TAD de arvore
	arvore* arv_criavazia(void){
		
		return NULL;
	}
	arvore* arv_cria(cliente* cliente,int qual_arquivo,arvore* e,arvore* d,arvore* father){
		arvore* p = (arvore*)malloc(sizeof(arvore));
		p->info = cliente;
		p->qual_arquivo = qual_arquivo;
		p->father = father;
		p->ls = e;
		p->rs = d;
		return p;
	}
	int pega_cod(arvore* quem){
		if(quem->info != NULL) return quem->info->codCliente;
		else return INF;
	}
	cliente* pega_cliente(arvore* quem){
		
		return quem->info;
	}
	void set_father(arvore* quem){
		quem->rs->father = quem;
		quem->ls->father = quem;
	}
	int arv_vazia(arvore* a){

		return a==NULL;
	}
	void arv_imprime_sim(arvore* a){
		
		if(!arv_vazia(a)){
			printf("<");
			printf("%d ",pega_cod(a));
			arv_imprime_sim(a->ls);
			arv_imprime_sim(a->rs);
			printf(">");
		}
		
	}
//Fim TAD de arvore

//Inicio da TAD de arquivo binario
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
//Fim da TAD de arquivo binario

//funcoes para selecao natural
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
		if(Ftemp->codCliente < menor && espaco > 0){// se o cod do registro lido for menor q o menor anterior, joga esse novo registro para o reservatorio
			return 2;
		}
		//se o cod do registro lido for maior que o menor anterior, joga esse novo 
		// registro para o array de registros
		if(Ftemp->codCliente > menor){
			return 3;
		}
	}
	// salva um registro no arquivo de saida
	void salva(cliente *client, FILE* out) {
	    fwrite(&client->codCliente, sizeof(int), 1, out);
		fwrite(client->nome, sizeof(char), sizeof(client->nome), out);
		fwrite(client->dataNascimento, sizeof(char), sizeof(client->dataNascimento), out);
	}
	// realiza o algoritmo de selecao natural
	fila* selecao_natural(fila* particoes,int* contParticoes){

		FILE* registros; // ponteiro para o arquivo de registros
		FILE* reservatorio;
		FILE* particao;

		cliente* Ftemp; // guardara um registro temporario
	 	cliente** reg;
	 	reg = (cliente**)malloc(sizeof(cliente*)*M); // guardara M[nesse caso 6] registros do arquivo na memoria
		
		char nome[20];
		char regName[50];
		
		int menor = 999999; // guarda menor valor do array
		int indice; // guarda o indice do menor valor no array
		int espaco; // espaco livre no reservatorio
		int contador = 0; // conta quantas particoes foram abertas
		int contador_controle=0;//usado para abrir somente 1 particao por vez
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

		int leu_menor=0;
		// lendo 8 registros, admitindo que a maquina so consegue ler 8 registros por vez
		for(int i=0;i<M;i++){
			reg[i] = le_cliente(registros);
		}
		while(running){
			espaco = N;
			leu_menor = 0;
			// enquanto ha espaco no reservatorio, realiza operacoes em uma mesma particao
			while(espaco >= 0){
				
				indice = achaMenor(reg,&menor);
				if(contador_controle == 0)
				{
					fila_insere(particoes,nome);//abre arquivo de particao no contador com nome atualizado
					particao = fila_abre_ultimo(particoes);
					contador_controle++;
				}
				if(indice>=0 && leu_menor == 0){
					salva(reg[indice],particao); // salvo o registro de menor valor na particao aberta atual
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
							salva(Ftemp,reservatorio); //salva Ftemp no reservatorio
							Ftemp = NULL;
							leu_menor=1;
							espaco--;
							if(espaco == 0) reg[indice] = NULL;
							break;
						
						case 3:
							reg[indice] = Ftemp; // insere o registro no array de registros pois o
							leu_menor=0;
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
								salva(reg[indice],particao);
								reg[indice] = NULL;
								controle = 1;
							} 
						}
						
						fclose(particao);// fecha a particao pois o reservatorio encheu
						contador++; // incrementa contador de particoes pois uma nova particao foi criada

						rewind(reservatorio);

						// carrega os M registros salvos do reservatorio, na memoria
						for(int i=0;i<M;i++){
							reg[i] = le_cliente(reservatorio);
						}

						reservatorio = fopen("reservatorio.dat","w+b");

						nome[8] += 1; // altera o nome de particao0.dat para particao1.dat para criar um novo arquivo de particao
						fila_insere(particoes,nome); // cria uma nova particao com o nome atualizado
						particao = fila_abre_ultimo(particoes);
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
							salva(reg[indice],particao);
							reg[indice] = NULL;
							controle = 1;
						} 
					}
					
					fclose(particao); // fecha a particao pois o reservatorio encheu
					contador++; // incrementa contador de particoes pois uma nova particao foi criada
					rewind(reservatorio);
					// carrega os M registros salvos do reservatorio, na memoria
					for(int i=0;i<M;i++){
						reg[i] = le_cliente(reservatorio);
					}
					indice = achaMenor(reg,&menor);
					// altera o nome de particao0.dat para particao1.dat para criar um novo arquivo de particao
					if(indice>=0){
						nome[8] += 1;
						fila_insere(particoes,nome);
						particao = fila_abre_ultimo(particoes);

						controle = 1;
						while(controle)
						{
							controle = 0;

							indice = achaMenor(reg,&menor);
							if(indice >= 0){
								salva(reg[indice],particao);
								reg[indice] = NULL;
								controle = 1;
							} 
						}
						printf("contador:%d\n",contador);
						fclose(particao); // fecha a particao pois o reservatorio encheu
					} // abre arquivo de particao no contador com nome atualizado
					running=0;
					espaco=-1;	
				}
			}
		}
		*contParticoes = contador;
		fclose(reservatorio);
		remove("reservatorio.dat");
		free(reg);
		return particoes;
	}
//fim das funcoes para selecao natural

//funcoes para intercalacao otima
	//realiza o algoritmo de intercalação otima
	fila* inter_otima(fila* particoes,int* nameFixer)
	{
		FILE** entrada;
		FILE* saida;
		char nome[20];
		char name[20];//me diz quais partições ir excluindo apos o uso
		cliente* teste;
		arvore* arvore_campeoes;

		int qual_estou = 0;
		int num_particoes = *nameFixer;
		int i;
		int quant_files=0;//me diz quantos arquivos eu criei para poder excluir depois.
		
		strcpy(name,"ParticaoA.dat");
		strcpy(nome,"saidaA.dat");
		entrada = (FILE**)malloc(sizeof(FILE*)*(F-1));
		saida = fopen(nome,"w+b");

		preenche_entrada(entrada,particoes);//pega ate 3 particoes e coloca nas entradas
		arvore_campeoes = cria_arvore_campeao(entrada);//cria a arvore de campeos e retorna a raiz

		//arv_imprime_sim(arvore_campeoes);oi
		while(1)//verifica quando existe somente 1 arquivo na fila de particoes
		{
			while(pega_cod(arvore_campeoes) != INF)//atualiza a arvore até as entradas estarem todas nulas, se tudo tiver nulo a raiz tera infinito.
			{
				salva(pega_cliente(arvore_campeoes),saida);
				arvore_campeoes = altera_arvore_campeao(arvore_campeoes,entrada);
				//arv_imprime_sim(arvore_campeoes);oi
				//puts(nome);
			}
			fila_insere(particoes,nome);
			fclose(saida);
			for(int i = 0; i<F-1;i++)//exclui as particoes usadas
			{
				remove(name);
				name[8]++;
			}
			if(quant_part(particoes) == 1) break;
			nome[5]++;
			quant_files++;
			saida = fopen(nome,"w+b");
			preenche_entrada(entrada,particoes);
			arvore_campeoes = cria_arvore_campeao(entrada);
			//arv_imprime_sim(arvore_campeoes);oi
		}
		strcpy(nome,"saidaA.dat");
		for(int i=0;i<quant_files;i++)
		{
			remove(nome);
		}
		strcpy(nome,"Ordenado.dat");
		rename(fila_nome_ini(particoes),nome);
		strcpy(particoes->ini->nome,nome);
		return particoes;
	}
	//pega os 3 primeiros do vetor de particoes e coloca no vetor de entrada.
	void preenche_entrada(FILE** entrada,fila* particoes)
	{
		for(int i=0;i<F-1; i++)
		{
			//puts(particoes->ini->nome);
			entrada[i] = fila_retira(particoes);
		}
	}
	//faz uma interação da arvore de campeões
	//essa aqui ainda tá sendo feita !!!!!!
	arvore* cria_arvore_campeao(FILE** entrada)
	{
		int i,j;//interadores do for
		int quant_leafs = F-1;//a quantidade de folhas da arvore
		int where=0;//me indica onde colocar o novo no
		
		arvore** leafs_array;

		leafs_array = (arvore**)malloc(quant_leafs*sizeof(arvore*));

		for(int i=0;i<quant_leafs;i++)
		{
			if(entrada[i]!=NULL)
			{
				rewind(entrada[i]);
				cliente* aux = le_cliente(entrada[i]);
				//printf("%d\n",aux->codCliente);
				leafs_array[i] = arv_cria(aux,i,NULL,NULL,NULL);//cria as folhas
			}
			else leafs_array[i] = arv_cria(NULL,i,NULL,NULL,NULL);
		}

		int heigh = ceil(log2(quant_leafs));//A altura da arvore que é possivel com o teto do log o numero de folhas na base 2.
		int quant_for_level = quant_leafs;//quantidade de nos por nivel.
		int codL,codR;//variaveis para retirar o codigo do cliente.

		for(i=0;i<heigh;i++)//interage diminuindo o nivel da arvore até chegar na raiz
		{
			//printf("nos no level:%d vezes interacao:%d\n",quant_for_level,(quant_for_level/2));
			for(j=0;j < quant_for_level - 1; j+=2)
			{

				codL = pega_cod(leafs_array[j]);//pega o codigo do no a esquerda
				codR = pega_cod(leafs_array[j+1]);//pega o codigo do no a direita

				//printf("codigos: codL=%d codR=%d j=%d where=%d\n",codL,codR,j,where);

				if(codL > codR)//compara os codigos
				{
					leafs_array[where] = arv_cria(pega_cliente(leafs_array[j+1]),leafs_array[j+1]->qual_arquivo,leafs_array[j],leafs_array[j+1],NULL);//cria o novo no e armazena no lugar certo
					set_father(leafs_array[where]);//ajeita os ponteiros dos filhos, para apontarem para o pai.
					where++;//autera o local a ser salvo no vetor
				}
				else if(codL < codR)
				{
					leafs_array[where] = arv_cria(pega_cliente(leafs_array[j]),leafs_array[j]->qual_arquivo,leafs_array[j],leafs_array[j+1],NULL);
					set_father(leafs_array[where]);
					where++;
				}
				else
				{
					leafs_array[where] = arv_cria(NULL,leafs_array[j]->qual_arquivo,leafs_array[j],leafs_array[j+1],NULL);
					where++;
				}
				//if(codR == 1)printf("kk e ai %d\n",pega_cod(leafs_array[2]));
			}
			if(quant_for_level%2 == 0)//verifica se a quantidade de nos é par ou impar para ter a quantidade do proximo nivel
			{

				quant_for_level = quant_for_level/2;
			}
			else{
				leafs_array[where] = arv_cria(pega_cliente(leafs_array[j]),leafs_array[j]->qual_arquivo,leafs_array[j]->ls,leafs_array[j]->rs,NULL);//se for impar sempre sobra um cara no final, eu coloco ele para manter atualizado o nivel.
				quant_for_level = quant_for_level/2 + 1;
			}
			where = 0;//reseta para o proximo nivel.Dessa forma os nos mais em cima da arvore sempre ficam no comeco do vetor
			//e a raiz vai ficar na posicao 0 do vetor.
		}

		return leafs_array[0];
	}
	arvore* altera_arvore_campeao(arvore* arvore_campeoes,FILE** entrada)
	{
		int running = 1;//me diz quando cheguei na folha.
		cliente* aux;
		arvore* temporario = arvore_campeoes;

		while(running)//encontra a folha a ser auterada
		{
			//verifica qual é o filho que é igual ao pai
			if(pega_cod(temporario->ls) == pega_cod(temporario)) temporario = temporario->ls;
			else if(pega_cod(temporario->rs) == pega_cod(temporario)) temporario = temporario->rs;
			if(temporario->ls == NULL && temporario->rs == NULL)
			{
				running = 0;
			}
		}

		aux = le_cliente(entrada[temporario->qual_arquivo]);//le o proximo cliente do arquivo

		free(pega_cliente(temporario));//libera o cliente da memoria
		temporario->info = aux;
		int codL,codR;//para comparar os codigos

		while(temporario->father != NULL)
		{
			if(temporario->father->ls == temporario)
			{
				codL = pega_cod(temporario);
				codR = pega_cod(temporario->father->rs);

				if(codL > codR)	temporario->father->info = pega_cliente(temporario->father->rs);//como o irmão é menor, ele coloca a info no pai.
				else temporario->father->info = pega_cliente(temporario);//como o temporario é menor que o irmão ele pega a info e coloca no pai.
			}
			else
			{
				codL = pega_cod(temporario->father->ls);
				codR = pega_cod(temporario);

				if(codL > codR)	temporario->father->info = pega_cliente(temporario);//como o temporario é menor que o irmão ele pega a info e coloca no pai.
				else temporario->father->info = pega_cliente(temporario->father->ls);//como o irmão é menor, ele coloca a info no pai.
			}
			temporario = temporario->father;
		}
		return temporario;
	}
//fim das funcoes para intercalacao otima
