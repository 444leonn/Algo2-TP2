#include "menu.h"
#include "constantes.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct formato_custom {
	void (*formato_custom)(char, char *);
} formato_custom_t;

// Estructura para almacenar cada opcion dentro de la tabla de hash de opciones
typedef struct opcion {
	char opcion;
	char *descripcion;
	bool (*funcion)(void *);
	void *ctx;
} opcion_t;

struct menu {
	char *nombre;
	bool tiene_nombre;
	hash_t *opciones;
	formato_custom_t formato_custom;
	enum formato_muestra formato;
	size_t cantidad;
	size_t largo_nombre;
	size_t largo_opcion;
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

void limpiar_buffer_menu()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void limpiar_pantalla()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

menu_t *menu_crear(char *nombre)
{
	struct menu *menu = calloc(1, sizeof(struct menu));
	if (menu == NULL)
		return NULL;

	if (nombre != NULL) {
		menu->nombre = copiar_nombre_aux(nombre);
		menu->largo_nombre = strlen(nombre);
		menu->tiene_nombre = true;
	}

	menu->opciones = hash_crear(CAPACIDAD_TABLA);
	if (menu->opciones == NULL ||
	    (menu->nombre == NULL && menu->tiene_nombre == true)) {
		free(menu->nombre);
		free(menu);
		return NULL;
	}
	menu->formato_custom.formato_custom = NULL;
	menu->formato = FORMATO_1;

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

	size_t largo_evaluar = strlen(descripcion);
	if (largo_evaluar > menu->largo_opcion && resultado == true)
		menu->largo_opcion = largo_evaluar;

	return resultado;
}

void menu_agregar_formato(menu_t *menu, void (*f_formato)(char, char *))
{
	if (menu == NULL || f_formato == NULL)
		return;

	menu->formato_custom.formato_custom = f_formato;
}

bool menu_seleccionar_formato(menu_t *menu, enum formato_muestra formato)
{
	if (menu == NULL)
		return false;
	menu->formato = formato;
	return true;
}

void mostrar_linea_iguales(size_t cantidad)
{
	for (int i = 0; i < cantidad; i++)
		printf(ANSI_COLOR_BOLD "=" ANSI_COLOR_RESET);
}

void mostrar_linea_completa(size_t cantidad)
{
	printf(ANSI_COLOR_BOLD "|" ANSI_COLOR_RESET);
	for (int i = 0; i < cantidad; i++)
		printf(" ");
	printf(ANSI_COLOR_BOLD "|" ANSI_COLOR_RESET);
}

void mostrar_linea_palabra(size_t cantidad, char *palabra,
			   size_t cantidad_total)
{
	printf(ANSI_COLOR_BOLD "|" ANSI_COLOR_RESET);
	size_t j = 0;
	for (int i = 0; i < cantidad + 2; i++) {
		printf(" ");
		j++;
	}

	printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET,
	       palabra);
	j += strlen(palabra);

	for (int i = 0; i < cantidad; i++) {
		printf(" ");
		j++;
	}
	if (j < cantidad_total) {
		for (int i = (int)j; i < cantidad_total; i++)
			printf(" ");
	}
	printf(ANSI_COLOR_BOLD "|" ANSI_COLOR_RESET);
}

void menu_mostrar_nombre(menu_t *menu)
{
	if (menu == NULL || menu->tiene_nombre == false)
		return;

	size_t cantidad_iguales = menu->largo_nombre * 4;
	size_t cantidad_espacios = cantidad_iguales - 2;
	size_t cantidad_espacios_impresion = cantidad_iguales / 3;

	mostrar_linea_iguales(cantidad_iguales);
	printf("\n");
	mostrar_linea_completa(cantidad_espacios);
	printf("\n");
	mostrar_linea_palabra(cantidad_espacios_impresion, menu->nombre,
			      cantidad_espacios);
	printf("\n");
	mostrar_linea_completa(cantidad_espacios);
	printf("\n");
	mostrar_linea_iguales(cantidad_iguales);
	printf("\n");
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

void menu_mostrar(menu_t *menu)
{
	if (menu == NULL || menu->opciones == NULL)
		return;

	hash_iterar(menu->opciones, menu_mostrar_opcion, menu);
}

bool menu_ejecutar_opcion(menu_t *menu, char c)
{
	if (menu == NULL || menu->opciones == NULL || c == '\0')
		return false;

	char _c[] = { c, '\0' };

	limpiar_pantalla();
	opcion_t *opcion = hash_buscar(menu->opciones, _c);
	if (opcion != NULL) {
		opcion->funcion(opcion->ctx);
		return true;
	}

	return false;
}

void menu_mostrar_completo(menu_t *menu)
{
	if (menu == NULL)
		return;

	limpiar_pantalla();
	menu_mostrar_nombre(menu);
	menu_mostrar(menu);
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