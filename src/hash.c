#include "hash.h"
#include "constantes.h"

#include <stdlib.h>
#include <string.h>

typedef struct par {
	char *clave;
	void *valor;
} par_t;

typedef struct nodo {
	struct par par;
	struct nodo *siguiente;
} nodo_t;

struct hash {
	struct nodo **tabla;
	size_t capacidad;
	size_t cantidad;
	float factor_carga;
};

float calcular_factor_carga(size_t cantidad, size_t capacidad)
{
	if (capacidad == 0)
		return ERROR;
	return (float)(cantidad / capacidad);
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *hash = calloc(1, sizeof(hash_t));
	if (hash == NULL)
		return NULL;

	if (capacidad_inicial < CAPACIDAD_MINIMA)
		capacidad_inicial = CAPACIDAD_MINIMA;

	hash->tabla = calloc(capacidad_inicial, sizeof(nodo_t *));
	if (hash->tabla == NULL) {
		free(hash);
		return NULL;
	}
	hash->capacidad = capacidad_inicial;

	return hash;
}

size_t hash_cantidad(hash_t *hash)
{
	if (hash == NULL)
		return 0;

	return hash->cantidad;
}

/*
** Hashea una string de clave a un valor numerico asociado.
** Devuelve un valor negativo en caso de Error.
*/
unsigned long hash_djb2(char *clave)
{
	unsigned long hash = 5381;
	int c;

	while ((c = *clave++)) {
		hash = ((hash << 5) + hash) + (unsigned long)c;
	}

	return hash;
}

int funcion_hash(char *clave, size_t capacidad_tabla)
{
	if (clave == NULL || capacidad_tabla == 0)
		return ERROR;

	unsigned long clave_hasheada = hash_djb2(clave);

    return (int)(clave_hasheada % capacidad_tabla);
}

/*
** Funcion para hacer Rehash a una Tabla de Hash.
** Redimensiona el tamaño al doble de su capacidad.
** Devuelve true si pudo redimensionar la tabla, false en caso contrario.
*/
bool rehash(hash_t *hash)
{
	if (hash == NULL)
		return false;

	size_t nueva_capacidad = 2 * hash->capacidad;
	nodo_t **nueva_tabla = calloc(nueva_capacidad, sizeof(nodo_t *));
	if (nueva_tabla == NULL)
		return false;

	for (size_t i = 0; i < hash->capacidad; i++) {
		nodo_t *actual = hash->tabla[i];
		while (actual != NULL) {
			nodo_t *siguiente = actual->siguiente;

			int nueva_pos = funcion_hash(actual->par.clave,
						     nueva_capacidad);

			actual->siguiente = nueva_tabla[nueva_pos];
			nueva_tabla[nueva_pos] = actual;

			actual = siguiente;
		}
	}

	free(hash->tabla);

	hash->tabla = nueva_tabla;
	hash->capacidad = nueva_capacidad;
	hash->factor_carga =
		calcular_factor_carga(hash->cantidad, hash->capacidad);

	return true;
}

char *copiar_clave(char *clave)
{
	if (clave == NULL)
		return NULL;

	size_t largo = strlen(clave) + 1;
	char *aux = malloc(largo * sizeof(char));
	if (aux == NULL)
		return NULL;

	return strcpy(aux, clave);
}

nodo_t *hash_insertar_recursivo(nodo_t *nodo, char *clave, void *valor,
				void **encontrado, bool *insertado,
				bool *reemplazado)
{
	if (nodo == NULL) {
		nodo = calloc(1, sizeof(nodo_t));
		if (nodo == NULL)
			return NULL;

		nodo->par.valor = valor;
		nodo->par.clave = copiar_clave(clave);
		if (nodo->par.clave == NULL) {
			free(nodo);
			return NULL;
		}

		*insertado = true;
		return nodo;
	} else if (strcmp(nodo->par.clave, clave) == 0) {
		if (encontrado != NULL)
			*encontrado = nodo->par.valor;
		nodo->par.valor = valor;
		*reemplazado = true;
		return nodo;
	}

	nodo->siguiente = hash_insertar_recursivo(nodo->siguiente, clave, valor,
						  encontrado, insertado,
						  reemplazado);

	return nodo;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	if (encontrado != NULL)
		*encontrado = NULL;

	if (hash == NULL || clave == NULL)
		return false;

	if (hash->factor_carga >= 0.75) {
		bool resultado_rehash = rehash(hash);
		if (resultado_rehash == false)
			return false;
	}

	int clave_hasheada = funcion_hash(clave, hash->capacidad);
	if (clave_hasheada == ERROR)
		return false;

	bool insertado = false, reemplazado = false;
	hash->tabla[clave_hasheada] = hash_insertar_recursivo(
		hash->tabla[clave_hasheada], clave, valor, encontrado,
		&insertado, &reemplazado);
	if (insertado == true) {
		hash->cantidad++;
		hash->factor_carga =
			calcular_factor_carga(hash->cantidad, hash->capacidad);
	}

	return insertado || reemplazado;
}

