TARGET = main
SRCS = main.c fast_trigo.c
OBJS = main.o fast_trigo.o
LIB = libfast_trigo.a
CC = gcc

# exe
$(TARGET): $(OBJS) $(LIB)
	$(CC) -o $@ main.o -L. -lfast_trigo -lm

# libreria statica
$(LIB): fast_trigo.o
	ar rcs $@ $^

# oggetto per fast_trigo.c
fast_trigo.o: fast_trigo.c
	$(CC) -Wall -I. -c fast_trigo.c -o fast_trigo.o

# oggetto per main.c
main.o: main.c
	$(CC) -Wall -I. -c main.c -o main.o