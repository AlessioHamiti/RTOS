all: server lettore casa_editrice

server: server.o list.o
	gcc -Wall -o server server.o list.o

casa_editrice: casa_editrice.o list.o
	gcc -Wall -o casa_editrice casa_editrice.o list.o

lettore: lettore.o list.o
	gcc -Wall -o lettore lettore.o list.o

server.o: server.c list.h
	gcc -Wall -c server.c

casa_editrice.o: casa_editrice.c list.h
	gcc -Wall -c casa_editrice.c

lettore.o: lettore.c list.h
	gcc -Wall -c lettore.c

list.o: list.c list.h
	gcc -Wall -c list.c

clean:
	rm -f server lettore casa_editrice *.o