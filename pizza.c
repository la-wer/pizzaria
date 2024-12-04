#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pizza.h"

void adicionarPizza(int id, char* nome, char tamanho, float preco, ingrediente* ingredientes, int qtd)
{
    pizza novaPizza;

    novaPizza.id = id;
    strcpy(novaPizza.nome, nome);
    novaPizza.tamanho = tamanho;
    novaPizza.preco = preco;
    novaPizza.qtd_ingredientes = qtd;

    for (int i = 0; i < qtd; i++) {
        strcpy(novaPizza.ingredientes[i].nome, ingredientes[i].nome);
    }

    FILE* f = fopen("./pizzas", "ab"); 
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fwrite(&novaPizza, sizeof(pizza), 1, f);
    fclose(f);

    printf("Pizza adicionada com sucesso!\n");
}

void visualizarPizza(int id)
{
    FILE* f = fopen("./pizzas", "rb");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    pizza p;
    while (fread(&p, sizeof(pizza), 1, f)) {
        if (p.id == id) {
            printf("ID: %d\n", p.id);
            printf("Nome: %s\n", p.nome);
            printf("Tamanho: %c\n", p.tamanho);
            printf("Preço: %.2f\n", p.preco);
            printf("Ingredientes:\n");
            for (int i = 0; i < p.qtd_ingredientes; i++) {
                printf("- %s\n", p.ingredientes[i].nome);
            }
            fclose(f);
            return;
        }
    }

    printf("Pizza com ID %d não encontrada.\n", id);
    fclose(f);
}

void editarPizza(int id, char* nome, char tamanho, float preco, ingrediente* ingredientes, int qtd)
{
    FILE* f = fopen("./pizzas", "rb+");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    pizza p;
    while (fread(&p, sizeof(pizza), 1, f)) {
        if (p.id == id) {

            strcpy(p.nome, nome);
            p.tamanho = tamanho;
            p.preco = preco;
            p.qtd_ingredientes = qtd;
            for (int i = 0; i < qtd; i++) {
                strcpy(p.ingredientes[i].nome, ingredientes[i].nome);
            }

            fseek(f, -sizeof(pizza), SEEK_CUR);
            fwrite(&p, sizeof(pizza), 1, f);
            printf("Pizza com ID %d editada com sucesso!\n", id);
            fclose(f);
            return;
        }
    }

    printf("Pizza com ID %d não encontrada.\n", id);
    fclose(f);
}

void removerPizza(int id)
{
    FILE* f = fopen("./pizzas", "rb");
    if (f == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    FILE* temp = fopen("./temp_pizzas", "wb");
    if (temp == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(f);
        return;
    }

    pizza p;
    int encontrada = 0;
    while (fread(&p, sizeof(pizza), 1, f)) {
        if (p.id == id) {
            encontrada = 1;
        } else {
            fwrite(&p, sizeof(pizza), 1, temp);
        }
    }

    fclose(f);
    fclose(temp);

    if (encontrada) {
        remove("./pizzas");
        rename("./temp_pizzas", "./pizzas");
        printf("Pizza com ID %d removida com sucesso!\n", id);
    } else {
        printf("Pizza com ID %d não encontrada.\n", id);
        remove("./temp_pizzas");
    }
}

void exportarCardapio(const char* arquivo)
{
    FILE* f = fopen(arquivo, "w");
    if (f == NULL) {
        perror("Erro ao abrir arquivo para exportação");
        return;
    }

    FILE* fp = fopen("./pizzas", "rb");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo de pizzas");
        fclose(f);
        return;
    }

    pizza p;
    while (fread(&p, sizeof(pizza), 1, fp)) {
        fprintf(f, "%d;%s;%c;%.2f;%d\n", p.id, p.nome, p.tamanho, p.preco, p.qtd_ingredientes);
        for (int i = 0; i < p.qtd_ingredientes; i++) {
            fprintf(f, "%d,%s,%.2f\n", p.ingredientes[i].id, p.ingredientes[i].nome, p.ingredientes[i].preco);
        }
    }

    fclose(fp);
    fclose(f);
    printf("Cardápio exportado para '%s' com sucesso.\n", arquivo);
}

