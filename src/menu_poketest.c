#include "menu_poketest.h"
#include "juego_poketest.h"

#include <stdio.h>
#include <unistd.h>
#include <stdio_ext.h>

typedef struct aux_pokemones {
	struct pokemon *pokemones;
	size_t cantidad;
} aux_pokemones_t;

void limpiar_pantalla_tp2()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
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
	}

    return true;
}

bool mostrar_menu_con_opcion_volver(menu_poketest_t *menu_poketest, enum MENUS menu_actual, char *opcion_seleccionada)
{
    if (menu_poketest == NULL || opcion_seleccionada == NULL)
        return true;

	limpiar_pantalla_tp2();
	menu_mostrar_nombre(menu_poketest->menu[menu_actual]);
    menu_mostrar(menu_poketest->menu[menu_actual], menu_poketest->formato);
	__fpurge(stdin);
    *opcion_seleccionada = menu_seleccionar_opcion(menu_poketest->menu[menu_actual]);
	if (evaluar_volver_menu(&opcion_seleccionada) == true)
		return true;
    return true;
}

bool cargar_archivo(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;
	
	getchar();
	printf(ANSI_COLOR_BLUE MENSAJE_INSTRUCCION_ARCHIVO ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_MAGENTA MENSAJE_VOLVER ANSI_COLOR_RESET "\n\n");
	
	printf(ANSI_COLOR_BOLD MENSAJE_ARCHIVO ANSI_COLOR_RESET);
	__fpurge(stdin);
	char *nombre_ingresado = leer_linea(stdin);
	if (nombre_ingresado == NULL) {
		printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET "\n\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}

	if (evaluar_volver_menu(nombre_ingresado) == true)
		return true;
	
	menu_poketest->archivo_pokemones = tp1_leer_archivo(nombre_ingresado);
	if (menu_poketest->archivo_pokemones == NULL) {
		printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET "\n\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		free(nombre_ingresado);
		return true;
	}
	free(nombre_ingresado);

	printf("\n" ANSI_COLOR_BOLD MENSAJE_CARGADO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();

	return true;
}

bool agregar_aux_pokemon(struct pokemon *pokemon, void *_aux_pokemones)
{
	if (_aux_pokemones == NULL)
		return false;
	
	aux_pokemones_t *aux_pokemones = _aux_pokemones;
	aux_pokemones->pokemones[aux_pokemones->cantidad] = *pokemon;

	size_t tamaño = strlen(pokemon->nombre) + 1;
	aux_pokemones->pokemones[aux_pokemones->cantidad].nombre = copiar_nombre(tamaño, pokemon->nombre);

	aux_pokemones->cantidad++;

	return true;
}

bool opcion_nombre(void *ctx)
{
	if (ctx == NULL)
		return false;	
	menu_poketest_t *menu_poketest = ctx;
	
	if (menu_poketest->opcion_busqueda == true) {
		printf(ANSI_COLOR_BOLD MENSAJE_INGRESAR_NOMBRE ANSI_COLOR_RESET);
		__fpurge(stdin);
		char *nombre_buscado = leer_linea(stdin);
		if (nombre_buscado == NULL) {
			printf(ANSI_COLOR_RED FALLO ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
			esperar_segundos(2);
			return true;
		}

		struct pokemon *pokemon_buscado = tp1_buscar_nombre(menu_poketest->archivo_pokemones, nombre_buscado);
		if (pokemon_buscado == NULL) {
			printf(ANSI_COLOR_RED NO_ENCONTRADO ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
			esperar_segundos(2);
			return true;
		}
		printf("\n" ANSI_COLOR_GREEN MENSAJE_POKEMON_ENCONTRADO ANSI_COLOR_RESET "\n");
		mostrar_pokemon(pokemon_buscado, NULL);
		printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
		__fpurge(stdin);
		getchar();

		free(nombre_buscado);
	}

	aux_pokemones_t aux_pokemones;
	aux_pokemones.cantidad = 0;
	size_t cantidad_total = tp1_cantidad(menu_poketest->archivo_pokemones);
	aux_pokemones.pokemones = malloc(cantidad_total * sizeof(struct pokemon));

	if (aux_pokemones.pokemones != NULL) {
		tp1_con_cada_pokemon(menu_poketest->archivo_pokemones, agregar_aux_pokemon, &aux_pokemones);
		
		ordenar_pokemones(aux_pokemones.pokemones, aux_pokemones.cantidad, comparador_nombre);
		for (int i = 0; i < aux_pokemones.cantidad; i++)
			mostrar_pokemon(&aux_pokemones.pokemones[i], NULL);

	}
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	__fpurge(stdin);
	getchar();

	free(aux_pokemones.pokemones);

	return true;
}

bool opcion_id(void *ctx)
{
	if (ctx == NULL)
		return false;	
	menu_poketest_t *menu_poketest = ctx;
	
	if (menu_poketest->opcion_busqueda == true) {
		printf(ANSI_COLOR_BOLD MENSAJE_INGRESAR_ID ANSI_COLOR_RESET);
		__fpurge(stdin);
		char *_id_buscado = leer_linea(stdin);
		if (_id_buscado == NULL) {
			printf(ANSI_COLOR_RED FALLO ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
			esperar_segundos(2);
			return true;
		}
	
		int id_buscado = atoi(_id_buscado);
		free(_id_buscado);

		struct pokemon *pokemon_buscado = tp1_buscar_id(menu_poketest->archivo_pokemones, id_buscado);
		if (pokemon_buscado == NULL) {
			printf(ANSI_COLOR_RED NO_ENCONTRADO ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
			esperar_segundos(2);
			return true;
		}
		printf(ANSI_COLOR_GREEN MENSAJE_POKEMON_ENCONTRADO ANSI_COLOR_RESET "\n");
		mostrar_pokemon(pokemon_buscado, NULL);
		printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
		getchar();
	}

	tp1_con_cada_pokemon(menu_poketest->archivo_pokemones, mostrar_pokemon, NULL);
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	__fpurge(stdin);
	getchar();

	return true;
}

bool buscar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;
	menu_poketest->opcion_busqueda = true;

    if (validar_archivo_pokemones(menu_poketest) == true)
        return true;

    char opcion;
    if (mostrar_menu_con_opcion_volver(menu_poketest, MENU_BUSQUEDA, &opcion) == true)
        return true;

    bool opcion_ejecutada = menu_ejecutar_opcion(menu_poketest->menu[MENU_BUSQUEDA], opcion);
	while (opcion_ejecutada == false) {
		__fpurge(stdin);
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
		esperar_segundos(2);
		opcion = menu_seleccionar_opcion(menu_poketest->menu[MENU_BUSQUEDA]);
		if (evaluar_volver_menu(&opcion) == true)
			return true;
		opcion_ejecutada = menu_ejecutar_opcion(menu_poketest->menu[MENU_BUSQUEDA], opcion);
	}

	return true;
}

bool mostrar(void *ctx)
{
	if (ctx == NULL)
		return false;
	menu_poketest_t *menu_poketest = ctx;
	menu_poketest->opcion_busqueda = false;

    if (validar_archivo_pokemones(menu_poketest) == true)
        return true;

    char opcion;
    if (mostrar_menu_con_opcion_volver(menu_poketest, MENU_MUESTRA, &opcion) == true)
        return true;

    bool opcion_ejecutada = menu_ejecutar_opcion(menu_poketest->menu[MENU_MUESTRA], opcion);
	while (opcion_ejecutada == false) {
		__fpurge(stdin);
		printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
		esperar_segundos(2);
		opcion = menu_seleccionar_opcion(menu_poketest->menu[MENU_MUESTRA]);
		if (evaluar_volver_menu(opcion) == true)
			return true;
		opcion_ejecutada = menu_ejecutar_opcion(menu_poketest->menu[MENU_MUESTRA], opcion);
	}

	return true;
}

// TODO: agregar funcionalidad de juego
bool jugar(void *ctx)
{
	if (ctx == NULL)
		return false;
	
	return true;
}

// TODO: agregar funcionalidad de juego con semilla (ver de si usar jugar, pero cambiando la semilla).
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

	menu_poketest_t *juego__poketest = ctx;
	
	printf(ANSI_COLOR_BLUE MENSAJE_ESTILO ANSI_COLOR_RESET "\n");
	esperar_segundos(2);
	getchar();
	if (juego__poketest->formato == FORMATO_1)
		juego__poketest->formato = FORMATO_2;
	else if (juego__poketest->formato == FORMATO_2)
		juego__poketest->formato = FORMATO_3;
	else
		juego__poketest->formato = FORMATO_1;

	return true;
}

bool salir(void *ctx)
{
	if (ctx == NULL)
		return false;

	menu_poketest_t *menu_poketest = ctx;

	if (menu_poketest->archivo_pokemones != NULL)
		tp1_destruir(menu_poketest->archivo_pokemones);

	menu_poketest->salir = true;

	return true;
}

menu_poketest_t *menu_poketest_crear()
{
    menu_poketest_t *menu_poketest = calloc(1, sizeof(menu_poketest_t));
    if (menu_poketest != NULL) {
        menu_poketest->menu[MENU_PRINCIPAL] = menu_crear(NOMBRE_JUEGO);
        menu_poketest->menu[MENU_BUSQUEDA] = menu_crear(BUSCAR);
        menu_poketest->menu[MENU_MUESTRA] = menu_crear(MOSTRAR);
        if (!menu_poketest->menu[MENU_PRINCIPAL] || !menu_poketest->menu[MENU_BUSQUEDA] || !menu_poketest->menu[MENU_MUESTRA]) {
            menu_destruir(menu_poketest->menu[MENU_PRINCIPAL]);
            menu_destruir(menu_poketest->menu[MENU_BUSQUEDA]);
            menu_destruir(menu_poketest->menu[MENU_MUESTRA]);
            free(menu_poketest);
        
            return NULL;
        }
        menu_poketest->formato = FORMATO_1;
        menu_poketest->semilla = CANTIDAD_SEMILLA;

        return menu_poketest;
    }
    return NULL;
}

bool agregar_opciones_menu_principal(menu_t *menu_principal, menu_poketest_t *menu_poketest)
{
	if (menu_principal == NULL || menu_poketest == NULL)
		return false;
	
	bool opcion_1 = menu_agregar_opcion(menu_principal, CHAR_C, CARGAR_ARCHIVO, cargar_archivo, menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_principal, CHAR_B, BUSCAR, buscar, menu_poketest);
	bool opcion_3 = menu_agregar_opcion(menu_principal, CHAR_M, MOSTRAR, mostrar, menu_poketest);
	bool opcion_4 = menu_agregar_opcion(menu_principal, CHAR_J, JUGAR, jugar, menu_poketest);
	bool opcion_5 = menu_agregar_opcion(menu_principal, CHAR_S, JUGAR_SEMILLA, jugar_semilla, menu_poketest);
	bool opcion_6 = menu_agregar_opcion(menu_principal, CHAR_E, ESTILO, cambiar_estilo, menu_poketest);
	bool opcion_7 = menu_agregar_opcion(menu_principal, CHAR_Q, SALIR, salir, menu_poketest);

	bool resultado = opcion_1 && opcion_2 && opcion_3 && opcion_4 && opcion_5 && opcion_6 && opcion_7;

	return resultado;
}

bool agregar_opcion_menu_busqueda(menu_t *menu_busqueda, menu_poketest_t *menu_poketest)
{
	if (menu_busqueda == NULL || menu_poketest == NULL)
		return false;
	
	bool opcion_1 = menu_agregar_opcion(menu_busqueda, CHAR_N, BUSCAR_NOMBRE, opcion_nombre, menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_busqueda, CHAR_I, BUSCAR_ID, opcion_id, menu_poketest);

    bool resultado = opcion_1 && opcion_2;

    return resultado;
}

bool agregar_opcion_menu_mostrar(menu_t *menu_muestra, menu_poketest_t *menu_poketest)
{
    if (menu_muestra == NULL || menu_poketest == NULL)
        return false;
    
    bool opcion_1 = menu_agregar_opcion(menu_muestra, CHAR_N, MOSTRAR_NOMBRE, opcion_nombre, menu_poketest);
	bool opcion_2 = menu_agregar_opcion(menu_muestra, CHAR_I, MOSTRAR_ID, opcion_id, menu_poketest);

    bool resultado = opcion_1 && opcion_2;

    return resultado;
}

bool menu_poketest_construir(menu_poketest_t *menu_poketest)
{
    if (menu_poketest == NULL)
        return false;
    
    bool resultado_principal = agregar_opciones_menu_principal(menu_poketest->menu[MENU_PRINCIPAL], menu_poketest);
    bool resultado_buscar = agregar_opcion_menu_busqueda(menu_poketest->menu[MENU_BUSQUEDA], menu_poketest);
    bool resultado_mostrar = agregar_opcion_menu_mostrar(menu_poketest->menu[MENU_MUESTRA], menu_poketest);

    return resultado_principal && resultado_buscar && resultado_mostrar;
}

bool *menu_poketest_comenzar(menu_poketest_t *menu_poketest)
{
	if (!menu_poketest || !menu_poketest->menu[MENU_PRINCIPAL] || !menu_poketest->menu[MENU_BUSQUEDA] || !menu_poketest->menu[MENU_MUESTRA] || !menu_poketest->archivo_pokemones)
		return false;

	while (menu_poketest->salir == false) {
		__fpurge(stdin);
		bool opcion_ejecutada = menu_mostrar_completo(menu_poketest->menu[MENU_PRINCIPAL], menu_poketest->formato);
		while (opcion_ejecutada == false) {
			__fpurge(stdin);
			printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET "\n");
			printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
			sleep(2);
			char opcion_elegida = menu_seleccionar_opcion(menu_poketest->menu[MENU_PRINCIPAL]);
			opcion_ejecutada = menu_ejecutar_opcion(menu_poketest->menu[MENU_PRINCIPAL], opcion_elegida);
		}
	}

	return true;
}

void menu_poketest_destruir(menu_poketest_t *menu_poketest)
{
	if (!menu_poketest || !menu_poketest->menu[MENU_PRINCIPAL] || !menu_poketest->menu[MENU_BUSQUEDA] || !menu_poketest->menu[MENU_MUESTRA] || !menu_poketest->archivo_pokemones)
		return;
		
	menu_destruir(menu_poketest->menu[MENU_PRINCIPAL]);
	menu_destruir(menu_poketest->menu[MENU_BUSQUEDA]);
	menu_destruir(menu_poketest->menu[MENU_MUESTRA]);
	tp1_destruir(menu_poketest->archivo_pokemones);
	free(menu_poketest);
}