#ifndef __JUEGO_POKETEST_H__
#define __JUEGO_POKETEST_H__

#include "ansi.h"
#include "constantes.h"
#include "tp1.h"
#include "lista.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

enum color_tarjeta {COLOR_AZUL, COLOR_ROJO};

typedef struct tarjeta {
    int numero_tarjeta;
    int id_pokemon;
    enum color_tarjeta color_actual;
    struct pokemon *valor;
} tarjeta_t;  

typedef struct registro_historial {
    char *registro;
    int primer_carta, segunda_carta;
    bool resultado;
} registro_historial_t;

typedef struct jugador {
    size_t puntaje;
} jugador_t;

typedef struct juego_poketest {
    tp1_t *archivo_pokemones;
    lista_t *historial;
    tarjeta_t *tarjetas;
    size_t semilla;
    size_t cantidad_cargada;
    size_t cantidad_tarjetas;
    jugador_t jugador_1, jugador_2;
    bool turno_jugador_1;
} juego_poketest_t;

/*
*   Crea una estructura de juego poketest con la semilla inicial dada.
*   Si la semilla dada es menor a 9 lo crea con ese valor como semilla.
*   Devuelve NULL en caso de error o la estructura en caso de exito.
*/
juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, size_t semilla);

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