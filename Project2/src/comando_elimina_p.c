/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_elimina_p

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Função remover_paragem recebe o endereço da variável sistema e o id de uma
paragem, e remove essa paragem do vetor das paragens */
void remover_paragem(Geral *sistema, int id_p) {
    int i;
    free(sistema->paragens[id_p].nome);
    for (i = id_p; i < (sistema->num_paragens - 1); i++) {
        sistema->paragens[i] = sistema->paragens[i + 1];
    }
    (sistema->num_paragens)--;
    sistema->paragens = realloc(sistema->paragens,
    sizeof(Paragem)*sistema->num_paragens);
}

/* Função remover_ligacoes_p recebe o endereço da variável sistema, o id de uma
ligação e o id de uma paragem, e altera a carreira da mesma ligação de acordo
com a posição da mesma paragem na carreira */
void remover_ligacoes_p(Geral *sistema, int id_l, int id_p) {
    /* cheg_fim - se existe alguma ligcao cuja primeira ou ultima paragem
    é diferente da ultima paragem da ligação com id id_l (ou seja, id_p) 
    (TRUE - não existe, pois chegou ao fim) */
    int i, cheg_fim = TRUE;
    if (extremo_carreira(sistema, id_l, id_p)) {
        percurso_encurtado(sistema, id_l, id_p);
    } else {
        /* procurar nova paragem para subtituir a que se quer remover na ligação
        id_l */
        for (i = (id_l + 1); i < sistema->num_ligacoes; i++) {
            if (sistema->ligacoes[i].id_primeira != 
            sistema->ligacoes[id_l].id_ultima) {
                sistema->ligacoes[id_l].id_ultima = 
                sistema->ligacoes[i].id_primeira;
                cheg_fim = FALSE;
                break;
            }
            if (sistema->ligacoes[i].id_ultima != 
            sistema->ligacoes[id_l].id_ultima) {
                sistema->ligacoes[id_l].id_ultima = 
                sistema->ligacoes[i].id_ultima;
                cheg_fim = FALSE;
                break;
            }         
        }
        if (cheg_fim) {
            substitui_ligacao(sistema, id_l, FALSE, i);
        } else {
            substitui_ligacao(sistema, id_l, TRUE, i);
        }
    }
}

/* Função extremo_carreira (função auxiliar a remover_ligacoes_p) recebe o
endereço da variável sistema, um id de uma ligação e o id de uma paragem e 
verifica se a ligação é a primeira ou ultima da carreira e se a paragem
é a primeira ou ultima da carreira */
int extremo_carreira(Geral *sistema, int id_l, int id_p) {
    if ((sistema->ligacoes[id_l].id_primeira == id_p && (id_l == 0 || 
    sistema->ligacoes[id_l].id_carreira != 
    sistema->ligacoes[id_l - 1].id_carreira)) || 
    (sistema->ligacoes[id_l].id_ultima == id_p && (id_l == 
    (sistema->num_ligacoes - 1) || 
    sistema->ligacoes[id_l].id_carreira != 
    sistema->ligacoes[id_l + 1].id_carreira))) {
        return TRUE;
    }
    return FALSE;
}

/* Função percurso_encurtado (função auxiliar a remover_ligacoes_p) recebe o
endereço da variável sistema, um id de uma ligação e o id de uma paragem e retira
a ligação do vetor de ligações, alterando a carreira da mesma ligação */
void percurso_encurtado(Geral *sistema, int id_l, int id_p) {
    int i;
    altera_par_car(sistema, id_l, id_p);
    (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
    num_paragens)--;
    if (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
    num_paragens == 1) {
        (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
        num_paragens)--;
    }
    sistema->carreiras[sistema->ligacoes[id_l].id_carreira].custo_total -= 
    sistema->ligacoes[id_l].custo;
    sistema->carreiras[sistema->ligacoes[id_l].id_carreira].duracao_total -= 
    sistema->ligacoes[id_l].duracao;
    for (i = id_l; i < (sistema->num_ligacoes - 1); i++) {
        sistema->ligacoes[i] = sistema->ligacoes[i + 1];
    }
    (sistema->num_ligacoes)--;
    sistema->ligacoes = realloc(sistema->ligacoes,
    sizeof(Ligacao)*sistema->num_ligacoes);
}

