/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_intersecoes

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função listar_carrs_paragem recebe o endereço da variavel sistema e o nome
de uma paragem (que tem mais de uma carreira), junta os ids das carreiras da 
paragem recebida num vetor, organiza-os por ordem alfabética (de acordo com o 
nome associado ao id), e imprime cada nome associado aos ids do vetor - 
os ids são os das carreiras no vetor carreiras */
void listar_carrs_paragem(Geral *sistema, char nome_paragem[]) {
    /* num_carrs - tamanho do vetor carrs_paragem, anterior_id_c - ultimo id a
    ser associado ao vetor */
    int i, num_carrs = 0, anterior_id_c = -1, 
    id_p = id_paragem(sistema, nome_paragem);
    int *carrs_paragem = (int*)safe_malloc(sizeof(int));
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if ((anterior_id_c != sistema->ligacoes[i].id_carreira) 
        && (sistema->ligacoes[i].id_primeira == id_p ||
        sistema->ligacoes[i].id_ultima == id_p)) {
            carrs_paragem = (int*)safe_realloc(carrs_paragem, 
            sizeof(int)*(num_carrs + 1));
            carrs_paragem[num_carrs] = sistema->ligacoes[i].id_carreira;
            num_carrs++;
            anterior_id_c = sistema->ligacoes[i].id_carreira;
        }
    }
    sort_print_carrs(sistema, carrs_paragem, num_carrs);
    free(carrs_paragem);
}

/* Função sort_print_carrs (auxiliar à função listar_carrs_paragem) recebe
um vetor com os ids de carreiras, e organiza-os utilizando o algortimo de sort 
"insertion sort", e imprime os nomes das carrewiras associados aos ids deste 
vetor */
void sort_print_carrs(Geral *sistema, int *carrs_paragem, int num_carrs) {
    int i, j;
    /* carreira_atual - id da carreira que vai ser "ordenada" no vetor dos ids
    de carreiras */
    int carreira_atual;

    for (i = 1; i < num_carrs; i++) {
        carreira_atual = carrs_paragem[i];
        j = i - 1;
        while (j >= 0 && strcmp(sistema->carreiras[carrs_paragem[j]].nome, 
        sistema->carreiras[carreira_atual].nome) > 0) {
            carrs_paragem[j + 1] = carrs_paragem[j];
            j--;
        }
        carrs_paragem[j + 1] = carreira_atual;
    }

    for (i = 0; i < num_carrs; i++) {
        printf(" %s", sistema->carreiras[carrs_paragem[i]].nome);
    }
    printf("\n");
}

