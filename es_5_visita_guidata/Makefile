all: server guida visitatori

server: server.o list.o
	gcc -Wall -o server server.o list.o

visitatori: visitatori.o list.o
	gcc -Wall -o visitatori visitatori.o list.o

guida: guida.o list.o
	gcc -Wall -o guida guida.o list.o

server.o: server.c list.h
	gcc -Wall -c server.c

visitatori.o: visitatori.c list.h
	gcc -Wall -c visitatori.c

guida.o: guida.c list.h
	gcc -Wall -c guida.c

list.o: list.c list.h
	gcc -Wall -c list.c

clean:
	rm -f server guida visitatori *.o