void *hash_buscar_recursivo(nodo_t *nodo, char *clave, bool *contiene)
{
	if (nodo == NULL)
		return NULL;

	if (strcmp(nodo->par.clave, clave) == 0) {
		*contiene = true;
		return nodo->par.valor;
	}

	return hash_buscar_recursivo(nodo->siguiente, clave, contiene);
}

void *hash_buscar(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;

	int clave_hasheada = funcion_hash(clave, hash->capacidad);
	if (clave_hasheada == ERROR)
		return NULL;

	bool contiene = false;
	return hash_buscar_recursivo(hash->tabla[clave_hasheada], clave,
				     &contiene);
}

bool hash_contiene(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return false;

	int clave_hasheada = funcion_hash(clave, hash->capacidad);
	if (clave_hasheada == ERROR)
		return false;

	bool contiene = false;
	hash_buscar_recursivo(hash->tabla[clave_hasheada], clave, &contiene);

	return contiene;
}

nodo_t *hash_quitar_recursivo(nodo_t *nodo, char *clave, void **aux,
			      bool *eliminado)
{
	if (nodo == NULL)
		return NULL;

	if (strcmp(nodo->par.clave, clave) == 0) {
		*aux = nodo->par.valor;
		nodo_t *siguiente = nodo->siguiente;
		free(nodo->par.clave);
		free(nodo);

		*eliminado = true;

		return siguiente;
	}
	nodo->siguiente =
		hash_quitar_recursivo(nodo->siguiente, clave, aux, eliminado);

	return nodo;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	if (hash == NULL || clave == NULL)
		return NULL;

	int clave_hasheada = funcion_hash(clave, hash->capacidad);
	if (clave_hasheada == ERROR)
		return NULL;

	bool eliminado = false;
	void *resultado = NULL;
	hash->tabla[clave_hasheada] = hash_quitar_recursivo(
		hash->tabla[clave_hasheada], clave, &resultado, &eliminado);
	if (eliminado == true) {
		hash->cantidad--;
		hash->factor_carga =
			calcular_factor_carga(hash->cantidad, hash->capacidad);
	}

	return resultado;
}

bool iterar_recursivo(nodo_t *nodo, bool (*f)(char *, void *, void *),
		      void *ctx, size_t *cantidad_aplicados)
{
	if (nodo == NULL)
		return true;
	bool resultado = true;

	resultado = f(nodo->par.clave, nodo->par.valor, ctx);
	*cantidad_aplicados += 1;
	if (resultado == false)
		return false;

	return iterar_recursivo(nodo->siguiente, f, ctx, cantidad_aplicados);
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	if (hash == NULL || f == NULL)
		return 0;

	bool resultado = true;
	size_t i = 0, cant_aplicados = 0;
	while (resultado == true && i < hash->capacidad) {
		resultado = iterar_recursivo(hash->tabla[i], f, ctx,
					     &cant_aplicados);
		i++;
	}

	return cant_aplicados;
}

void eliminar_recursivo(nodo_t *nodo, void (*destructor)(void *))
{
	if (nodo == NULL)
		return;

	eliminar_recursivo(nodo->siguiente, destructor);

	if (destructor != NULL)
		destructor(nodo->par.valor);
	free(nodo->par.clave);
	free(nodo);
}

void hash_destruir(hash_t *hash)
{
	if (hash == NULL)
		return;

	for (size_t i = 0; i < hash->capacidad; i++)
		eliminar_recursivo(hash->tabla[i], NULL);

	free(hash->tabla);
	free(hash);
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	if (hash == NULL)
		return;

	for (size_t i = 0; i < hash->capacidad; i++)
		eliminar_recursivo(hash->tabla[i], destructor);

	free(hash->tabla);
	free(hash);
}
