/* 

    Pedro Silveira 106642
    Header file - declaração de macros, estruturas e protótipos

*/

#ifndef PROJECT2_H
#define PROJECT2_H

#define CARREIRAS_ARGUMENTOS 2  /*dimensão vetor com argumentos comando c*/
#define PARAGENS_ARGUMENTOS 3   /*dimensão vetor com argumentos comando p*/
#define LIGACOES_ARGUMENTOS 5   /*dimensão vetor com argumentos comando l*/
#define REMOVE_ARGUMENTOS 1     /*dimensão vetor com argumentos comando e e r*/
#define TAMANHO_INVERSO 8       /*dimensão vetor com argumentos comando i*/
#define INVERSO "inverso"       /*palavra inverso - c*/

/* ERROS */
#define ERRO_INVERSO "incorrect sort option." /*inverso formato incorreto*/  
#define ERRO_PARAGEM_NAO_EXISTE "no such stop." /*paragem não existe*/
#define ERRO_PARAGEM_JA_EXISTE "stop already exists."/*paragem já existe*/
#define ERRO_CARREIRA_NAO_EXISTE "no such line."/*carreira não existe*/
#define ERRO_LIG_PARAGEM_NAO_EXISTE "no such stop."/*paragem não existe - l*/
#define ERRO_N_EXTREM "link cannot be associated with bus line."/*não extremo*/
#define ERRO_VALOR_NEGATIVO "negative cost or duration."/*valor negativo*/
#define ERRO_NAO_HA_MEMORIA "No memory." /*memória esgotada*/

#define TRUE 1 /*verdadeiro*/
#define FALSE 0 /*falso*/
#define ID_NAO_EXISTE -1 /*id de carreira ou paragem não encontrado*/


/* TIPOS DE DADOS */

typedef struct {
    char *nome;
    double latitude;
    double longitude;
    int num_carreiras;
} Paragem;

typedef struct {
    char *nome;
    double custo_total;
    double duracao_total;
    int num_paragens, id_primeira, id_ultima;
} Carreira;

typedef struct {
    int id_carreira, id_primeira, id_ultima;
    double custo;
    double duracao;
} Ligacao;
  
typedef struct {
    Carreira *carreiras;
    int num_carreiras;
    Paragem *paragens;
    int num_paragens;
    Ligacao *ligacoes;
    int num_ligacoes;
} Geral;

typedef unsigned long var_size;

/* FUNCOES */

/* PRINCIPAIS */
int acionador_comandos(Geral *sistema);

void free_sistema(Geral *sistema);

void* safe_malloc(var_size tamanho);

void* safe_realloc(void *ptr, var_size novo_tamanho);

void free_comando(char **comando, int num_pal_comando);

void input_to_vetor(char ***comando, int *num_pal_comando);

int id_carreira(Geral *sistema, char nome_carreira[]);

int id_paragem(Geral *sistema, char nome_paragem[]);

void comando_carreiras(Geral *sistema);

void comando_paragens(Geral *sistema);

void comando_ligacoes(Geral *sistema);

void comando_intersecoes(Geral *sistema);

void comando_apaga(Geral *sistema);

void comando_remove_c(Geral *sistema);

void comando_elimina_p(Geral *sistema);

/* COMANDO CARREIRAS */
void listar_carreiras(Geral *sistema);

void listar_paragens_carr(Geral *sistema, char nome_carreira[], int inverso);

void criar_carreira(Geral *sistema, char nome_carreira[]);

int inverso_valido(char palavra[]);

/* COMANDO PARAGENS */
void listar_paragens(Geral *sistema);

void listar_coords_paragem(Geral *sistema, char nome_paragem[]);

void criar_parag(Geral *sistema, char nome_paragem[], double lat, double lon);

/* COMANDO LIGACOES */
int erros_ligacao(Geral *sistema, char **comando);

int erro_nao_existe_par(Geral *sistema, char** comando);

int erro_paragem_extremo(Geral *sistema, char **comando);

void iniciar_ligacao(Geral *sistema, char **comando);

void posicionar_ligac(Geral *sistema, char **comando, int ids[], int *inicio);

void mover_ligacoes(Geral *sistema, int posicao);

void criar_ligacao(Geral *sistema, char **comando, int ids[], int posicao);

void alterar_carreira(Geral *sistema, int ids[], int inicio, char **comando);

void alterar_paragens(Geral *sistema, int ids[]);

int par_ligada_carr(Geral *sistema, int indice_lig, int id_c, int id_p);

void alter_n_carrs_par(Geral *sistema, int ids[], int par1_alt, int par2_alt);

/* COMANDO INTERSECOES */
void listar_carrs_paragem(Geral *sistema, char nome_paragem[]);

void sort_print_carrs(Geral *sistema, int *carrs_paragem, int num_carrs);

/* COMANDO REMOVER CARREIRAS */
void remover_carreira(Geral *sistema, int id_c);

void remover_ligacoes_c(Geral *sistema, int id_c);

void altera_num_carreiras(Geral *sistema, int id_c);

int elemento_vetor(int lista[], int tamanho, int elemento);

/* COMANDO ELIMINAR PARAGENS */
void remover_paragem(Geral *sistema, int id_p);

void remover_ligacoes_p(Geral *sistema, int id_l, int id_p);

int extremo_carreira(Geral *sistema, int id_l, int id_p);

void percurso_encurtado(Geral *sistema, int id_l, int id_p);

void substitui_ligacao(Geral *sistema, int id_l, int found, int indice_lig);

void altera_par_car(Geral *sistema, int id_l, int id_p);

void alterar_lig_carr(Geral *sistema, int id_p);

#endif