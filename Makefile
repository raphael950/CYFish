

exec: main.o player.o map.o
	gcc -o exec main.o player.o map.o

main.o : main.c player.h map.h
	gcc -o main.o -c main.c

player.o : player.c player.h
	gcc -o player.o -c player.c

map.o: map.c map.h
	gcc -o map.o -c map.c