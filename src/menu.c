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

void menu_mostrar_nombre(menu_t *menu)
{
    if (menu == NULL || menu->tiene_nombre == false)
        return;
    size_t opcion_larga = (menu->largo_nombre > menu->largo_opcion) ? menu->largo_nombre : menu->largo_opcion;

    size_t cantidad_iguales = opcion_larga + 6;
    size_t cantidad_espacios = opcion_larga + 4;
    size_t cantidad_espacios_impresion = cantidad_espacios / 3;

    for (int i = 0; i < cantidad_iguales; i++)
        printf("=");
    
    printf("\n");
    printf("|");
    for (int i = 0; i < cantidad_espacios; i++)
        printf(" ");    
    printf("|");

    printf("\n");
    printf("|");
    for (int i = 0; i < cantidad_espacios_impresion; i++)
        printf(" ");
    printf("%s", menu->nombre);
    for (int i = 0; i < cantidad_espacios_impresion; i++)
        printf(" ");
    printf("|");

    printf("\n");
    printf("|");
    for (int i = 0; i < cantidad_espacios; i++)
        printf(" ");    
    printf("|");

    printf("\n");
    for (int i = 0; i < cantidad_iguales; i++)
        printf("=");
}
