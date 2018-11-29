#include "B+.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

//funções de busca
	int busca(int chave,int* pagi,int* posi,FILE* metadados,FILE* index,FILE* dados)
	{
		int tipo_raiz,posi_raiz;
		int p,i;
		*pagi = -1;
		*posi = -1;
		rewind(metadados);
		if(fread(&tipo_raiz,sizeof(int),1,metadados) <= 0)
		{
			inicializa_B(metadados,dados);
			printf("Busca invalida, não existe nenhum cliente.\n");
			*pagi = 0;
			*posi = 0;
			return 0;
		}
		else fread(&posi_raiz,sizeof(int),1,metadados);
		p = posi_raiz;
		//printf("%d %d\n",p,tipo_raiz);
		if(tipo_raiz == 1)
		{
			*pagi = p;
			return busca_em_folha(chave,*pagi,posi,dados);
		}
		else{
			fseek(index,tamanho_pagina()*p,SEEK_SET);
			pagina* aux = le_pagina(index);

			while(p != -1)
			{
				*posi = 0;
				*pagi = p;
				for (i = 0; i < aux->quantos; i++)
				{
					//printf("%d\n",aux->indice[i]);
					if(chave > aux->indice[i])
					{
						*posi = i + 1;
					}
					else
					{
						if(chave == aux->indice[i])
						{
							if(aux->tipo_filho)
							{
								*pagi = aux->folhas[i + 1];
								//printf("%d %d\n",i+1,*pagi);
								return busca_em_folha(chave,*pagi,posi,dados);
							}
							else{
								p = aux->folhas[i + 1];
								fseek(index,tamanho_pagina()*p,SEEK_SET);
								i = aux->quantos+2;
								aux = le_pagina(index);
							}
						}
						else
						{
							p = aux->folhas[i];
							if(aux->tipo_filho == 0)
							{
								fseek(index,tamanho_pagina()*p,SEEK_SET);
								//printf("proxima pagina: %d\n",p);
								i = aux->quantos+1;
								aux = le_pagina(index);
							}
							else
							{
								*pagi=p;
								return busca_em_folha(chave,p,posi,dados);
							}
						}

					}
				}
				if(i == aux->quantos){
					*pagi = aux->folhas[i];
					//printf("%d %d\n",i,aux->folhas[i]);
					if(aux->tipo_filho == 0)
					{
						fseek(index,tamanho_pagina()*(*pagi),SEEK_SET);
						aux = le_pagina(index);
					}
					else
					{
						return busca_em_folha(chave,*pagi,posi,dados);
					}
				}
			}
		}
		return 0;
	}
	int busca_em_folha(int chave,int pagi,int* posi,FILE* dados)
	{
		int i;
		folha* aux;
		fseek(dados,tamanho_folha()*pagi,SEEK_SET);
		aux = le_folha(dados);
		*posi = 0;
		if(aux->status != 1)
		{
			for(i = 0; i < aux->quantos;i++)
			{
				if(chave > aux->clientes[i]->codCliente)
				{
					*posi = i + 1;
				}
				else
				{
					if(chave == aux->clientes[i]->codCliente)
					{
						return 1;
					}
					i = aux->quantos+1;
				}
			}
		}
		return 0;
	}
//auxiliares busca
	void busca_aux(int chave,FILE* metadados,FILE* index,FILE* dados)
	{
		int posi,pagina;
		if(busca(chave,&pagina,&posi,metadados,index,dados)){
			printf("Codigo encontrado, na pagina: %d na posição: %d\n",pagina,posi);
		}
		else printf("Codigo não encontrado, tente novamente.\n");
	}
	void inicializa_B(FILE* metadados,FILE* dados)
	{
		int aux=1;
		cliente** clientes;
		clientes = (cliente**)malloc(sizeof(cliente*)*2*ORDEM);
		rewind(metadados);
		for (int i = 0; i < 2*ORDEM; i++)
		{
			clientes[i] = NULL;
		}
		fwrite(&aux,sizeof(int),1,metadados);
		aux=0;
		fwrite(&aux,sizeof(int),1,metadados);
		fflush(metadados);
		folha* fo = cria_folha(-1,0,clientes,-1);

		rewind(dados);
		adiciona_folha(fo,dados);
	}




