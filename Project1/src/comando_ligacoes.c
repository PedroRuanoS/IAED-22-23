/* 

    Pedro Silveira 106642
    C file - funções auxiliares à função comando_ligacoes

*/

#include "project1.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* TODAS AS FUNÇÕES DESTE DOCUMENTO (EXCETO mover_ligacoes, par_ligada_carr,
id_carreira e id_paragem): recebe o endereço da variável sistema e um ponteiro 
para um ponteiro (que é uma lista de ponteiros, cujo cada ponteiro aponta para
um elemento/palavra do comando) */

/* Função erros_ligacao verifica se o comando tem um erro, se sim,
imprime o erro e devolve TRUE */
int erros_ligacao(Geral *sistema, char **comando) {
    if (erro_nao_existe_carr(sistema, comando)) {
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

/* Função erro_nao_existe_carr (função auxiliar à função erros ligacao)
verifica se existe uma carreira com o nome da carreira do comando */
int erro_nao_existe_carr(Geral *sistema, char **comando) {
    /* carr_dif - se todas as carreiras são diferentes da do comando*/
    int i, carr_dif = TRUE;
    for (i = 0; i < sistema->num_carreiras; i++) {
        if (!strcmp(comando[0], sistema->carreiras[i].nome)) {
            carr_dif = FALSE;
        }
    }
    if (carr_dif) {
        return TRUE;
    }
    return FALSE;
}

/* Função erro_nao_existe_par (função auxiliar à função erros ligacao) 
verifica se existe uma paragem com o nome da primeira paragem do comando
e uma com o nome da segunda paragem do comando */
int erro_nao_existe_par(Geral *sistema, char **comando) {
    /* pri_par_dif - se todas as paragens são diferentes da 
    primeira paragem do comando, pri_par_dif - se todas as paragens são 
    diferentes da segunda paragem do comando*/
    int i, pri_par_dif = TRUE, seg_par_dif = TRUE;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (!strcmp(comando[1], sistema->paragens[i].nome)) {
            pri_par_dif = FALSE;
        }
        if (!strcmp(comando[2], sistema->paragens[i].nome)) {
            seg_par_dif = FALSE;
        }
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
    /* indice da carreira no vetor de carreiras do sistema */
    int id_carr;
    id_carr = id_carreira(sistema, comando);
    if (sistema->carreiras[id_carr].primeira == NULL && 
    sistema->carreiras[id_carr].ultima == NULL) {
        return FALSE;
    } else if (strcmp(comando[1], sistema->carreiras[id_carr].ultima->nome) && 
    strcmp(comando[2], sistema->carreiras[id_carr].primeira->nome)) {
        return TRUE;
    }
    return FALSE;

}

/* Função iniciar_ligacao cria a ligacao do comando, 
alterando as paragens do comando e a carreira do comando */
void iniciar_ligacao(Geral *sistema, char **comando) {
    /* vetor com os indices das paragens e da carreira do comando nos vetores
    do sistema */
    int ids[3] = {0};
    /* se a paragem se vai adicionar no inicio da carreira ou não (fim) */
    int inicio = TRUE;
    ids[0] = id_carreira(sistema, comando[0]);
    ids[1] = id_paragem(sistema, comando[1]);
    ids[2] = id_paragem(sistema, comando[2]);
    if (sistema->num_ligacoes == MAX_LIGACOES) {
        return;
    }
    alterar_paragens(sistema, ids, comando); 
    /* se carreira não tem paragem, não é necessário posicionar a ligação */
    if (sistema->carreiras[ids[0]].primeira == NULL && 
    sistema->carreiras[ids[0]].ultima == NULL) {
        criar_ligacao(sistema, comando, ids, sistema->num_ligacoes);
    } else {
        posicionar_ligac(sistema, comando, ids, &inicio);                                      
    } 
    alterar_carreira(sistema, ids, inicio, comando);
}

/* Função id_carreira (função auxiliar às funções erro_paragem_extremo e 
iniciar_ligacao) recebe um nome de carreira devolve o indice da carreira do
comando no vetor de carreiras do sistema */
int id_carreira(Geral *sistema, char nome_carreira[]) {
    int i;
    for (i = 0; i < sistema->num_carreiras; i++) {
        if (!strcmp(nome_carreira, sistema->carreiras[i].nome)) {
            return i;
        }
    }
    return i;
}

/* Função id_paragem (função auxiliar à função iniciar_ligacao) recebe 
um nome de carreira e devolve o indice da paragem do comando no vetor
de paragens do sistema */
int id_paragem(Geral *sistema, char nome_paragem[]) {
    int i;
    for (i = 0; i < sistema->num_paragens; i++) {
        if (!strcmp(nome_paragem, sistema->paragens[i].nome)) {
            return i;
        }
    }
    return i;
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
    if (!strcmp(sistema->carreiras[ids[0]].ultima->nome, comando[1])) {
        *inicio = FALSE;
    }
    if (*inicio) {
        for (i = 0; i < sistema->num_ligacoes; i++) {
            if (!strcmp(comando[0], sistema->ligacoes[i].carreira->nome)) {
                posicao = i;
                break;
            }
        }
    } else if (!(*inicio)) {
        for (i = (sistema->num_ligacoes - 1); i >= 0; i--) {
            if (!strcmp(comando[0], sistema->ligacoes[i].carreira->nome)) {
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
    for (i = sistema->num_ligacoes; i >= posicao; i--) {
        sistema->ligacoes[i+1] = sistema->ligacoes[i];
    }
}

/* Função criar_ligacao (função auxiliar à função posicionar_ligac e 
iniciar_ligacao) recebe os ids (posicionar_ligac/iniciar_ligacao) e
a posicao (mesma que mover_ligacoes recebe) e cria a ligacao */
void criar_ligacao(Geral *sistema, char **comando, int ids[], int posicao) {
    sistema->ligacoes[posicao].carreira = &sistema->carreiras[ids[0]];
    sistema->ligacoes[posicao].primeira = &sistema->paragens[ids[1]];
    sistema->ligacoes[posicao].ultima = &sistema->paragens[ids[2]];
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
    if (sistema->carreiras[ids[0]].primeira == NULL && 
    sistema->carreiras[ids[0]].ultima == NULL) {
        sistema->carreiras[ids[0]].primeira = &sistema->paragens[ids[1]];
        sistema->carreiras[ids[0]].ultima = &sistema->paragens[ids[2]];
        num_par_adicionar = 2;
    } else if (inicio) {
        sistema->carreiras[ids[0]].primeira = &sistema->paragens[ids[1]];
        num_par_adicionar = 1;
    } else if (!inicio) {
        sistema->carreiras[ids[0]].ultima = &sistema->paragens[ids[2]];
        num_par_adicionar = 1;
    }
    sistema->carreiras[ids[0]].custo_total += custo;
    sistema->carreiras[ids[0]].duracao_total += duracao;
    sistema->carreiras[ids[0]].num_paragens += num_par_adicionar;
} 

/* Função alterar_carreira (função auxiliar à função posicionar_ligac e 
iniciar_ligacao) recebe os ids (inicar_ligacao) e altera as paragens do 
comando */
void alterar_paragens(Geral *sistema, int ids[], char **comando) {
    /* paragem1_ja_alterada - se a primeira paragem do comando já está
    em alguma ligação com a carreira do comando, paragem2_ja_alterada - se a 
    segunda paragem do comando já está em alguma ligação com a carreira do
    comando*/
    int i, paragem1_ja_alterada = FALSE, paragem2_ja_alterada = FALSE; 
    for (i = 0; i < sistema->num_ligacoes; i++) {
        if (par_ligada_carr(sistema, i, comando[0], comando[1])) {
            paragem1_ja_alterada = TRUE;
        }
        if (par_ligada_carr(sistema, i, comando[0], comando[2])) {
            paragem2_ja_alterada = TRUE;
        }
    }
    if (paragem1_ja_alterada && !paragem2_ja_alterada) {
        (sistema->paragens[ids[2]].num_carreiras)++;
    } else if (paragem2_ja_alterada && !paragem1_ja_alterada) {
        (sistema->paragens[ids[1]].num_carreiras)++;
    } else if (!paragem1_ja_alterada && !paragem2_ja_alterada) {
        (sistema->paragens[ids[2]].num_carreiras)++;
        (sistema->paragens[ids[1]].num_carreiras)++;
    }
}

/* Função par_ligada_carr (função auxiliar à função alterar_paragens) recebe
o indice da ligação, no vetor das ligações do sistema, uma carreira e uma 
paragem e devolve TRUE se e a paragem já está em alguma ligação com a 
carreira */
int par_ligada_carr(Geral *sistema, int indice_lig, char carr[], char par[]) {
    if (!strcmp(sistema->ligacoes[indice_lig].carreira->nome, carr) &&
        (!strcmp(sistema->ligacoes[indice_lig].primeira->nome, par) ||
        !strcmp(sistema->ligacoes[indice_lig].ultima->nome, par))) {
        return TRUE;
    } else {
        return FALSE;
    }
}


