#include "juego_poketest.h"

#include "ansi.h"
#include "constantes.h"
#include "aux.h"
#include "lista.h"

#include <string.h>
#include <time.h>

struct tarjeta {
	size_t numero_tarjeta;
	int id_pokemon;
	enum color_tarjeta color_actual;
	struct pokemon *valor;
};

struct registro_historial {
	char *registro;
	int primer_carta, segunda_carta;
	bool resultado;
};

struct jugador {
	size_t puntaje;
};

struct juego_poketest {
	tp1_t *archivo_pokemones;
	lista_t *historial;
	tarjeta_t *tarjetas;
	int semilla;
	size_t cantidad_cargada;
	size_t cantidad_tarjetas;
	jugador_t jugador_1, jugador_2;
	bool turno_jugador_1;
};

void limpiar_pantalla_juego()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

void limpiar_buffer_juego()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void imprimir_linea_tarjeta(tarjeta_t tarjeta, int linea)
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
		       color, tarjeta.numero_tarjeta);
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
				imprimir_linea_tarjeta(tarjetas[j], linea);
			}
			printf("\n");
		}
		i = limite;

		if (i < cantidad_total)
			printf("\n");
	}

	printf("\n");
}

int comparador_pokemones(const void *_pokemon_a, const void *_pokemon_b)
{
	const struct pokemon *pokemon_a = _pokemon_a;
	const struct pokemon *pokemon_b = _pokemon_b;
	return pokemon_a->id - pokemon_b->id;
}

tarjeta_t *crear_tarjetas(size_t cantidad)
{
	if (cantidad == 0)
		return NULL;
	tarjeta_t *tarjetas = calloc(cantidad, sizeof(tarjeta_t));
	if (tarjetas == NULL)
		return NULL;
	for (size_t i = 0; i < cantidad; i++)
		tarjetas[i].numero_tarjeta = i;

	return tarjetas;
}

juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, int semilla,
				       size_t cantidad_tarjetas)
{
	if (archivo_pokemones == NULL || tp1_cantidad(archivo_pokemones) == 0)
		return NULL;

	juego_poketest_t *juego_poketest = calloc(1, sizeof(juego_poketest_t));
	if (juego_poketest == NULL)
		return NULL;

	if (semilla != 0)
		juego_poketest->semilla = semilla;

	juego_poketest->archivo_pokemones = archivo_pokemones;
	juego_poketest->cantidad_cargada = 0;
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

bool juego_poketest_cargar(juego_poketest_t *juego_poketest)
{
	if (!juego_poketest || !juego_poketest->archivo_pokemones ||
	    !juego_poketest->tarjetas)
		return false;

	if (juego_poketest->semilla != 0)
		srand((unsigned int)juego_poketest->semilla);
	else
		srand((unsigned int)time(NULL));

	int i = 0;
	while (i < juego_poketest->cantidad_tarjetas) {
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

		juego_poketest->cantidad_cargada += 2;
		i += 2;
	}

	return true;
}

void seleccionar_tarjetas(juego_poketest_t *juego_poketest, int *carta_1,
			  int *carta_2)
{
	if (juego_poketest == NULL || carta_1 == NULL || carta_2 == NULL)
		return;

	if (juego_poketest->turno_jugador_1 == true)
		printf("\n" ANSI_COLOR_BOLD JUGADOR_1 ANSI_COLOR_RESET "\n");
	else
		printf("\n" ANSI_COLOR_BOLD JUGADOR_2 ANSI_COLOR_RESET "\n");

	printf(MENSAJE_SELECCION_CARTA_1);
	if (scanf("%d", carta_1) != 1)
		limpiar_buffer_juego();

	printf(MENSAJE_SELECCION_CARTA_2);
	if (scanf("%d", carta_2) != 1)
		limpiar_buffer_juego();
}

bool evaluar_tarjetas(juego_poketest_t *juego_poketest, int carta_1,
		      int carta_2)
{
	if (carta_1 < 0 || carta_1 >= juego_poketest->cantidad_tarjetas ||
	    carta_2 < 0 || carta_2 >= juego_poketest->cantidad_tarjetas)
		return false;
	else if (carta_1 == carta_2)
		return false;

	esperar_segundos(1);
	printf(ANSI_COLOR_BOLD
	       "\n        Cartas Seleccionadas:\n" ANSI_COLOR_RESET);
	esperar_segundos(1);
	printf(ANSI_COLOR_CYAN "            %d: %s" ANSI_COLOR_RESET "\n",
	       carta_1, juego_poketest->tarjetas[carta_1].valor->nombre);
	esperar_segundos(1);
	printf(ANSI_COLOR_CYAN "            %d: %s" ANSI_COLOR_RESET "\n",
	       carta_2, juego_poketest->tarjetas[carta_2].valor->nombre);
	esperar_segundos(1);

	if (comparador_pokemones(juego_poketest->tarjetas[carta_1].valor,
				 juego_poketest->tarjetas[carta_2].valor) == 0)
		if (juego_poketest->tarjetas[carta_1].color_actual ==
			    COLOR_AZUL &&
		    juego_poketest->tarjetas[carta_2].color_actual ==
			    COLOR_AZUL) {
			juego_poketest->tarjetas[carta_1].color_actual =
				COLOR_ROJO;
			juego_poketest->tarjetas[carta_2].color_actual =
				COLOR_ROJO;
			return true;
		}
	return false;
}

void juego_poketest_mostrar_resultados(juego_poketest_t *juego_poketest)
{
	if (!juego_poketest)
		return;

	printf(ANSI_COLOR_BOLD FIN_JUEGO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BOLD "Resultados:" ANSI_COLOR_RESET "\n");
	printf(ANSI_COLOR_BOLD JUGADOR_1 ": %ld" ANSI_COLOR_RESET "\n",
	       juego_poketest->jugador_1.puntaje);
	printf(ANSI_COLOR_BOLD JUGADOR_2 ": %ld" ANSI_COLOR_RESET "\n",
	       juego_poketest->jugador_2.puntaje);

	if (juego_poketest->jugador_1.puntaje >
	    juego_poketest->jugador_2.puntaje)
		printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "\n" MENSAJE_GANADOR JUGADOR_1 ANSI_COLOR_RESET
		       "\n\n\n");
	else if (juego_poketest->jugador_1.puntaje <
		 juego_poketest->jugador_2.puntaje)
		printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD
		       "\n" MENSAJE_GANADOR JUGADOR_2 ANSI_COLOR_RESET
		       "\n\n\n");
	else
		printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD EMPATE ANSI_COLOR_RESET
		       "\n");
	printf(ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();
}

