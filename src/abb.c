#include "abb.h"
#include "abb_estructura_privada.h"

abb_t *abb_crear(int (*comparador)(const void *, const void *))
{
	if (comparador == NULL)
		return NULL;

	abb_t *abb = malloc(sizeof(abb_t));
	if (abb == NULL) {
		return NULL;
	}

	abb->cantidad = 0;
	abb->comparador = comparador;
	abb->raiz = NULL;

	return abb;
}

nodo_t *insertar_recursivo(nodo_t *nodo, void *dato,
			   int (*comparador)(const void *, const void *))
{
	if (nodo == NULL) {
		nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
		if (nuevo_nodo == NULL)
			return NULL;
		nuevo_nodo->dato = dato;
		return nuevo_nodo;
	}

	if (comparador(dato, nodo->dato) <= 0)
		nodo->izq = insertar_recursivo(nodo->izq, dato, comparador);
	else
		nodo->der = insertar_recursivo(nodo->der, dato, comparador);

	return nodo;
}

bool abb_insertar(abb_t *abb, void *dato)
{
	if (abb == NULL)
		return false;

	if (abb_esta_vacio(abb)) {
		abb->raiz = calloc(1, sizeof(nodo_t));
		if (abb->raiz == NULL)
			return false;
		abb->raiz->dato = dato;
		abb->cantidad++;
		return true;
	}

	insertar_recursivo(abb->raiz, dato, abb->comparador);
	abb->cantidad++;

	return true;
}

void *buscar_recursivo(nodo_t *nodo, void *dato,
		       int (*comparador)(const void *, const void *))
{
	if (nodo == NULL)
		return NULL;

	if (comparador(dato, nodo->dato) == 0)
		return nodo->dato;

	if (comparador(dato, nodo->dato) < 0)
		return buscar_recursivo(nodo->izq, dato, comparador);
	else
		return buscar_recursivo(nodo->der, dato, comparador);
}

bool abb_existe(abb_t *abb, void *dato)
{
	if (abb == NULL || abb_esta_vacio(abb))
		return false;

	void *dato_obtenido =
		buscar_recursivo(abb->raiz, dato, abb->comparador);

	return abb->comparador(dato, dato_obtenido) == 0;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	if (abb == NULL || abb_esta_vacio(abb))
		return NULL;

	return buscar_recursivo(abb->raiz, dato, abb->comparador);
}

void buscar_reasignar_predecesor_inorden(nodo_t *nodo)
{
	nodo_t *nodo_aux = nodo->izq;
	nodo_t *nodo_anterior = nodo;

	while (nodo_aux->der != NULL) {
		nodo_anterior = nodo_aux;
		nodo_aux = nodo_aux->der;
	}
	nodo->dato = nodo_aux->dato;

	if (nodo_anterior == nodo)
		nodo->izq = nodo_aux->izq;
	else
		nodo_anterior->der = nodo_aux->izq;

	free(nodo_aux);
}

nodo_t *eliminar_recursivo(nodo_t *nodo, void *dato,
			   int (*comparador)(const void *, const void *),
			   void **p_dato_eliminado, size_t *cantidad)
{
	if (nodo == NULL)
		return NULL;

	if (comparador(dato, nodo->dato) == 0) {
		*p_dato_eliminado = nodo->dato;
		*cantidad = *cantidad - 1;
		if (nodo->izq == NULL && nodo->der == NULL) {
			free(nodo);
			return NULL;
		} else if (nodo->izq != NULL && nodo->der == NULL) {
			nodo_t *nodo_aux = nodo->izq;
			free(nodo);
			return nodo_aux;
		} else if (nodo->izq == NULL && nodo->der != NULL) {
			nodo_t *nodo_aux = nodo->der;
			free(nodo);
			return nodo_aux;
		} else {
			buscar_reasignar_predecesor_inorden(nodo);
			return nodo;
		}
	}

	if (comparador(dato, nodo->dato) < 0)
		nodo->izq = eliminar_recursivo(nodo->izq, dato, comparador,
					       p_dato_eliminado, cantidad);
	else
		nodo->der = eliminar_recursivo(nodo->der, dato, comparador,
					       p_dato_eliminado, cantidad);

	return nodo;
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	if (abb == NULL || abb_esta_vacio(abb))
		return NULL;

	void *dato_eliminado = NULL;
	abb->raiz = eliminar_recursivo(abb->raiz, dato, abb->comparador,
				       &dato_eliminado, &abb->cantidad);

	return dato_eliminado;
}

void *abb_raiz(abb_t *abb)
{
	if (abb != NULL)
		return abb->raiz->dato;
	return NULL;
}

size_t abb_cantidad(abb_t *abb)
{
	if (abb != NULL)
		return abb->cantidad;
	return 0;
}

bool abb_esta_vacio(abb_t *abb)
{
	if (abb == NULL)
		return true;
	return abb->cantidad == 0;
}

bool inorden_recursivo_con_funcion(nodo_t *nodo_raiz, bool (*f)(void *, void *),
				   void *extra, size_t *cantidad)
{
	if (nodo_raiz == NULL)
		return true;
	bool continuar = true;

	continuar = inorden_recursivo_con_funcion(nodo_raiz->izq, f, extra,
						  cantidad);
	if (continuar == false)
		return false;

	continuar = f(nodo_raiz->dato, extra);
	*cantidad += 1;
	if (continuar == false)
		return false;

	continuar = inorden_recursivo_con_funcion(nodo_raiz->der, f, extra,
						  cantidad);
	if (continuar == false)
		return false;

	return true;
}

