#ifndef __JUEGO_POKETEST_H__
#define __JUEGO_POKETEST_H__

#include "tp1.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define CANTIDAD_JUGADORES 2

enum COLOR_TARJETA { COLOR_AZUL, COLOR_ROJO };

enum JUGADORES { JUGADOR_1, JUGADOR_2 };

typedef struct tarjeta tarjeta_t;

typedef struct registro_historial {
	char *registro;
	int primer_carta, segunda_carta;
	bool resultado;
} registro_historial_t;

typedef struct jugador jugador_t;

typedef struct juego_poketest juego_poketest_t;

/*
*   Crea una estructura de juego poketest con la semilla inicial dada.
*	Si la semilla es 0, genera una de manera aleatoria.
*   Devuelve NULL en caso de error o la estructura en caso de exito.
*/
juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, int semilla,
				       size_t cantidad_tarjetas);

/*
*   Carga cierto contenido del archivo de pokemones para usar en el juego.
*   Devuelve la cantidad de pokemones cargados en el juego.
*/
bool juego_poketest_cargar(juego_poketest_t *juego_poketest);

/*
*	Realiza la jugada con los numeros de las tarjetas.
*	Devuelve true si la jugada realiza fue exitosa.
*	Devuelve false en caso de que el juego sea invalido o la jugada sea incorrecta.
*/
bool juego_poketest_jugada(juego_poketest_t *juego_poketest, enum JUGADORES jugador, size_t tarjeta_1, size_t tarjeta_2);

/*
*	Devuelve el ultimo registro del historial de jugadas.
*	True si el ultimo registro fue de una jugada exitosa.
*	False si la jugada fue incorrecta.
*/
bool juego_poketest_ultimo_registro(juego_poketest_t *juego_poketest);

/*
*	Devuelve la cantidad de tarjetas restantes por seleccionar en el juego.
*	Devuelve un valor negativo en caso de error.
*/
int juego_poketest_progreso(juego_poketest_t *juego_poketest);

/*
*	Devuelve el puntaje total actual del jugador.
*/
size_t juego_poketest_puntaje(juego_poketest_t *juego_poketest, enum JUGADORES jugador);

/*
*	Muestra las tarjetas del juego por pantalla.
*	Color rojo aquellas tarjetas ya seleccioandas.
*	Color azul las no seleccionadas.
*/
void juego_poketest_mostrar(juego_poketest_t *juego_poketest);

/*
*   Libera la memoria empleada para el juego.
*/
void juego_poketest_destruir(juego_poketest_t *juego_poketest);

#endif // __JUEGO_POKETEST_H__