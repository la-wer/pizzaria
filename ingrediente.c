#include <stdio.h>
#include <string.h>
#include "ingrediente.h"

void adicionarIngrediente(int id, char* nome, float preco)
{
    ingrediente novoIngrediente;
    novoIngrediente.id = id;
    strcpy(novoIngrediente.nome, nome);
    novoIngrediente.preco = preco;

    FILE* f = fopen("./ingredientes", "ab");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fwrite(&novoIngrediente, sizeof(ingrediente), 1, f);
    fclose(f);

    printf("Ingrediente adicionado com sucesso!\n");
}

void visualizarIngrediente(int id)
{
    FILE* f = fopen("./ingredientes", "rb");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    ingrediente ing;
    while (fread(&ing, sizeof(ingrediente), 1, f)) {
        if (ing.id == id) {
            printf("ID: %d\n", ing.id);
            printf("Nome: %s\n", ing.nome);
            printf("Preço: %.2f\n", ing.preco);
            fclose(f);
            return;
        }
    }

    printf("Ingrediente com ID %d não encontrado.\n", id);
    fclose(f);
}

void editarIngrediente(int id, char* nome, float preco)
{
    FILE* f = fopen("./ingredientes", "rb+");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    ingrediente ing;
    while (fread(&ing, sizeof(ingrediente), 1, f)) {
        if (ing.id == id) {
            strcpy(ing.nome, nome);
            ing.preco = preco;

            fseek(f, -sizeof(ingrediente), SEEK_CUR);
            fwrite(&ing, sizeof(ingrediente), 1, f);
            printf("Ingrediente com ID %d editado com sucesso!\n", id);
            fclose(f);
            return;
        }
    }

    printf("Ingrediente com ID %d não encontrado.\n", id);
    fclose(f);
}

void removerIngrediente(int id)
{
    FILE* f = fopen("./ingredientes", "rb");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    FILE* temp = fopen("./temp_ingredientes", "wb");
    if (temp == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(f);
        return;
    }

    ingrediente ing;
    int encontrada = 0;
    while (fread(&ing, sizeof(ingrediente), 1, f)) {
        if (ing.id == id) {
            encontrada = 1;
        } else {
            fwrite(&ing, sizeof(ingrediente), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);

    if (encontrada) {
        remove("./ingredientes");
        rename("./temp_ingredientes", "./ingredientes");
        printf("Ingrediente com ID %d removido com sucesso!\n", id);
    } else {
        printf("Ingrediente com ID %d não encontrado.\n", id);
        remove("./temp_ingredientes");
    }
}

void exportarIngredientes(const char* arquivo)
{
    FILE* f = fopen(arquivo, "w");
    if (f == NULL) {
        perror("Erro ao abrir arquivo para exportação");
        return;
    }

    FILE* fi = fopen("./ingredientes", "rb");
    if (fi == NULL) {
        perror("Erro ao abrir arquivo de ingredientes");
        fclose(f);
        return;
    }

    ingrediente ing;
    while (fread(&ing, sizeof(ingrediente), 1, fi)) {
        fprintf(f, "%d;%s;%.2f\n", ing.id, ing.nome, ing.preco);
    }

    fclose(fi);
    fclose(f);
    printf("Ingredientes exportados para '%s' com sucesso.\n", arquivo);
}

void importarIngredientes(const char* arquivo)
{
    FILE* f = fopen(arquivo, "r");
    if (f == NULL) {
        perror("Erro ao abrir arquivo para importação");
        return;
    }

    FILE* fi = fopen("./ingredientes", "wb");
    if (fi == NULL) {
        perror("Erro ao abrir arquivo de ingredientes");
        fclose(f);
        return;
    }

    ingrediente ing;
    while (fscanf(f, "%d;%[^;];%f\n", &ing.id, ing.nome, &ing.preco) == 3) {
        fwrite(&ing, sizeof(ingrediente), 1, fi);
    }

    fclose(f);
    fclose(fi);
    printf("Ingredientes importados de '%s' com sucesso.\n", arquivo);
}