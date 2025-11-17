#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "src/menu_poketest.h"

int main(int argc, char *argv[])
{

	menu_poketest_t* menu_juego = menu_poketest_crear();
	if (menu_juego == NULL) {
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD FALLO ANSI_COLOR_RESET "\n");
		return ERROR;
	}
	if (!menu_juego->menu[MENU_PRINCIPAL] || 
		!menu_juego->menu[MENU_BUSQUEDA] || 
		!menu_juego->menu[MENU_MUESTRA]) {
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD FALLO ANSI_COLOR_RESET "\n");
		menu_poketest_destruir(menu_juego);
		return ERROR;
	}

	bool resultado_construir = menu_poketest_construir(menu_juego);
	if (resultado_construir == false) {
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD FALLO ANSI_COLOR_RESET "\n");
		return ERROR;
	}

	menu_poketest_comenzar(menu_juego);

	menu_poketest_destruir(menu_juego);

	return 0;
}