//funções incerções
	void insere(FILE* metadados,FILE* index,FILE* dados)
	{
		int pagi,posi;
		cliente* adicionar = cria_cliente();
		int validade = busca(adicionar->codCliente,&pagi,&posi,metadados,index,dados);

		if(!validade){
			fseek(dados,tamanho_folha()*pagi,SEEK_SET);
			//printf("%d\n",pagi);
			folha *who = le_folha(dados);

			if(who->quantos < 2*ORDEM){
				who->clientes[who->quantos] = adicionar;
				if(who->quantos != posi)
				{
					cliente* swap;
					for (int i = who->quantos; i > posi ; i--)
					{
						swap = who->clientes[i - 1];
						who->clientes[i - 1] = who->clientes[i];
						who->clientes[i] = swap;
					}
				}
				who->quantos++;
				who->status=0;
				fseek(dados,tamanho_folha()*pagi,SEEK_SET);
				adiciona_folha(who,dados);
			}
			else
			{

				particionamento_folha(pagi,who,adicionar,metadados,index,dados);
			}
		}
		else printf("Codigo já existe, informe outro codigo\n");
	}
	void particionamento_folha(int where_who,folha* who,cliente* adicionar,FILE* metadados,FILE* index,FILE* dados)
	{
		printf("particiona folha %d\n",where_who);
		int i,codigo_subir,onde,pai;
		cliente** clientes;
		clientes = (cliente**)malloc(sizeof(cliente*)*(2*ORDEM+1));

		inicializa_clientes(clientes,who,adicionar);

		ordena_folha(clientes);

		cliente** for_brother;
		for_brother = (cliente**)malloc(sizeof(cliente*)*(2*ORDEM));

		inicializa_brother(clientes,for_brother);

		folha* brother = cria_folha(who->pai,ORDEM + 1,for_brother,who->prox);

		who->prox = acha_espaco(1,index,dados);

		fseek(dados,who->prox*tamanho_folha(),SEEK_SET);
		adiciona_folha(brother,dados);

		codigo_subir = brother->clientes[0]->codCliente;


		free(who->clientes);
		who->clientes=clientes;
		who->quantos = ORDEM;

		fseek(dados,where_who*tamanho_folha(),SEEK_SET);
		adiciona_folha(who,dados);

		onde = who->prox;
		pai = who->pai;

		if(pai == -1)
		{
			int indice[2*ORDEM];
			int folhas[2*ORDEM + 1];

			inicializa_ind_fol(indice,folhas);

			indice[0] = brother->clientes[0]->codCliente;
			folhas[0] = where_who;
			folhas[1] = who->prox;

			pai=acha_espaco(0,index,dados);

			pagina* father = cria_pagina(-1,1,indice,folhas,1);

			fseek(index,pai*tamanho_pagina(),SEEK_SET);

			adiciona_pagina(father,index);

			//imprime_ind_fol(father);

			ajeita_pai(pai,father,index,dados);

			reescreve_raiz(0,pai,metadados);

			fflush(metadados);

			father = libera_pagina(father);
		}
		else
		{
			fseek(index,who->pai*tamanho_pagina(),SEEK_SET);
			pagina *father = le_pagina(index);
			if(father->quantos < 2*ORDEM)
			{
				father->indice[father->quantos] = brother->clientes[0]->codCliente;
				father->folhas[father->quantos+1] = who->prox;

				ordena_pagina(father);

				father->quantos+=1;

				fseek(index,who->pai*tamanho_pagina(),SEEK_SET);
				adiciona_pagina(father,index);

				father = libera_pagina(father);
			}
			else
			{
				father = libera_pagina(father);
				particionamento_pagina(codigo_subir,onde,pai,metadados,index,dados);
			}
		}
		brother = libera_folha(brother);
		who = libera_folha(who);
	}
	void particionamento_pagina(int codigo_subir,int local_new_node,int where_who,FILE* metadados,FILE* index,FILE* dados)
	{
		printf("particiona pagina %d\n",where_who);
		fseek(index,where_who*tamanho_pagina(),SEEK_SET);
		pagina* who = le_pagina(index);

		int *indices = (int*)malloc(sizeof(int)*(2*ORDEM + 1));
		int *folhas = (int*)malloc(sizeof(int)*(2*ORDEM + 2));
		int i;
		int onde;
		int subiu;
		int pai;

		inicializa_indices_folhas(indices,folhas,who,codigo_subir,local_new_node);

		ordena_indices_folhas(indices,folhas,who->quantos);

		int *indices_for_brother = (int*)malloc(sizeof(int)*(2*ORDEM));
		int *folhas_for_brother = (int*)malloc(sizeof(int)*(2*ORDEM + 1));

		inicializa_brother_pag(indices,folhas,indices_for_brother,folhas_for_brother);
		pagina* brother = cria_pagina(who->pai,ORDEM,indices_for_brother,folhas_for_brother,who->tipo_filho);

		onde = acha_espaco(0,index,dados);

		fseek(index,onde*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(brother,index);

		ajeita_pai(onde,brother,index,dados);

		brother = libera_pagina(brother);

		free(who->indice);
		free(who->folhas);

		subiu = indices[ORDEM];
		indices[ORDEM] = -1;

		who->indice = indices;
		who->folhas = folhas;
		who->quantos = ORDEM;
		pai = who->pai;

		fseek(index,where_who*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(who,index);

		ajeita_pai(where_who,who,index,dados);

		who = libera_pagina(who);

		if(pai == -1){

			inicializa_ind_fol(indices_for_brother,folhas_for_brother);

			indices_for_brother[0] = subiu;
			folhas_for_brother[0] = where_who;
			folhas_for_brother[1] = onde;

			pagina* father = cria_pagina(-1,1,indices_for_brother,folhas_for_brother,0);

			onde = acha_espaco(0,index,dados);

			fseek(index,tamanho_pagina()*onde,SEEK_SET);
			adiciona_pagina(father,index);

			ajeita_pai(onde,father,index,dados);

			reescreve_raiz(0,onde,metadados);

			libera_pagina(father);
		}
		else
		{
			fseek(index,tamanho_pagina()*pai,SEEK_SET);
			pagina* father = le_pagina(index);

			if(father->quantos < 2*ORDEM)
			{
				father->indice[father->quantos] = subiu;
				father->folhas[father->quantos+1] = onde;

				ordena_pagina(father);

				father->quantos++;

				fseek(index,tamanho_pagina()*pai,SEEK_SET);
				adiciona_pagina(father,index);

				libera_pagina(father);
			}
			else
			{
				libera_pagina(father);
				particionamento_pagina(subiu,onde,pai,metadados,index,dados);
			}
		}
	}
//auxiliares incerção
	void ordena_folha(cliente** clientes)
	{
		int i;
		cliente* adicionar;
		for(i = 2*ORDEM; i > 0; i--)
		{
			if(clientes[i]->codCliente < clientes[i - 1]->codCliente)
			{
				adicionar = clientes[i-1];
				clientes[i-1] = clientes[i];
				clientes[i] = adicionar;
			}
		}
	}
	void ordena_pagina(pagina* father)
	{
		int i;
		int swap;

		for (i = father->quantos; i > 0; i--)
		{
			if(father->indice[i] < father->indice[i-1])
			{
				swap = father->indice[i-1];
				father->indice[i-1] = father->indice[i];
				father->indice[i] = swap;

				swap = father->folhas[i];
				father->folhas[i] = father->folhas[i+1];
				father->folhas[i+1] = swap;
			}
		}
	}
	int acha_espaco(int qual,FILE* index,FILE* dados)
	{
		int conta;
		if(qual)
		{
			rewind(dados);
			folha* aux = le_folha(dados);
			conta = 0;
			while(aux != NULL)
			{
				if(aux->status)
				{
					break;
				}
				else
				{
					conta++;
					aux = le_folha(dados);
				}
			}
		}
		else
		{
			rewind(index);
			pagina* aux = le_pagina(index);
			conta = 0;
			while(aux != NULL)
			{
				if(aux->status)
				{
					break;
				}
				else
				{
					conta++;
					aux = le_pagina(index);
				}
			}
		}
		return conta;
	}
	void inicializa_clientes(cliente** clientes,folha* who,cliente* adicionar)
	{
		int i;
		for(i = 0; i < 2*ORDEM; ++i)
		{
			clientes[i] = who->clientes[i];
		}
		clientes[i] = adicionar;
	}
	void inicializa_brother(cliente** clientes,cliente** for_brother)
	{
		int i;
		for(i = 0; i < 2*ORDEM + 1; ++i)
		{
			if(ORDEM + i < 2*ORDEM + 1)
			{
				for_brother[i] = clientes[ORDEM+i];
				clientes[ORDEM+i] = NULL;
			}
			else if(i<2*ORDEM) for_brother[i] = NULL;
		}
	}
	void inicializa_brother_pag(int *indices,int *folhas,int *indices_for_brother,int *folhas_for_brother)
	{
		int i;
		for(i = 0; i < 2*ORDEM; ++i)
		{
			if(i + 1 + ORDEM < 2*ORDEM + 1)
			{
				indices_for_brother[i] = indices[ORDEM + 1 + i];
				folhas_for_brother[i] = folhas[ORDEM + 1 + i];
				//printf("for_brother[%d] = %d\n",i,for_brother[i]->codCliente);
				indices[ORDEM+1+i] = -1;
				folhas[ORDEM+1+i] = -1;
			}
			else if(i + 1 + ORDEM == 2*ORDEM + 1)
			{
				indices_for_brother[i] = -1;
				folhas_for_brother[i] = folhas[ORDEM+1+i];
			}
			else
			{
				indices_for_brother[i] = -1;
				folhas_for_brother[i] = -1;
			}
		}
		folhas_for_brother[i] = -1;
	}
	void inicializa_ind_fol(int *indices,int *folhas)
	{
		int i;
		for(i = 0; i < 2*ORDEM; ++i)
		{
			indices[i] = -1;
			folhas[i]=-1;
		}
		folhas[i] = -1;
	}
	void inicializa_indices_folhas(int *indices,int *folhas, pagina* who,int codigo_subir, int local_new_node)
	{
		int i;
		for (i = 0; i < 2*ORDEM; ++i)
		{
			indices[i] = who->indice[i];
			folhas[i] = who->folhas[i];
		}
		indices[i] = codigo_subir;
		folhas[i] = who->folhas[i];
		i++;
		folhas[i] = local_new_node;
	}
	void ordena_indices_folhas(int* indices,int *folhas,int quantos)
	{
		int swap;
		int i;

		for (i = quantos; i > 0; i--)
		{
			if(indices[i] < indices[i-1])
			{
				swap = indices[i-1];
				indices[i-1] = indices[i];
				indices[i] = swap;

				swap = folhas[i];
				folhas[i] = folhas[i+1];
				folhas[i+1] = swap;
			}
		}
	}



//funções remoção
	void remocao(int chave,FILE* metadados,FILE* index,FILE* dados)
	{
		int pagi,posicao,achou;

		achou = busca(chave,&pagi,&posicao,metadados,index,dados);
		if(achou){
			fseek(dados,pagi*tamanho_folha(),SEEK_SET);
			folha* retirado = le_folha(dados);

			int controle = retirado->quantos;

			remove_cliente(posicao,retirado);

			if(controle > ORDEM)
			{
				fseek(dados,pagi*tamanho_folha(),SEEK_SET);
				adiciona_folha(retirado,dados);

				retirado = libera_folha(retirado);
			}
			else
			{
				if(retirado->pai != -1)
				{
					int i;
					folha* brother;

					fseek(index,retirado->pai*tamanho_pagina(),SEEK_SET);
					pagina* father = le_pagina(index);

					for(i = 0; i < father->quantos + 1; ++i)
					{
						if(father->folhas[i] == pagi) break;
					}
					if(i == 0)
					{
						fseek(dados,father->folhas[i+1]*tamanho_folha(),SEEK_SET);
						brother = le_folha(dados);

						if(brother->quantos + retirado->quantos < 2*ORDEM) concatena_folha(father,brother,retirado,retirado->pai,father->folhas[i+1],pagi,metadados,index,dados);
						else redistribui(father,brother,retirado,retirado->pai,father->folhas[i+1],pagi,index,dados);

					}
					else if(i == father->quantos)
					{
						fseek(dados,father->folhas[i-1]*tamanho_folha(),SEEK_SET);
						brother = le_folha(dados);

						if(brother->quantos + retirado->quantos < 2*ORDEM) concatena_folha(father,retirado,brother,retirado->pai,pagi,father->folhas[i-1],metadados,index,dados);
						else redistribui(father,retirado,brother,retirado->pai,pagi,father->folhas[i-1],index,dados);
					}
					else
					{
						fseek(dados,father->folhas[i-1]*tamanho_folha(),SEEK_SET);
						brother = le_folha(dados);
						if(brother->quantos + retirado->quantos >= 2*ORDEM)
						{
							redistribui(father,retirado,brother,retirado->pai,pagi,father->folhas[i-1],index,dados);
						}
						else
						{
							brother = libera_folha(brother);
							fseek(dados,father->folhas[i+1]*tamanho_folha(),SEEK_SET);
							brother = le_folha(dados);

							if(brother->quantos + retirado->quantos >= 2*ORDEM) redistribui(father,brother,retirado,retirado->pai,father->folhas[i+1],pagi,index,dados);
							else concatena_folha(father,brother,retirado,retirado->pai,father->folhas[i+1],pagi,metadados,index,dados);
						}
					}
				}
				else
				{
					fseek(dados,pagi*tamanho_folha(),SEEK_SET);
					adiciona_folha(retirado,dados);
				}
			}
		}
		else printf("Não existe esse cliente, informe um codigo valido para a remocao\n");
	}
	void redistribui(pagina* father,folha* who,folha* brother,int onde_father,int onde_who,int onde_brother,FILE* index,FILE* dados)
	{
		printf("redistribui %d em %d\n",onde_who,onde_brother);
		int total_cliente = who->quantos + brother->quantos;
		cliente** todos = (cliente**)malloc(sizeof(cliente*)*total_cliente);

		inicializa_redis(todos,total_cliente,brother->clientes,brother->quantos,who->clientes);

		int quantos_ficam = total_cliente/2; //pega quantos ficam em uma pagina

		distriu_clientes(todos,who,brother,quantos_ficam,total_cliente);

		fseek(dados,onde_who*tamanho_folha(),SEEK_SET);
		adiciona_folha(who,dados);

		fseek(dados,onde_brother*tamanho_folha(),SEEK_SET);
		adiciona_folha(brother,dados);

		int subiu = who->clientes[0]->codCliente;//me diz o novo indice que fica no pai.
		ajeita_incide_pai(father,onde_brother,subiu);

		fseek(index,onde_father*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(father,index);

		ajeita_pai(onde_father,father,index,dados);
		free(todos);

		/*father = libera_pagina(father);
		brother = libera_folha(brother);
		who = libera_folha(who);*/
	}
	void reditribui_pagina(pagina* father,pagina* who,pagina* brother,int onde_father,int onde_who,int onde_brother,FILE* index,FILE* dados)
	{
		printf("redistribui pagina %d em %d\n",onde_who,onde_brother);
		int total_cliente = who->quantos + brother->quantos + 1;
		int* todos_indice = (int*)malloc(sizeof(int)*total_cliente);
		int* todos_folhas = (int*)malloc(sizeof(int)*(total_cliente + 1));

		inicializa_redis_pagina(todos_indice,todos_folhas,total_cliente,brother,father,who,onde_brother);

		int quantos_ficam = total_cliente/2; //pega quantos ficam em uma pagina

		distriu_indice_folhas(todos_indice,todos_folhas,brother,father,who,quantos_ficam,total_cliente,onde_brother);

		fseek(index,onde_who*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(who,index);

		fseek(index,onde_brother*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(brother,index);

		free(todos_indice);
		free(todos_folhas);

		fseek(index,onde_father*tamanho_pagina(),SEEK_SET);
		adiciona_pagina(father,index);

		ajeita_pai(onde_father,father,index,dados);
		ajeita_pai(onde_brother,brother,index,dados);
		ajeita_pai(onde_who,who,index,dados);

		/*father = libera_pagina(father);
		brother = libera_pagina(brother);
		who = libera_pagina(who);*/
	}
	void concatena_folha(pagina* father,folha* who,folha* brother,int onde_father,int onde_who,int onde_brother,FILE* metadados,FILE* index,FILE* dados)
	{
		printf("concatena folha %d em %d\n",onde_who,onde_brother);
		pega_clientes(brother,who);

		brother->prox = who->prox;
		who->status = 1;

		fseek(dados,tamanho_folha()*onde_who,SEEK_SET);
		adiciona_folha(who,dados);

		fseek(dados,tamanho_folha()*onde_brother,SEEK_SET);
		adiciona_folha(brother,dados);

		retira_indice_pai(father,onde_brother);

		fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
		adiciona_pagina(father,index);

		ajeita_pai(onde_father,father,index,dados);

		if(father->pai == -1 && father->quantos == 0)
		{
			reescreve_raiz(1,onde_brother,metadados);

			fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
			father->status=1;
			adiciona_pagina(father,index);

			int fim = -1;

			ajeita_pai(fim,father,index,dados);
		}
		else if(father->quantos < ORDEM && father->pai != -1)
		{
			int i;
			fseek(index,tamanho_pagina()*father->pai,SEEK_SET);
			pagina* new_father = le_pagina(index);

			for(i = 0; i <= new_father->quantos; ++i)
			{
				if(new_father->folhas[i] == onde_father) break;
			}

			if(i == 0)
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i+1],SEEK_SET);
				pagina* new_brother = le_pagina(index);
				if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,metadados,index,dados);
				else reditribui_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,index,dados);
			}
			else if(i == new_father->quantos)
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i-1],SEEK_SET);
				pagina* new_brother = le_pagina(index);

				if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],metadados,index,dados);
				else reditribui_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],index,dados);
			}
			else
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i+1],SEEK_SET);
				pagina* new_brother = le_pagina(index);

				if(new_brother->quantos + father->quantos >= 2*ORDEM)
				{
					reditribui_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,index,dados);
				}
				else
				{
					fseek(index,tamanho_pagina()*new_father->folhas[i-1],SEEK_SET);
					pagina* new_brother = le_pagina(index);

					if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],metadados,index,dados);
					else reditribui_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],index,dados);
				}
			}
		}
		else
		{
			ajeita_pai(onde_father,father,index,dados);
			fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
			adiciona_pagina(father,index);
		}

		/*who = libera_folha(who);
		brother = libera_folha(brother);
		father = libera_pagina(father);*/
	}
	void concatena_pagina(pagina* father,pagina* who,pagina* brother,int onde_father,int onde_who,int onde_brother,FILE* metadados,FILE* index,FILE* dados)
	{
		printf("concatena pagina%d em %d\n",onde_who,onde_brother);
		pega_indices_folhas(brother,father,who,onde_brother);

		who->status=1;
		fseek(index,tamanho_pagina()*onde_who,SEEK_SET);
		adiciona_pagina(who,index);

		fseek(index,tamanho_pagina()*onde_brother,SEEK_SET);
		adiciona_pagina(brother,index);

		retira_indice_pai(father,onde_brother);

		fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
		adiciona_pagina(father,index);

		ajeita_pai(onde_brother,brother,index,dados);

		ajeita_pai(onde_father,father,index,dados);

		if(father->pai == -1 && father->quantos == 0)
		{
			reescreve_raiz(0,onde_brother,metadados);

			fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
			father->status=1;
			adiciona_pagina(father,index);

			int fim = -1;

			ajeita_pai(fim,father,index,dados);
		}
		else if(father->quantos < ORDEM && father->pai != -1)
		{
			int i;
			fseek(index,tamanho_pagina()*father->pai,SEEK_SET);
			pagina* new_father = le_pagina(index);

			for(i = 0; i <= new_father->quantos; ++i)
			{
				if(new_father->folhas[i] == onde_father) break;
			}

			if(i == 0)
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i+1],SEEK_SET);
				pagina* new_brother = le_pagina(index);

				if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,metadados,index,dados);
				else reditribui_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,index,dados);
			}
			else if(i == new_father->quantos)
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i-1],SEEK_SET);
				pagina* new_brother = le_pagina(index);

				if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],metadados,index,dados);
				else reditribui_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],index,dados);
			}
			else
			{
				fseek(index,tamanho_pagina()*new_father->folhas[i+1],SEEK_SET);
				pagina* new_brother = le_pagina(index);

				if(new_brother->quantos + father->quantos >= 2*ORDEM)
				{
					reditribui_pagina(new_father,new_brother,father,father->pai,new_father->folhas[i+1],onde_father,index,dados);
				}
				else
				{
					fseek(index,tamanho_pagina()*new_father->folhas[i-1],SEEK_SET);
					pagina* new_brother = le_pagina(index);

					if(new_brother->quantos + father->quantos < 2*ORDEM) concatena_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],metadados,index,dados);
					else reditribui_pagina(new_father,father,new_brother,father->pai,onde_father,new_father->folhas[i-1],index,dados);
				}
			}
		}
		else
		{
			fseek(index,tamanho_pagina()*onde_father,SEEK_SET);
			adiciona_pagina(father,index);
		}


		/*if(who) who = libera_pagina(who);
		if(brother) brother = libera_pagina(brother);
		if(father) father = libera_pagina(father);*/
	}
