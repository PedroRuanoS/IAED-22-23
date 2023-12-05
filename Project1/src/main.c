/* 

    Pedro Silveira 106642
    C file - funcoes principais do projeto1

*/

#include "project1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função main cria o sistema de transportes e chama as funções que põeem o
mesmo a zeros e que recebem os comandos do input */
int main() {
    /* Sistema de tranportes - vetores com carreiras, ligações e paragens */
    static Geral sistema = {0}; 
    zerador_sistema(&sistema);
    while(acionador_comandos(&sistema));
    return 0;   
}

/* Função zerador_sistema recebe o endereço da variável sistema e põe as 
componentes da mesma (vetores) a zeros, assim como
as componentes de cada elemento dos vetores */
void zerador_sistema(Geral *sistema) {
    int i;
    Paragem paragem_nula = {0}; /* Paragem a zeros */
    Carreira carreira_nula = {0}; /* Carreira a zeros */
    Ligacao ligacao_nula = {0}; /* Ligação a zeros */
    carreira_nula.primeira = &paragem_nula;
    carreira_nula.ultima = &paragem_nula;
    ligacao_nula.carreira = &carreira_nula;
    ligacao_nula.primeira = &paragem_nula;
    ligacao_nula.ultima = &paragem_nula;
    for (i = 0; i < MAX_CARREIRAS; i++) {
        sistema->carreiras[i] = carreira_nula;
    }
    for (i = 0; i < MAX_PARAGENS; i++) {
        sistema->paragens[i] = paragem_nula;
    }
    for (i = 0; i < MAX_LIGACOES; i++) {
        sistema->ligacoes[i] = ligacao_nula;
    }
}

/* Função zerador_sistema recebe o endereço da variável sistema e pede o 
input da letra que dita a ação a fazer */
int acionador_comandos(Geral *sistema) {
    char c = getchar(); /* letra (ação) */
    switch(c) {
        case 'q':
            return FALSE;
        case 'c':
            comando_carreiras(sistema);
            break;
        case 'p':
            comando_paragens(sistema);
            break;
        case 'l':
            comando_ligacoes(sistema);
            break;
        case 'i':
            comando_intersecoes(sistema);
            break;
        default:
            break;
    }
    return TRUE;
}

/* Função input_to_vetor recebe um ponteiro que aponta para um vetor de 
ponteiros (função auxiliar às funções dos comandos) e um ponteiro 
de inteiro, e atribui o valor apontado pelos ponteiros do vetor, que
são a(s) palavra(s) do comando (elementos do comando podem conter mais
do que uma palavra, estando entre aspas)*/
void input_to_vetor(char** comando, int* num_pal_comando) {
    int i = 0, j = 0, aspa = FALSE; /* aspa - se caracter esta entre aspas */
    char c = getchar();
    if (c != '\n') {
        while (c != '\n') {
            if (c == '\"') {
                aspa = !aspa;
            } else if (c == ' ' && aspa == FALSE && j != 0) {
                comando[i++][j] = '\0'; /* acaba a string e aumenta o índice */
                j = 0;
                (*num_pal_comando)++;
            } else if ((j == 0 && (c != ' ' || aspa == TRUE)) || j != 0) {
                comando[i][j++] = c;
            }
            c = getchar();
        }
        (*num_pal_comando)++;
        comando[i][j] = '\0';
    } else {
        *num_pal_comando = 0;
    }
}

