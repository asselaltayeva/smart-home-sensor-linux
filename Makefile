main: main.o print.o
	gcc -o main main.o print.o

main.o: main.c print.h device.h
	gcc -c main.c

print.o: print.c print.h device.h
	gcc -c print.c
