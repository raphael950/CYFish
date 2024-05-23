all: exec

exec: main.o player.o map.o penguin.o window.o
	gcc -o exec main.o player.o map.o penguin.o window.o -lncurses

main.o : main.c player.h map.h penguin.h window.h
	gcc -o main.o -c main.c

window.o : window.c window.o
	gcc -o window.o -c window.c

penguin.o: penguin.c penguin.h
	gcc -o penguin.o -c penguin.c

player.o : player.c player.h
	gcc -o player.o -c player.c

map.o: map.c map.h
	gcc -o map.o -c map.c

clean:
	rm -f *.o

mrproper: clean
	rm -f exec