#include "constantes.h"
#include "tp1.h"
#include "aux_tp1.h"

#include <stdio.h>
#include <string.h>

struct tp1 {
	struct pokemon *pokemones;
	size_t cantidad;
	size_t capacidad;
};

tp1_t *crear_tp1()
{
	tp1_t *tp1 = malloc(sizeof(tp1_t));
	if (tp1 == NULL)
		return NULL;

	tp1->capacidad = CAPACIDAD_INICIAL;
	tp1->cantidad = 0;
	tp1->pokemones = reservar_pokemones(tp1->capacidad);
	if (tp1->pokemones == NULL) {
		free(tp1);
		return NULL;
	}
	return tp1;
}

void agregar_pokemon(tp1_t *tp1, struct pokemon pokemon)
{
	if (tp1->cantidad >= tp1->capacidad) {
		tp1->capacidad *= 2;
		void *aux = realloc(tp1->pokemones,
				    sizeof(struct pokemon) * tp1->capacidad);
		if (!aux) {
			return;
		}
		tp1->pokemones = aux;
	}

	tp1->pokemones[tp1->cantidad] = pokemon;

	size_t tamaño = strlen(pokemon.nombre) + 1;
	tp1->pokemones[tp1->cantidad].nombre =
		copiar_nombre(tamaño, pokemon.nombre);
	if (!tp1->pokemones[tp1->cantidad].nombre)
		free(tp1->pokemones[tp1->cantidad].nombre);
	else
		tp1->cantidad++;
}

tp1_t *tp1_leer_archivo(const char *nombre)
{
	if (nombre == NULL)
		return NULL;

	FILE *arch = fopen(nombre, MODO_LECTURA);
	if (!arch)
		return NULL;

	struct tp1 *tp1 = crear_tp1();
	if (!tp1) {
		fclose(arch);
		return NULL;
	}

	struct pokemon *pokemon;
	char *linea;
	while ((linea = leer_linea((arch))) != NULL) {
		pokemon = parsear_linea(linea);
		if (pokemon != NULL) {
			bool ya_cargado = false;
			for (size_t i = 0; i < tp1->cantidad; i++) {
				if (tp1->pokemones[i].id == pokemon->id)
					ya_cargado = true;
			}

			if (!ya_cargado)
				agregar_pokemon(tp1, *pokemon);

			free(pokemon->nombre);
			free(pokemon);
		}
		free(linea);
	}

	fclose(arch);
	if (tp1->cantidad == 0) {
		tp1_destruir(tp1);
		return NULL;
	}

	ordenar_pokemones(tp1->pokemones, tp1_cantidad(tp1), comparador_id);

	return tp1;
}

size_t tp1_cantidad(tp1_t *tp1)
{
	if (tp1 == NULL)
		return 0;

	return tp1->cantidad;
}

tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *nombre)
{
	if (tp1 == NULL || nombre == NULL)
		return NULL;

	FILE *archivo = fopen(nombre, MODO_ESCRITURA);
	if (!archivo)
		return NULL;

	char *tipo;
	for (int i = 0; i < tp1->cantidad; i++) {
		tipo = parsear_tipo(tp1->pokemones[i].tipo);
		fprintf(archivo, "%d,%s,%s,%d,%d,%d\n", tp1->pokemones[i].id,
			tp1->pokemones[i].nombre, tipo,
			tp1->pokemones[i].ataque, tp1->pokemones[i].defensa,
			tp1->pokemones[i].velocidad);
	}

	fclose(archivo);

	return tp1;
}

tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;

	tp1_t *tp1_union = crear_tp1();
	if (tp1_union == NULL)
		return NULL;

	size_t i = 0, j = 0;
	while (i < un_tp->cantidad && j < otro_tp->cantidad) {
		if (un_tp->pokemones[i].id < otro_tp->pokemones[j].id) {
			agregar_pokemon(tp1_union, un_tp->pokemones[i]);
			i++;
		} else if (un_tp->pokemones[i].id > otro_tp->pokemones[j].id) {
			agregar_pokemon(tp1_union, otro_tp->pokemones[j]);
			j++;
		} else {
			agregar_pokemon(tp1_union, un_tp->pokemones[i]);
			i++;
			j++;
		}
	}

	while (i < un_tp->cantidad) {
		agregar_pokemon(tp1_union, un_tp->pokemones[i]);
		i++;
	}

	while (j < otro_tp->cantidad) {
		agregar_pokemon(tp1_union, otro_tp->pokemones[j]);
		j++;
	}

	return tp1_union;
}

tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;

	tp1_t *tp1_interseccion = crear_tp1();
	if (tp1_interseccion == NULL)
		return NULL;

	size_t i = 0, j = 0;
	while (i < un_tp->cantidad && j < otro_tp->cantidad) {
		if (un_tp->pokemones[i].id < otro_tp->pokemones[j].id) {
			i++;
		} else if (un_tp->pokemones[i].id > otro_tp->pokemones[j].id) {
			j++;
		} else {
			agregar_pokemon(tp1_interseccion, un_tp->pokemones[i]);
			i++;
			j++;
		}
	}

	return tp1_interseccion;
}

tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (un_tp == NULL || otro_tp == NULL)
		return NULL;

	tp1_t *tp1_diferencia = crear_tp1();
	if (tp1_diferencia == NULL)
		return NULL;

	bool duplicado = false;
	for (int i = 0; i < un_tp->cantidad; i++) {
		for (int j = 0; j < otro_tp->cantidad; j++)
			if (un_tp->pokemones[i].id == otro_tp->pokemones[j].id)
				duplicado = true;

		if (!duplicado) {
			agregar_pokemon(tp1_diferencia, un_tp->pokemones[i]);
		}
		duplicado = false;
	}

	return tp1_diferencia;
}

struct pokemon *tp1_buscar_nombre(tp1_t *tp, const char *nombre)
{
	if (tp == NULL || nombre == NULL)
		return NULL;

	bool encontrado = false;
	size_t i = 0, cantidad_tp = tp1_cantidad(tp);
	while (i < cantidad_tp && !encontrado) {
		if (strcmp(tp->pokemones[i].nombre, nombre) == 0) {
			encontrado = true;

			return &tp->pokemones[i];
		}
		i++;
	}

	return NULL;
}

struct pokemon *tp1_buscar_id(tp1_t *tp, int id)
{
	if (tp == NULL || !id)
		return NULL;

	size_t inf = 0, sup = tp->cantidad - 1, mit;
	bool terminado = false;

	while (!terminado) {
		if ((id < tp->pokemones[inf].id) ||
		    (id > tp->pokemones[sup].id)) {
			terminado = true;
		} else {
			mit = inf + ((sup - inf) / 2);

			if (id == tp->pokemones[mit].id) {
				terminado = true;
				return &tp->pokemones[mit];

			} else if (id > tp->pokemones[mit].id) {
				inf = mit + 1;
			} else {
				sup = mit - 1;
			}
		}
	}

	return NULL;
}

size_t tp1_con_cada_pokemon(tp1_t *un_tp, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	if (un_tp == NULL || f == NULL)
		return 0;

	size_t cantidad_tp = tp1_cantidad(un_tp), cantidad_aplicados = 0;
	bool aplicado = true;

	size_t i = 0;
	while (i < cantidad_tp && aplicado) {
		if (f(&un_tp->pokemones[i], extra) == false) {
			aplicado = false;
		}
		i++;
		cantidad_aplicados++;
	}

	return cantidad_aplicados;
}

void tp1_destruir(tp1_t *tp1)
{
	if (!tp1)
		return;

	for (int i = 0; i < tp1->cantidad; i++)
		free(tp1->pokemones[i].nombre);
	free(tp1->pokemones);
	free(tp1);
}
