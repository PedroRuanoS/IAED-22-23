/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_remove_c

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função remover_paragem recebe o endereço da variável sistema e o id de uma
carreira, e remove essa carreira do vetor das carreiras, remove as ligações da
carreira e atualiza os ids das carreiras à frente da removida no vetor das
ligações */
void remover_carreira(Geral *sistema, int id_c) {
    int i;
    if (sistema->carreiras[id_c].num_paragens > 0) {
        altera_num_carreiras(sistema, id_c);
        remover_ligacoes_c(sistema, id_c);
    }
    free(sistema->carreiras[id_c].nome);
    for (i = id_c; i < (sistema->num_carreiras - 1); i++) {
        sistema->carreiras[i] = sistema->carreiras[i + 1];
    }
    (sistema->num_carreiras)--;
    sistema->carreiras = realloc(sistema->carreiras,
    sizeof(Carreira)*sistema->num_carreiras);
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (sistema->ligacoes[i].id_carreira > id_c) {
            sistema->ligacoes[i].id_carreira--;
        }
    }

}

/* Função remover_ligacoes_c (função auxiliar a remover_carreira) recebe o 
endereço da variável sistema e o id de uma carreira e remove todas as ligações
da carreira cujo id é id_c */
void remover_ligacoes_c(Geral *sistema, int id_c) {
    /* id_prim_lig - id da primeira ligação da carreira
    id_ult_lig - id da ultima ligação da carreira 
    num_ligs - número de ligações da carreira */
    int i, id_prim_lig = 0, id_ult_lig = 0, num_ligs = 0;
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (sistema->ligacoes[i].id_carreira == id_c) {
            id_prim_lig = i;
            break;
        }
    } 
    for (i = (sistema->num_ligacoes - 1); i >= 0; i--) {
        if (sistema->ligacoes[i].id_carreira == id_c) {
            id_ult_lig = i;
            break;
        } 
    }
    num_ligs = (id_ult_lig - id_prim_lig + 1);
    for (i = id_prim_lig; i < (sistema->num_ligacoes - num_ligs); i++) {
        sistema->ligacoes[i] = sistema->ligacoes[i + num_ligs];
    }
    sistema->num_ligacoes -= num_ligs;
    sistema->ligacoes = realloc(sistema->ligacoes, 
    sizeof(Ligacao)*sistema->num_ligacoes);
}  

/* Função altera_num_carreiras (função auxiliar a remover carreira) recebe o 
endereço da variável sistema e o id de uma carreira e altera o número de carreiras
(diminui) de todas as paragens que estavam na carreira que vai ser removida */
void altera_num_carreiras(Geral *sistema, int id_c) {
    /* id_par_falta - salva o id da paragem que falta analisar
    pars_carr_num - número de elementos do vetor paragens_carr */
    int i, id_par_falta = 0, pars_carr_num = 0;
    /* vetor que contem os ids das paragens cujo número de carreiras
    deve ser alterado */
    int *paragens_carr = safe_malloc(sizeof(int));
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (sistema->ligacoes[i].id_carreira == id_c) {
            /* se a paragem já está no vetor de paragens cujo número de 
            carreiras vai ser alterado */
            if (!elemento_vetor(paragens_carr, pars_carr_num,
            sistema->ligacoes[i].id_primeira)) {
                (sistema->paragens[sistema->ligacoes[i].id_primeira]
                .num_carreiras)--;
                id_par_falta = sistema->ligacoes[i].id_ultima;
                paragens_carr = safe_realloc(paragens_carr, 
                sizeof(int)*(pars_carr_num + 1));
                paragens_carr[pars_carr_num] =sistema->ligacoes[i].id_primeira;
                pars_carr_num++;
            }
        }
    }
    if (!elemento_vetor(paragens_carr, pars_carr_num,
        id_par_falta)) {
        (sistema->paragens[id_par_falta].num_carreiras)--;
    }
    free(paragens_carr);
}

/* Função elemento_vetor (função auxiliar a altera_num_carreiras) recebe um 
vetor, o tamanho do mesmo, e um possível elemento da mesma e verifica se esse 
elemento já está no mesmo ou não */
int elemento_vetor(int vetor[], int tamanho, int elemento) {
    int i;
    for (i = 0; i < tamanho; i++) {
        if (vetor[i] == elemento) {
            return TRUE; 
        }
    }
    return FALSE; 
}
