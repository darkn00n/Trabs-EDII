#include "hash_interna.h"

#define M 7
#define p(a, b) printf("%s = %d\n",b,a);
#define oi printf("oi\n");

int hash(int codigo){
    return codigo % M;
}

FILE* inicializa_hash(){
	FILE* tabHash = fopen("tabHash.dat", "w+b");
	cliente* temp = (cliente*) malloc(tamanho_cliente());
	temp->status = -1;
	temp->codCliente = -1;
	for (int i = 0; i < M; i++)
	{
		temp->prox = i;
		fwrite(&temp->codCliente, sizeof(int), 1, tabHash);
		fseek(tabHash, sizeof(char)*100, SEEK_CUR);
		fwrite(&temp->prox, sizeof(int), 1, tabHash);
		fwrite(&temp->status, sizeof(int), 1, tabHash);
	}

	free(temp);
	rewind(tabHash);
	return tabHash;
}

void insere_interna(cliente * novo_cliente, FILE* tabHash){
    int achou = 0;
    int endereco = 0;
    busca(novo_cliente->codCliente, tabHash, &achou, &endereco); // faz uma busca pelo registro
    //p(endereco, "endereco inserte")
    //printf("------------------insercao--------------------\n");
    int j, i, temp;
    int proximo_aux;
    if(achou != 1){// se nao achou, insere
        if(endereco) j = endereco;
        else{
            i = 0;
            j = hash(novo_cliente->codCliente);
            while(i < M){

                fseek(tabHash, j * tamanho_cliente(), SEEK_SET);
                //p(j,"j da insercao")
                if(le_status_keep(tabHash) == 0){// se estiver ocupado, avanca na tabela
                    j = (j + 1) % M; //lista giratoria
                    i++;
                    //printf("========= %d, %d, ==========\n", le_status_keep(tabHash), le_codigo_keep(tabHash));
                }
                else i = M + 2;
            }
            if(i == M){
                printf("Overflow\n");
                return;
            }
            fseek(tabHash, hash(novo_cliente->codCliente)*tamanho_cliente(), SEEK_SET);//vai pro registro situado em h(x)
            if(le_proximo_keep(tabHash) != hash(novo_cliente->codCliente)){//se o registro apontar pra outro registro, avanca
                do{
                    proximo_aux = le_proximo_keep(tabHash);
                    fseek(tabHash, proximo_aux*tamanho_cliente(), SEEK_SET);
                    busca(le_codigo_keep(tabHash),tabHash,&achou, &endereco);
                }while(le_proximo_keep(tabHash) != endereco); //avanca ate chegarmos no ultimo da lista
            }
            //atualiza os ponteiros
            temp = le_proximo_keep(tabHash);

            sobrescreve_prox(tabHash, j);

            fseek(tabHash, hash(j)*tamanho_cliente(), SEEK_SET);
            sobrescreve_prox(tabHash, temp);
        }
        //vai pro compartimento de h(j)
        fseek(tabHash, hash(j)*tamanho_cliente(), SEEK_SET);
        //insere o cliente na tabela
        adiciona_cliente(novo_cliente, tabHash);
        //coloca o proximo da tabela como o proprio indice do compartimento
        fseek(tabHash, hash(j)*tamanho_cliente(), SEEK_SET);
        sobrescreve_prox(tabHash, j);

    }
    else printf("Erro, codigo ja consta na tabela!!\n");
}

void remove_interna(int codigo, FILE* tabHash){
    int achou = 0, endereco = 0;
    int menosUm = -1;
    busca(codigo, tabHash,&achou, &endereco);
    if(achou == 1){
        fseek(tabHash, endereco*tamanho_cliente(), SEEK_SET);
        sobrescreve_status(tabHash, -1); //escreve liberado no status do registro
    }
    else{
        printf("Registro nao consta na tabela\n");
    }

}

void busca(int codigo, FILE *tabHash, int* achou, int *endereco){
    *achou = 0;
    *endereco = hash(codigo);
    int j = 0;


    while(*achou == 0){
        fseek(tabHash,*endereco*sizeof(cliente), SEEK_SET); // pula pra h(x)
        //p(*endereco);
        if(le_status_keep(tabHash) == -1){//se estiver liberado
            j = *endereco;
        }
        //printf("--------busca-------\n");
        if(le_codigo_keep(tabHash) == codigo && le_status_keep(tabHash) == 0){
            *achou = 1;//se o codigo for igual e o compartimento estiver ocupado, achou
        }
        else{
            if(*endereco == le_proximo_keep(tabHash)){
                *achou = 2; // se o endereco apontar pra ele mesmo, e o ultimo da lista, logo nao achou
                *endereco = j; //endereco recebe o valor de j
            }
            else *endereco = le_proximo_keep(tabHash); //se nao for o ultimo, avanca
            //p(*endereco, "ender")
        }

    }
    //printf("--------fim da busca-------\n");

}
