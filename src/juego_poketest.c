#include "juego_poketest.h"

#include "ansi.h"
#include "constantes.h"
#include "aux.h"
#include "lista.h"

#include <string.h>
#include <time.h>

struct jugador {
	size_t puntaje;
};

struct juego_poketest {
	tp1_t *archivo_pokemones;
	lista_t *historial;
	tarjeta_t *tarjetas;
	jugador_t jugadores[CANTIDAD_JUGADORES];
	int semilla;
	size_t cantidad_tarjetas, cantidad_seleccionadas;
	float progreso;
};

void imprimir_linea_tarjeta(tarjeta_t tarjeta, size_t numero_tarjeta, int linea)
{
	char *color = (tarjeta.color_actual == COLOR_ROJO) ? ANSI_COLOR_RED :
							     ANSI_COLOR_BLUE;

	switch (linea) {
	case 0:
		printf("%s" ANSI_COLOR_BOLD
		       "   ┌———————————┐   " ANSI_COLOR_RESET,
		       color);
		break;
	case 1:
	case 2:
		printf("%s" ANSI_COLOR_BOLD
		       "   |           |   " ANSI_COLOR_RESET,
		       color);
		break;
	case 3:
		printf("%s" ANSI_COLOR_BOLD
		       "   |    %2zu     |   " ANSI_COLOR_RESET,
		       color, numero_tarjeta);
		break;
	case 4:
	case 5:
		printf("%s" ANSI_COLOR_BOLD
		       "   |           |   " ANSI_COLOR_RESET,
		       color);
		break;
	case 6:
		printf("%s" ANSI_COLOR_BOLD
		       "   └———————————┘   " ANSI_COLOR_RESET,
		       color);
		break;

	default:
		break;
	}
}

void mostrar_tarjetas(tarjeta_t *tarjetas, size_t cantidad_total)
{
	if (tarjetas == NULL)
		return;

	size_t i = 0;
	while (i < cantidad_total) {
		size_t limite = (i + 6 < cantidad_total) ? i + 6 :
							   cantidad_total;

		for (int linea = 0; linea < 7; linea++) {
			for (size_t j = i; j < limite; j++) {
				imprimir_linea_tarjeta(tarjetas[j], j, linea);
			}
			printf("\n");
		}
		i = limite;

		if (i < cantidad_total)
			printf("\n");
	}

	printf("\n");
}

tarjeta_t *crear_tarjetas(size_t cantidad)
{
	if (cantidad == 0)
		return NULL;
	tarjeta_t *tarjetas = calloc(cantidad, sizeof(tarjeta_t));
	if (tarjetas == NULL)
		return NULL;

	return tarjetas;
}

juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, int semilla,
				       size_t cantidad_tarjetas)
{
	if (archivo_pokemones == NULL || tp1_cantidad(archivo_pokemones) == 0 ||
	    cantidad_tarjetas == 0)
		return NULL;

	juego_poketest_t *juego_poketest = calloc(1, sizeof(juego_poketest_t));
	if (juego_poketest == NULL)
		return NULL;

	juego_poketest->semilla = semilla;
	juego_poketest->archivo_pokemones = archivo_pokemones;
	juego_poketest->cantidad_tarjetas = cantidad_tarjetas;
	juego_poketest->tarjetas =
		crear_tarjetas(juego_poketest->cantidad_tarjetas);
	juego_poketest->historial = lista_crear();
	if (juego_poketest->tarjetas == NULL ||
	    juego_poketest->historial == NULL) {
		free(juego_poketest->tarjetas);
		lista_destruir(juego_poketest->historial);
		free(juego_poketest);

		return NULL;
	}

	return juego_poketest;
}

struct pokemon *obtener_random_pokemon(tp1_t *pokemones)
{
	int random_id = (int)rand() % (int)tp1_cantidad(pokemones);
	struct pokemon *random_pokemon = tp1_buscar_id(pokemones, random_id);
	while (random_pokemon == NULL) {
		random_id = (int)rand() % (int)tp1_cantidad(pokemones);
		random_pokemon = tp1_buscar_id(pokemones, random_id);
	}

	return random_pokemon;
}

size_t obtener_posicion_random(tarjeta_t *tarjetas, size_t cantidad)
{
	size_t posicion = (size_t)rand() % cantidad;
	while (tarjetas[posicion].valor != NULL)
		posicion = (size_t)rand() % cantidad;

	return posicion;
}

size_t juego_poketest_cargar(juego_poketest_t *juego_poketest)
{
	if (juego_poketest == NULL)
		return false;

	if (juego_poketest->semilla != 0)
		srand((unsigned int)juego_poketest->semilla);
	else
		srand((unsigned int)time(NULL));

	size_t cantidad_cargada = 0;
	while (cantidad_cargada < juego_poketest->cantidad_tarjetas) {
		struct pokemon *random_pokemon = obtener_random_pokemon(
			juego_poketest->archivo_pokemones);

		size_t posicion_1 = obtener_posicion_random(
			juego_poketest->tarjetas,
			juego_poketest->cantidad_tarjetas);

		juego_poketest->tarjetas[posicion_1].id_pokemon =
			random_pokemon->id;
		juego_poketest->tarjetas[posicion_1].color_actual = COLOR_AZUL;
		juego_poketest->tarjetas[posicion_1].valor = random_pokemon;

		size_t posicion_2 = obtener_posicion_random(
			juego_poketest->tarjetas,
			juego_poketest->cantidad_tarjetas);
		juego_poketest->tarjetas[posicion_2].id_pokemon =
			random_pokemon->id;
		juego_poketest->tarjetas[posicion_2].color_actual = COLOR_AZUL;
		juego_poketest->tarjetas[posicion_2].valor = random_pokemon;

		cantidad_cargada += 2;
	}

	return cantidad_cargada;
}

