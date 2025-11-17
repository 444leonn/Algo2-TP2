#include "menu.h"
#include "constantes.h"
#include "ansi.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

void limpiar_buffer_menu() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

menu_t *menu_crear(char *nombre)
{
    struct menu *menu = calloc(1, sizeof(struct menu));
    if (menu == NULL)
        return NULL;

    if (nombre != NULL) {
        menu->nombre = copiar_nombre_aux(nombre);
        menu->largo_nombre = strlen(nombre);
        menu->largo_mayor = strlen(nombre);
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

bool menu_agregar_opcion(menu_t *menu, char c, char *descripcion, bool (*funcion)(void *), void *ctx)
{
    if (menu == NULL || c == '\0' || descripcion == NULL || funcion == NULL)
        return false;

    struct opcion *nueva_opcion = malloc(sizeof(struct opcion));
    if (nueva_opcion == NULL)
        return false;
    nueva_opcion->descripcion = copiar_nombre_aux(descripcion);
    nueva_opcion->funcion = funcion;
    nueva_opcion->ctx = ctx;

    char _c[] = { c, '\0' };
    bool resultado = hash_insertar(menu->opciones, _c, nueva_opcion, NULL);

    size_t largo_evaluar = strlen(descripcion);
    if (largo_evaluar > menu->largo_opcion && resultado == true)
        menu->largo_opcion = largo_evaluar;

    if (resultado == true)
        menu->largo_mayor = (menu->largo_nombre > menu->largo_opcion) ? menu->largo_nombre : menu->largo_opcion;

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
    
    printf(ANSI_COLOR_BOLD ANSI_COLOR_YELLOW "%s" ANSI_COLOR_RESET, palabra);
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

    size_t cantidad_iguales = menu->largo_mayor * 4;
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

bool menu_mostrar_opcion(char *clave, void *_opcion, void *_ctx)
{
    opcion_t *opcion = _opcion;
    enum formato_muestra *formato = _ctx;

    switch (*formato)
    {
    case FORMATO_1:
        printf(ANSI_COLOR_BOLD "%s) " ANSI_COLOR_RESET "%s", clave, opcion->descripcion);
        printf("\n");
        break;
    
    case FORMATO_2:
        printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET ". " "%s", clave, opcion->descripcion);
        printf("\n");
        break;

    case FORMATO_3:
        printf(ANSI_COLOR_GREEN "┌—————┐\n" ANSI_COLOR_RESET);
        printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET ANSI_COLOR_BOLD "  %s  " ANSI_COLOR_RESET ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET, clave);
        printf(" %s\n", opcion->descripcion);
        printf(ANSI_COLOR_GREEN"└—————┘\n\n" ANSI_COLOR_RESET);
        break;

    default:
        break;
    }

    return true;
}

void menu_mostrar(menu_t *menu, enum formato_muestra formato)
{
    if (menu == NULL || menu->opciones == NULL)
        return;

    hash_iterar(menu->opciones, menu_mostrar_opcion, &formato);
}

char menu_seleccionar_opcion(menu_t *menu)
{
    if (menu == NULL)
        return '\0';

    size_t cantidad_iguales = menu->largo_mayor * 4;
    mostrar_linea_iguales(cantidad_iguales);
    printf("\n");

    printf(ANSI_COLOR_BOLD MENSAJE_SELECCION ANSI_COLOR_RESET);
    int aux = fgetc(stdin);
    int siguiente = fgetc(stdin);
    if ((char)siguiente != '\n' && (char)siguiente != EOF) {
        ungetc(siguiente, stdin);
    }

    char c = (char)aux;
    printf("\n");

    return c;
}

bool menu_ejecutar_opcion(menu_t *menu, char c)
{
    if (menu == NULL || menu->opciones == NULL || c == '\0')
        return false;
    
    char _c[] = {c, '\0'};

    opcion_t *opcion = hash_buscar(menu->opciones, _c);
    if (opcion != NULL) {
        opcion->funcion(opcion->ctx);
        return true;
    }

    return false;
}

void limpiar_pantalla()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

bool menu_mostrar_completo(menu_t *menu, enum formato_muestra formato)
{
    if (menu == NULL)
        return false;

    limpiar_pantalla();
    menu_mostrar_nombre(menu);
    menu_mostrar(menu, formato);
    char opcion_seleccionada = menu_seleccionar_opcion(menu);
    bool resultado_ejecucion = menu_ejecutar_opcion(menu, opcion_seleccionada);

    return resultado_ejecucion;
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