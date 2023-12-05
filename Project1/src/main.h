/* 

    Pedro Silveira 106642
    Header file - declaração de macros, estruturas e protótipos

*/

#ifndef PROJECT1_H
#define PROJECT1_H

#define MAX_NOME_CARREIRA 21    /*dimensão max do nome de uma carreira*/
#define MAX_NOME_PARAGEM 51     /*dimensão max do nome de uma paragem*/
#define MAX_CARREIRAS 200       /*número max de carreiras*/
#define MAX_PARAGENS 10000      /*número max de paragens*/
#define MAX_LIGACOES 30000      /*número max de ligacoes*/
#define CARREIRAS_ARGUMENTOS 2  /*dimensão vetor com argumentos do comando c*/
#define PARAGENS_ARGUMENTOS 3   /*dimensão vetor com argumentos do comando p*/
#define LIGACOES_ARGUMENTOS 5   /*dimensão vetor com argumentos do comando l*/
#define TAMANHO_INVERSO 8       /*dimensão vetor com argumentos do comando i*/
#define INVERSO "inverso"       /*palavra inverso - c*/

/* ERROS */
#define ERRO_INVERSO "incorrect sort option." /*inverso formato incorreto*/  
#define ERRO_PARAGEM_NAO_EXISTE "no such stop." /*paragem não existe*/
#define ERRO_PARAGEM_JA_EXISTE "stop already exists."/*paragem já existe*/
#define ERRO_CARREIRA_NAO_EXISTE "no such line."/*carreira não existe*/
#define ERRO_LIG_PARAGEM_NAO_EXISTE "no such stop."/*paragem não existe - l*/
#define ERRO_N_EXTREM "link cannot be associated with bus line."/*não extremo*/
#define ERRO_VALOR_NEGATIVO "negative cost or duration."/*valor negativo*/

#define TRUE 1 /*verdadeiro*/
#define FALSE 0 /*falso*/

/* TIPOS DE DADOS */
typedef struct {
    char nome[MAX_NOME_PARAGEM];
    double latitude;
    double longitude;
    short num_carreiras;
} Paragem;

typedef struct {
    char nome[MAX_NOME_CARREIRA];
    double custo_total;
    double duracao_total;
    short num_paragens;
    Paragem *primeira;
    Paragem *ultima;
} Carreira;

typedef struct {
    Carreira *carreira;
    Paragem *primeira;
    Paragem *ultima;
    double custo;
    double duracao;
} Ligacao;
  
typedef struct {
    Carreira carreiras[MAX_CARREIRAS];
    short num_carreiras;
    Paragem paragens[MAX_PARAGENS];
    short num_paragens;
    Ligacao ligacoes[MAX_LIGACOES];
    short num_ligacoes;
} Geral;

/* FUNÇÕES */

/* PRINCIPAIS */
void zerador_sistema(Geral *sistema);

int acionador_comandos(Geral *sistema);

void comando_carreiras(Geral *sistema);

void comando_paragens(Geral *sistema);

void comando_ligacoes(Geral *sistema);

void comando_intersecoes(Geral *sistema);

void input_to_vetor(char** comando, int* num_pal_comando);

/* COMANDO CARREIRAS */
void listar_carreiras(Geral *sistema);

void listar_paragens_carr(Geral *sistema, char nome_carreira[], int inverso);

void criar_carreira(Geral *sistema, char nome_carreira[]);

int inverso_valido(char palavra[]);

/* COMANDO PARAGENS */
void listar_paragens(Geral *sistema);

void listar_coords_paragem(Geral *sistema, char nome_paragem[]);

void criar_parag(Geral *sistema, char nome_paragem[], double lat, double lon);

/* COMANDO LIGAÇÕES */
int erros_ligacao(Geral *sistema, char **comando);

int erro_nao_existe_carr(Geral *sistema, char** comando);

int erro_nao_existe_par(Geral *sistema, char** comando);

int erro_paragem_extremo(Geral *sistema, char **comando);

void iniciar_ligacao(Geral *sistema, char **comando);

int id_carreira(Geral *sistema, char nome_carreira[]);

int id_paragem(Geral *sistema, char nome_paragem[]);

void posicionar_ligac(Geral *sistema, char **comando, int ids[], int *inicio);

void mover_ligacoes(Geral *sistema, int posicao);

void criar_ligacao(Geral *sistema, char **comando, int ids[], int posicao);

void alterar_carreira(Geral *sistema, int ids[], int inicio, char **comando);

void alterar_paragens(Geral *sistema, int ids[], char **comando);

int par_ligada_carr(Geral *sistema, int indice_lig, char carr[], char par[]);

/* COMANDO INTERSEÇÕES */
void listar_carrs_paragem(Geral *sistema, char nome_paragem[]);

void sort_print_carrs(char carrs_paragem[][MAX_NOME_CARREIRA], int num_carrs);

#endif
