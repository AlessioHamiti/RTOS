all: server fornitore centro_vaccinale

server: server.o list.o
	gcc -Wall -o server server.o list.o

centro_vaccinale: centro_vaccinale.o list.o
	gcc -Wall -o centro_vaccinale centro_vaccinale.o list.o

fornitore: fornitore.o list.o
	gcc -Wall -o fornitore fornitore.o list.o

server.o: server.c list.h
	gcc -Wall -c server.c

centro_vaccinale.o: centro_vaccinale.c list.h
	gcc -Wall -c centro_vaccinale.c

fornitore.o: fornitore.c list.h
	gcc -Wall -c fornitore.c

list.o: list.c list.h
	gcc -Wall -c list.c

clean:
	rm -f server fornitore centro_vaccinale *.o