/* Função comando_carreiras recebe o endereço da variável sistema 
e cria uma carreira ou lista as carreiras do sistema ou lista
as paragens de uma carreira ou detetar erro */
void comando_carreiras(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int i, num_pal_comando = 0;
    /* vetor com os comandos do input (nome da carreira e inverso)*/
    char comando[CARREIRAS_ARGUMENTOS][MAX_NOME_CARREIRA] = {0};
    /* vetor de ponteiros (que vão apontar para o primeiro elemento
    de cada elemento do comando) */
    char *vetor_auxiliar[CARREIRAS_ARGUMENTOS];
    /* ponteiro para ponteiro (vai apontar para vetor auxiliar) */
    char **ponteiro_comando;
    vetor_auxiliar[0] = &comando[0][0];
    vetor_auxiliar[1] = &comando[1][0];  
    ponteiro_comando = vetor_auxiliar;
    input_to_vetor(ponteiro_comando, &num_pal_comando);
    if (num_pal_comando == 0) {
        listar_carreiras(sistema);
    } else if (num_pal_comando == 1) {
        for (i = 0; i < sistema->num_carreiras; i++) {
            if (!strcmp(comando[0], sistema->carreiras[i].nome)) {
                listar_paragens_carr(sistema, comando[0], FALSE);
                return;
            }
        }
        criar_carreira(sistema, comando[0]);
    } else if (num_pal_comando == 2 && inverso_valido(comando[1])) {
        listar_paragens_carr(sistema, comando[0], TRUE);
    } else if (num_pal_comando == 2 && !inverso_valido(comando[1])) {
        printf("%s\n", ERRO_INVERSO);
    }
}

/* Função comando_paragens recebe o endereço da variável sistema 
e cria uma paragem ou lista as paragens do sistema ou lista
as coordenadas de uma paragem ou detetar erro */
void comando_paragens(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int num_pal_comando = 0;
    /* vetor com os comandos do input (nome da paragem, latitude e longitude)*/
    char comando[PARAGENS_ARGUMENTOS][MAX_NOME_PARAGEM] = {0};
    /* vetor de ponteiros (que vão apontar para o primeiro elemento
    de cada elemento do comando) */
    char *vetor_auxiliar[PARAGENS_ARGUMENTOS];
    /* ponteiro para ponteiro (vai apontar para vetor auxiliar) */
    char **ponteiro_comando;
    vetor_auxiliar[0] = &comando[0][0];
    vetor_auxiliar[1] = &comando[1][0];
    vetor_auxiliar[2] = &comando[2][0];
    ponteiro_comando = vetor_auxiliar;
    input_to_vetor(ponteiro_comando, &num_pal_comando);
    if (num_pal_comando == 0) {
        listar_paragens(sistema);
    } else if (num_pal_comando == 1) {
        listar_coords_paragem(sistema, comando[0]);
    } else if (num_pal_comando == 3) {
        criar_parag(sistema, comando[0], atof(comando[1]), atof(comando[2]));
    }
}

/* Função comando_ligacoes recebe o endereço da variável sistema 
e cria uma ligação ou detetar erro */
void comando_ligacoes(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int num_pal_comando = 0;
    /* vetor com os comandos do input (nome da carreira, nome da primeira e
    segunda paragens, custo e duração)*/
    char comando[LIGACOES_ARGUMENTOS][MAX_NOME_PARAGEM] = {0};
    /* vetor de ponteiros (que vão apontar para o primeiro elemento
    de cada elemento do comando) */
    char *vetor_auxiliar[LIGACOES_ARGUMENTOS];
    /* ponteiro para ponteiro (vai apontar para vetor auxiliar) */
    char **ponteiro_comando;
    vetor_auxiliar[0] = &comando[0][0];
    vetor_auxiliar[1] = &comando[1][0];
    vetor_auxiliar[2] = &comando[2][0];
    vetor_auxiliar[3] = &comando[3][0];
    vetor_auxiliar[4] = &comando[4][0];
    ponteiro_comando = vetor_auxiliar;
    input_to_vetor(ponteiro_comando, &num_pal_comando);
    if (erros_ligacao(sistema, ponteiro_comando)) {
        return;
    } else if (!erros_ligacao(sistema, ponteiro_comando)) {
        iniciar_ligacao(sistema, ponteiro_comando);
    }
}

/* Função comando_ligacoes recebe o endereço da variável sistema 
e lista as carreiras das interseções (paragens com mais de uma carreira) */
void comando_intersecoes(Geral *sistema) {
    int i;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (sistema->paragens[i].num_carreiras > 1) {
            printf("%s %d:", sistema->paragens[i].nome,
            sistema->paragens[i].num_carreiras);
            listar_carrs_paragem(sistema, sistema->paragens[i].nome);
        }
    }
}