void importarCardapio(const char* arquivo)
{
    FILE* f = fopen(arquivo, "r");
    if (f == NULL) {
        perror("Erro ao abrir arquivo para importação");
        return;
    }

    FILE* fp = fopen("./pizzas", "wb");
    if (fp == NULL) {
        perror("Erro ao abrir arquivo de pizzas");
        fclose(f);
        return;
    }

    pizza p;
    while (fscanf(f, "%d;%[^;];%c;%f;%d\n", &p.id, p.nome, &p.tamanho, &p.preco, &p.qtd_ingredientes) == 5) {
        for (int i = 0; i < p.qtd_ingredientes; i++) {
            fscanf(f, "%d,%[^,],%f\n", &p.ingredientes[i].id, p.ingredientes[i].nome, &p.ingredientes[i].preco);
        }
        fwrite(&p, sizeof(pizza), 1, fp);
    }

    fclose(f);
    fclose(fp);
    printf("Cardápio importado de '%s' com sucesso.\n", arquivo);
}

void listarPizzas()
{
    FILE* fp = fopen("./pizzas", "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de pizzas.\n");
        return;
    }

    pizza p;
    printf("Cardápio de Pizzas:\n");
    printf("ID\tNome\t\tTamanho\tPreço\tIngredientes\n");
    while (fread(&p, sizeof(pizza), 1, fp)) {
        printf("%d\t%s\t\t%c\t%.2f\t", p.id, p.nome, p.tamanho, p.preco);
        for (int i = 0; i < p.qtd_ingredientes; i++) {
            printf("%s ", p.ingredientes[i].nome);
        }
        printf("\n");
    }
    fclose(fp);
}

void listarIngredientes()
{
    FILE* fi = fopen("./ingredientes", "rb");
    if (fi == NULL) {
        printf("Erro ao abrir o arquivo de ingredientes.\n");
        return;
    }

    ingrediente ing;
    printf("Ingredientes Disponíveis:\n");
    printf("ID\tNome\t\tPreço\n");
    while (fread(&ing, sizeof(ingrediente), 1, fi)) {
        printf("%d\t%s\t\t%.2f\n", ing.id, ing.nome, ing.preco);
    }
    fclose(fi);
}

void registrarVenda()
{
    int pizzaId;
    int extraId;
    char continuar;
    float precoFinal = 0;

    listarPizzas();
    printf("Digite o ID da pizza que deseja comprar: ");
    scanf("%d", &pizzaId);

    FILE* fp = fopen("./pizzas", "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o arquivo de pizzas.\n");
        return;
    }

    pizza p;
    int encontrouPizza = 0;
    while (fread(&p, sizeof(pizza), 1, fp)) {
        if (p.id == pizzaId) {
            encontrouPizza = 1;
            precoFinal += p.preco;
            printf("Você escolheu a pizza '%s' de tamanho '%c' por %.2f.\n", p.nome, p.tamanho, p.preco);
            break;
        }
    }
    fclose(fp);

    if (!encontrouPizza) {
        printf("Pizza com ID %d não encontrada.\n", pizzaId);
        return;
    }

    do {
        listarIngredientes();
        printf("Digite o ID do ingrediente extra (ou -1 para finalizar): ");
        scanf("%d", &extraId);

        if (extraId == -1) break;

        FILE* fi = fopen("./ingredientes", "rb");
        if (fi == NULL) {
            printf("Erro ao abrir o arquivo de ingredientes.\n");
            return;
        }

        ingrediente ing;
        int encontrouIngrediente = 0;
        while (fread(&ing, sizeof(ingrediente), 1, fi)) {
            if (ing.id == extraId) {
                encontrouIngrediente = 1;
                precoFinal += ing.preco;
                printf("Adicionado ingrediente extra '%s' por %.2f.\n", ing.nome, ing.preco);
                break;
            }
        }
        fclose(fi);

        if (!encontrouIngrediente) {
            printf("Ingrediente com ID %d não encontrado.\n", extraId);
        }

        printf("Deseja adicionar outro ingrediente? (S/N): ");
        scanf(" %c", &continuar);
    } while (continuar == 'S' || continuar == 's');

    printf("Venda registrada com sucesso!\n");
    printf("Pizza: %s\n", p.nome);
    printf("Preço final: %.2f\n", precoFinal);
}