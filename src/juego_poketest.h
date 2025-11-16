#ifndef __JUEGO_POKETEST_H_
#define __JUEGO_POKETEST_H_

#include "ansi.h"
#include "constantes.h"
#include "menu.h"
#include "tp1.h"
#include "aux_tp1.h"

#include <stdlib.h>

enum MENUS { MENU_PRINCIPAL, MENU_BUSQUEDA, MENU_MUESTRA };

typedef struct juego_poketest {
    menu_t **menu;
	size_t semilla;
	enum formato_muestra formato;
	tp1_t *archivo_pokemones;
	bool opcion_busqueda;
	bool salir;
} juego_poketest_t;

/*
*	Crea una estructura de juego poketest, sin valores.
*	Retorna la estructura o NULL en caso de error.
*/
juego_poketest_t *juego_poketest_crear();

/*
*	Contruye un juego poketest, agregando sus diferentes menus.
*	Si el juego pasado por parametro es invalido y/o ocurre un error, devuelve false.
*	Devuelve true en el caso de construccion exitosa.
*/
bool juego_poketest_construir(juego_poketest_t *juego_poketest);

/*
*	Permite comenzar un juego poketest.
*	Mostrando el primer menu.
*	Habilitando todas las funcionalidades.
*	Si el juego pasado por parametro es invalido y/o ocurre un error, devuelve false.
*/
bool *juego_poketest_jugar(juego_poketest_t *juego_poketest);

/*
*	Libera la memoria empleada para toda la estructura del juego.
*/
void juego_poketest_destruir(juego_poketest_t *juego_poketest);

#endif // __JUEGO_POKETEST_H_