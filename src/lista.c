#include "lista.h"
#include "aux.h"
#include "constantes.h"
#include <stdio.h>

struct lista {
	nodo_t *primer_nodo;
	nodo_t *ultimo_nodo;
	size_t cantidad;
};

struct lista_iterador {
	lista_t *lista;
	nodo_t *nodo_actual;
};

lista_t *lista_crear()
{
	return calloc(1, sizeof(lista_t));
}

bool lista_vacia(lista_t *lista)
{
	if (lista == NULL)
		return false;
	return lista->cantidad == 0;
}

size_t lista_cantidad(lista_t *lista)
{
	if (lista == NULL)
		return 0;
	return lista->cantidad;
}

bool lista_agregar(lista_t *lista, void *dato)
{
	if (lista == NULL)
		return false;

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL)
		return false;
	nuevo_nodo->dato = dato;
	nuevo_nodo->proximo = NULL;

	if (lista->primer_nodo == NULL) {
		lista->primer_nodo = nuevo_nodo;
		lista->ultimo_nodo = nuevo_nodo;
		lista->cantidad++;
		return true;
	}
	lista->ultimo_nodo->proximo = nuevo_nodo;
	lista->ultimo_nodo = lista->ultimo_nodo->proximo;
	lista->cantidad++;

	return true;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (lista == NULL || lista_vacia(lista))
		return false;

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (nuevo_nodo == NULL)
		return false;
	nuevo_nodo->dato = elemento;

	if (posicion == 0) {
		nuevo_nodo->proximo = lista->primer_nodo;
		lista->primer_nodo = nuevo_nodo;
		lista->cantidad++;
		return true;
	}

	nodo_t *p_nodo = lista->primer_nodo;
	nodo_t *nodo_aux = p_nodo;
	size_t i = 0;
	while (i < posicion) {
		if (p_nodo == NULL) {
			free(nuevo_nodo);
			return false;
		}
		nodo_aux = p_nodo;
		p_nodo = p_nodo->proximo;
		i++;
	}

	nodo_aux->proximo = nuevo_nodo;
	nuevo_nodo->proximo = p_nodo;
	lista->cantidad++;

	return true;
}

void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL || posicion >= lista->cantidad || lista_vacia(lista))
		return NULL;

	nodo_t *p_nodo = lista->primer_nodo;
	nodo_t *nodo_aux = p_nodo;
	if (p_nodo == NULL)
		return NULL;

	if (posicion == 0) {
		lista->primer_nodo = p_nodo->proximo;
		void *dato_eliminado = p_nodo->dato;
		lista->cantidad--;
		free(p_nodo);

		return dato_eliminado;
	}

	size_t i = 0;
	while (i < posicion) {
		nodo_aux = p_nodo;
		p_nodo = p_nodo->proximo;
		if (p_nodo == NULL)
			return NULL;
		i++;
	}

	if (i != posicion)
		return NULL;

	nodo_aux->proximo = p_nodo->proximo;
	void *dato_elimnado = p_nodo->dato;
	lista->cantidad--;
	free(p_nodo);

	return dato_elimnado;
}

int lista_buscar_posicion(lista_t *lista, void *elemento,
			  int (*comparador)(const void *, const void *))
{
	if (lista == NULL)
		return POSICION_INVALIDA;

	int posicion = 0;
	nodo_t *p_nodo = lista->primer_nodo;
	while (p_nodo != NULL) {
		if (comparador(p_nodo->dato, elemento) == 0) {
			return posicion;
		}
		p_nodo = p_nodo->proximo;
		posicion++;
	}

	return POSICION_INVALIDA;
}

void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (lista == NULL || posicion >= lista->cantidad)
		return NULL;

	nodo_t *p_nodo = lista->primer_nodo;
	size_t i = 0;
	while (i < posicion) {
		p_nodo = p_nodo->proximo;
		i++;
	}

	return p_nodo->dato;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	if (lista == NULL || f == NULL || lista_vacia(lista))
		return 0;

	size_t cantidad_aplicados = 0;
	bool aplicado = true;

	nodo_t *p_nodo = lista->primer_nodo;
	while (p_nodo != NULL && aplicado) {
		aplicado = f(p_nodo->dato, extra);
		p_nodo = p_nodo->proximo;
		cantidad_aplicados++;
	}

	return cantidad_aplicados;
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (lista == NULL)
		return;

	nodo_t *p_nodo = lista->primer_nodo;
	while (p_nodo != NULL) {
		nodo_t *nodo_aux = p_nodo;
		p_nodo = p_nodo->proximo;
		if (nodo_aux != NULL) {
			if (nodo_aux->dato != NULL)
				destructor(nodo_aux->dato);
			free(nodo_aux);
		}
	}

	free(lista);
}

void lista_destruir(lista_t *lista)
{
	if (lista == NULL)
		return;

	nodo_t *p_nodo = lista->primer_nodo;
	while (p_nodo != NULL) {
		nodo_t *nodo_aux = p_nodo;
		p_nodo = p_nodo->proximo;
		if (nodo_aux != NULL)
			free(nodo_aux);
	}

	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (lista == NULL || lista_vacia(lista))
		return NULL;

	lista_iterador_t *iterador = malloc(sizeof(lista_iterador_t));
	if (iterador == NULL)
		return NULL;

	iterador->lista = lista;
	iterador->nodo_actual = lista->primer_nodo;

	return iterador;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	if (it == NULL || lista_vacia(it->lista))
		return false;

	if (it->nodo_actual == NULL)
		return false;
	return true;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (it != NULL && it->nodo_actual != NULL)
		it->nodo_actual = it->nodo_actual->proximo;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (it == NULL)
		return NULL;
	if (it->nodo_actual != NULL)
		return it->nodo_actual->dato;
	return NULL;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	if (it != NULL)
		free(it);
}
