/* 

    Pedro Silveira 106642
    C file - funcoes principais do projeto 2

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função main cria o sistema de transportes e chama as funções que libertam
a memóriado mesmo e que recebem os comandos do input */
int main() {
    /* Sistema de transportes - vetores com carreiras, ligações e paragens */
    Geral sistema = {0}; 
    sistema.carreiras = safe_malloc(sizeof(Carreira));
    sistema.paragens = safe_malloc(sizeof(Paragem));
    sistema.ligacoes = safe_malloc(sizeof(Ligacao));
    sistema.num_carreiras = 0;
    sistema.num_paragens = 0;
    sistema.num_ligacoes = 0;
    while(acionador_comandos(&sistema));
    free_sistema(&sistema);
    return 0;   
}

/* Função acionador_comandos recebe o endereço da variável sistema e pede o 
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
        case 'a':
            comando_apaga(sistema);
            break;
        case 'r': 
            comando_remove_c(sistema);
            break;
        case 'e':
            comando_elimina_p(sistema);
    }
    return TRUE;
}

/* Função free_sistema recebe o endereço da variável sistema e liberta toda a
memória préviamente alocada para o mesmo */
void free_sistema(Geral *sistema) {
    int i;
    for (i = 0; i < sistema->num_carreiras; i++) {
        free(sistema->carreiras[i].nome);
    }
    free(sistema->carreiras);
    for (i = 0; i < sistema->num_paragens; i++) {
        free(sistema->paragens[i].nome);
    }
    free(sistema->paragens);
    free(sistema->ligacoes);
}

/* Função safe_malloc recebe a quantidade de memória a alocar e devolve um
ponteiro com essa memória alocada (prevendo não haver mais memória) */
void* safe_malloc(var_size tamanho) {
    /* ponteiro cuja memória vai ser alocada para */
    void *ptr = malloc(tamanho);
    if (ptr == NULL) {
        printf("%s\n", ERRO_NAO_HA_MEMORIA);
        exit(0);    
    } else {
        return ptr;
    }
}

/* Função safe_realloc recebe a quantidade de memória a alocar e o ponteiro
que já contém memória alocada e devolve um ponteiro com a memória total 
(prevendo não haver mais memória) */
void* safe_realloc(void *ptr, var_size novo_tamanho) {
    /* ponteiro cuja memória vai ser alocada para */
    void *novo_ptr = realloc(ptr, novo_tamanho);
    if (novo_ptr == NULL) {
        printf("%s\n", ERRO_NAO_HA_MEMORIA);
        exit(0);
    } else {
        return novo_ptr;
    }
}

/* Função free_comando recebe um ponteiro para um vetor de ponteiros e o 
tamanho do vetor e liberta toda a memória que foi alocada para o mesmo */
void free_comando(char **comando, int num_pal_comando) {
    int i;
    for (i = 0; i < num_pal_comando; i++) {
        free(comando[i]);
    }
    free(comando);
}

/* Função input_to_vetor recebe o endereço para um ponteiro para um vetor de
ponteiros e modifica-o, ou seja, adiciona elementos o vetor de ponteiros que 
apontam para as diferentes palavras/argumentos dos comandos dados ao computador
pelo utilizador (aumentando o valor da variável num_pal_comando - que também
recebe) */
void input_to_vetor(char ***comando, int *num_pal_comando) {
    /* aspa - se caracter esta entre aspas, tamanh_lin - tamanho de cada argumento
    (elemento apontado pelo ponteiro dao vetor) */
    int i = 0, j = 0, aspa = FALSE, tamanh_lin = 0;
    /* caracter a ser analisado */
    char c = getchar();
    if (c != '\n') {
        while (c != '\n') {
            if (c == '\"') {
                aspa = !aspa;
            } else if (c == ' ' && aspa == FALSE && j != 0) {
                /* aumenta memória para último caracter da palavra */
                (*comando)[i] = (char*)safe_realloc((*comando)[i], 
                (++tamanh_lin)*sizeof(char));
                (*comando)[i++][j] = '\0'; /* acaba string, aumenta o índice */
                j = 0;
                (*num_pal_comando)++;
                tamanh_lin = 0;
            } else if ((j == 0 && (c != ' ' || aspa == TRUE)) || j != 0) {
                /* aumenta a memória para novo caracter da palavra*/
                (*comando)[i] = (char*)safe_realloc((*comando)[i], 
                (++tamanh_lin)*sizeof(char)); 
                (*comando)[i][j++] = c;
            }
            c = getchar();
        }
        (*num_pal_comando)++;
        /* aumenta memória para último caracter da última palavra */
        (*comando)[i] = (char*)safe_realloc((*comando)[i], 
        (++tamanh_lin)*sizeof(char));
        (*comando)[i][j] = '\0';
    } else {
        *num_pal_comando = 0;
    }
}

