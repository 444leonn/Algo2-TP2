#ifndef __MENU_H__
#define __MENU_H__

#include "hash.h"

typedef enum formato_muestra { FORMATO_1, FORMATO_2, FORMATO_3 }; 

// Estructura para almacenar cada opcion dentro de la tabla de hash de opciones
typedef struct opcion {
    char *descripcion;
    bool (*funcion)(void *);
};

typedef struct menu {
    char *nombre;
    bool tiene_nombre;
    hash_t *opciones;
    size_t cantidad;
    size_t largo_opcion;
} menu_t;

/*
*   Crea un menu con el nombre indicado, devuelve el menu.
*   En caso de nombre NULL, se crea un menu sin nombre.
*   En caso de error devuelve NULL.
*/
menu_t *menu_crear(char *nombre);

/*
*   Agrega una opcion al menu.
*   Recibiendo el caracter con el que se identifica aquella opcion, la descripcion y una funcion.
*   Devuelve true si se logro agregar la opcion.
*   Devuelve false si no se logro agregar la opcion.
*/
bool menu_agregar_opcion(char *c, char *descripcion, bool (*funcion)(void *));

/*
*   Muestra el menu, segun el formato de dibujado que sea elegido.
*   Si no el menu es NULL, no se muestra nada.
*/
void menu_mostrar(menu_t *menu, enum formato_muestra);

/*
*   Permite seleccioanr una opcion.
*   En caso de error devuelve caracter nulo '\0'
*/
char menu_seleccionar_opcion(menu_t *menu);

/*
*   Ejecuta una opcion seleccionada una opcion del menu y ejecuta la funcion que debe realizar.
*   Devuelve lo retornado por la funcion.
*/
bool menu_ejecutar_opcion(char c);

/*
*   Recibe un menu y lo destruye.
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_H__