#ifndef __MENU_H__
#define __MENU_H__

#include "hash.h"

enum formato_muestra { FORMATO_1, FORMATO_2, FORMATO_3, FORMATO_CUSTOM };

typedef struct formato_custom {
    void (*formato_custom)(void *);
} formato_custom_t;

// Estructura para almacenar cada opcion dentro de la tabla de hash de opciones
typedef struct opcion {
    char clave;
    char *descripcion;
    bool (*funcion)(void *);
    void *ctx;
} opcion_t;

typedef struct menu {
    char *nombre;
    bool tiene_nombre;
    hash_t *opciones;
    size_t cantidad;
    size_t largo_nombre;
    size_t largo_opcion;
    size_t largo_mayor;
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
bool menu_agregar_opcion(menu_t *menu, char c, char *descripcion, bool (*funcion)(void *), void *ctx);

/*
*   Muestra el nombre del menu, si es que posee alguno.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar_nombre(menu_t *menu);

/*
*   Muestra el menu, segun el formato de dibujado que sea elegido.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar(menu_t *menu, enum formato_muestra formato);

/*
*   Ejecuta una opcion seleccionada una opcion del menu y ejecuta la funcion que debe realizar.
*   Devuelve lo retornado por la funcion.
*/
bool menu_ejecutar_opcion(menu_t *menu, char c);

/*
*   Muestra el menu completo (nombre, opciones, seleccion y ejecucion de opcion), segun el formato de dibujado que sea elegido.
*   Si el menu es NULL, no se muestra nada.
*   Devuelve true en caso de que se haya encontrado y ejecutado la opcion, y false en caso contrario.
*/
bool menu_mostrar_completo(menu_t *menu, enum formato_muestra formato);

/*
*   Recibe un menu y lo destruye.
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_H__