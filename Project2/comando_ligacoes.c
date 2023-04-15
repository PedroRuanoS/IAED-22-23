/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_ligacoes

*/

#include "project2.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* TODAS AS FUNÇÕES DESTE DOCUMENTO (EXCETO mover_ligacoes, alterar_paragens,
par_ligada_carr e alter_n_carrs_par): recebem o endereço da variável sistema
e um ponteiro para um ponteiro (que é uma lista de ponteiros, cujo cada 
ponteiro aponta para um elemento/palavra do comando) - resto das variáveis
recebidas estarão na descrição de cada função */

/* Função erros_ligacao verifica se o comando tem um erro, se sim,
imprime o erro e devolve TRUE */
int erros_ligacao(Geral *sistema, char **comando) {
    if (id_carreira(sistema, comando[0]) < 0) {
        printf("%s: %s\n", comando[0], ERRO_CARREIRA_NAO_EXISTE);
        return TRUE;
    } else if (erro_nao_existe_par(sistema, comando) == 1) {
        printf("%s: %s\n", comando[1], ERRO_LIG_PARAGEM_NAO_EXISTE);
        return TRUE;
    } else if (erro_nao_existe_par(sistema, comando) == 2) {
        printf("%s: %s\n", comando[2], ERRO_LIG_PARAGEM_NAO_EXISTE);
        return TRUE;
    }
    if (atof(comando[3]) < 0 || atof(comando[4]) < 0) {
        printf("%s\n", ERRO_VALOR_NEGATIVO);
        return TRUE;
    }
    if (erro_paragem_extremo(sistema, comando)) {
        printf("%s\n", ERRO_N_EXTREM);
        return TRUE;
    }
    return FALSE;
}

/* Função erro_nao_existe_par (função auxiliar à função erros ligacao) 
verifica se existe uma paragem com o nome da primeira paragem do comando
e uma com o nome da segunda paragem do comando */
int erro_nao_existe_par(Geral *sistema, char **comando) {
    /* pri_par_dif - se todas as paragens são diferentes da 
    primeira paragem do comando, seg_par_dif - se todas as paragens são 
    diferentes da segunda paragem do comando*/
    int pri_par_dif = TRUE, seg_par_dif = TRUE;
    if (id_paragem(sistema, comando[1]) >= 0) {
        pri_par_dif = FALSE;
    }
    if (id_paragem(sistema, comando[2]) >= 0) {
        seg_par_dif = FALSE;
    }
    if ((pri_par_dif && !seg_par_dif) || (pri_par_dif && seg_par_dif)) {
        return 1; 
    } else if (seg_par_dif && !pri_par_dif) {
        return 2;
    }
    return FALSE;
}

/* Função erro_paragem_extremo (função auxiliar à função erros ligacao) 
verifica se uma das paragens do comando é um dos extremos da carreira do
comando */
int erro_paragem_extremo(Geral *sistema, char **comando) {
    /* id_carr - indice da carreira no vetor de carreiras do sistema, 
    id_prim_par - indice da primeira paragem no vetor de paragens do sistema, 
    id_seg_par - indice da segunda paragem no vetor de paragens do sistema */
    int id_carr, id_prim_par, id_seg_par;
    id_carr = id_carreira(sistema, comando[0]);
    id_prim_par = id_paragem(sistema, comando[1]);
    id_seg_par = id_paragem(sistema, comando[2]);
    if (sistema->carreiras[id_carr].id_primeira == 0 && 
    sistema->carreiras[id_carr].id_ultima == 0) {
        return FALSE;
    } else if (id_prim_par != sistema->carreiras[id_carr].id_ultima && 
    id_seg_par != sistema->carreiras[id_carr].id_primeira) {
        return TRUE;
    }
    return FALSE;

}

/* Função iniciar_ligacao cria a ligacao do comando, alterando as paragens do
comando e a carreira do comando */
void iniciar_ligacao(Geral *sistema, char **comando) {
    /* vetor com os indices das paragens e da carreira do comando nos vetores
    do sistema */
    int ids[3] = {0};
    /* se a paragem se vai adicionar no inicio da carreira ou não (fim) */
    int inicio = TRUE;
    ids[0] = id_carreira(sistema, comando[0]);
    ids[1] = id_paragem(sistema, comando[1]);
    ids[2] = id_paragem(sistema, comando[2]);
    alterar_paragens(sistema, ids); 
    /* se carreira não tem paragens, não é necessário posicionar a ligação */
    if (sistema->carreiras[ids[0]].num_paragens == 0) {
        criar_ligacao(sistema, comando, ids, sistema->num_ligacoes);
    } else {
        posicionar_ligac(sistema, comando, ids, &inicio);                                      
    } 
    alterar_carreira(sistema, ids, inicio, comando);
}

/* Função posicionar_ligac (função auxiliar à função iniciar_ligacao) recebe um
vetor com ids e o endereço da variável inicio (iniciar_ligacao) e cria a
ligação do comando, posicionando-a no sitio certo do vetor das ligações do
sistema, movendo o resto das ligações, de forma às paragens da mesma carreira
estarem juntas e por ordem */
void posicionar_ligac(Geral *sistema, char **comando, int ids[], int *inicio) {
    /* posicao - indice futuro da ligacao no vetor das ligacoes do sistema */
    int i, posicao = 0;
    /* se a primeira paragem do comando for a ultima da carreira, a ligação
    será inserida no fim da carreira, independentemente da outra paragem do
    comando for a primeira da carreira ou não */
    if (sistema->carreiras[ids[0]].id_ultima == ids[1]) {
        *inicio = FALSE;
    }
    if (*inicio) {
        for (i = 0; i < sistema->num_ligacoes; i++) {
            if (ids[0] == sistema->ligacoes[i].id_carreira) {
                posicao = i;
                break;
            }
        }
    } else if (!(*inicio)) {
        for (i = (sistema->num_ligacoes - 1); i >= 0; i--) {
            if (ids[0] == sistema->ligacoes[i].id_carreira) {
                posicao = i + 1;
                break;
            }
        }
    }
    mover_ligacoes(sistema, posicao);
    criar_ligacao(sistema, comando, ids, posicao);
}


