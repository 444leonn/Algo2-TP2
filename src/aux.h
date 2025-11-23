#ifndef __AUX_H__
#define __AUX_H__

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "tp1.h"

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
*   Evalua un archivo de pokemones y si es invalido muestra un mensaje por pantalla.
*/
bool validar_archivo_pokemones(tp1_t *archivo_pokemones);

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

/*
*   Muestra una opcion por pantalla.
*/
bool mostrar_formato_1(char *clave, void *_opcion, void *aux);

/*
*   Muestra una opcion por pantalla.
*/
bool mostrar_formato_2(char *clave, void *_opcion, void *aux);

/*
*   Muestra una opcion por pantalla.
*/
bool mostrar_formato_3(char *clave, void *_opcion, void *aux);

/*
*   Muestra un string con cierto formato.
*/
void mostrar_nombre_menu(char *nombre);

/*
*   Muestra un mensaje con el progreso del juego.
*/
void mostrar_progreso_juego(float progreso);

/*
*   Muestra un mensaje y pide el ingreso de tarjetas.
*/
void seleccionar_tarjetas(int *carta_1, int *carta_2);

/*
*   Compara dos pokemones por id.
*/
int comparador_pokemones_id(const void *_pokemon_a, const void *_pokemon_b);

/*
*   Compara dos pokemones por nombre.
*/
int comparador_pokemones_nombre(const void *_pokemon_a, const void *_pokemon_b);

#endif // __AUX_H__