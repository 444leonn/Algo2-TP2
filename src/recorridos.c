#include "recorridos.h"

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
