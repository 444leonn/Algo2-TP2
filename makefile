VALGRIND_FLAGS=--leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes
CFLAGS =-std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g
CC = gcc
PARAMETROS?=

all: clean valgrind-alumno

valgrind-alumno: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

pruebas_alumno: src/*.c pruebas_alumno.c
	$(CC) $(CFLAGS) src/*.c pruebas_alumno.c -o pruebas_alumno

tp2: src/*.c main.c
	$(CC) $(CFLAGS) src/*.c main.c -o tp2

ejecutar: tp2
	./tp2 $(PARAMETROS)

valgrind: tp2
	valgrind $(VALGRIND_FLAGS) ./tp2 $(PARAMETROS)

clean:
	rm -f pruebas_alumno
