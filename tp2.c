#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

// TODO: mover todo esto a ./src/juego.c  y juego.h


int main(int argc, char *argv[])
{
	
	menu_t *menu_juego = menu_crear(NOMBRE_JUEGO);
	if (menu_juego == NULL)
		return ERROR;
	poketest_t juego_poketest = inicializar_poketest(menu_juego);
	
	bool agregado = agregar_opciones_juego(menu_juego, &juego_poketest);
	if (agregado == false) {
		menu_destruir(menu_juego);
		return ERROR;
	}
	
	while (juego_poketest.salir == false) {
		__fpurge(stdin);
		bool opcion_ejecutada = menu_mostrar_completo(menu_juego, juego_poketest.formato);
		while (opcion_ejecutada == false) {
			__fpurge(stdin);
			printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
			sleep(2);
			char opcion_elegida = menu_seleccionar_opcion(menu_juego);
			opcion_ejecutada = menu_ejecutar_opcion(menu_juego, opcion_elegida);
		}
	}
	menu_destruir(menu_juego);

	return 0;
}
