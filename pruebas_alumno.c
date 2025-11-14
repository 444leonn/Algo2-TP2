#include "pa2m.h"
#include "src/constantes.h"
#include "src/menu.h"

bool funcion_prueba_menu()
{
	return true;
}

void menu_crear_sin_nombre_lo_crea_correctamente()
{
	menu_t *menu = menu_crear(NULL);

	pa2m_afirmar(menu != NULL, "Crear un Menú sin nombre lo crea correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_crear_con_nombre_lo_crea_correctamente()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);
	
	pa2m_afirmar(menu != NULL, "Crear un menú con nombre lo crea correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_menu_invalido()
{
	bool resultado = menu_agregar_opcion(NULL, 'a', "Opcion Prueba", funcion_prueba_menu);
	pa2m_afirmar(resultado == false, "Intentar agregar una opción en un Menú inválido devuelve false.");
}

void menu_agregar_opcion_devuelve_false_si_caracter_elegido_es_invalido()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, '\0', "Opcion Prueba", funcion_prueba_menu);

	pa2m_afirmar(resultado == false, "Intentar agregar una opción en un Menú con caracter inválido devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_descripcion_invalida()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, 'a', NULL, funcion_prueba_menu);

	pa2m_afirmar(resultado == false, "Intentar agregar una opción en un Menú con descripción inválida devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_false_para_opcion_sin_funcion()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, 'a', "Opcion Prueba", NULL);

	pa2m_afirmar(resultado == false, "Intentar agregar una opción en un Menú con sin funcion devuelve false.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_agregar_opcion_devuelve_true_si_agrega_opcion_correctamente()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	bool resultado = menu_agregar_opcion(menu, 'a', "Opcion Prueba", funcion_prueba_menu);

	pa2m_afirmar(resultado == true, "Agregar un opción en Menú la agrega correctamente.");

	if (menu != NULL)
		menu_destruir(menu);
}

void menu_mostrar_nombre_muestra_nombre_correctamemte()
{
	menu_t *menu = menu_crear(MENU_PRUEBA);

	menu_mostrar_nombre(menu);
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

	pa2m_nuevo_grupo("Pruebas de Mostrar Nombre:");
	menu_mostrar_nombre_muestra_nombre_correctamemte();
	
	return pa2m_mostrar_reporte();
}
