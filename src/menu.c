#include "menu.h"

#include "aux.h"
#include "constantes.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct menu {
	char *nombre;
	hash_t *opciones;
	size_t cantidad;
};

char *copiar_nombre_aux(char *clave)
{
	if (clave == NULL)
		return NULL;

	size_t largo = strlen(clave) + 1;
	char *aux = malloc(largo * sizeof(char));
	if (aux == NULL)
		return NULL;

	return strcpy(aux, clave);
}

menu_t *menu_crear(char *nombre)
{
	struct menu *menu = calloc(1, sizeof(struct menu));
	if (menu == NULL)
		return NULL;

	if (nombre != NULL)
		menu->nombre = copiar_nombre_aux(nombre);
		
	menu->opciones = hash_crear(CAPACIDAD_TABLA);
	if (menu->opciones == NULL) {
		if (menu->nombre != NULL)
			free(menu->nombre);
		free(menu);
		return NULL;
	}

	return menu;
}

bool menu_agregar_opcion(menu_t *menu, char c, char *descripcion,
			 bool (*funcion)(void *), void *ctx)
{
	if (menu == NULL || c == '\0' || descripcion == NULL || funcion == NULL)
		return false;

	struct opcion *nueva_opcion = malloc(sizeof(struct opcion));
	if (nueva_opcion == NULL)
		return false;
	nueva_opcion->opcion = c;
	nueva_opcion->descripcion = copiar_nombre_aux(descripcion);
	nueva_opcion->funcion = funcion;
	nueva_opcion->ctx = ctx;

	char _c[] = { c, '\0' };
	bool resultado = hash_insertar(menu->opciones, _c, nueva_opcion, NULL);

	return resultado;
}

bool menu_quitar_opcion(menu_t *menu, char c)
{
	char _c[] = { c, '\0' };
	return hash_quitar(menu->opciones, _c);
}

void mostrar_formato_predeterminado_1(char clave, char *descripcion)
{
	printf(ANSI_COLOR_BOLD "%c) " ANSI_COLOR_RESET "%s", clave,
	       descripcion);
	printf("\n");
}

void mostrar_formato_predeterminado_2(char clave, char *descripcion)
{
	printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "%c" ANSI_COLOR_RESET ". "
					      "%s",
	       clave, descripcion);
	printf("\n");
}

void mostrar_formato_predeterminado_3(char clave, char *descripcion)
{
	printf(ANSI_COLOR_GREEN "┌—————┐\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET ANSI_COLOR_BOLD
				"  %c  " ANSI_COLOR_RESET ANSI_COLOR_GREEN
				"|" ANSI_COLOR_RESET,
	       clave);
	printf(" %s\n", descripcion);
	printf(ANSI_COLOR_GREEN "└—————┘\n\n" ANSI_COLOR_RESET);
}

bool menu_mostrar_opcion(char *clave, void *_opcion, void *_ctx)
{
	opcion_t *opcion = _opcion;
	menu_t *menu = _ctx;

	switch (menu->formato) {
	case FORMATO_1:
		mostrar_formato_predeterminado_1(opcion->opcion,
						 opcion->descripcion);
		break;

	case FORMATO_2:
		mostrar_formato_predeterminado_2(opcion->opcion,
						 opcion->descripcion);
		break;

	case FORMATO_3:
		mostrar_formato_predeterminado_3(opcion->opcion,
						 opcion->descripcion);
		break;

	case FORMATO_CUSTOM:
		if (menu->formato_custom.formato_custom != NULL)
			menu->formato_custom.formato_custom(
				opcion->opcion, opcion->descripcion);
		else
			mostrar_formato_predeterminado_1(opcion->opcion,
							 opcion->descripcion);
		break;

	default:
		break;
	}

	return true;
}

char *menu_obtener_nombre(menu_t *menu)
{
	if (menu != NULL)
		return menu->nombre;
	return NULL;
}

void menu_mostrar(menu_t *menu, bool (*funcion_muestra)(char *, void *, void *))
{
	if (menu == NULL || menu->opciones == NULL)
		return;

	hash_iterar(menu->opciones, funcion_muestra, NULL);
}

bool menu_ejecutar_opcion(menu_t *menu, char c)
{
	if (menu == NULL || menu->opciones == NULL || c == '\0')
		return false;

	char _c[] = { c, '\0' };

	opcion_t *opcion = hash_buscar(menu->opciones, _c);
	if (opcion != NULL) {
		opcion->funcion(opcion->ctx);
		return true;
	}

	return false;
}

void destructor_opciones(void *_opcion)
{
	if (_opcion != NULL) {
		opcion_t *opcion = _opcion;
		free(opcion->descripcion);
		free(opcion);
	}
}

void menu_destruir(menu_t *menu)
{
	if (menu == NULL)
		return;

	hash_destruir_todo(menu->opciones, destructor_opciones);
	free(menu->nombre);
	free(menu);
}