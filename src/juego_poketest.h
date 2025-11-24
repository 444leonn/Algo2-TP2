#ifndef __JUEGO_POKETEST_H__
#define __JUEGO_POKETEST_H__

#include "tp1.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define CANTIDAD_JUGADORES 2

enum COLOR_TARJETA { COLOR_AZUL, COLOR_ROJO };

enum JUGADORES { J1, J2 };

typedef struct tarjeta {
	int id_pokemon;
	enum COLOR_TARJETA color_actual;
	struct pokemon *valor;
} tarjeta_t;

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
*   Devuelve la cantidad de tarjetas cargadas en el juego.
*/
size_t juego_poketest_cargar(juego_poketest_t *juego_poketest);

/*
*	Realiza la jugada con dos tarjetas.
*	Devuelve un booleano con el resultado de la jugada.
*/
bool juego_poketest_jugada(juego_poketest_t *juego_poketest, enum JUGADORES jugador, int tarjeta_1, int tarjeta_2);

/*
*	Devuelve el ultimo registro del historial de jugadas.
*	True si el ultimo registro fue de una jugada exitosa.
*	False si la jugada fue incorrecta.
*/
registro_historial_t *juego_poketest_ultimo_registro(juego_poketest_t *juego_poketest);

/*
*	Devuelve el porcentaje del progreso actual del juego.
*	Devuelve un valor negativo en caso de error.
*/
float juego_poketest_progreso(juego_poketest_t *juego_poketest);

/*
*	Devuelve el puntaje total actual del jugador.
*	Devuelve un valor negativo en caso de error.
*/
int juego_poketest_puntaje(juego_poketest_t *juego_poketest, enum JUGADORES jugador);

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