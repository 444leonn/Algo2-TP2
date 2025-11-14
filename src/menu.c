#include "menu.h"
#include "constantes.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *copiar_clave_aux(char *clave)
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

    if (nombre != NULL) {
        menu->nombre = copiar_clave_aux(nombre);
        menu->largo_nombre = strlen(nombre);
        menu->tiene_nombre = true;
    }

    menu->opciones = hash_crear(CAPACIDAD_TABLA);
    if (menu->opciones == NULL || (menu->nombre == NULL && menu->tiene_nombre == true)) {
        free(menu->nombre);
        free(menu);
        return NULL;
    }

    return menu;
}

bool menu_agregar_opcion(menu_t *menu, char c, char *descripcion, bool (*funcion)(void *))
{
    if (menu == NULL || c == '\0' || descripcion == NULL || funcion == NULL)
        return false;

    struct opcion *nueva_opcion = malloc(sizeof(struct opcion));
    if (nueva_opcion == NULL)
        return false;
    
    bool resultado = hash_insertar(menu->opciones, &c, nueva_opcion, NULL);

    size_t largo_evaluar = strlen(descripcion);
    if (largo_evaluar > menu->largo_opcion && resultado == true)
        menu->largo_opcion = largo_evaluar;

    return resultado;
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

void mostrar_linea_palabra(size_t cantidad, char *palabra, size_t cantidad_total)
{
    printf(ANSI_COLOR_BOLD "|" ANSI_COLOR_RESET);
    size_t j = 0;
    for (int i = 0; i < cantidad; i++) {
        printf(" ");
        j++;
    }
    
    printf(ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, palabra);
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
    size_t opcion_larga = (menu->largo_nombre > menu->largo_opcion) ? menu->largo_nombre : menu->largo_opcion;

    size_t cantidad_iguales = opcion_larga * 4;
    size_t cantidad_espacios = cantidad_iguales - 2;
    size_t cantidad_espacios_impresion = cantidad_iguales / 3;

    mostrar_linea_iguales(cantidad_iguales);
    printf("\n");
    mostrar_linea_completa(cantidad_espacios);
    printf("\n");
    mostrar_linea_palabra(cantidad_espacios_impresion, menu->nombre, cantidad_espacios);
    printf("\n");
    mostrar_linea_completa(cantidad_espacios);
    printf("\n");
    mostrar_linea_iguales(cantidad_iguales);
    printf("\n");
}

