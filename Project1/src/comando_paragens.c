/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_paragens

*/

#include "project1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função listar_paragens recebe o endereço da variável sistema e lista
todas as paragens do mesmo */
void listar_paragens(Geral *sistema) {
    int i;
    for (i = 0; i < sistema->num_paragens; i++) {
        printf("%s: %16.12f %16.12f %d\n", sistema->paragens[i].nome,
        sistema->paragens[i].latitude, sistema->paragens[i].longitude,
        sistema->paragens[i].num_carreiras);
    } 
}

/* Função listar_coords_paragem recebe o endereço da variável sistema e o nome
de uma paragem (vetor) e verifica se a paragem existe, se não existe, imprime
a mensagem associada a este tipo de erro, se existe, então imprime as 
coordenadas da paragem recebida */
void listar_coords_paragem(Geral *sistema, char nome_paragem[]) {
    /* paragem_existe - se a paragem existe, id_paragem - indice da paragem
    no vetor das paragens cujas coordenadas devem ser impressas */
    int i, paragem_existe = FALSE, id_paragem;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (!strcmp(nome_paragem, sistema->paragens[i].nome)) {
            paragem_existe = TRUE;
            id_paragem = i;
        }
    }
    if (!paragem_existe) {
        printf("%s: %s\n", nome_paragem, ERRO_PARAGEM_NAO_EXISTE);
    } else {
        printf("%16.12f %16.12f\n", sistema->paragens[id_paragem].latitude,
        sistema->paragens[id_paragem].longitude);
    }
}

/* Função criar_parag recebe o endereço da variável sistema, o nome de uma 
paragem (vetor) e a latitude e longitude da mesma (int) e verifica se a 
paragem já existe, se já existe, imprime a mensagem associada a este tipo de 
erro, se não existe cria uma paragem com o nome e coordenadas recebidos */
void criar_parag(Geral *sistema, char nome_paragem[], double lat, double lon) {
    /* paragem_existe - se a paragem existe */
    int i, paragem_existe = FALSE;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (!strcmp(nome_paragem, sistema->paragens[i].nome)) {
            paragem_existe = TRUE;
        }
    }
    if (paragem_existe) {
        printf("%s: %s\n", nome_paragem, ERRO_PARAGEM_JA_EXISTE);
        return;
    }
    if (sistema->num_paragens == MAX_PARAGENS) {
        return;
    }
    strcpy(sistema->paragens[sistema->num_paragens].nome, nome_paragem);
    sistema->paragens[sistema->num_paragens].latitude = lat;
    sistema->paragens[sistema->num_paragens].longitude = lon;
    sistema->paragens[sistema->num_paragens].num_carreiras = 0;
    (sistema->num_paragens)++;
}