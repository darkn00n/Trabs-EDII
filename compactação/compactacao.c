#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OI puts("oi");
typedef struct No{
	char valor;
	int fre;
	int isLeaf;
	struct No* esq;
	struct No* dir;
	struct No* pai;
}no;

FILE* openFile();
char toChar(int,char*); // converte o indice passado para o char especifico dele no vetor de simbolos
void fillSymbolsVet(char*,int,char*); // preenchera um vetor de caracteres distintos que sera usado para contagem de frequencia
void achaFrequencia(char*,int*,char*,int); // contara a frequencia de caracteres na string
void showInfo(char*,int*,int,char*); // mostra o vetor de simbolos, o vetor de frequencia e a string lida
void subir(no** heap,int posi);
void descer(no**,int,int);
void inserir(no** heap,int*,int,no*);
no* remocao(no** heap,int*);
void cria_heap(no**,int,int*,char*);
no* cria_arv_pre(no** heap,int n);
void arv_imprime_pre(no* a);
int arv_vazia(no* a);
void cria_tabela(char**,int,no*,char*);
void mostra_compactado(char** ,char*,int );
void descompacta(void);

int main(){

	// Espaço para variaveis que serão usadas

	// =================================================
	int symbSize; // indica quantos simbolos ha no alfabeto
	int tam; // armazena o tamanho da string
	int* frequencia; // guardará a frequencia de cada caractere na string
	char* string; // para armazenar a string a ser compactada
	char tempStr[255]; // vetor de 255 caracteres para armazenar os textos
	char* symbols; // vetor para guardar quais sao os simbolos existentes na string
	FILE* archive;// arquivo para ler as informações
	no* raiz_pre;//raiz da arvore de prefiços.
	char** tabela_cod;//variavel com a tabela de simbolos.
	no** heap;//vetor que serve como heap.
	// =================================================

	archive = openFile(); // abrir o arquivo txt

	fscanf(archive,"%d",&tam); // pega primeira linha do arquivo para obter o tamanho da string a ser trabalhada
	fgetc(archive); // para esvaziar o buffer -- tirar o \n
	fgets(tempStr,tam+1,archive); // le a string a ser compactada de acordo com o tamanho que foi dado
	
	fscanf(archive,"%d",&symbSize); // le a quantidade de simbolos do alfabeto

	frequencia = (int*)malloc(symbSize*sizeof(int)); // aloca o vetor para guardar as frequencias dos caracteres
	symbols = (char*)malloc(symbSize*sizeof(char)); // aloca espaço suficiente no vetor de symbolos para alocar os caracteres existentes na string
	tabela_cod = (char**)malloc(sizeof(char*)*symbSize);//aloca o vetor de codigos com o tamanho de simbolos
	for (int i = 0; i < symbSize; ++i)
	{
		tabela_cod[i] = (char*)malloc(sizeof(char)*symbSize);
	}
	heap = (no**)malloc(sizeof(no*)*symbSize);//a loka a heap.

	fillSymbolsVet(symbols,symbSize,tempStr); 
	achaFrequencia(tempStr,frequencia,symbols,symbSize);

	showInfo(symbols,frequencia,symbSize,tempStr);

	cria_heap(heap,symbSize,frequencia,symbols);
	
	raiz_pre = cria_arv_pre(heap,symbSize);
	
	free(heap);

	cria_tabela(tabela_cod,symbSize,raiz_pre,symbols);
	free(frequencia);
	free(symbols);

	mostra_compactado(tabela_cod,tempStr,symbSize);

	descompacta();

	return 0;
}
FILE* openFile(){
	FILE* archive;
	char arqName[25]; // para guardar o nome do arquivo;

	puts("Digite o nome do arquivo a ser lido, sem a extensao .txt");
	scanf("%s",arqName);

	strcat(arqName,".txt"); // concatena o .txt para poder abrir o arquivo

	if(!(archive = fopen(arqName,"r"))){
		puts("Não foi possivel abrir o arquivo");
		exit(1);
	}

	return archive;
}
char toChar(int i,char* symbols){
	
	return symbols[i];
}
void showInfo(char* symbols,int* frequencia,int symbSize,char* str){
	printf("\nString lida: %s\n\n",str);
	for(int i = 0;i<symbSize;i++){
		printf("symbols[%d]: %c\n",i,symbols[i]);
	}
	puts("");
	for(int i = 0;i<symbSize;i++){
		char tmp = toChar(i,symbols);
		printf("F(%c): %d\n",tmp,frequencia[i]);
	}
	puts("");
}
void achaFrequencia(char* str,int* frequencia,char* symbols,int symbSize){
	int count = 0; // para realizar a contagem do caractere na string

	for (int i = 0; i < symbSize; i++)
	{
		for(int j = 0;j<strlen(str);j++)
		{
			// se o caractere de str[j] for igual ao simbolo que esta sendo procurado
			// contabiliza
			if(symbols[i] == str[j]){
				count++;
			}
		}
		frequencia[i] = count;
		count = 0; // zera o contador para a contagem do novo caractere do vetor de caracteres
	}
}
void fillSymbolsVet(char* symbols,int symbSize,char* str){
	int count = 0; // conta quantos caracteres diferentes ja achei na string

	// enquanto nao visitou minha string toda e meu contador ainda eh menor que a 
	// quantidade de caracteres diferentes no alfabeto

	for(int i = 0;i<strlen(str) && count < symbSize;i++){
		int isOkay = 1; // flag para decidir se achou algum caractere igual anteriormente na string

		// como eh o primeiro caractere, ja salva na primeira posicao do vetor de caracteres distintos
		if(count == 0){
			symbols[i] = str[i];
			count++;
		}
		else{
			// verifica a partir da posicao atual da string ate o inicio se ja computei esse caractere
			for(int j=i-1;j>=0;j--){
				isOkay = 1;
				if(str[i] == str[j]){
					isOkay = 0; // se achou algum diferente, troca a flag e para o for
					break;
				}
			}
			// se foi ate o inicio da string sem achar o caractere, adiciona ele ao vetor de caracteres distintos
			if(isOkay == 1){
				symbols[count] = str[i];
				count++;
			}
		}
	}
}
//parte de heap.
void cria_heap(no** heap,int n,int* frequencia, char* symbols){
	for(int i = 0 ; i < n; i++)
	{
		heap[i] = (no*)malloc(sizeof(no)*n);
		heap[i]->valor = toChar(i,symbols);
		heap[i]->fre = frequencia[i];
		heap[i]->isLeaf = 1;
		heap[i]->esq = NULL;
		heap[i]->dir = NULL;
	}
	for(int i = n/2;i>=0;i--){
		descer(heap,i,n);
	}
}
void subir(no** heap,int i){
	int j;
	if(i<=2){
		j=0;
	}else j = i/2 - 0/5;
	no* aux;
	if(j>=0)
	{
		if(heap[i]->fre < heap[j]->fre)
		{
			aux = heap[i];
			heap[i] = heap[j];
			heap[j] = aux;
			subir(heap,j);
		}
	}
}
void descer(no** heap,int i,int n){
	int j=1;
	if(i != 0){
		j += 2*i;
	}
	no* aux;
	if(j < n){
		if(j < n-1){
			if(heap[j+1]->fre < heap[j]->fre){
				j++;
			}
		}
		if(heap[i]->fre > heap[j]->fre){
			aux = heap[i];
			heap[i] = heap[j];
			heap[j] = aux;
			descer(heap,j,n);
		}
	}
}
void inserir(no** heap,int* n,int M,no* novo){
	if(*n < M){
		heap[*n] = novo;
		*n = *n + 1;
		subir(heap,*n - 1);
	}else printf("OVERFLOW\n");
}
no* remocao(no** heap,int* n){
	no* aux;
	if(*n!=0){
		aux = heap[0];
		heap[0] = heap[*n-1];
		*n = *n-1;
		descer(heap,0,*n);
	}else{
		printf("UNDERFLOW\n");
		return NULL;
	}
	return aux;
}
no* cria_arv_pre(no** heap,int n){
	int quant_atual = n;
	no *aux;
	aux = (no*)malloc(sizeof(no));
	aux->valor = '\0';
	aux->fre = 0;
	aux->isLeaf = 0;
	aux->pai = NULL;
	
	while(quant_atual)
	{
		aux->esq = remocao(heap,&quant_atual);
		aux->esq->pai = aux;
		aux->dir = remocao(heap,&quant_atual);
		aux->dir->pai = aux;
		if(quant_atual)
		{	
			aux->fre = aux->esq->fre + aux->dir->fre;
			inserir(heap,&quant_atual,n,aux);

			aux = (no*)malloc(sizeof(no));
			aux->valor = '\0';
			aux->fre = 0;
			aux->isLeaf = 0;
		}
	}
	return aux;
}
void arv_imprime_pre(no* a){
	printf("<");
	if(!arv_vazia(a)){
		printf("%d ",a->fre);
		printf("%c",a->valor);
		arv_imprime_pre(a->esq);
		arv_imprime_pre(a->dir);
	}
	printf(">");
}
int arv_vazia(no* a){

	return a==NULL;
}
void cria_tabela(char** tabela_cod,int n,no* raiz,char* symbols){
	no* aux = raiz;
	no* temp;
	char tmp[n];
	int i;
	for (int i = 0; i < n; ++i)
	{
		strcpy(tabela_cod[i],"\0");
	}
	strcpy(tmp,"\0");
	while(aux!= NULL)
	{
		if(aux->isLeaf)
		{
			for(i = 0; i<n;i++){
				if(aux->valor == symbols[i]) break;
			}

			char qual[2];
			qual[0] = aux->valor;
			qual[1] = '\0';
			strcpy(tabela_cod[i],qual);
			strcat(tabela_cod[i],tmp);
			tmp[strlen(tmp)-1] = '\0';
			temp = aux;
			aux = aux->pai;
			if(aux->esq == temp){
				free(aux->esq);
				aux->esq = NULL;
			}
			else
			{
				free(aux->dir);
				aux->dir = NULL;
			}
		}
		else
		{
			if(aux->esq != NULL){
				aux = aux->esq;
				strcat(tmp,"0");
			}
			else if(aux->dir != NULL){
				aux = aux->dir;
				strcat(tmp,"1");
			}
			else if(aux->pai != NULL){
				tmp[strlen(tmp)-1] = '\0';
				temp = aux;
				aux = aux->pai;
				if(aux->esq == temp){
					free(aux->esq);
					aux->esq = NULL;
				}
				else
				{
					free(aux->dir);
					aux->dir = NULL;
				}
			}
			else{
				free(aux);
				aux=NULL;
			}
		}
	}
	for (int j = 0; j<n; j++)
	{
		printf("%c -> ",tabela_cod[j][0]);
		for (int k = 1; tabela_cod[j][k]!= '\0'; k++)
		{
			printf("%c",tabela_cod[j][k]);
		}
		printf("\n");
	}
}
void mostra_compactado(char** tabela_cod,char* str,int tam){
	FILE* saida = fopen("saida.txt","w");
	int i,j;
	for (i = 0; i < strlen(str); i++)
	{
		for (j = 0; j < tam; j++)
		{
			if(tabela_cod[j][0] == str[i])
			{
				for (int k = 1; tabela_cod[j][k]!= '\0'; k++)
				{
					printf("%c",tabela_cod[j][k]);
					fprintf(saida,"%c",tabela_cod[j][k]);
				}
			}
		}
	}
	printf("\n");
	fprintf(saida,"\n\n");
	
	for(i = 0; i < tam; ++i)
	{
		fprintf(saida,"%c ",tabela_cod[i][0]);
		for (int k = 1; tabela_cod[i][k]!= '\0'; k++)
		{
			fprintf(saida,"%c",tabela_cod[i][k]);
		}
		if(i < tam - 1) fprintf(saida,"\n");
	}
	fclose(saida);
}
void descompacta(){
	FILE* archive;
	FILE* saida = fopen("saidadescompac.txt","w");
	char arqName[25]; // para guardar o nome do arquivo;

	puts("Digite o nome do arquivo a ser descompactado, sem a extensao .txt");
	scanf("%s",arqName);

	strcat(arqName,".txt"); // concatena o .txt para poder abrir o arquivo

	if(!(archive = fopen(arqName,"r"))){
		puts("Não foi possivel abrir o arquivo");
		exit(1);
	}
	char aux;
	int conta = 0;
	if(fscanf(archive,"%c",&aux) && aux!='\n') conta++;	
	while(aux != '\n')
	{
		if(fscanf(archive,"%c",&aux) && aux != '\n')
		{
			conta++;
		}
		else break;
	}
	char string[conta];
	string[1] = '\0';
	rewind(archive);
	fscanf(archive,"%s",string);

	fgetc(archive);//retira o fim de linha
	fgetc(archive);//retira linha vazia

	char quem;
	char codigo[conta];
	conta=0;
	while(fscanf(archive,"%c %s",&quem,codigo) != EOF)
	{
		fgetc(archive);//retira fim da linha
		//printf("%c %s\n",quem,codigo);
		conta++;
		//printf("%c\n",quem);//,codigo);
	}
	rewind(archive);

	fscanf(archive,"%s",string);
	fgetc(archive);//retira o fim de linha
	fgetc(archive);//retira linha vazia

	char** tabela_cod = (char**)malloc(sizeof(char*)*conta);
	char temporario[conta];
	strcpy(temporario,"\0");
	char qual[2];
	int i=0;
	for(int i=0;i<conta;i++)
	{
		tabela_cod[i] = (char*)malloc(sizeof(char)*conta);
		strcpy(tabela_cod[i],"\0");
		fscanf(archive,"%c %s",&quem,temporario);
		qual[0] = quem;
		qual[1] = '\0';
		strcat(tabela_cod[i],qual);
		strcat(tabela_cod[i],temporario);
		fgetc(archive);//retira fim da linha
	}
	int j,k,controle,tamanho_string_saida=0;
	for(i = 0; i < strlen(string); i++)
	{
		for (j = 0; j < conta; j++)
		{
			for (k = 0; k < strlen(tabela_cod[j]) - 1; k++)
			{
				//printf("veremos: %c %c eita em:%d\n",tabela_cod[j][1+k],string[i+k],i+k);
				if(tabela_cod[j][1+k] != string[i+k])
				{
					break;
				}
				else if(k == strlen(tabela_cod[j]) - 2)
				{
					printf("%c",tabela_cod[j][0]);
					fprintf(saida,"%c",tabela_cod[j][0]);
					tamanho_string_saida++;					
					controle=1;
					i = i + k;
				}
			}
			if(controle == 1){
				controle=0;
				break;
			}
		}
	}
	printf("\n");
}