/* Função id_carreira recebe o endereço da variável sistema e o nome de uma 
carreira e devolve o indice da carreira do comando no vetor de carreiras do 
sistema, ou -1, caso a carreira não exista */
int id_carreira(Geral *sistema, char nome_carreira[]) {
    int i;
    for (i = 0; i < sistema->num_carreiras; i++) {
        if (!strcmp(nome_carreira, sistema->carreiras[i].nome)) {
            return i;
        }
    }
    return ID_NAO_EXISTE;
}

/* Função id_paragem recebe o endereço da variável sistema e o nome de uma 
paragem e devolve o indice da paragem do comando no vetor de paragens do 
sistema, ou -1, caso a paragem não exista */
int id_paragem(Geral *sistema, char nome_paragem[]) {
    int i;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (!strcmp(nome_paragem, sistema->paragens[i].nome)) {
            return i;
        }
    }
    return ID_NAO_EXISTE;
}

/* Função comando_carreiras recebe o endereço da variável sistema 
e cria uma carreira ou lista as carreiras do sistema ou lista
as paragens de uma carreira ou deteta erro */
void comando_carreiras(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int i, num_pal_comando = 0;
    /* ponteiro para vetor de ponteiros (que apontam para os argumentos
    introduzidos no input) */
    char **comando = (char**)safe_malloc(CARREIRAS_ARGUMENTOS*sizeof(char*));
    comando[0] = safe_malloc(sizeof(char));
    comando[1] = safe_malloc(sizeof(char));
    input_to_vetor(&comando, &num_pal_comando);
    if (num_pal_comando == 0) {
        listar_carreiras(sistema);
    } else if (num_pal_comando == 1) {
        for (i = 0; i < sistema->num_carreiras; i++) {
            if (!strcmp(comando[0], sistema->carreiras[i].nome)) {
                listar_paragens_carr(sistema, comando[0], FALSE);
                free_comando(comando, CARREIRAS_ARGUMENTOS);
                return;
            }
        }
        criar_carreira(sistema, comando[0]);
    } else if (num_pal_comando == 2 && inverso_valido(comando[1])) {
        listar_paragens_carr(sistema, comando[0], TRUE);
    } else if (num_pal_comando == 2 && !inverso_valido(comando[1])) {
        printf("%s\n", ERRO_INVERSO);
    }
    free_comando(comando, CARREIRAS_ARGUMENTOS);
}

/* Função comando_paragens recebe o endereço da variável sistema e cria uma
paragem ou lista as paragens do sistema ou lista as coordenadas de uma paragem
ou deteta erro */
void comando_paragens(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int num_pal_comando = 0;
    /* ponteiro para vetor de ponteiros (que apontam para os argumentos
    introduzidos no input) */
    char **comando = (char**)safe_malloc(PARAGENS_ARGUMENTOS*sizeof(char*));
    comando[0] = safe_malloc(sizeof(char));
    comando[1] = safe_malloc(sizeof(char));
    comando[2] = safe_malloc(sizeof(char));
    input_to_vetor(&comando, &num_pal_comando);
    if (num_pal_comando == 0) {
        listar_paragens(sistema);
    } else if (num_pal_comando == 1) {
        listar_coords_paragem(sistema, comando[0]);
    } else if (num_pal_comando == 3) {
        criar_parag(sistema, comando[0], atof(comando[1]), atof(comando[2]));
    }
    free_comando(comando, PARAGENS_ARGUMENTOS);
}

