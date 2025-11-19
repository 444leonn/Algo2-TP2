#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "src/aux_tp1.h"
#include "src/juego_poketest.h"
#include "src/menu.h"

typedef struct aux_pokemones {
	struct pokemon *pokemones;
	size_t cantidad;
} aux_pokemones_t;

enum MENUS { MENU_PRINCIPAL, MENU_BUSQUEDA, MENU_MUESTRA };

typedef struct menu_poketest {
	menu_t *menu[CANTIDAD_MENUS];
	int semilla;
	size_t cantidad_tarjetas;
	enum formato_muestra formato;
	tp1_t *archivo_pokemones;
	bool salir;
} menu_poketest_t;

void limpiar_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

bool evaluar_opciones(char *opcion_evaluada, char *opciones_disponibles)
{
	return strstr(opciones_disponibles, opcion_evaluada) != NULL;
}

void esperar_segundos(size_t cantidad)
{
	for (int i = 0; i < cantidad; i++)
		sleep(1);
}

bool evaluar_volver_menu(char *opcion_evaluada)
{
	if (evaluar_opciones(opcion_evaluada, OPCION_VOLVER) == true) {
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	return false;
}

bool validar_archivo_pokemones(menu_poketest_t *menu_poketest)
{
	if (menu_poketest != NULL && menu_poketest->archivo_pokemones == NULL) {
		printf(ANSI_COLOR_RED FALTA_ARCHIVO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	return false;
}

char seleccionar_opcion()
{
	printf("\n\n" ANSI_COLOR_BOLD MENSAJE_SELECCION ANSI_COLOR_RESET);
	int aux = fgetc(stdin);
	int siguiente = fgetc(stdin);
	if ((char)siguiente != '\n' && (char)siguiente != EOF) {
		ungetc(siguiente, stdin);
	}
	char c = (char)aux;
	printf("\n");

	return c;
}

void mostrar_mensaje_cargar()
{
	printf(ANSI_COLOR_BLUE MENSAJE_CARGAR_ARCHIVO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_INSTRUCCION_ARCHIVO ANSI_COLOR_RESET
	       "\n");
	printf(ANSI_COLOR_MAGENTA MENSAJE_VOLVER ANSI_COLOR_RESET "\n\n");
	esperar_segundos(2);
}

void mostrar_mensaje_fallo_lectura()
{
	printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
	esperar_segundos(2);
}

void mostrar_mensaje_opcion_invalida()
{
	printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET
	       "\n");
	printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
	sleep(2);
}

char *leer_nombre_archivo()
{
	char *nombre = leer_linea(stdin);
	if (nombre == NULL) {
		mostrar_mensaje_fallo_lectura();
		return NULL;
	}
	return nombre;
}

bool agregar_aux_pokemon(struct pokemon *pokemon, void *_aux_pokemones)
{
	if (_aux_pokemones == NULL)
		return false;

	aux_pokemones_t *aux_pokemones = _aux_pokemones;
	aux_pokemones->pokemones[aux_pokemones->cantidad] = *pokemon;

	size_t tamaño = strlen(pokemon->nombre) + 1;
	aux_pokemones->pokemones[aux_pokemones->cantidad].nombre =
		copiar_nombre(tamaño, pokemon->nombre);

	aux_pokemones->cantidad++;

	return true;
}

bool cargar_archivo(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	mostrar_mensaje_cargar();
	printf(ANSI_COLOR_BOLD MENSAJE_ARCHIVO ANSI_COLOR_RESET);
	char *nombre_ingresado = leer_nombre_archivo();
	if (nombre_ingresado == NULL)
		return false;

	if (evaluar_volver_menu(nombre_ingresado) == true) {
		free(nombre_ingresado);
		return true;
	}
	menu_poketest->archivo_pokemones = tp1_leer_archivo(nombre_ingresado);
	if (menu_poketest->archivo_pokemones == NULL) {
		mostrar_mensaje_fallo_lectura();
		free(nombre_ingresado);
		return true;
	}
	free(nombre_ingresado);

	printf("\n" ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       MENSAJE_CARGADO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();

	return true;
}

bool buscar_nombre(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	printf(ANSI_COLOR_BOLD MENSAJE_INGRESAR_NOMBRE ANSI_COLOR_RESET);
	char *nombre_buscado = leer_linea(stdin);
	if (nombre_buscado == NULL) {
		printf(ANSI_COLOR_RED FALLO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}

	struct pokemon *pokemon_buscado = tp1_buscar_nombre(
		menu_poketest->archivo_pokemones, nombre_buscado);
	if (pokemon_buscado == NULL) {
		printf(ANSI_COLOR_RED NO_ENCONTRADO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	printf("\n" ANSI_COLOR_GREEN MENSAJE_POKEMON_ENCONTRADO ANSI_COLOR_RESET
	       "\n");
	mostrar_pokemon(pokemon_buscado, NULL);
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();
	limpiar_buffer();

	free(nombre_buscado);

	return true;
}

bool mostrar_nombre(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	aux_pokemones_t aux_pokemones;
	aux_pokemones.cantidad = 0;
	size_t cantidad_total = tp1_cantidad(menu_poketest->archivo_pokemones);
	aux_pokemones.pokemones =
		malloc(cantidad_total * sizeof(struct pokemon));

	if (aux_pokemones.pokemones != NULL) {
		tp1_con_cada_pokemon(menu_poketest->archivo_pokemones,
				     agregar_aux_pokemon, &aux_pokemones);

		ordenar_pokemones(aux_pokemones.pokemones,
				  aux_pokemones.cantidad, comparador_nombre);
		for (int i = 0; i < aux_pokemones.cantidad; i++) {
			mostrar_pokemon(&aux_pokemones.pokemones[i], NULL);
			free(aux_pokemones.pokemones[i].nombre);
		}

		free(aux_pokemones.pokemones);
	}
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();

	return true;
}

bool buscar_id(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	printf(ANSI_COLOR_BOLD MENSAJE_INGRESAR_ID ANSI_COLOR_RESET);
	char *_id_buscado = leer_linea(stdin);
	if (_id_buscado == NULL) {
		printf(ANSI_COLOR_RED FALLO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}

	int id_buscado = atoi(_id_buscado);
	free(_id_buscado);

	struct pokemon *pokemon_buscado =
		tp1_buscar_id(menu_poketest->archivo_pokemones, id_buscado);
	if (pokemon_buscado == NULL) {
		printf(ANSI_COLOR_RED NO_ENCONTRADO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	printf(ANSI_COLOR_GREEN MENSAJE_POKEMON_ENCONTRADO ANSI_COLOR_RESET
	       "\n");
	mostrar_pokemon(pokemon_buscado, NULL);
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();
	limpiar_buffer();

	return true;
}

bool mostrar_id(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	tp1_con_cada_pokemon(menu_poketest->archivo_pokemones, mostrar_pokemon,
			     NULL);
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();

	return true;
}

bool buscar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	if (validar_archivo_pokemones(menu_poketest) == true)
		return true;

	menu_mostrar_completo(menu_poketest->menu[MENU_BUSQUEDA]);
	char opcion_elegida = seleccionar_opcion();
	bool opcion_ejecutada = menu_ejecutar_opcion(
		menu_poketest->menu[MENU_BUSQUEDA], opcion_elegida);

	while (opcion_ejecutada == false) {
		mostrar_mensaje_opcion_invalida();
		opcion_elegida = seleccionar_opcion();
		opcion_ejecutada = menu_ejecutar_opcion(
			menu_poketest->menu[MENU_BUSQUEDA], opcion_elegida);
		if (opcion_ejecutada == true && opcion_elegida == CHAR_A)
			return true;
	}

	return true;
}

bool mostrar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	if (validar_archivo_pokemones(menu_poketest) == true)
		return true;

	menu_mostrar_completo(menu_poketest->menu[MENU_MUESTRA]);
	char opcion_elegida = seleccionar_opcion();
	bool opcion_ejecutada = menu_ejecutar_opcion(
		menu_poketest->menu[MENU_MUESTRA], opcion_elegida);

	while (opcion_ejecutada == false) {
		mostrar_mensaje_opcion_invalida();
		opcion_elegida = seleccionar_opcion();
		opcion_ejecutada = menu_ejecutar_opcion(
			menu_poketest->menu[MENU_MUESTRA], opcion_elegida);
		if (opcion_ejecutada == true && opcion_elegida == CHAR_A)
			return true;
	}

	return true;
}

bool jugar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	juego_poketest_t *juego_poketest = juego_poketest_crear(
		menu_poketest->archivo_pokemones, menu_poketest->semilla,
		menu_poketest->cantidad_tarjetas);
	if (juego_poketest == NULL) {
		mostrar_mensaje_fallo_lectura();
		return true;
	}

	bool resultado_carga = juego_poketest_cargar(juego_poketest);
	if (resultado_carga == false) {
		mostrar_mensaje_fallo_lectura();
		juego_poketest_destruir(juego_poketest);

		return true;
	}
	juego_poketest_jugar(juego_poketest);
	juego_poketest_destruir(juego_poketest);

	return true;
}

bool jugar_semilla(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	printf(ANSI_COLOR_BOLD MENSAJE_SEMILLA ANSI_COLOR_RESET);
	scanf("%d", &menu_poketest->semilla);
	limpiar_buffer();

	return jugar(menu_poketest);
}

bool cambiar_estilo(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *juego_poketest = ctx;

	printf(ANSI_COLOR_BLUE MENSAJE_ESTILO ANSI_COLOR_RESET "\n");
	esperar_segundos(2);
	if (juego_poketest->formato == FORMATO_1)
		juego_poketest->formato = FORMATO_2;
	else if (juego_poketest->formato == FORMATO_2)
		juego_poketest->formato = FORMATO_3;
	else if (juego_poketest->formato == FORMATO_3)
		juego_poketest->formato = FORMATO_CUSTOM;
	else
		juego_poketest->formato = FORMATO_1;

	for (int i = 0; i < CANTIDAD_MENUS; i++)
		menu_seleccionar_formato(juego_poketest->menu[i],
					 juego_poketest->formato);

	return true;
}

bool volver_menu(void *ctx)
{
	printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
	esperar_segundos(2);

	return true;
}

bool salir(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;
	menu_poketest->salir = true;
	return true;
}

menu_poketest_t *menu_poketest_crear()
{
	menu_poketest_t *menu_poketest = calloc(1, sizeof(menu_poketest_t));
	if (menu_poketest == NULL)
		return NULL;
	menu_poketest->menu[MENU_PRINCIPAL] = menu_crear(NOMBRE_JUEGO);
	menu_poketest->menu[MENU_BUSQUEDA] = menu_crear(BUSCAR);
	menu_poketest->menu[MENU_MUESTRA] = menu_crear(MOSTRAR);

	if (!menu_poketest->menu[MENU_PRINCIPAL] ||
	    !menu_poketest->menu[MENU_BUSQUEDA] ||
	    !menu_poketest->menu[MENU_MUESTRA]) {
		for (int i = 0; i < CANTIDAD_MENUS; i++)
			menu_destruir(menu_poketest->menu[i]);
		free(menu_poketest);
		return NULL;
	}
	menu_poketest->formato = FORMATO_1;
	menu_poketest->cantidad_tarjetas = CANTIDAD_TARJETAS;

	return menu_poketest;
}

bool agregar_opciones_menu_principal(menu_t *menu_principal,
				     menu_poketest_t *menu_poketest)
{
	if (menu_principal == NULL || menu_poketest == NULL)
		return false;
	bool opcion_1 = menu_agregar_opcion(menu_principal, CHAR_C,
					    CARGAR_ARCHIVO, cargar_archivo,
					    menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_principal, CHAR_B, BUSCAR,
					    buscar, menu_poketest);
	bool opcion_3 = menu_agregar_opcion(menu_principal, CHAR_M, MOSTRAR,
					    mostrar, menu_poketest);
	bool opcion_4 = menu_agregar_opcion(menu_principal, CHAR_J, JUGAR,
					    jugar, menu_poketest);
	bool opcion_5 = menu_agregar_opcion(menu_principal, CHAR_S,
					    JUGAR_SEMILLA, jugar_semilla,
					    menu_poketest);
	bool opcion_6 = menu_agregar_opcion(menu_principal, CHAR_E, ESTILO,
					    cambiar_estilo, menu_poketest);
	bool opcion_7 = menu_agregar_opcion(menu_principal, CHAR_Q, SALIR,
					    salir, menu_poketest);

	bool resultado = opcion_1 && opcion_2 && opcion_3 && opcion_4 &&
			 opcion_5 && opcion_6 && opcion_7;
	return resultado;
}

bool agregar_opcion_menu_busqueda(menu_t *menu_busqueda,
				  menu_poketest_t *menu_poketest)
{
	if (menu_busqueda == NULL || menu_poketest == NULL)
		return false;
	bool opcion_1 = menu_agregar_opcion(menu_busqueda, CHAR_N,
					    BUSCAR_NOMBRE, buscar_nombre,
					    menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_busqueda, CHAR_I, BUSCAR_ID,
					    buscar_id, menu_poketest);
	bool opcion_3 = menu_agregar_opcion(menu_busqueda, CHAR_A, VOLVER_MENU,
					    volver_menu, menu_poketest);

	bool resultado = opcion_1 && opcion_2 && opcion_3;

	return resultado;
}

bool agregar_opcion_menu_mostrar(menu_t *menu_muestra,
				 menu_poketest_t *menu_poketest)
{
	if (menu_muestra == NULL || menu_poketest == NULL)
		return false;
	bool opcion_1 = menu_agregar_opcion(menu_muestra, CHAR_N,
					    MOSTRAR_NOMBRE, mostrar_nombre,
					    menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_muestra, CHAR_I, MOSTRAR_ID,
					    mostrar_id, menu_poketest);
	bool opcion_3 = menu_agregar_opcion(menu_muestra, CHAR_A, VOLVER_MENU,
					    volver_menu, menu_poketest);

	bool resultado = opcion_1 && opcion_2 && opcion_3;

	return resultado;
}

bool menu_poketest_construir(menu_poketest_t *menu_poketest)
{
	if (menu_poketest == NULL)
		return false;
	bool resultado_principal = agregar_opciones_menu_principal(
		menu_poketest->menu[MENU_PRINCIPAL], menu_poketest);
	bool resultado_buscar = agregar_opcion_menu_busqueda(
		menu_poketest->menu[MENU_BUSQUEDA], menu_poketest);
	bool resultado_mostrar = agregar_opcion_menu_mostrar(
		menu_poketest->menu[MENU_MUESTRA], menu_poketest);

	return resultado_principal && resultado_buscar && resultado_mostrar;
}

bool menu_poketest_comenzar(menu_poketest_t *menu_poketest)
{
	if (!menu_poketest)
		return false;

	while (menu_poketest->salir == false) {
		menu_mostrar_completo(menu_poketest->menu[MENU_PRINCIPAL]);
		char opcion_elegida = seleccionar_opcion();
		bool opcion_ejecutada = menu_ejecutar_opcion(
			menu_poketest->menu[MENU_PRINCIPAL], opcion_elegida);
		while (opcion_ejecutada == false) {
			mostrar_mensaje_opcion_invalida();
			menu_mostrar_completo(
				menu_poketest->menu[MENU_PRINCIPAL]);
			opcion_elegida = seleccionar_opcion(
				menu_poketest->menu[MENU_PRINCIPAL]);
			opcion_ejecutada = menu_ejecutar_opcion(
				menu_poketest->menu[MENU_PRINCIPAL],
				opcion_elegida);
		}
	}

	return true;
}

void menu_poketest_destruir(menu_poketest_t *menu_poketest)
{
	if (menu_poketest == NULL)
		return;
	for (int i = 0; i < CANTIDAD_MENUS; i++)
		menu_destruir(menu_poketest->menu[i]);
	tp1_destruir(menu_poketest->archivo_pokemones);
	free(menu_poketest);
}

int main(int argc, char *argv[])
{
	menu_poketest_t *menu_juego = menu_poketest_crear();
	if (menu_juego == NULL) {
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD FALLO ANSI_COLOR_RESET
		       "\n");
		return ERROR;
	}

	bool resultado_construir = menu_poketest_construir(menu_juego);
	if (resultado_construir == false) {
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD FALLO ANSI_COLOR_RESET
		       "\n");
		return ERROR;
	}
	menu_poketest_comenzar(menu_juego);
	menu_poketest_destruir(menu_juego);

	return 0;
}