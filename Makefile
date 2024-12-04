# Variáveis
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
OBJS = ingrediente.o pizza.o main.o
TARGET = pizzaria.exe

# Regra principal
all: $(TARGET)

# Regra para criar o executável
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Regras para compilar cada arquivo objeto
ingrediente.o: ingrediente.c ingrediente.h
	$(CC) $(CFLAGS) -c ingrediente.c -o ingrediente.o

pizza.o: pizza.c pizza.h ingrediente.h
	$(CC) $(CFLAGS) -c pizza.c -o pizza.o

main.o: main.c pizza.h ingrediente.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Regra para limpar os arquivos gerados
clean:
	rm -f $(OBJS) $(TARGET)

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)
