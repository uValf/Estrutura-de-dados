#ifndef PROCESSO_H
#define PROCESSO_H

#define MAX_PROCESSOS 1000
#define MAX_STRING 30
#define MAX_MULTIVALORADO 10

typedef struct {
    int id;
    char numero[MAX_STRING];
    char data_ajuizamento[MAX_STRING];
    int id_classe[MAX_MULTIVALORADO];
    int qtd_id_classe;
    int id_assunto[MAX_MULTIVALORADO];
    int qtd_id_assunto;
    int ano_eleicao;
} Processo;

void ler_processos(const char *nome_arquivo, Processo processos[], int *qtd);
void ordenar_por_id(Processo processos[], int qtd);
void ordenar_por_data(Processo processos[], int qtd);
void salvar_csv(const char *nome_arquivo, Processo processos[], int qtd);
int contar_por_id_classe(Processo processos[], int qtd, int id_classe);
int contar_assuntos(Processo processos[], int qtd);
void listar_multiplos_assuntos(Processo processos[], int qtd);
void dias_em_tramitacao(Processo processos[], int qtd);

#endif
