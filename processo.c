#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

void parse_multivalorado(char *str, int valores[], int *qtd) {
    *qtd = 0;
    char *token = strtok(str, "{} ,");
    while (token != NULL && *qtd < MAX_MULTIVALORADO) {
        valores[(*qtd)++] = atoi(token);
        token = strtok(NULL, "{} ,");
    }
}

void ler_processos(const char *nome_arquivo, Processo processos[], int *qtd) {
    FILE *fp = fopen(nome_arquivo, "r");
    if (!fp) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    char linha[256];
    fgets(linha, sizeof(linha), fp);  // Ignora cabeçalho
    *qtd = 0;

    while (fgets(linha, sizeof(linha), fp) && *qtd < MAX_PROCESSOS) {
        Processo *p = &processos[*qtd];
        char classe_str[50], assunto_str[50];

        sscanf(linha, "%d,\"%[^\"]\",%[^,],%[^,],%[^,],%d",
               &p->id, p->numero, p->data_ajuizamento,
               classe_str, assunto_str, &p->ano_eleicao);

        parse_multivalorado(classe_str, p->id_classe, &p->qtd_id_classe);
        parse_multivalorado(assunto_str, p->id_assunto, &p->qtd_id_assunto);

        (*qtd)++;
    }

    fclose(fp);
}

void ordenar_por_id(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (processos[i].id > processos[j].id) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

void ordenar_por_data(Processo processos[], int qtd) {
    for (int i = 0; i < qtd - 1; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(processos[i].data_ajuizamento, processos[j].data_ajuizamento) < 0) {
                Processo temp = processos[i];
                processos[i] = processos[j];
                processos[j] = temp;
            }
        }
    }
}

void salvar_csv(const char *nome_arquivo, Processo processos[], int qtd) {
    FILE *fp = fopen(nome_arquivo, "w");
    if (!fp) {
        perror("Erro ao salvar CSV");
        return;
    }

    fprintf(fp, "\"id\",\"numero\",\"data_ajuizamento\",\"id_classe\",\"id_assunto\",\"ano_eleicao\"\n");

    for (int i = 0; i < qtd; i++) {
        fprintf(fp, "%d,\"%s\",%s,{", processos[i].id, processos[i].numero, processos[i].data_ajuizamento);

        for (int j = 0; j < processos[i].qtd_id_classe; j++) {
            fprintf(fp, "%d", processos[i].id_classe[j]);
            if (j < processos[i].qtd_id_classe - 1) fprintf(fp, ",");
        }

        fprintf(fp, "},{");

        for (int j = 0; j < processos[i].qtd_id_assunto; j++) {
            fprintf(fp, "%d", processos[i].id_assunto[j]);
            if (j < processos[i].qtd_id_assunto - 1) fprintf(fp, ",");
        }

        fprintf(fp, "},%d\n", processos[i].ano_eleicao);
    }

    fclose(fp);
}

int contar_por_id_classe(Processo processos[], int qtd, int id_classe) {
    int cont = 0;
    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < processos[i].qtd_id_classe; j++) {
            if (processos[i].id_classe[j] == id_classe) {
                cont++;
                break;
            }
        }
    }
    return cont;
}

int contar_assuntos(Processo processos[], int qtd) {
    int unicos[1000] = {0}, total = 0;
    for (int i = 0; i < qtd; i++) {
        for (int j = 0; j < processos[i].qtd_id_assunto; j++) {
            int id = processos[i].id_assunto[j], encontrado = 0;
            for (int k = 0; k < total; k++) {
                if (unicos[k] == id) {
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                unicos[total++] = id;
            }
        }
    }
    return total;
}

void listar_multiplos_assuntos(Processo processos[], int qtd) {
    printf("Processos com mais de um assunto:\n");
    for (int i = 0; i < qtd; i++) {
        if (processos[i].qtd_id_assunto > 1) {
            printf("ID: %d | Número: %s\n", processos[i].id, processos[i].numero);
        }
    }
}

void dias_em_tramitacao(Processo processos[], int qtd) {
    time_t agora = time(NULL);
    struct tm data_proc = {0};

    printf("Dias em tramitação:\n");
    for (int i = 0; i < qtd; i++) {
        sscanf(processos[i].data_ajuizamento, "%d-%d-%d",
               &data_proc.tm_year, &data_proc.tm_mon, &data_proc.tm_mday);
        data_proc.tm_year -= 1900;
        data_proc.tm_mon -= 1;

        time_t inicio = mktime(&data_proc);
        double dias = difftime(agora, inicio) / (60 * 60 * 24);
        printf("ID: %d | Número: %s -> %.0f dias\n", processos[i].id, processos[i].numero, dias);
    }
}