//auxiliares remoção
	void remove_cliente(int posi,folha* retirado)
	{
		int i;

		free(retirado->clientes[posi]);
		retirado->clientes[posi] = NULL;

		for (i = posi; i < retirado->quantos - 1; ++i)
		{
			retirado->clientes[i] = retirado->clientes[i + 1];
		}
		retirado->clientes[i] = NULL;

		retirado->quantos--;
		if(retirado->quantos == 0) retirado->status = 1;
	}
	void inicializa_redis(cliente** todos,int total_cliente,cliente** brother,int total_brother,cliente** who)
	{
		for (int i = 0; i < total_cliente; i++)
		{
			if(i < total_brother)
				todos[i] = brother[i];
			else todos[i] = who[i - total_brother];
		}
	}
	void inicializa_redis_pagina(int* todos_indice,int* todos_folhas,int total_cliente,pagina* brother,pagina* father,pagina* who,int onde_brother)
	{
		int i,j;

		for (j = 0; j < father->quantos; ++j)
		{
			if(father->folhas[j] == onde_brother)
			{
				for (i = 0; i < total_cliente; i++)
				{
					if(i < brother->quantos)
					{
						todos_indice[i] = brother->indice[i];
						todos_folhas[i] = brother->folhas[i];
					}
					else if(i == brother->quantos)
					{
						todos_indice[i] = father->indice[j];
						todos_folhas[i] = brother->folhas[i];
					}
					else
					{
						todos_indice[i] = who->indice[i - (brother->quantos + 1)];
						todos_folhas[i] = who->folhas[i - (brother->quantos + 1)];
					}
				}
				todos_folhas[i] = who->folhas[i - (brother->quantos + 1)];
				break;
			}
		}
	}
	void distriu_clientes(cliente** total,folha* who,folha* brother,int quantos_ficam,int total_cliente)
	{
		for (int i = 0; i < total_cliente; ++i)
		{
			if(i < quantos_ficam) brother->clientes[i] = total[i];
			else who->clientes[i - quantos_ficam] = total[i];
		}

		brother->quantos = quantos_ficam;
		who->quantos = total_cliente - quantos_ficam;
		brother->status=0;
		who->status=0;
	}
	void distriu_indice_folhas(int* todos_indice,int* todos_folhas,pagina* brother,pagina* father,pagina* who,int quantos_ficam,int total_cliente,int onde_brother)
	{
		int i,j;

		for(j = 0; j < father->quantos; ++j)
		{
			if(father->folhas[j] == onde_brother)
				break;
		}

		for(i = 0; i < total_cliente; ++i)
		{
			if(i < quantos_ficam){
				brother->indice[i] = todos_indice[i];
				brother->folhas[i] = todos_folhas[i];
			}
			else if(i == quantos_ficam)
			{
				father->indice[j] = todos_indice[i];
				brother->folhas[i] = todos_folhas[i];
			}
			else
			{
				who->indice[i - (quantos_ficam+1)] = todos_indice[i];
				who->folhas[i - (quantos_ficam+1)] = todos_folhas[i];
			}
		}
		who->folhas[i - (quantos_ficam+1)] = todos_folhas[i];

		brother->quantos = quantos_ficam;
		who->quantos = total_cliente - (quantos_ficam+1);

		for(i = who->quantos; i < 2*ORDEM; ++i)
		{
			who->indice[i] = -1;
			who->folhas[i+1] = -1;
		}
		for(i = brother->quantos; i < 2*ORDEM; ++i)
		{
			brother->indice[i] = -1;
			brother->folhas[i+1] = -1;
		}
	}
	void ajeita_incide_pai(pagina* father,int onde_brother,int subiu)
	{
		for (int i = 0; i < father->quantos; ++i)
		{
			if(father->folhas[i] == onde_brother)
			{
				father->indice[i] = subiu;
				break;
			}
		}
	}
	void pega_clientes(folha* brother,folha* who)
	{
		for (int i = brother->quantos; i < brother->quantos + who->quantos; ++i)
		{
			brother->clientes[i] = who->clientes[i - brother->quantos];
		}
		brother->quantos += who->quantos;
		brother->status=0;
	}
	void retira_indice_pai(pagina* father,int onde_brother)
	{
		int i;
		int swap;
		int j;
		for (i = 0; i < father->quantos; ++i)
		{
			if(father->folhas[i] == onde_brother)
			{
				for (j = i; j < father->quantos - 1; ++j)
				{
					father->indice[j] = father->indice[j+1];

					father->folhas[j+1] = father->folhas[j+2];
				}
				father->indice[j] = -1;
				father->folhas[j+1] = -1;
				break;
			}
		}
		father->quantos--;
	}
	void pega_indices_folhas(pagina* brother,pagina* father,pagina* who,int onde_brother)
	{
		int i;

		for (i = 0; i < father->quantos+1; ++i)
		{
			if(father->folhas[i] == onde_brother) break;
		}

		brother->indice[brother->quantos] = father->indice[i];
		brother->quantos++;

		for (i = brother->quantos; i < brother->quantos + who->quantos; ++i)
		{
			brother->indice[i] = who->indice[i - brother->quantos];
			brother->folhas[i] = who->folhas[i - brother->quantos];
		}
		brother->folhas[i] = who->folhas[i - brother->quantos];
		brother->status = 0;
		brother->quantos += who->quantos;
	}


