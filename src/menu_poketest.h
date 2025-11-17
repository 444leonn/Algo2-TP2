#ifndef __MENU_POKETEST_H__
#define __MENU_POKETEST_H__

#include "ansi.h"
#include "constantes.h"
#include "menu.h"
#include "tp1.h"
#include "aux_tp1.h"

#include <stdlib.h>
#include <string.h>

enum MENUS { MENU_PRINCIPAL, MENU_BUSQUEDA, MENU_MUESTRA };

typedef struct menu_poketest {
    menu_t *menu[3];
	size_t semilla;
	enum formato_muestra formato;
	tp1_t *archivo_pokemones;
	bool opcion_busqueda;
	bool salir;
} menu_poketest_t;

/*
*	Crea una estructura de juego poketest, sin valores.
*	Retorna la estructura o NULL en caso de error.
*/
menu_poketest_t *menu_poketest_crear();

/*
*	Contruye un juego poketest, agregando sus diferentes menus.
*	Si el juego pasado por parametro es invalido y/o ocurre un error, devuelve false.
*	Devuelve true en el caso de construccion exitosa.
*/
bool menu_poketest_construir(menu_poketest_t *menu_poketest);

/*
*	Permite comenzar un juego poketest.
*	Mostrando el primer menu.
*	Habilitando todas las funcionalidades.
*	Si el juego pasado por parametro es invalido y/o ocurre un error, devuelve false.
*/
bool menu_poketest_comenzar(menu_poketest_t *menu_poketest);

/*
*	Libera la memoria empleada para toda la estructura del juego.
*/
void menu_poketest_destruir(menu_poketest_t *menu_poketest);

#endif // __MENU_POKETEST_H__