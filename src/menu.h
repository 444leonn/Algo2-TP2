#ifndef __MENU_H__
#define __MENU_H__

#include "hash.h"

enum formato_muestra { FORMATO_1, FORMATO_2, FORMATO_3, FORMATO_CUSTOM };

typedef struct menu menu_t;

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
bool menu_agregar_opcion(menu_t *menu, char c, char *descripcion,
			 bool (*funcion)(void *), void *ctx);

/*
*	Quita una opcion del menu.
*	Recibiendo el caracter con el que se identifica aquella opcion.
*	Devuelve true si se logro quitar la opcion. Devuelve false en caso contrario.
*/
bool menu_quitar_opcion(menu_t *menu, char c);

/*
*   Permite agregar un formato de muestra para el menu.
*   Recibe el caracter que identifica la opcion y la descripcion de esa opcion.
*/
void menu_agregar_formato(menu_t *menu, void (*f_formato)(char, char *));

/*
*	Permite modificar el formato que se esta utilizando para mostrar el menu.
*/
bool menu_seleccionar_formato(menu_t *menu, enum formato_muestra formato);

/*
*   Muestra el nombre del menu, si es que posee alguno.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar_nombre(menu_t *menu);

/*
*   Muestra el menu, segun el formato de dibujado que sea elegido.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar(menu_t *menu);

/*
*   Ejecuta una opcion seleccionada una opcion del menu y ejecuta la funcion que debe realizar.
*   Devuelve lo retornado por la funcion.
*/
bool menu_ejecutar_opcion(menu_t *menu, char c);

/*
*   Muestra el menu completo (nombre, y opciones), segun el formato de dibujado que sea elegido.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar_completo(menu_t *menu);

/*
*   Recibe un menu y lo destruye.
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_H__