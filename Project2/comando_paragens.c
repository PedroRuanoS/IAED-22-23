/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_paragens

*/

#include "project2.h"

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
    /* paragem_existe - se a paragem existe, id_p - indice da paragem
    no vetor das paragens cujas coordenadas devem ser impressas */
    int paragem_existe = FALSE, id_p = id_paragem(sistema, nome_paragem);
    if (id_p >= 0) {
        paragem_existe = TRUE;
    }
    if (!paragem_existe) {
        printf("%s: %s\n", nome_paragem, ERRO_PARAGEM_NAO_EXISTE);
    } else {
        printf("%16.12f %16.12f\n", sistema->paragens[id_p].latitude,
        sistema->paragens[id_p].longitude);
    }
}

/* Função criar_parag recebe o endereço da variável sistema, o nome de uma 
paragem (vetor) e a latitude e longitude da mesma (int) e verifica se a 
paragem já existe, se já existe, imprime a mensagem associada a este tipo de 
erro, se não existe cria uma paragem com o nome e coordenadas recebidos */
void criar_parag(Geral *sistema, char nome_paragem[], double lat, double lon) {
    /* paragem_existe - se a paragem existe */
    int paragem_existe = FALSE;
    if (id_paragem(sistema, nome_paragem) >= 0) {
        paragem_existe = TRUE;
    }
    if (paragem_existe) {
        printf("%s: %s\n", nome_paragem, ERRO_PARAGEM_JA_EXISTE);
        return;
    }
    sistema->paragens = (Paragem*)safe_realloc(sistema->paragens, 
    sizeof(Paragem)*(sistema->num_paragens + 1));
    sistema->paragens[sistema->num_paragens].nome = 
    safe_malloc(sizeof(char)*(strlen(nome_paragem) + 1));
    strcpy(sistema->paragens[sistema->num_paragens].nome, nome_paragem);
    sistema->paragens[sistema->num_paragens].latitude = lat;
    sistema->paragens[sistema->num_paragens].longitude = lon;
    sistema->paragens[sistema->num_paragens].num_carreiras = 0;
    (sistema->num_paragens)++;
}