#include "pa2m.h"
#include "src/constantes.h"
#include "src/menu.h"

bool funcion_prueba_menu(void *ctx)
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

void menu_mostrar_nombre_muestra_nombre_correctamemte_para_nombre_test_1()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_mostrar_nombre(menu);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_nombre_muestra_nombre_correctamemte_para_nombre_test_2()
{
	menu_t *menu = menu_crear(NOMBRE_JUEGO);

	menu_mostrar_nombre(menu);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_muestra_correctamemte_para_formato_1()
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

	menu_mostrar(menu, FORMATO_1);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_muestra_correctamemte_para_formato_2()
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

	menu_mostrar(menu, FORMATO_2);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_muestra_correctamemte_para_formato_3()
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

	menu_mostrar(menu, FORMATO_3);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_1()
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

	menu_mostrar_completo(menu, FORMATO_1);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_2()
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

	menu_mostrar_completo(menu, FORMATO_2);

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_3()
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

	menu_mostrar_completo(menu, FORMATO_3);

	if (menu != NULL)
		menu_destruir(menu);
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

	pa2m_nuevo_grupo("Pruebas de Mostrar:");
	menu_mostrar_nombre_muestra_nombre_correctamemte_para_nombre_test_1();
	menu_mostrar_nombre_muestra_nombre_correctamemte_para_nombre_test_2();

	pa2m_nuevo_grupo("Pruebas de Mostrar Opciones:");
	menu_mostrar_muestra_correctamemte_para_formato_1();
	menu_mostrar_muestra_correctamemte_para_formato_2();
	menu_mostrar_muestra_correctamemte_para_formato_3();

	pa2m_nuevo_grupo("Pruebas Mostrar Completo: ");
	// menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_1();
	// printf("\n\n\n\n");
	//
	// menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_2();
	// printf("\n\n\n\n");

	menu_mostrar_completo_muestra_el_menu_correctamente_para_formato_3();
	printf("\n\n\n\n");

	return pa2m_mostrar_reporte();
}