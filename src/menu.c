#include "menu.h"
#include "constantes.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>

char *copiar_clave(char *clave)
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
        menu->nombre = copiar_clave(nombre);
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
    
    bool resultado = hash_insertar(menu->opciones, c, nueva_opcion, NULL);

    size_t largo_evaluar = strlen(descripcion);
    if (largo_evaluar > menu->largo_opcion && resultado == true)
        menu->largo_opcion = largo_evaluar;

    return resultado;
}