size_t inorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				    void **vector, size_t indice_actual)
{
	if (nodo_raiz == NULL)
		return indice_actual;

	indice_actual = inorden_recursivo_con_vector(nodo_raiz->izq, cant,
						     vector, indice_actual);

	if (indice_actual < cant) {
		vector[indice_actual] = nodo_raiz->dato;
		indice_actual++;
	}

	indice_actual = inorden_recursivo_con_vector(nodo_raiz->der, cant,
						     vector, indice_actual);

	return indice_actual;
}

bool preorden_recursivo_con_funcion(nodo_t *nodo_raiz,
				    bool (*f)(void *, void *), void *extra,
				    size_t *cantidad)
{
	if (nodo_raiz == NULL)
		return true;
	bool continuar = true;

	continuar = f(nodo_raiz->dato, extra);
	*cantidad += 1;
	if (continuar == false)
		return false;

	continuar = preorden_recursivo_con_funcion(nodo_raiz->izq, f, extra,
						   cantidad);
	if (continuar == false)
		return false;

	continuar = preorden_recursivo_con_funcion(nodo_raiz->der, f, extra,
						   cantidad);
	if (continuar == false)
		return false;

	return true;
}

size_t preorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				     void **vector, size_t indice_actual)
{
	if (nodo_raiz == NULL)
		return indice_actual;

	if (indice_actual < cant) {
		vector[indice_actual] = nodo_raiz->dato;
		indice_actual++;
	}

	indice_actual = preorden_recursivo_con_vector(nodo_raiz->izq, cant,
						      vector, indice_actual);
	indice_actual = preorden_recursivo_con_vector(nodo_raiz->der, cant,
						      vector, indice_actual);

	return indice_actual;
}

bool postorden_recursivo_con_funcion(nodo_t *nodo_raiz,
				     bool (*f)(void *, void *), void *extra,
				     size_t *cantidad)
{
	if (nodo_raiz == NULL)
		return true;
	bool continuar = true;

	continuar = postorden_recursivo_con_funcion(nodo_raiz->izq, f, extra,
						    cantidad);
	if (continuar == false)
		return false;

	continuar = postorden_recursivo_con_funcion(nodo_raiz->der, f, extra,
						    cantidad);
	if (continuar == false)
		return false;

	continuar = f(nodo_raiz->dato, extra);
	*cantidad += 1;
	if (continuar == false)
		return false;

	return true;
}

size_t postorden_recursivo_con_vector(nodo_t *nodo_raiz, size_t cant,
				      void **vector, size_t indice_actual)
{
	if (nodo_raiz == NULL)
		return indice_actual;

	indice_actual = postorden_recursivo_con_vector(nodo_raiz->izq, cant,
						       vector, indice_actual);
	indice_actual = postorden_recursivo_con_vector(nodo_raiz->der, cant,
						       vector, indice_actual);

	if (indice_actual < cant) {
		vector[indice_actual] = nodo_raiz->dato;
		indice_actual++;
	}

	return indice_actual;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido tipo_recorrido,
			     bool (*f)(void *, void *), void *extra)
{
	if (abb == NULL || f == NULL || abb_esta_vacio(abb))
		return 0;

	size_t cantidad_aplicado = 0;

	switch (tipo_recorrido) {
	case ABB_INORDEN:
		inorden_recursivo_con_funcion(abb->raiz, f, extra,
					      &cantidad_aplicado);
		break;

	case ABB_PREORDEN:
		preorden_recursivo_con_funcion(abb->raiz, f, extra,
					       &cantidad_aplicado);
		break;

	case ABB_POSTORDEN:
		postorden_recursivo_con_funcion(abb->raiz, f, extra,
						&cantidad_aplicado);
		break;

	default:
		break;
	}

	return cantidad_aplicado;
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	if (abb == NULL || vector == NULL || abb_esta_vacio(abb))
		return 0;

	size_t cantidad_guardada = 0;
	switch (tipo_recorrido) {
	case ABB_INORDEN:
		cantidad_guardada = inorden_recursivo_con_vector(
			abb->raiz, cant, vector, 0);
		break;

	case ABB_PREORDEN:
		cantidad_guardada = preorden_recursivo_con_vector(
			abb->raiz, cant, vector, 0);
		break;

	case ABB_POSTORDEN:
		cantidad_guardada = postorden_recursivo_con_vector(
			abb->raiz, cant, vector, 0);
		break;

	default:
		break;
	}

	return cantidad_guardada;
}

nodo_t *destruir_recursivo(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL)
		return NULL;

	destruir_recursivo(nodo->izq, destructor);
	destruir_recursivo(nodo->der, destructor);

	if (destructor != NULL)
		destructor(nodo->dato);
	free(nodo);

	return NULL;
}

void abb_destruir(abb_t *abb)
{
	if (abb == NULL)
		return;

	destruir_recursivo(abb->raiz, NULL);
	free(abb);
}

void abb_destruir_todo(abb_t *abb, void (*destructor)(void *))
{
	if (abb == NULL)
		return;

	destruir_recursivo(abb->raiz, destructor);
	free(abb);
}
