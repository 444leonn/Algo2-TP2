#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#include "src/menu.h"
#include "src/juego_poketest.h"
#include "src/aux.h"
#include "src/tp1.h"
#include "src/aux_tp1.h"
#include "src/abb.h"
#include "src/ansi.h"
#include "src/constantes.h"

enum MENUS { MENU_PRINCIPAL, MENU_BUSQUEDA, MENU_MUESTRA };

typedef struct menu_poketest {
	menu_t *menu[CANTIDAD_MENUS];
	int semilla;
	size_t cantidad_tarjetas;
	enum formato_muestra formato;
	tp1_t *archivo_pokemones;
	abb_t *abb_pokemones_nombre;
	bool salir;
} menu_poketest_t;

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
	mostrar_mensaje_continuar();

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
	mostrar_mensaje_continuar();

	free(nombre_buscado);

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
	mostrar_mensaje_continuar();

	return true;
}

bool mostrar_pokemon_abb(void *_pokemon, void *extra)
{
	struct pokemon *pokemon = _pokemon;

	char *tipo = parsear_tipo(pokemon->tipo);

	printf("| ID: %d | NOMBRE: %s | TIPO: %s | ATAQUE: %d | DEFENSA: %d | VELOCIDAD: %d |\n",
	       pokemon->id, pokemon->nombre, tipo, pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);

	return true;
}

int comparador_pokemones_nombre(const void *_pokemon_a, const void *_pokemon_b)
{
	const struct pokemon *pokemon_a = _pokemon_a;
	const struct pokemon *pokemon_b = _pokemon_b;

	return strcmp(pokemon_a->nombre, pokemon_b->nombre);
}

bool cargar_pokemones_abb(struct pokemon *pokemon, void *_abb)
{
	abb_t *abb = _abb;
	return abb_insertar(abb, pokemon);
}

bool mostrar_nombre(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	tp1_con_cada_pokemon(menu_poketest->archivo_pokemones,
			     cargar_pokemones_abb,
			     menu_poketest->abb_pokemones_nombre);
	abb_con_cada_elemento(menu_poketest->abb_pokemones_nombre, ABB_INORDEN,
			      mostrar_pokemon_abb, NULL);
	mostrar_mensaje_continuar();

	return true;
}

bool mostrar_id(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	tp1_con_cada_pokemon(menu_poketest->archivo_pokemones, mostrar_pokemon,
			     NULL);
	mostrar_mensaje_continuar();

	return true;
}

void ejecutar_menu(menu_t *menu)
{
	menu_mostrar_completo(menu);
	char opcion = seleccionar_opcion();
	bool ejecutada = menu_ejecutar_opcion(menu, opcion);

	while (ejecutada == false) {
		mostrar_mensaje_opcion_invalida();
		opcion = seleccionar_opcion();
		ejecutada = menu_ejecutar_opcion(menu, opcion);
	}
}

bool buscar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	if (validar_archivo_pokemones(menu_poketest) == true)
		return true;
	ejecutar_menu(menu_poketest->menu[MENU_BUSQUEDA]);

	return true;
}

bool mostrar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	if (validar_archivo_pokemones(menu_poketest) == true)
		return true;
	ejecutar_menu(menu_poketest->menu[MENU_MUESTRA]);

	return true;
}

bool jugar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;

	if (validar_archivo_pokemones(menu_poketest) == true)
		return true;

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
	if (scanf("%d", &menu_poketest->semilla) != 1)
		limpiar_buffer();

	return jugar(menu_poketest);
}

void mostrar_formato_predeterminado_1(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return;
	
	opcion_t *opcion = _opcion;
	printf(ANSI_COLOR_BOLD "%s) " ANSI_COLOR_RESET "%s", clave,
	       opcion->descripcion);
	printf("\n");
}

void mostrar_formato_predeterminado_2(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return;

	opcion_t *opcion = _opcion;

	printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET ". "
					      "%s",
	       clave, opcion->descripcion);
	printf("\n");
}

void mostrar_formato_predeterminado_3(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return;

	opcion_t *opcion = _opcion;

	printf(ANSI_COLOR_GREEN "┌—————┐\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET ANSI_COLOR_BOLD
				"  %s  " ANSI_COLOR_RESET ANSI_COLOR_GREEN
				"|" ANSI_COLOR_RESET,
	       clave);
	printf(" %s\n", opcion->descripcion);
	printf(ANSI_COLOR_GREEN "└—————┘\n\n" ANSI_COLOR_RESET);
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
	bool reservado = true;

	menu_poketest->menu[MENU_PRINCIPAL] = menu_crear(NOMBRE_JUEGO);
	menu_poketest->menu[MENU_BUSQUEDA] = menu_crear(BUSCAR);
	menu_poketest->menu[MENU_MUESTRA] = menu_crear(MOSTRAR);
	menu_poketest->abb_pokemones_nombre =
		abb_crear(comparador_pokemones_nombre);

	for (int i = 0; i < CANTIDAD_MENUS; i++)
		if (menu_poketest->menu[i] == NULL)
			reservado = false;

	if (reservado == false || menu_poketest->abb_pokemones_nombre == NULL) {
		for (int i = 0; i < CANTIDAD_MENUS; i++)
			menu_destruir(menu_poketest->menu[i]);
		abb_destruir(menu_poketest->abb_pokemones_nombre);
		free(menu_poketest);
		return NULL;
	}
	menu_poketest->formato = FORMATO_1;
	menu_poketest->cantidad_tarjetas = CANTIDAD_TARJETAS;

	return menu_poketest;
}

bool construir_menu_principal(menu_t *menu_principal,
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

bool construir_menu_busqueda(menu_t *menu_busqueda,
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

bool construir_menu_mostrar(menu_t *menu_muestra,
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
	bool resultado_principal = construir_menu_principal(
		menu_poketest->menu[MENU_PRINCIPAL], menu_poketest);
	bool resultado_buscar = construir_menu_busqueda(
		menu_poketest->menu[MENU_BUSQUEDA], menu_poketest);
	bool resultado_mostrar = construir_menu_mostrar(
		menu_poketest->menu[MENU_MUESTRA], menu_poketest);

	return resultado_principal && resultado_buscar && resultado_mostrar;
}

bool menu_poketest_comenzar(menu_poketest_t *menu_poketest)
{
	if (!menu_poketest)
		return false;

	while (menu_poketest->salir == false)
		ejecutar_menu(menu_poketest->menu[MENU_PRINCIPAL]);
	return true;
}

void menu_poketest_destruir(menu_poketest_t *menu_poketest)
{
	if (menu_poketest == NULL)
		return;
	for (int i = 0; i < CANTIDAD_MENUS; i++)
		menu_destruir(menu_poketest->menu[i]);
	abb_destruir(menu_poketest->abb_pokemones_nombre);
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