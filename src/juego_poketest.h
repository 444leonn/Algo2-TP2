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

// TODO: agregar pre y post condiciones

juego_poketest_t *juego_poketest_crear();

juego_poketest_t juego_poketest_construir(juego_poketest_t *juego_poketest);

juego_poketest_t *juego_poketest_jugar(juego_poketest_t *juego_poketest);

void *juego_poketest_destruir(juego_poketest_t *juego_poketest);

#endif // __JUEGO_POKETEST_H_