registro_historial_t *crear_registro_historial(char *jugador,
					       int primer_tarjeta,
					       int tarjeta_2, bool resultado)
{
	registro_historial_t *registro = malloc(sizeof(registro_historial_t));
	if (registro == NULL)
		return NULL;
	registro->primer_carta = primer_tarjeta;
	registro->segunda_carta = tarjeta_2;
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

void mostrar_juego(tarjeta_t *tarjetas, size_t cantidad, lista_t *historial)
{
	limpiar_pantalla_juego();
	printf("\n" ANSI_COLOR_BOLD INICIO_JUEGO ANSI_COLOR_RESET "\n\n");
	mostrar_tarjetas(tarjetas, cantidad);
	lista_con_cada_elemento(historial, mostrar_registro, NULL);
}

bool juego_poketest_jugar(juego_poketest_t *juego_poketest)
{
	if (!juego_poketest || !juego_poketest->archivo_pokemones ||
	    !juego_poketest->tarjetas || !juego_poketest->historial)
		return false;
	juego_poketest->turno_jugador_1 = true;

	size_t cantidad_rojas = 0;
	while (cantidad_rojas < juego_poketest->cantidad_tarjetas) {
		mostrar_juego(juego_poketest->tarjetas,
			      juego_poketest->cantidad_tarjetas,
			      juego_poketest->historial);

		int carta_1 = POSICION_INVALIDA, carta_2 = POSICION_INVALIDA;
		seleccionar_tarjetas(juego_poketest, &carta_1, &carta_2);
		bool resultado =
			evaluar_tarjetas(juego_poketest, carta_1, carta_2);

		registro_historial_t *registro_actual = NULL;
		if (juego_poketest->turno_jugador_1 == true)
			registro_actual = crear_registro_historial(
				JUGADOR_1, carta_1, carta_2, resultado);
		else
			registro_actual = crear_registro_historial(
				JUGADOR_2, carta_1, carta_2, resultado);

		if (registro_actual != NULL) {
			if (lista_cantidad(juego_poketest->historial) >=
			    MAX_HISTORIAL) {
				registro_historial_t *eliminado =
					lista_eliminar_elemento(
						juego_poketest->historial, 0);
				destructor_registro_historial(eliminado);
			}
			lista_agregar(juego_poketest->historial,
				      registro_actual);
		}

		if (resultado) {
			if (juego_poketest->turno_jugador_1 == true) {
				juego_poketest->turno_jugador_1 = false;
				juego_poketest->jugador_1.puntaje++;
			} else {
				juego_poketest->turno_jugador_1 = true;
				juego_poketest->jugador_2.puntaje++;
			}
			cantidad_rojas += 2;
		} else if (juego_poketest->turno_jugador_1 == true)
			juego_poketest->turno_jugador_1 = false;
		else
			juego_poketest->turno_jugador_1 = true;
	}

	mostrar_juego(juego_poketest->tarjetas,
		      juego_poketest->cantidad_tarjetas,
		      juego_poketest->historial);
	juego_poketest_mostrar_resultados(juego_poketest);
	mostrar_mensaje_continuar();

	return true;
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