bool evaluar_tarjetas(tarjeta_t *tarjetas, int tarjeta_1, int tarjeta_2,
		      size_t cantidad_tarjetas)
{
	if (tarjeta_1 < 0 || tarjeta_1 >= cantidad_tarjetas || tarjeta_2 < 0 ||
	    tarjeta_2 >= cantidad_tarjetas)
		return false;
	else if (tarjeta_1 == tarjeta_2)
		return false;

	esperar_segundos(1);
	printf(ANSI_COLOR_BOLD
	       "\n        Cartas Seleccionadas:\n" ANSI_COLOR_RESET);
	esperar_segundos(1);
	printf(ANSI_COLOR_CYAN "            %d: %s" ANSI_COLOR_RESET "\n",
	       tarjeta_1, tarjetas[tarjeta_1].valor->nombre);
	esperar_segundos(1);
	printf(ANSI_COLOR_CYAN "            %d: %s" ANSI_COLOR_RESET "\n",
	       tarjeta_2, tarjetas[tarjeta_2].valor->nombre);
	esperar_segundos(1);

	if (comparador_pokemones_id(tarjetas[tarjeta_1].valor,
				    tarjetas[tarjeta_2].valor) == 0)
		if (tarjetas[tarjeta_1].color_actual == COLOR_AZUL &&
		    tarjetas[tarjeta_2].color_actual == COLOR_AZUL) {
			tarjetas[tarjeta_1].color_actual = COLOR_ROJO;
			tarjetas[tarjeta_2].color_actual = COLOR_ROJO;

			return true;
		}
	return false;
}

registro_historial_t *crear_registro_historial(char *jugador,
					       int primer_tarjeta,
					       int segunda_tarjeta,
					       bool resultado)
{
	registro_historial_t *registro = malloc(sizeof(registro_historial_t));
	if (registro == NULL)
		return NULL;
	registro->primer_carta = primer_tarjeta;
	registro->segunda_carta = segunda_tarjeta;
	registro->resultado = resultado;

	size_t largo = strlen(jugador) + 1;

	registro->registro = malloc(largo * sizeof(char));
	if (registro->registro == NULL) {
		free(registro);
		return NULL;
	}
	strcpy(registro->registro, jugador);

	return registro;
}

void destructor_registro_historial(void *_registro)
{
	registro_historial_t *registro = _registro;
	free(registro->registro);
	free(registro);
}

bool juego_poketest_jugada(juego_poketest_t *juego_poketest,
			   enum JUGADORES jugador, int tarjeta_1, int tarjeta_2)
{
	if (juego_poketest == NULL)
		return false;

	bool resultado = evaluar_tarjetas(juego_poketest->tarjetas, tarjeta_1,
					  tarjeta_2,
					  juego_poketest->cantidad_tarjetas);

	registro_historial_t *registro_jugada = NULL;
	if (jugador == J1)
		registro_jugada = crear_registro_historial(
			JUGADOR_1, tarjeta_1, tarjeta_2, resultado);
	else
		registro_jugada = crear_registro_historial(
			JUGADOR_2, tarjeta_1, tarjeta_2, resultado);

	if (registro_jugada != NULL) {
		if (lista_cantidad(juego_poketest->historial) >=
		    MAX_HISTORIAL) {
			registro_historial_t *eliminado =
				lista_eliminar_elemento(
					juego_poketest->historial, 0);
			destructor_registro_historial(eliminado);
		}
		lista_agregar(juego_poketest->historial, registro_jugada);
	}

	if (resultado == true) {
		juego_poketest->jugadores[jugador].puntaje++;
		juego_poketest->cantidad_seleccionadas += 2;

		juego_poketest->progreso =
			(float)((juego_poketest->cantidad_seleccionadas * 100) /
				juego_poketest->cantidad_tarjetas);

		return true;
	}

	return false;
}

float juego_poketest_progreso(juego_poketest_t *juego_poketest)
{
	if (juego_poketest == NULL)
		return ERROR;

	return juego_poketest->progreso;
}

int juego_poketest_puntaje(juego_poketest_t *juego_poketest,
			   enum JUGADORES jugador)
{
	if (juego_poketest == NULL)
		return ERROR;

	return (int)juego_poketest->jugadores[jugador].puntaje;
}

bool mostrar_registro(void *_registro, void *ctx)
{
	registro_historial_t *registro = _registro;
	if (registro->resultado == true) {
		printf(ANSI_COLOR_GREEN TILDE ANSI_COLOR_RESET);
		printf(" %s: %d - %d \n", registro->registro,
		       registro->primer_carta, registro->segunda_carta);
	} else {
		printf(ANSI_COLOR_RED CRUZ ANSI_COLOR_RESET);
		printf(" %s: %d - %d \n", registro->registro,
		       registro->primer_carta, registro->segunda_carta);
	}

	return true;
}

void juego_poketest_mostrar(juego_poketest_t *juego_poketest)
{
	limpiar_pantalla();
	printf("\n" ANSI_COLOR_BOLD INICIO_JUEGO ANSI_COLOR_RESET "\n\n");

	mostrar_tarjetas(juego_poketest->tarjetas,
			 juego_poketest->cantidad_tarjetas);
	lista_con_cada_elemento(juego_poketest->historial, mostrar_registro,
				NULL);
}

void juego_poketest_destruir(juego_poketest_t *juego_poketest)
{
	if (!juego_poketest)
		return;

	if (juego_poketest->tarjetas != NULL)
		free(juego_poketest->tarjetas);
	if (juego_poketest->historial != NULL)
		lista_destruir_todo(juego_poketest->historial,
				    destructor_registro_historial);
	free(juego_poketest);
}