/* Função mover_ligacoes (função auxiliar à função posicionar_ligac) recebe
o indice da ligacao a adicionar ao vetor ligacoes do sistema e move todas os
elementos deste vetor uma posição para a frente de forma a colocar a ligação
na posição posicao */
void mover_ligacoes(Geral *sistema, int posicao) {
    int i;
    for (i = (sistema->num_ligacoes - 1); i >= posicao; i--) {
        sistema->ligacoes[i+1] = sistema->ligacoes[i];
    }
}

/* Função criar_ligacao (função auxiliar à função posicionar_ligac e 
iniciar_ligacao) recebe os ids (posicionar_ligac/iniciar_ligacao) e
a posicao (mesma que mover_ligacoes recebe) e cria a ligacao */
void criar_ligacao(Geral *sistema, char **comando, int ids[], int posicao) {
    /* + 2 (no realloc) para o caso de, na proxima adição de ligação, se tiver
    de mover as ligações da lista */
    sistema->ligacoes = (Ligacao*)safe_realloc(sistema->ligacoes, 
    sizeof(Ligacao)*(sistema->num_ligacoes + 2));
    sistema->ligacoes[posicao].id_carreira = ids[0];
    sistema->ligacoes[posicao].id_primeira = ids[1];
    sistema->ligacoes[posicao].id_ultima = ids[2];
    sistema->ligacoes[posicao].custo = atof(comando[3]);
    sistema->ligacoes[posicao].duracao = atof(comando[4]);
    (sistema->num_ligacoes)++;    
}

/* Função alterar_carreira (função auxiliar à função posicionar_ligac e 
iniciar_ligacao) recebe os ids e o inteiro inicio (inicar_ligacao)
e altera a carreira do comando */
void alterar_carreira(Geral *sistema, int ids[], int inicio, char **comando) {
    /* valor a adicionar ao custo total (custo da ligação) */
    double custo = atof(comando[3]);
    /* valor a adicionar à duração total (duração da ligação) */
    double duracao = atof(comando[4]);
    /* número de paragens a adicionar ao total de paragens */
    int num_par_adicionar;
    if (sistema->carreiras[ids[0]].num_paragens == 0) {
        sistema->carreiras[ids[0]].id_primeira = ids[1];
        sistema->carreiras[ids[0]].id_ultima = ids[2];
        num_par_adicionar = 2;
    } else if (inicio) {
        sistema->carreiras[ids[0]].id_primeira = ids[1];
        num_par_adicionar = 1;
    } else if (!inicio) {
        sistema->carreiras[ids[0]].id_ultima = ids[2];
        num_par_adicionar = 1;
    }
    sistema->carreiras[ids[0]].custo_total += custo;
    sistema->carreiras[ids[0]].duracao_total += duracao;
    sistema->carreiras[ids[0]].num_paragens += num_par_adicionar;
}

/* Função alterar_carreira (função auxiliar à função posicionar_ligac e 
iniciar_ligacao) recebe os ids (inicar_ligacao) e altera as paragens do 
comando */
void alterar_paragens(Geral *sistema, int ids[]) {
    /* par1_alt - se a primeira paragem do comando já está
    em alguma ligação com a carreira do comando, par2_alt - se a 
    segunda paragem do comando já está em alguma ligação com a carreira do
    comando*/
    int i, par1_alt = FALSE, par2_alt = FALSE; 
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (par_ligada_carr(sistema, i, ids[0], ids[1])) {
            par1_alt = TRUE;
        }
        if (par_ligada_carr(sistema, i, ids[0], ids[2])) {
            par2_alt = TRUE;
        }
    }
    alter_n_carrs_par(sistema, ids, par1_alt, par2_alt);
}

/* Função par_ligada_carr (função auxiliar à função alterar_paragens) recebe
o indice da ligação, no vetor das ligações do sistema, ids de uma carreira e 
uma paragem e devolve TRUE se e a paragem já está em alguma ligação com a 
carreira */
int par_ligada_carr(Geral *sistema, int indice_lig, int id_c, int id_p) {
    if ((sistema->ligacoes[indice_lig].id_carreira == id_c) &&
        (sistema->ligacoes[indice_lig].id_primeira == id_p ||
        sistema->ligacoes[indice_lig].id_ultima == id_p)) {
        return TRUE;
    } else {
        return FALSE;
    }
} 

/* Função alter_n_carrs_par (função auxiliar à função alterar_paragens) recebe
os ids (alterar paragem) e se a a primeira ou segunda paragem já foram alteradas
antes (par1_alt e par2_alt) e altera o número de carreiras nas paragens a 
alterar */
void alter_n_carrs_par(Geral *sistema, int ids[], int par1_alt, int par2_alt) {
    if (ids[1] == ids[2] && !par1_alt) {
        (sistema->paragens[ids[1]].num_carreiras)++;
    } else if (ids[1] != ids[2]) {
        if (par1_alt && !par2_alt) {
            (sistema->paragens[ids[2]].num_carreiras)++;
        } else if (par2_alt && !par1_alt) {
            (sistema->paragens[ids[1]].num_carreiras)++;
        } else if (!par1_alt && !par2_alt) {
            (sistema->paragens[ids[2]].num_carreiras)++;
            (sistema->paragens[ids[1]].num_carreiras)++;
        }
    }
}