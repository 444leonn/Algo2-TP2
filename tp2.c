#include "src/ansi.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "src/tp1.h"
#include "src/menu.h"
#include "src/aux_tp1.h"
#include "src/constantes.h"

typedef struct archivo {
	char *nombre;
	tp1_t *pokemones;
} archivo_t;

typedef struct poketest {
	menu_t *menu;
	size_t semilla_actual;
	enum formato_muestra formato;
	archivo_t archivo;
	bool salir;
} poketest_t;

poketest_t crear_poketest(menu_t *menu)
{
	poketest_t poketest;

	poketest.menu = menu;
	poketest.semilla_actual = 5;
	poketest.formato = FORMATO_1;
	poketest.archivo.nombre = NULL;
	poketest.archivo.pokemones = NULL;
	poketest.salir = false;

	return poketest;
}

bool cargar_archivo(void *ctx)
{
	if (ctx == NULL)
		return false;
	
	poketest_t *poketest = ctx;

	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
	getchar();

	printf(ANSI_COLOR_BOLD MENSAJE_ARCHIVO ANSI_COLOR_RESET);
	char *nombre_ingresado = leer_linea(stdin);

	size_t largo = strlen(nombre_ingresado) + 1;
	poketest->archivo.nombre = malloc(largo * sizeof(char));
	if (poketest->archivo.nombre == NULL) {
		printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET);
		return false;
	}
	
	poketest->archivo.pokemones = tp1_leer_archivo(poketest->archivo.nombre);
	if (poketest->archivo.pokemones == NULL) {
		printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET "\n\n");
		free(poketest->archivo.nombre);
		return false;
	}

	printf(ANSI_COLOR_BOLD MENSAJE_CARGADO ANSI_COLOR_RESET);

	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);

	return true;
}

bool buscar(void *ctx)
{
	if (ctx == NULL)
		return false;
	
	// PREGUNTAR SI VER BUSCAR POR NOMBRE O POR ID

	// ACORDARSE QUE ESTA ORDENADO POR ID

	return true;
}

bool mostrar(void *ctx)
{
	if (ctx == NULL)
		return false;

	
	// PREGUNTAR SI VER ORDENADO POR NOMBRE O POR ID

	// ACORDARSE QUE ESTA ORDENADO POR ID

	return true;
}

bool jugar(void *ctx)
{
	if (ctx == NULL)
		return false;
	
	return true;
}

bool jugar_semilla(void *ctx)
{
	if (ctx == NULL)
		return false;

	return true;
}

bool cambiar_estilo(void *ctx)
{
	if (ctx == NULL)
		return false;

	poketest_t *poketest = ctx;

	if (poketest->formato == FORMATO_1)
		poketest->formato = FORMATO_2;
	if (poketest->formato == FORMATO_2)
		poketest->formato = FORMATO_3;
	if (poketest->formato == FORMATO_3)
		poketest->formato = FORMATO_1;

	return true;
}

bool salir(void *ctx)
{
	if (ctx == NULL)
		return false;

	poketest_t *poketest = ctx;

	if (poketest->archivo.nombre != NULL)
		free(poketest->archivo.nombre);
	if (poketest->archivo.pokemones != NULL)
		tp1_destruir(poketest->archivo.pokemones);

	poketest->salir = true;

	return true;
}

bool agregar_opciones_juego(menu_t *menu, poketest_t *poketest)
{
	if (menu == NULL)
		return false;
	
	bool opcion_1 = menu_agregar_opcion(menu, CHAR_C, CARGAR_ARCHIVO, cargar_archivo, poketest);
	bool opcion_2 = menu_agregar_opcion(menu, CHAR_B, BUSCAR, buscar, poketest);
	bool opcion_3 = menu_agregar_opcion(menu, CHAR_M, MOSTRAR, mostrar, poketest);
	bool opcion_4 = menu_agregar_opcion(menu, CHAR_J, JUGAR, jugar, poketest);
	bool opcion_5 = menu_agregar_opcion(menu, CHAR_S, JUGAR_SEMILLA, jugar_semilla, poketest);
	bool opcion_6 = menu_agregar_opcion(menu, CHAR_E, ESTILO, cambiar_estilo, poketest);
	bool opcion_7 = menu_agregar_opcion(menu, CHAR_Q, SALIR, salir, poketest);

	bool resultado = opcion_1 && opcion_2 && opcion_3 && opcion_4 && opcion_5 && opcion_6 && opcion_7;

	return resultado;
}

int main(int argc, char *argv[])
{
	printf(ANSI_RESET_SCREEN);

	menu_t *menu_juego = menu_crear(NOMBRE_JUEGO);
	if (menu_juego == NULL)
		return ERROR;
	
	poketest_t juego_poketest = crear_poketest(menu_juego);

	bool agregado = agregar_opciones_juego(menu_juego, &juego_poketest);
	if (agregado == false) {
		menu_destruir(menu_juego);
		return ERROR;
	}
	
	while (juego_poketest.salir == false) {
		bool opcion_ejecutada = menu_mostrar_completo(menu_juego, juego_poketest.formato);
		while (opcion_ejecutada == false) {
			getchar();
			printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET "\n\n");
			printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
			char opcion_elegida = menu_seleccionar_opcion(menu_juego);
			opcion_ejecutada = menu_ejecutar_opcion(menu_juego, opcion_elegida);
		}
	}

	menu_destruir(menu_juego);

	return 0;
}
