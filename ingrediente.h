#ifndef INGREDIENTE_H
#define INGREDIENTE_H

typedef struct ingrediente
{
    int id;
    char nome[50];
    float preco;

} ingrediente;

void adicionarIngrediente(int id, char* nome, float preco);

void visualizarIngrediente(int id);

void editarIngrediente(int id, char* nome, float preco);

void removerIngrediente(int id);

void exportarIngredientes(const char* arquivo);

void importarIngredientes(const char* arquivo);

#endif