//auxiliares gerais
	void ajeita_pai(int onde,pagina* pag,FILE* index,FILE* dados)
	{
		for(int i = 0; i <= pag->quantos; i++)
		{
			if(pag->tipo_filho)
			{
				fseek(dados,tamanho_folha()*pag->folhas[i],SEEK_SET);
				fwrite(&onde,sizeof(int), 1, dados);
			}
			else
			{
				fseek(index,tamanho_pagina()*pag->folhas[i],SEEK_SET);
				fwrite(&onde,sizeof(int), 1, index);
			}
		}
	}
	void imprime_ind_fol(pagina* father)
	{
		int i;
		for (i = 0; i < 2*ORDEM; ++i)
		{
			printf("indice[%d] = %d\n",i,father->indice[i]);
			printf("folhas[%d] = %d\n",i,father->folhas[i]);
		}
		printf("folhas[%d] = %d\n",i,father->folhas[i]);
	}
	void reescreve_raiz(int tipo,int onde,FILE* metadados)
	{
		rewind(metadados);
		fwrite(&tipo,sizeof(int), 1, metadados);
		fwrite(&onde,sizeof(int), 1, metadados);
	}
	folha* libera_folha(folha* quem)
	{
		for(int i = 0; i < 2*ORDEM; ++i)
		{
			if(quem->clientes[i] != NULL){
				free(quem->clientes[i]);
				quem->clientes[i] = NULL;
			}
		}
		if(quem->clientes) free(quem->clientes);
		quem->clientes = NULL;
		free(quem);
		return NULL;
	}
	pagina* libera_pagina(pagina* quem)
	{
		if(quem->indice)
			free(quem->indice);
		quem->indice=NULL;
		if(quem->indice)
			free(quem->folhas);
		quem->folhas = NULL;
		free(quem);

		return NULL;
	}
