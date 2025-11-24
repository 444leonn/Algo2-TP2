#include "pa2m.h"
#include "src/constantes.h"
#include "src/menu.h"
#include "src/juego_poketest.h"
#include "src/tp1.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

bool funcion_prueba_menu(void *ctx)
{
	return true;
}

bool funcion_prueba_muestra(char *clave, void *opcion, void *aux)
{
	return true;
}

void menu_crear_sin_nombre_lo_crea_correctamente()
{
	menu_t *menu = menu_crear(NULL);

	pa2m_afirmar(menu != NULL,
		     "Crear un Menú sin nombre lo crea correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_crear_con_nombre_lo_crea_correctamente()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	pa2m_afirmar(menu != NULL,
		     "Crear un menú con nombre lo crea correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_menu_invalido()
{
	bool resultado = menu_agregar_opcion(NULL, 'a', "Opcion Prueba",
					     funcion_prueba_menu, NULL);
	pa2m_afirmar(
		resultado == false,
		"Intentar agregar una opción en un Menú inválido devuelve false.");
}

void menu_agregar_opcion_devuelve_false_si_caracter_elegido_es_invalido()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, '\0', "Opcion Prueba",
					     funcion_prueba_menu, NULL);

	pa2m_afirmar(
		resultado == false,
		"Intentar agregar una opción en un Menú con caracter inválido devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_descripcion_invalida()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado =
		menu_agregar_opcion(menu, 'a', NULL, funcion_prueba_menu, NULL);

	pa2m_afirmar(
		resultado == false,
		"Intentar agregar una opción en un Menú con descripción inválida devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_opcion_sin_funcion()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado =
		menu_agregar_opcion(menu, 'a', "Opcion Prueba", NULL, NULL);

	pa2m_afirmar(
		resultado == false,
		"Intentar agregar una opción en un Menú con sin funcion devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_true_si_agrega_opcion_correctamente()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, 'a', "Opcion Prueba",
					     funcion_prueba_menu, NULL);

	pa2m_afirmar(resultado == true,
		     "Agregar un opción en Menú la agrega correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_quitar_opcion_devuelve_false_para_menu_invalido()
{
	bool resultado = menu_quitar_opcion(NULL, CHAR_A);

	pa2m_afirmar(resultado == false,
		     "Quitar opcion de Menu invalido devuelve false.");
}

void menu_quitar_opcion_devuelve_false_para_caracter_invalido()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_agregar_opcion(menu, 'a', "Opcion Prueba", funcion_prueba_menu,
			    NULL);

	bool resultado = menu_quitar_opcion(menu, '\0');

	pa2m_afirmar(resultado == false,
		     "Quitar opcion invalida de Menu devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_quitar_opcion_devuelve_true_si_quita_opcion()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_agregar_opcion(menu, CHAR_A, "Opcion Prueba", funcion_prueba_menu,
			    NULL);

	bool resultado = menu_quitar_opcion(menu, CHAR_A);

	pa2m_afirmar(resultado == true, "Quitar opcion de Menu devuelve true.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_obtener_nombre_devuelve_NULL_para_menu_invalido()
{
	char *resultado = menu_obtener_nombre(NULL);

	pa2m_afirmar(resultado == NULL,
		     "Obtener nombre del menu invalido devuelve NULL.");
}

void menu_obtener_nombre_devuelve_NULL_para_menu_sin_nombre()
{
	menu_t *menu = menu_crear(NULL);

	char *resultado = menu_obtener_nombre(menu);

	pa2m_afirmar(resultado == NULL,
		     "Obtener nombre de menu sin nombre devuelve NULL.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_obtener_nombre_devuelve_nombre_del_menu()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	char *resultado = menu_obtener_nombre(menu);

	pa2m_afirmar(
		resultado != NULL,
		"Obtener nombre de menu devuelve correctamente el nombre.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_muestra_correctamemte()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_agregar_opcion(menu, CHAR_C, CARGAR_ARCHIVO, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_B, BUSCAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_M, MOSTRAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_J, JUGAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_S, JUGAR_SEMILLA, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_E, ESTILO, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_Q, SALIR, funcion_prueba_menu, NULL);

	size_t cantidad_mostrada = menu_mostrar(menu, funcion_prueba_muestra);

	pa2m_afirmar(
		cantidad_mostrada == 7,
		"Menu mostrar, devuelve correctamente la cantidad de los elementos. (devolvio: %ld, esperado: %ld)",
		cantidad_mostrada, 7);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_ejecutar_opcion_devuelve_false_para_menu_invalido()
{
	bool resultado = menu_ejecutar_opcion(NULL, CHAR_A);

	pa2m_afirmar(resultado == false,
		     "Ejecutar opcion en menu invalido devuelve false.");
}

void menu_ejecutar_opcion_devuelve_false_para_opcion_invalida()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_ejecutar_opcion(menu, '\0');

	pa2m_afirmar(resultado == false,
		     "Ejecutar una opcion invalida en menu devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_ejecutar_opcion_devuelve_false_para_opcion_inexistente()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_agregar_opcion(menu, CHAR_C, CARGAR_ARCHIVO, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_B, BUSCAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_M, MOSTRAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_J, JUGAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_S, JUGAR_SEMILLA, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_E, ESTILO, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_Q, SALIR, funcion_prueba_menu, NULL);

	bool resultado = menu_ejecutar_opcion(menu, CHAR_A);

	pa2m_afirmar(resultado == false,
		     "Ejecutar una opcion inexistente en menu devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_ejecutar_opcion_devuelve_true_si_logra_ejecutar_la_opcion()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_agregar_opcion(menu, CHAR_C, CARGAR_ARCHIVO, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_B, BUSCAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_M, MOSTRAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_J, JUGAR, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_S, JUGAR_SEMILLA, funcion_prueba_menu,
			    NULL);
	menu_agregar_opcion(menu, CHAR_E, ESTILO, funcion_prueba_menu, NULL);
	menu_agregar_opcion(menu, CHAR_Q, SALIR, funcion_prueba_menu, NULL);

	bool resultado = menu_ejecutar_opcion(menu, CHAR_C);

	pa2m_afirmar(resultado == true,
		     "Ejecutar una opcion en menu devuelve true.");

	if (menu != NULL)
		menu_destruir(menu);
}

void juego_poketest_crear_devuelve_NULL_para_archivo_invalido()
{
	juego_poketest_t *juego =
		juego_poketest_crear(NULL, 0, CANTIDAD_TARJETAS);

	pa2m_afirmar(
		juego == NULL,
		"Crear un juego con un archivo de pokemones invalido devuelve NULL.");
}

void juego_poketest_crear_devuelve_NULL_para_cantidad_invalida_de_tarjetas()
{
	tp1_t *archivo_pokemones =
		tp1_leer_archivo("./ejemplos/semilla-base.csv");

	juego_poketest_t *juego = juego_poketest_crear(archivo_pokemones, 0, 0);

	pa2m_afirmar(
		juego == NULL,
		"Crear un juego con una cantidad invalida de tarjetas devuelve NULL.");

	if (archivo_pokemones != NULL)
		tp1_destruir(archivo_pokemones);
}

void juego_poketest_crear_lo_crea_correctamente()
{
	tp1_t *archivo_pokemones =
		tp1_leer_archivo("./ejemplos/semilla-base.csv");

	juego_poketest_t *juego =
		juego_poketest_crear(archivo_pokemones, 0, CANTIDAD_TARJETAS);

	pa2m_afirmar(juego != NULL,
		     "Crear un juego lo crea y devuelve correctamente.");

	if (archivo_pokemones != NULL)
		tp1_destruir(archivo_pokemones);
	if (juego != NULL)
		juego_poketest_destruir(juego);
}

void juego_poketest_cargar_devuelve_0_para_juego_invalido()
{
	size_t resultado = juego_poketest_cargar(NULL);

	pa2m_afirmar(
		resultado == 0,
		"Intentar cargar un juego invalido, no lo carga y devuelve 0.");
}

void juego_poketest_cargar_devuelve_correctamente_cantidad_cargada_coincidente_con_cantidad_de_tarjetas()
{
	tp1_t *archivo_pokemones =
		tp1_leer_archivo("./ejemplos/semilla-base.csv");
	juego_poketest_t *juego =
		juego_poketest_crear(archivo_pokemones, 0, CANTIDAD_TARJETAS);

	size_t resultado = juego_poketest_cargar(juego);

	pa2m_afirmar(
		resultado == CANTIDAD_TARJETAS,
		"Cargar un juego, lo carga correctamente. (devolvio: %ld, esperado: %ld)",
		resultado, CANTIDAD_TARJETAS);

	if (archivo_pokemones != NULL)
		tp1_destruir(archivo_pokemones);
	if (juego != NULL)
		juego_poketest_destruir(juego);
}

void juego_poketest_ultimo_registro_devuelve_NULL_para_juego_invalido()
{
	registro_historial_t *registro = juego_poketest_ultimo_registro(NULL);

	pa2m_afirmar(
		registro == NULL,
		"Intentar obtener el ultimo registro de un juego invalido devuelve NULL.");
}

void juego_poketest_ultimo_registro_devuelve_NULL_para_juego_sin_registros()
{
	tp1_t *archivo_pokemones =
		tp1_leer_archivo("./ejemplos/semilla-base.csv");
	juego_poketest_t *juego =
		juego_poketest_crear(archivo_pokemones, 0, CANTIDAD_TARJETAS);

	juego_poketest_cargar(juego);

	registro_historial_t *ultimo = juego_poketest_ultimo_registro(juego);

	pa2m_afirmar(
		ultimo == NULL,
		"Intentar obtener el ultimo registro de un juego sin registros devuelve NULL.");

	if (juego != NULL)
		juego_poketest_destruir(juego);
	if (archivo_pokemones != NULL)
		tp1_destruir(archivo_pokemones);
}

void juego_poketest_ultimo_registro_devuelve_correctamente_el_registro()
{
	tp1_t *archivo_pokemones =
		tp1_leer_archivo("./ejemplos/semilla-base.csv");
	juego_poketest_t *juego =
		juego_poketest_crear(archivo_pokemones, 0, CANTIDAD_TARJETAS);

	juego_poketest_cargar(juego);

	juego_poketest_jugada(juego, J1, 1, 2);

	registro_historial_t *ultimo = juego_poketest_ultimo_registro(juego);

	pa2m_afirmar(
		ultimo != NULL,
		"Obtener el ultimo registro del juego lo devuelve correctamente.");

	if (juego != NULL)
		juego_poketest_destruir(juego);
	if (archivo_pokemones != NULL)
		tp1_destruir(archivo_pokemones);
}

int main()
{
	pa2m_nuevo_grupo("============== Pruebas TDA Menu ===============");
	pa2m_nuevo_grupo("Pruebas de Creacion:");
	menu_crear_sin_nombre_lo_crea_correctamente();
	menu_crear_con_nombre_lo_crea_correctamente();

	pa2m_nuevo_grupo("Pruebas de Agregar Opcion:");
	menu_agregar_opcion_devuelve_false_para_menu_invalido();
	menu_agregar_opcion_devuelve_false_si_caracter_elegido_es_invalido();
	menu_agregar_opcion_devuelve_false_para_descripcion_invalida();
	menu_agregar_opcion_devuelve_false_para_opcion_sin_funcion();
	menu_agregar_opcion_devuelve_true_si_agrega_opcion_correctamente();

	pa2m_nuevo_grupo("Pruebas de Quitar Opcion:");
	menu_quitar_opcion_devuelve_false_para_menu_invalido();
	menu_quitar_opcion_devuelve_false_para_caracter_invalido();
	menu_quitar_opcion_devuelve_true_si_quita_opcion();

	pa2m_nuevo_grupo("Pruebas de Obtener Nombre:");
	menu_obtener_nombre_devuelve_NULL_para_menu_invalido();
	menu_obtener_nombre_devuelve_NULL_para_menu_sin_nombre();
	menu_obtener_nombre_devuelve_nombre_del_menu();

	pa2m_nuevo_grupo("Pruebas de Mostrar:");
	menu_mostrar_muestra_correctamemte();

	pa2m_nuevo_grupo("Pruebas de Ejecutar Opcion:");
	menu_ejecutar_opcion_devuelve_false_para_menu_invalido();
	menu_ejecutar_opcion_devuelve_false_para_opcion_invalida();
	menu_ejecutar_opcion_devuelve_false_para_opcion_inexistente();
	menu_ejecutar_opcion_devuelve_true_si_logra_ejecutar_la_opcion();

	pa2m_nuevo_grupo("============== Pruebas TDA Juego ===============");
	pa2m_nuevo_grupo("Pruebas de Crear Juego:");
	juego_poketest_crear_devuelve_NULL_para_archivo_invalido();
	juego_poketest_crear_devuelve_NULL_para_cantidad_invalida_de_tarjetas();
	juego_poketest_crear_lo_crea_correctamente();

	pa2m_nuevo_grupo("Pruebas de Cargar Pokemones:");
	juego_poketest_cargar_devuelve_0_para_juego_invalido();
	juego_poketest_cargar_devuelve_correctamente_cantidad_cargada_coincidente_con_cantidad_de_tarjetas();

	pa2m_nuevo_grupo("Pruebas de Obtener Ultimo Registro:");
	juego_poketest_ultimo_registro_devuelve_NULL_para_juego_invalido();
	juego_poketest_ultimo_registro_devuelve_NULL_para_juego_sin_registros();
	juego_poketest_ultimo_registro_devuelve_correctamente_el_registro();

	return pa2m_mostrar_reporte();
}