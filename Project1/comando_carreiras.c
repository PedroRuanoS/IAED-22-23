/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_carreiras

*/

#include "project1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função listar_carreiras recebe o endereço da variável sistema
e lista todas as carreiras do mesmo */
void listar_carreiras(Geral *sistema) {
    /* tem_ligacoes - se carreira tem ligações */
    int i, j, tem_ligacoes = FALSE;
    for (i = 0; i < sistema->num_carreiras; i++) {
        printf("%s ", sistema->carreiras[i].nome);
        for (j = 0; j < sistema->num_ligacoes; j++) {
            /* se alguma ligação tem como carreira a carreira com indice i*/
            if (!strcmp(sistema->carreiras[i].nome, 
            sistema->ligacoes[j].carreira->nome)) {
                tem_ligacoes = TRUE; 
                break;
            }
        }
        if (tem_ligacoes) {
            printf("%s %s ", sistema->carreiras[i].primeira->nome,
            sistema->carreiras[i].ultima->nome);
        }
        printf("%d %.2f %.2f\n", sistema->carreiras[i].num_paragens,
        sistema->carreiras[i].custo_total,
        sistema->carreiras[i].duracao_total);
        tem_ligacoes = FALSE;
    }
}

/* Função listar_carreiras recebe o endereço da variável sistema, 
o nome de uma carreira (lista) e um inteiro (0 ou 1), se o mesmo for 0, 
a função lista as paragens da carreira recebida da origem ao destino, se for 1, 
do destino à origem */
void listar_paragens_carr(Geral *sistema, char nome_carreira[], int inverso) {
    /* id_paragem_falta salva o id da ligação cuja ultima paragem falta 
    imprimir */
    int i, id_paragem_falta = 0;
    if (!inverso && sistema->num_ligacoes > 0) {
        for (i = 0; i < sistema->num_ligacoes; i++) {
            if (!strcmp(nome_carreira, sistema->ligacoes[i].carreira->nome)) {
                printf("%s, ", sistema->ligacoes[i].primeira->nome);
                id_paragem_falta = i;
            }
        }
        printf("%s\n", sistema->ligacoes[id_paragem_falta].ultima->nome);
    } else if (inverso && sistema->num_ligacoes > 0) {
        for (i = (sistema->num_ligacoes - 1); i >= 0; i--) {
            if (!strcmp(nome_carreira, sistema->ligacoes[i].carreira->nome)) {
                printf("%s, ", sistema->ligacoes[i].ultima->nome);
                id_paragem_falta = i;
            }
        }
        printf("%s\n", sistema->ligacoes[id_paragem_falta].primeira->nome);
    } 
}

/* Função criar_carreira recebe o endereço da variável sistema e
o nome de uma carreira (vetor) e cria uma carreira, com o nome recebido
e o resto põe a zeros */
void criar_carreira(Geral *sistema, char nome_carreira[]) {
    if (sistema->num_carreiras == MAX_CARREIRAS) {
        return;
    }
    strcpy(sistema->carreiras[sistema->num_carreiras].nome, nome_carreira);
    sistema->carreiras[sistema->num_carreiras].custo_total = 0;
    sistema->carreiras[sistema->num_carreiras].duracao_total = 0;
    sistema->carreiras[sistema->num_carreiras].num_paragens = 0;
    sistema->carreiras[sistema->num_carreiras].primeira = NULL;
    sistema->carreiras[sistema->num_carreiras].ultima = NULL;
    (sistema->num_carreiras)++;
}

/* Função inverso_valido recebe o segundo elemento do comando c
e verifica se é valido (ou se a sua abreviação o é) */
int inverso_valido(char inverso[]) {
    /* len - tamanho da palavra (vetor) inverso + 1 (\0)*/
    int i, len = (strlen(inverso) + 1);
    if (len <= 3 || len > TAMANHO_INVERSO) {
        return FALSE;
    }
    for (i = 0; i < (len - 1); i++) {
        if (tolower(inverso[i]) != INVERSO[i]) {
            return FALSE;
        } 
    }
    return TRUE;
}