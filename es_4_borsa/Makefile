all: server investitore agenteDiBorsa

server: server.o list.o
	gcc -Wall -o server server.o list.o

agenteDiBorsa: agenteDiBorsa.o list.o
	gcc -Wall -o agenteDiBorsa agenteDiBorsa.o list.o

investitore: investitore.o list.o
	gcc -Wall -o investitore investitore.o list.o

server.o: server.c list.h
	gcc -Wall -c server.c

agenteDiBorsa.o: agenteDiBorsa.c list.h
	gcc -Wall -c agenteDiBorsa.c

investitore.o: investitore.c list.h
	gcc -Wall -c investitore.c

list.o: list.c list.h
	gcc -Wall -c list.c

clean:
	rm -f server investitore agenteDiBorsa *.o