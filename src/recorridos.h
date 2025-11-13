#ifndef RECORRIDOS_H_
#define RECORRIDOS_H_

#include <stdbool.h>
#include "abb_estructura_privada.h"

/*
 * Recorre un arbol de manera recursiva en INORDEN, y aplica la funcion f, incrementando el puntero de cantidad.
 * Retorna un valor booleano que determina si continuar iterando o no.
 */
bool inorden_recursivo_con_funcion(nodo_t *nodo_raiz, bool (*f)(void *, void *),
				   void *extra, size_t *cantidad);

/*
 * Recorre un arbol de manera recusiva en INORDEN, guardando sus datos en un vector.
 */
size_t inorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				    void **vector, size_t indice_actual);

/*
 * Recorre un arbol de manera recursiva en PREORDEN, y aplica la funcion f, incrementando el puntero de cantidad.
 * Retorna un valor booleano que determina si continuar iterando o no.
 */
bool preorden_recursivo_con_funcion(nodo_t *nodo_raiz,
				    bool (*f)(void *, void *), void *extra,
				    size_t *cantidad);

/*
 * Recorre un arbol de manera recursiva en PREORDEN, guardando sus datos en un vector.
 */
size_t preorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				     void **vector, size_t indice_actual);

/*
 * Recorre un arbol de manera recursiva en INORDEN, y aplica la funcion f, incrementando el puntero de cantidad.
 * Retorna un valor booleano que determina si continuar iterando o no.
 */
bool postorden_recursivo_con_funcion(nodo_t *nodo_raiz,
				     bool (*f)(void *, void *), void *extra,
				     size_t *cantidad);

/*
 * Recorre un arbol de manera recursiva en POSTORDEN, guardando sus datos en un vector.
 */
size_t postorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				      void **vector, size_t indice_actual);

#endif // RECORRIDOS_H_