/* Função comando_ligacoes recebe o endereço da variável sistema e cria uma 
ligação ou deteta erro */
void comando_ligacoes(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input*/
    int num_pal_comando = 0;
    /* ponteiro para vetor de ponteiros (que apontam para os argumentos
    introduzidos no input) */
    char **comando = (char**)safe_malloc(LIGACOES_ARGUMENTOS*sizeof(char*));
    comando[0] = safe_malloc(sizeof(char));
    comando[1] = safe_malloc(sizeof(char));
    comando[2] = safe_malloc(sizeof(char));
    comando[3] = safe_malloc(sizeof(char));
    comando[4] = safe_malloc(sizeof(char));
    input_to_vetor(&comando, &num_pal_comando);
    if (!erros_ligacao(sistema, comando)) {
        iniciar_ligacao(sistema, comando);
    }
    free_comando(comando, LIGACOES_ARGUMENTOS);
}

/* Função comando_ligacoes recebe o endereço da variável sistema e lista as 
carreiras das interseções (paragens com mais de uma carreira) */
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

/* Função comando_apaga recebe o endereço da variável sistema e apaga todos
os dados do sistema */
void comando_apaga(Geral *sistema) {
    sistema->carreiras = NULL;
    sistema->paragens = NULL;
    sistema->ligacoes = NULL;
    sistema->num_carreiras = 0;
    sistema->num_paragens = 0;
    sistema->num_ligacoes = 0;
    free_sistema(sistema);
}

/* Função comando_remove_c recebe o endereço da variável sistema e remove uma
carreira e todas as suas ligações */
void comando_remove_c(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input, id_c - id
    da carreira no vetor das carreiras */
    int num_pal_comando = 0, id_c;
    /* ponteiro para vetor de ponteiros (que apontam para os argumentos
    introduzidos no input) */
    char **comando = (char**)safe_malloc(REMOVE_ARGUMENTOS*sizeof(char*));
    comando[0] = safe_malloc(sizeof(char));
    input_to_vetor(&comando, &num_pal_comando);
    id_c = id_carreira(sistema, comando[0]);
    if (id_c >= 0) {
        remover_carreira(sistema, id_c);
    } else if (id_c < 0) {
        printf("%s: %s\n", comando[0], ERRO_CARREIRA_NAO_EXISTE);
    }
    free_comando(comando, REMOVE_ARGUMENTOS);
}

/* Função comando_remove_c recebe o endereço da variável sistema e remove uma
paragem e todas as ligações que a conteém como origem ou destino */
void comando_elimina_p(Geral *sistema) {
    /* num_pal_comando - número de palavras/elementos do input, id_l - id
    da ligação no vetor das ligções, id_p - id da paragem no vetor das
    paragens, ligs_removidas - se todas as ligações com a paragem já foram
    removidas */
    int num_pal_comando = 0, i = 0, id_p, id_l, ligs_removidas = TRUE;
    /* ponteiro para vetor de ponteiros (que apontam para os argumentos
    introduzidos no input) */
    char **comando = (char**)safe_malloc(REMOVE_ARGUMENTOS*sizeof(char*));
    comando[0] = safe_malloc(sizeof(char));
    input_to_vetor(&comando, &num_pal_comando);
    id_p = id_paragem(sistema, comando[0]);
    if (id_p >= 0) {
        remover_paragem(sistema, id_p);
        /* enquanto existem ligações com a paragem a ser removida nas mesmas */
        while (TRUE) {
            for (i = 0; i < sistema->num_ligacoes; i++) {
                if (sistema->ligacoes[i].id_ultima == id_p || 
                sistema->ligacoes[i].id_primeira == id_p) {
                    id_l = i;
                    ligs_removidas = FALSE;
                    break;
                }
            }
            if (!ligs_removidas) {
                remover_ligacoes_p(sistema, id_l, id_p);
                ligs_removidas = TRUE;
            } else {
                break;
            }
        }
        alterar_lig_carr(sistema, id_p);
    } else if (id_p < 0) {
        printf("%s: %s\n", comando[0], ERRO_PARAGEM_NAO_EXISTE);
    }
    free_comando(comando, REMOVE_ARGUMENTOS);
}
