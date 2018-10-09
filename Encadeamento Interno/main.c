#include "hash_interna.c"
#include "clientes.c"

int main(void){
    printf("============Encadeamento Interno===========\nAlunos:\nDouglas Custodio de Araujo\nJose Lucas Alves Gomes\nVitor Pinheiro David\n");
    printf("===========================================\n\n");
    FILE* hash_interna;

    hash_interna = fopen("tabHash.dat","r+b");
    if(hash_interna == NULL) hash_interna = inicializa_hash();
    
    int achou, endereco;
    /*
    cliente* lul = cria_cliente();
    insere_interna(lul, hash_interna);

    rewind(hash_interna);
    /*sobrescreve_cod(hash_interna, 90);
    sobrescreve_cod(hash_interna, 7);
    sobrescreve_cod(hash_interna, 4);
    sobrescreve_prox(hash_interna, 5);
    sobrescreve_prox(hash_interna, 6);
    sobrescreve_status(hash_interna, 5);
    sobrescreve_status(hash_interna, 4);

    ler_clientes(hash_interna);

    busca(7, hash_interna, &achou, &endereco);
    printf("%d Achou\n", achou);
    printf("---------------------------------\n");
    //remove_interna(7, hash_interna);
    insere_interna(cria_cliente(), hash_interna);
    ler_clientes(hash_interna);
    insere_interna(cria_cliente(), hash_interna);
    ler_clientes(hash_interna);*/


    int option = 1;
    int remove;
    int codigo;
    do{
        printf("Digite o que quer fazer com a tabela:\n");
        printf("1 - Inserir\n2 - Remover\n3 - Mostrar Tabela\n4 - Busca\n0 - Sair\n");
        scanf("%d", &option);
        switch(option){
        case 0:
            break;
        case 1:
           insere_interna(cria_cliente(), hash_interna);
           break;
        case 2:
            printf("Digite codigo a ser removido:\n");
            scanf("%d", &remove);
            remove_interna(remove, hash_interna);
            break;
        case 3:
            ler_clientes(hash_interna);
            break;
        case 4:
            printf("Digite codigo a ser buscado:\n");
            scanf("%d", &codigo);
            busca(codigo, hash_interna, &achou, &endereco);
            if(achou == 1)
                printf("Registro Encontrado no compartimento %d\n", endereco);
            else if(achou == 2)
                printf("Registro nao consta na tabela\n");
            break;
        default:
            printf("Opcao nao existe\n");
        }
    }while(option);

    return 0;
}
