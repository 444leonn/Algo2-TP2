#ifndef __JUEGO_POKETEST_H__
#define __JUEGO_POKETEST_H__

#include "ansi.h"
#include "constantes.h"
#include "tp1.h"
#include "lista.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

enum color_tarjeta { COLOR_AZUL, COLOR_ROJO };

typedef struct tarjeta tarjeta_t;

typedef struct registro_historial registro_historial_t;

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
*   Inicia el juego con la estructura dada.
*   Devuelve false en caso de error.
*/
bool juego_poketest_jugar(juego_poketest_t *juego_poketest);

/*
*   Libera la memoria empleada para el juego.
*/
void juego_poketest_destruir(juego_poketest_t *juego_poketest);

#endif // __JUEGO_POKETEST_H__