/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_carreiras

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função listar_carreiras recebe o endereço da variável sistema
e lista todas as carreiras do mesmo */
void listar_carreiras(Geral *sistema) {
    int i;
    for (i = 0; i < sistema->num_carreiras; i++) {
        printf("%s ", sistema->carreiras[i].nome);
        if (sistema->carreiras[i].num_paragens > 0) {
            printf("%s %s ", 
            sistema->paragens[sistema->carreiras[i].id_primeira].nome,
            sistema->paragens[sistema->carreiras[i].id_ultima].nome);
        }
        printf("%d %.2f %.2f\n", sistema->carreiras[i].num_paragens,
        sistema->carreiras[i].custo_total,
        sistema->carreiras[i].duracao_total);
    }
}

/* Função listar_paragens_carr recebe o endereço da variável sistema, o nome de uma
carreira e um inteiro (0 ou 1), se o mesmo for 0, a função lista as paragens da
carreira recebida da origem ao destino, se for 1, do destino à origem */
void listar_paragens_carr(Geral *sistema, char nome_carreira[], int inverso) {
    /* id_par_falta - salva o id da carreira cuja ultima paragem falta 
    imprimir, id_c - id da carreira na lista das carreiras */
    int i, id_par_falta = 0, id_c = id_carreira(sistema, nome_carreira);
    if (!inverso && sistema->carreiras[id_c].num_paragens > 0) {
        for (i = 0; i < sistema->num_ligacoes; i++) {
            if (sistema->ligacoes[i].id_carreira == id_c) {
                printf("%s, ", 
                sistema->paragens[sistema->ligacoes[i].id_primeira].nome);
                id_par_falta = i;
            }
        }
        printf("%s\n", 
        sistema->paragens[sistema->ligacoes[id_par_falta].id_ultima].nome);
    } else if (inverso && sistema->carreiras[id_c].num_paragens > 0) {
        for (i = (sistema->num_ligacoes - 1); i >= 0; i--) {
            if (sistema->ligacoes[i].id_carreira == id_c) {
                printf("%s, ", 
                sistema->paragens[sistema->ligacoes[i].id_ultima].nome);
                id_par_falta = i;
            }
        }
        printf("%s\n", 
        sistema->paragens[sistema->ligacoes[id_par_falta].id_primeira].nome);
    } 
}

/* Função criar_carreira recebe o endereço da variável sistema e
o nome de uma carreira (vetor) e cria uma carreira, com o nome recebido
e o resto põe a zeros (alocando a memória necessária) */
void criar_carreira(Geral *sistema, char nome_carreira[]) {
    sistema->carreiras = (Carreira*)safe_realloc(sistema->carreiras, 
    sizeof(Carreira)*(sistema->num_carreiras + 1));
    sistema->carreiras[sistema->num_carreiras].nome = 
    safe_malloc(sizeof(char)*(strlen(nome_carreira) + 1));
    strcpy(sistema->carreiras[sistema->num_carreiras].nome, nome_carreira);
    sistema->carreiras[sistema->num_carreiras].custo_total = 0;
    sistema->carreiras[sistema->num_carreiras].duracao_total = 0;
    sistema->carreiras[sistema->num_carreiras].num_paragens = 0;
    sistema->carreiras[sistema->num_carreiras].id_primeira = 0;
    sistema->carreiras[sistema->num_carreiras].id_ultima = 0;
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