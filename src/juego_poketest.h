#ifndef __JUEGO_POKETEST_H__
#define __JUEGO_POKETEST_H__

#include "tp1.h"

typedef struct juego_poketest {
    tp1_t *archivo_pokemones;
    size_t semilla;
} juego_poketest_t;

/*
*   Crea una estructura de juego poketest con la semilla inicial dada.
*   Devuelve NULL en caso de error o la estructura en caso de exito.
*/
juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, size_t semilla);

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