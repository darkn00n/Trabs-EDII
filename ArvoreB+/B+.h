#ifndef BPLUS_H
#define BPLUS_H

#include "clientes.h"
#include "pagina.h"
#include "folha.h"

//Vitor Pinheiro David
//Douglas Custodio de Araujo
//Jose Lucas Alves gomes

//função de busca
	int busca(int chave,int* pagina,int* posi,FILE* metadados,FILE* index,FILE* dados);
	int busca_em_folha(int chave,int pagi,int* posi,FILE* dados);
//auxiliares de busca
	void busca_aux(int chave,FILE* metadados,FILE* index,FILE* dados);
	void inicializa_B(FILE* metadados,FILE* dados);


//funções incerções
	void insere(FILE* metadados,FILE* index,FILE* dados);
	void particionamento_folha(int where_who,folha* who,cliente* adicionar,FILE* metadados,FILE* index,FILE* dados);
	void particionamento_pagina(int codigo,int adicionar,int who,FILE* metadados,FILE* index,FILE* dados);
//auxiliares incerção
	void ordena_folha(cliente** clientes);
	void ordena_pagina(pagina* father);
	int acha_espaco(int qual,FILE* index,FILE* dados);
	void inicializa_clientes(cliente** clientes,folha* who,cliente* adicionar);
	void inicializa_brother_pag(int *indices,int *folhas,int *indices_for_brother,int *folhas_for_brother);
	void inicializa_brother(cliente** clientes,cliente** for_brother);
	void inicializa_ind_fol(int *indices,int *folhas);
	void inicializa_indices_folhas(int *indices,int *folhas, pagina* who,int codigo_subir, int local_new_node);
	void ordena_indices_folhas(int* indices,int *folhas,int quantos);


//funções remoção
	void remocao(int chave,FILE* metadados,FILE* index,FILE* dados);
	void redistribui(pagina* father,folha* who,folha* brother,int onde_father,int onde_who,int onde_brother,FILE* index,FILE* dados);
	void concatena_folha(pagina* father,folha* who,folha* brother,int onde_father,int onde_who,int onde_brother,FILE* metadados,FILE* index,FILE* dados);
	void concatena_pagina(pagina* father,pagina* who,pagina* brother,int onde_father,int onde_who,int onde_brother,FILE* metadados,FILE* index,FILE* dados);
	void reditribui_pagina(pagina* father,pagina* who,pagina* brother,int onde_father,int onde_who,int onde_brother,FILE* index,FILE* dados);
//auxiliares remoção
	void remove_cliente(int posi,folha* retirado);
	void inicializa_redis(cliente** total,int total_cliente,cliente** brother,int total_brother,cliente** who);
	void inicializa_redis_pagina(int* todos_indice,int* todos_folhas,int total_cliente,pagina* brother,pagina* father,pagina* who,int onde_brother);
	void distriu_clientes(cliente** total,folha* who,folha* brother,int quantos_ficam,int total_cliente);
	void distriu_indice_folhas(int* todos_indice,int* todos_folhas,pagina* brother,pagina* father,pagina* who,int quantos_ficam,int total_cliente,int onde_brother);
	void ajeita_incide_pai(pagina* father,int onde_brother,int onde_who);
	void pega_clientes(folha* brother,folha* who);
	void retira_indice_pai(pagina* father,int onde_brother);
	void pega_indices_folhas(pagina* brother,pagina* father,pagina* who,int onde_brother);


//auxiliares gerais
	void ajeita_pai(int onde,pagina* pag,FILE* index,FILE* dados);
	void imprime_ind_fol(pagina* father);
	void reescreve_raiz(int tipo,int onde,FILE* metadados);
	folha* libera_folha(folha* quem);
	pagina* libera_pagina(pagina* quem);

#endif // final do ifdef BPLUS_H
