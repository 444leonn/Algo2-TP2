#ifndef __AUX_H__
#define __AUX_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

/*
*   Limpia el buffer de entrada.
*/
void limpiar_buffer();

/*
*   Limpia la terminal.
*/
void limpiar_pantalla();

/*
*   Evalua dos opciones.
*/
bool evaluar_opciones(char *opcion_evaluada, char *opciones_disponibles);

/*
*   Espera la cantidad de segundos pasada por parametro.
*/
void esperar_segundos(size_t cantidad);

/*
*   Evalua si la opcion es la de volver, y si lo es muestra un mensaje por pantalla.
*/
bool evaluar_volver_menu(char *opcion_evaluada);

/*
*   Muestra un mensaje y pide el ingreso de un caracter.
*/
char seleccionar_opcion();

/*
*   Muestra un mensaje.
*/
void mostrar_mensaje_cargar();

/*
*   Muestra un mensaje
*/
void mostrar_mensaje_fallo_lectura();

/*
*   Muestra un mensaje.
*/
void mostrar_mensaje_opcion_invalida();

/*
*   Muestra un mensaje y espera el ingreso de un caracter.
*/
void mostrar_mensaje_continuar();

/*
*   Lee por stdin el nombre de un archivo.
*   Si falla muestra un mensaje de error.
*/
char *leer_nombre_archivo();

#endif // __AUX_H__