/* Função substitui_ligacao (função auxiliar a remover_ligacoes_p) recebe o
endereço da variável sistema, um id de uma ligação, se foi encontrada alguma
ligação nova e o índice desta mesma nova ligação */
void substitui_ligacao(Geral *sistema, int id_l, int found, int indice_lig) {
    /* num_ligs - número de ligações a remover do vetor de ligações */
    int num_ligs, i; 
    if (found) {
        /* Se foi encontrada altera-se id_l e remove-se todas as ligações entre
        id_l e indice_lig (incluindo indice_lig) */
        num_ligs = (indice_lig - id_l);
        sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
        num_paragens -= num_ligs;
        sistema->ligacoes[id_l].custo += sistema->ligacoes[indice_lig].custo;
        sistema->ligacoes[id_l].duracao += sistema->ligacoes[indice_lig]
        .duracao;
        for (i = (id_l + 1); i < (sistema->num_ligacoes - num_ligs); i++) {
            sistema->ligacoes[i] = sistema->ligacoes[i + num_ligs];
        }
        sistema->num_ligacoes -= num_ligs;
        sistema->ligacoes = realloc(sistema->ligacoes, 
        sizeof(Ligacao)*sistema->num_ligacoes);
    } else {
        /* Se não foi encontrada remove-se todas as ligações desta carreira 
        que estão para a frente de id_l */
        sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
        num_paragens -= (sistema->num_ligacoes - (id_l + 1));
        if (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
        num_paragens == 1) {
            (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].
            num_paragens)--;
        }
        sistema->num_ligacoes -= (sistema->num_ligacoes - (id_l + 1));
        sistema->ligacoes = realloc(sistema->ligacoes, 
        sizeof(Ligacao)*id_l);
    }
}

/* Função altera_par_car (função auxiliar a percurso_encurtado) recebe o
endereço da variável sistema, um id de uma ligação e o id de uma paragem e 
altera o id da primeira ou ultima paragem da carreira de id_l */
void altera_par_car(Geral *sistema, int id_l, int id_p) {
    if (sistema->carreiras[sistema->ligacoes[id_l].id_carreira].id_primeira
    == id_p) {
        sistema->carreiras[sistema->ligacoes[id_l].id_carreira].id_primeira
        = sistema->ligacoes[id_l].id_ultima;
    } else if (sistema->carreiras[sistema->ligacoes[id_l].id_carreira]
    .id_ultima == id_p) {
        sistema->carreiras[sistema->ligacoes[id_l].id_carreira].id_ultima
        = sistema->ligacoes[id_l].id_primeira;
    }
}

/* Função alterar_lig_carr recebe o endereço da variável sistema e o id de uma
paragem e atualiza os ids das paragens que foram movidas no vetor paragens nos
vetores de ligações e carreiras */
void alterar_lig_carr(Geral *sistema, int id_p) {
    int i;
    /* todas as paragens que estavam depois da removida diminuiram o id em 1 */
    for (i = 0; i < sistema->num_ligacoes; i++) {
            if (sistema->ligacoes[i].id_primeira > id_p) {
                sistema->ligacoes[i].id_primeira--;
            }
            if (sistema->ligacoes[i].id_ultima > id_p) {
                sistema->ligacoes[i].id_ultima--;
            }
        }
    for (i = 0; i < sistema->num_carreiras; i++) {
        if (sistema->carreiras[i].id_primeira > id_p) {
            sistema->carreiras[i].id_primeira--;
        }
        if (sistema->carreiras[i].id_ultima > id_p) {
            sistema->carreiras[i].id_ultima--;
        }
    }
}