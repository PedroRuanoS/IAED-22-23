/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_intersecoes

*/

#include "project1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função listar_carrs_paragem recebe o endereço da variavel sistema e o nome
de uma paragem (que tem mais de uma carreira), junta os nomes das carreiras da 
paragem recebida num vetor, organiza-os por ordem alfabética, e imprime cada 
elemento do vetor */
void listar_carrs_paragem(Geral *sistema, char nome_paragem[]) {
    /* num_carrs - tamanho do vetor carrs_paragem, carr_no_vetor - se a 
    carreira a ser analisada já está no vetor carrs_paragem*/
    int i, j, num_carrs = 0, carr_no_vetor = FALSE;
    char carrs_paragem[MAX_CARREIRAS][MAX_NOME_CARREIRA] = {0};
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (!strcmp(nome_paragem, sistema->ligacoes[i].primeira->nome) || 
        !strcmp(nome_paragem, sistema->ligacoes[i].ultima->nome)) {
            for (j = 0; j < num_carrs; j++) {
                if (!strcmp(sistema->ligacoes[i].carreira->nome, 
                carrs_paragem[j])) {
                    carr_no_vetor = TRUE;
                    break;
                }
            }
            if (!carr_no_vetor) {
                strcpy(carrs_paragem[num_carrs], 
                sistema->ligacoes[i].carreira->nome);
                num_carrs++;
            }
        }
        carr_no_vetor = FALSE;
    }
    sort_print_carrs(carrs_paragem, num_carrs);
}

/* Função sort_print_carrs (auxiliar à função listar_carrs_paragem) recebe
um vetor com nomes de carreiras, organiza-os utilizando o algortimo de sort 
"insertion sort", e imprime os elementos deste vetor */
void sort_print_carrs(char carrs_paragem[][MAX_NOME_CARREIRA], int num_carrs) {
    int i, j;
    /* palavra (vetor) do vetor carr_paragem que está a ser analisada*/
    char carreira_atual[MAX_NOME_CARREIRA];

    for (i = 1; i < num_carrs; i++) {
        strcpy(carreira_atual, carrs_paragem[i]);
        j = i - 1;
        while (j >= 0 && strcmp(carrs_paragem[j], carreira_atual) > 0) {
            strcpy(carrs_paragem[j + 1], carrs_paragem[j]);
            j--;
        }
        strcpy(carrs_paragem[j + 1], carreira_atual);
    }

    for (i = 0; i < num_carrs; i++) {
        printf(" %s", carrs_paragem[i]);
    }
    printf("\n");
}
