#ifndef __MENU_H__
#define __MENU_H__

#include "hash.h"

// Estructura para almacenar cada opcion dentro de la tabla de hash de opciones
typedef struct opcion {
	char opcion;
	char *descripcion;
	bool (*funcion)(void *);
	void *ctx;
} opcion_t;

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
*	Devuelve el nombre de un menu.
*	Retorna NULL si el menu es invalido.
*/
char *menu_obtener_nombre(menu_t *menu);

/*
*   Muestra las opciones del menu.
*	Se utiliza la funcion pasada por parametro para la muestra.
*	La misma recibe como primer parametro la clave, luego la opcion_t completa y un puntero auxiliar.
*   Si el menu es NULL, no se muestra nada.
*/
void menu_mostrar(menu_t *menu,
		  bool (*funcion_muestra)(char *, void *, void *));

/*
*   Ejecuta una opcion seleccionada una opcion del menu y ejecuta la funcion que debe realizar.
*   Devuelve lo retornado por la funcion.
*/
bool menu_ejecutar_opcion(menu_t *menu, char c);

/*
*   Recibe un menu y lo destruye.
*/
void menu_destruir(menu_t *menu);

#endif // __MENU_H__