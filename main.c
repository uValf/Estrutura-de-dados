#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

int main() {
    Processo processos[MAX_PROCESSOS];
    int qtd;

    ler_processos("processo_043_202409032338.csv", processos, &qtd);
    printf("Foram lidos %d processos do arquivo.\n", qtd);

    ordenar_por_id(processos, qtd);
    salvar_csv("ordenado_por_id.csv", processos, qtd);

    ordenar_por_data(processos, qtd);
    salvar_csv("ordenado_por_data.csv", processos, qtd);

    int id_classe = 11778;
    int total = contar_por_id_classe(processos, qtd, id_classe);
    printf("Total de processos com id_classe %d: %d\n", id_classe, total);

    int total_assuntos = contar_assuntos(processos, qtd);
    printf("Total de id_assuntos distintos: %d\n", total_assuntos);

    listar_multiplos_assuntos(processos, qtd);
    dias_em_tramitacao(processos, qtd);

    system("pause"); // para manter o terminal aberto no Windows
    return 0;
}
