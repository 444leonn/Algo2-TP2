#include "cola.h"
#include "lista.h"

struct cola {
	lista_t *lista;
};

cola_t *cola_crear()
{
	cola_t *cola = malloc(sizeof(cola_t));
	if (cola == NULL)
		return NULL;

	cola->lista = lista_crear();
	if (cola->lista == NULL) {
		free(cola);
		return NULL;
	}

	return cola;
}

bool cola_encolar(cola_t *cola, void *elemento)
{
	if (cola == NULL)
		return false;

	lista_agregar(cola->lista, elemento);

	return true;
}

void *cola_desencolar(cola_t *cola)
{
	if (cola == NULL || cola_cantidad(cola) == 0)
		return NULL;
	return lista_eliminar_elemento(cola->lista, 0);
}

void *cola_ver_primero(cola_t *cola)
{
	if (cola == NULL || cola_cantidad(cola) == 0)
		return NULL;
	return lista_buscar_elemento(cola->lista, 0);
}

size_t cola_cantidad(cola_t *cola)
{
	if (cola == NULL)
		return 0;
	return lista_cantidad(cola->lista);
}

void cola_destruir(cola_t *cola)
{
	if (cola == NULL)
		return;

	lista_destruir(cola->lista);

	free(cola);
}
