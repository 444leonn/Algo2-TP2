#ifndef AUX_H_
#define AUX_H_
#include "tp1.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Lee una linea de un archivo, devuelve la linea con \0 o null en caso de error.
*/
char *leer_linea(FILE *archivo);

/**
 * Reserva memoria, para n pokemones.
*/
struct pokemon *reservar_pokemones(size_t capacidad_inicial);

/**
 * Parsea una linea leida al struct de pokemon.
*/
struct pokemon *parsear_linea(char *linea);

/**
 * Obtiene el tipo de un pokemon dado su nombre.
*/
int obtener_tipo(char *tipo);

/**
 * Convierte el numero de un tipo pokemon a su string correspondiente.
*/
char *parsear_tipo(enum tipo_pokemon);

/**
 * Recibe un tamaño para un string y un buffer, reserva memoria y lo copia.
*/
char *copiar_nombre(size_t tamaño, char *buffer);

/**
 * Funcion para mostrar un pokemon, adaptada para cumplir con tp1_con_cada_pokemon()
*/
bool mostrar_pokemon(struct pokemon *pokemon, void *extra);

/**
 * Comparadores por id y por nombre
 * Retorna: 0 si son iguales, > 0 si el primero es mayor y < 0 si el primero es menor.
*/
int comparador_nombre(struct pokemon un_pokemon, struct pokemon otro_pokemon);

int comparador_id(struct pokemon un_pokemon, struct pokemon otro_pokemon);

/**
 * Ordena los pokemones por id
*/
void ordenar_pokemones(struct pokemon *pokemones, size_t cantidad_pokemones,
		       int (*comparador)(struct pokemon, struct pokemon));

#endif
