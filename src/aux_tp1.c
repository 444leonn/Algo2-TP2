#include "constantes.h"
#include "aux_tp1.h"

#include <stdbool.h>
#include <string.h>

char *leer_linea(FILE *archivo)
{
	if (!archivo)
		return NULL;

	size_t tamaño = TAMAÑO_INICIAL, bytes_leidos = 0;
	char *buffer = malloc(tamaño * sizeof(char));
	if (!buffer)
		return NULL;

	while (fgets(buffer + bytes_leidos, (int)(tamaño - bytes_leidos),
		     archivo)) {
		tamaño *= 2;
		bytes_leidos = strlen(buffer);
		if (buffer[bytes_leidos - 1] == '\n') {
			buffer[bytes_leidos - 1] = '\0';
			return buffer;
		} else if (feof(archivo)) {
			buffer[bytes_leidos] = '\0';
			return buffer;
		}

		char *aux = realloc(buffer, tamaño * sizeof(char));
		if (!aux) {
			free(buffer);
			return NULL;
		}
		buffer = aux;
	}

	if (bytes_leidos == 0) {
		free(buffer);
		return NULL;
	}

	return buffer;
}

int obtener_tipo(char *tipo)
{
	if (strcmp(tipo, "ELEC") == 0)
		return TIPO_ELEC;
	if (strcmp(tipo, "FUEG") == 0)
		return TIPO_FUEG;
	if (strcmp(tipo, "PLAN") == 0)
		return TIPO_PLAN;
	if (strcmp(tipo, "AGUA") == 0)
		return TIPO_AGUA;
	if (strcmp(tipo, "NORM") == 0)
		return TIPO_NORM;
	if (strcmp(tipo, "FANT") == 0)
		return TIPO_FANT;
	if (strcmp(tipo, "PSI") == 0)
		return TIPO_PSI;
	if (strcmp(tipo, "LUCH") == 0)
		return TIPO_LUCH;

	return TIPO_INVALIDO;
}

char *parsear_tipo(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_ELEC:
		return "ELEC";
		break;

	case TIPO_FUEG:
		return "FUEG";
		break;

	case TIPO_PLAN:
		return "PLAN";
		break;

	case TIPO_AGUA:
		return "AGUA";
		break;

	case TIPO_NORM:
		return "NORM";
		break;

	case TIPO_FANT:
		return "FANT";
		break;

	case TIPO_PSI:
		return "PSI";
		break;

	case TIPO_LUCH:
		return "LUCH";
		break;

	default:
		return "Invalido";
		break;
	}
}

struct pokemon *reservar_pokemones(size_t capacidad_inicial)
{
	return malloc(capacidad_inicial * sizeof(struct pokemon));
}

struct pokemon *parsear_linea(char *linea)
{
	if (linea == NULL || linea[0] == '\0' ||
	    linea[strcspn(linea, ";")] == ';')
		return NULL;

	struct pokemon *pokemon = reservar_pokemones(1);
	if (!pokemon)
		return NULL;

	int tipo_buscado;
	size_t i = 0, cont = 0, tamaño = strlen(linea);
	while (i < tamaño) {
		size_t pos_coma = strcspn(linea + i, ",");
		char campo[pos_coma + 1];
		strncpy(campo, linea + i, pos_coma);
		campo[pos_coma] = '\0';

		switch (cont) {
		case 0:
			pokemon->id = atoi(campo);
			break;

		case 1:
			pokemon->nombre = copiar_nombre(pos_coma + 1, campo);
			if (!pokemon->nombre) {
				free(pokemon);
				return NULL;
			}
			break;

		case 2:
			tipo_buscado = obtener_tipo(campo);
			if (tipo_buscado == TIPO_INVALIDO) {
				free(pokemon->nombre);
				free(pokemon);
				return NULL;
			}
			pokemon->tipo = tipo_buscado;
			break;

		case 3:
			pokemon->ataque = atoi(campo);
			break;

		case 4:
			pokemon->defensa = atoi(campo);
			break;

		case 5:
			pokemon->velocidad = atoi(campo);

		default:
			break;
		}

		cont++;
		i += pos_coma + 1;
	}

	if (cont != 6) {
		free(pokemon->nombre);
		free(pokemon);
		return NULL;
	}

	return pokemon;
}

char *copiar_nombre(size_t tamaño, char *buffer)
{
	if (tamaño <= 0 || buffer == NULL)
		return NULL;

	char *nombre_copiado = malloc(tamaño * sizeof(char));
	if (nombre_copiado == NULL || tamaño <= 0)
		return NULL;
	strcpy(nombre_copiado, buffer);

	return nombre_copiado;
}

bool mostrar_pokemon(struct pokemon *pokemon, void *extra)
{
	if (pokemon == NULL)
		return false;

	char *tipo = parsear_tipo(pokemon->tipo);

	printf("| ID: %d | NOMBRE: %s | TIPO: %s | ATAQUE: %d | DEFENSA: %d | VELOCIDAD: %d |\n",
	       pokemon->id, pokemon->nombre, tipo, pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);

	return true;
}

int comparador_nombre(struct pokemon un_pokemon, struct pokemon otro_pokemon)
{
	return strcmp(un_pokemon.nombre, otro_pokemon.nombre);
}

int comparador_id(struct pokemon un_pokemon, struct pokemon otro_pokemon)
{
	if (un_pokemon.id > otro_pokemon.id)
		return 1;
	if (un_pokemon.id < otro_pokemon.id)
		return -1;
	if (un_pokemon.id == otro_pokemon.id)
		return 0;

	return 0;
}

void ordenar_pokemones(struct pokemon *pokemones, size_t cantidad_pokemones,
		       int (*comparador)(struct pokemon, struct pokemon))
{
	for (size_t i = 0; i < cantidad_pokemones - 1; i++) {
		for (size_t j = 0; j < cantidad_pokemones - i - 1; j++) {
			if (comparador(pokemones[j], pokemones[j + 1]) > 0) {
				struct pokemon aux = pokemones[j];
				pokemones[j] = pokemones[j + 1];
				pokemones[j + 1] = aux;
			}
		}
	}
}
