#pragma once
#include "ingrediente.h"

typedef struct pizza
{
    int id;
    char nome[50];
    char tamanho;
    float preco;
    ingrediente ingredientes[10];
    int qtd_ingredientes;

} pizza;

void adicionarPizza(int id, char* nome, char tamanho, float preco, ingrediente* ingredientes, int qtd);

void visualizarPizza(int id);

void editarPizza(int id, char* nome, char tamanho, float preco, ingrediente* ingredientes, int qtd);

void removerPizza(int id);

void exportarCardapio(const char* arquivo);

void importarCardapio(const char* arquivo);