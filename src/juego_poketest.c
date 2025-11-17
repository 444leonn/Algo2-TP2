#include "juego_poketest.h"

#include <string.h>

typedef struct tarjeta {
    bool es_rojo;
} tarjeta_t;

void limpiar_pantalla_juego()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

void mostrar_tarjetas(size_t *cantidad_actual, size_t cantidad_total, tarjeta_t *tarjetas)
{
    int i = 0;
    while (i < 5 && i < cantidad_total) {
        if (tarjetas[i].es_rojo)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD " ___________   " ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD " ___________   " ANSI_COLOR_RESET);
        i++;
    }
    printf("\n");

    for (int j = 0; j < 2; j++) {
        i = 0;
        while (i < 5 && i < cantidad_total) {
            if (tarjetas[i].es_rojo)
                printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            i++;
        }
        printf("\n");
    }

    i = 0;
    while (i < 5 && i < cantidad_total) {
        if (tarjetas[i].es_rojo)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|     %ld     |  " ANSI_COLOR_RESET, *cantidad_actual);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|     %ld     |  " ANSI_COLOR_RESET, *cantidad_actual);
        *cantidad_actual += 1;
        i++;
    }
    printf("\n");

    for (int j = 0; j < 2; j++) {
        i = 0;
        while (i < 5 && i < cantidad_total) {
            if (tarjetas[i].es_rojo)
                printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            i++;
        }
        printf("\n");
    }

    i = 0;
    while (i < 5 && i < cantidad_total) {
        if (tarjetas[i].es_rojo)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD " ‾‾‾‾‾‾‾‾‾‾‾   " ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD " ‾‾‾‾‾‾‾‾‾‾‾   " ANSI_COLOR_RESET);
        i++;
    }
    printf(ANSI_COLOR_RESET "\n");

    printf("\n");
}

int comparador_pokemones(const void *_pokemon_a, const void *_pokemon_b)
{
    const struct pokemon *pokemon_a = _pokemon_a;
    const struct pokemon *pokemon_b = _pokemon_b;

    return strcmp(pokemon_a->nombre, pokemon_b->nombre);
}

juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, size_t semilla)
{
    if (archivo_pokemones == NULL || tp1_cantidad(archivo_pokemones) < 3)
        return NULL;
    
    if (semilla < 3)
        semilla = 3;
    else if (semilla > tp1_cantidad(archivo_pokemones))
        semilla = tp1_cantidad(archivo_pokemones);

    printf("\n\nSEMILLA: %ld\n\n", semilla);

    juego_poketest_t *juego_poketest = calloc(1, sizeof(juego_poketest_t));
    if (juego_poketest == NULL)
        return NULL;
    
    juego_poketest->archivo_pokemones = archivo_pokemones;
    juego_poketest->semilla = semilla;
    juego_poketest->cantidad_tarjetas = 2 * juego_poketest->semilla;
    juego_poketest->pokemones_juego = abb_crear(comparador_pokemones);
    if (juego_poketest->pokemones_juego == NULL) {
        free(juego_poketest);
        return NULL;
    }

    juego_poketest->vector_1 = malloc(juego_poketest->semilla * sizeof(void*));
    juego_poketest->vector_2 = malloc(juego_poketest->semilla * sizeof(void*));
    if (juego_poketest->vector_1 == NULL ||juego_poketest->vector_2 == NULL) {
        abb_destruir(juego_poketest->pokemones_juego);
        free(juego_poketest);

        return NULL;
    }

    return juego_poketest;
}

bool cargar_pokemon(struct pokemon *pokemon, void *_juego_poketest)
{
    juego_poketest_t *juego_poketest = _juego_poketest;
    if (juego_poketest->cantidad_cargada < juego_poketest->semilla) {
        abb_insertar(juego_poketest->pokemones_juego, pokemon);
        juego_poketest->cantidad_cargada++;
        return true;
    }

    return false;
}

bool juego_poketest_cargar(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->pokemones_juego)
        return false;

    tp1_con_cada_pokemon(juego_poketest->archivo_pokemones, cargar_pokemon, juego_poketest);

    abb_vectorizar(juego_poketest->pokemones_juego, ABB_INORDEN, juego_poketest->semilla, juego_poketest->vector_1);
    abb_vectorizar(juego_poketest->pokemones_juego, ABB_PREORDEN, juego_poketest->semilla, juego_poketest->vector_2);

    return true;
}

void seleccionar_opcion(juego_poketest_t *juego_poketest, int *carta_1, int *carta_2)
{
    if (juego_poketest == NULL || carta_1 == NULL || carta_2 == NULL)
        return;
    
    if (juego_poketest->turno_jugador_1 == true) {
        printf("\n" ANSI_COLOR_BOLD JUGADOR_1 ANSI_COLOR_RESET "\n");
    } else {
        printf("\n" ANSI_COLOR_BOLD JUGADOR_2 ANSI_COLOR_RESET "\n");
    }

    printf(MENSAJE_SELECCION_CARTA_1);
    scanf("%d", carta_1);

    printf(MENSAJE_SELECCION_CARTA_2);
    scanf("%d", carta_2);
}

bool evaluar_cartas(juego_poketest_t *juego_poketest, int carta_1, int carta_2, int *indice_1, int *indice_2)
{
    if (carta_1 < 0 || carta_1 > juego_poketest->cantidad_tarjetas || carta_2 < 0 || carta_2 > juego_poketest->cantidad_tarjetas)
        return false;
    if (carta_1 == carta_2)
        return false;

    void *_pokemon_1, *_pokemon_2;
    if (carta_1 >= juego_poketest->semilla) {
        *indice_1 = carta_1 - (int)juego_poketest->semilla;
        _pokemon_1 = juego_poketest->vector_2[*indice_1];
    } else {
        *indice_1 = carta_1;
        _pokemon_1 = juego_poketest->vector_1[*indice_1];
    }
    if (carta_2 >= juego_poketest->semilla) {
        *indice_2 = carta_2 - (int)juego_poketest->semilla;
        _pokemon_2 = juego_poketest->vector_2[*indice_2];
    } else {
        *indice_2 = carta_2;
        _pokemon_2 = juego_poketest->vector_1[*indice_2];
    }

    struct pokemon *pokemon_1 = _pokemon_1;
    struct pokemon *pokemon_2 = _pokemon_2;

    if (comparador_pokemones(pokemon_1, pokemon_2) == 0)
        return true;
    
    return false;
}

void juego_poketest_mostrar_resultados(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->pokemones_juego)
        return;

    printf(ANSI_COLOR_BOLD FIN_JUEGO ANSI_COLOR_RESET"\n");
    printf(ANSI_COLOR_BOLD "Resultados:" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_BOLD JUGADOR_1": %ld" ANSI_COLOR_RESET "\n", juego_poketest->jugador_1.puntaje);
    printf(ANSI_COLOR_BOLD JUGADOR_2": %ld" ANSI_COLOR_RESET "\n", juego_poketest->jugador_2.puntaje);

    if (juego_poketest->jugador_1.puntaje > juego_poketest->jugador_2.puntaje)
        printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "\n Ganador: " JUGADOR_1 ANSI_COLOR_RESET "\n");
    else if (juego_poketest->jugador_1.puntaje < juego_poketest->jugador_2.puntaje)
        printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "\n Ganador: " JUGADOR_2 ANSI_COLOR_RESET "\n");
    else
        printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD EMPATE ANSI_COLOR_RESET "\n");

    printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
	sleep(4);
}

bool juego_poketest_jugar(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->pokemones_juego)
        return false;
    juego_poketest->turno_jugador_1 = true;

    tarjeta_t tarjetas[juego_poketest->cantidad_tarjetas];
    for (int i = 0; i < juego_poketest->cantidad_tarjetas; i++)
        tarjetas[i].es_rojo = false;

    size_t cantidad_rojos = 0;
    while (cantidad_rojos < juego_poketest->cantidad_tarjetas) {
        limpiar_pantalla_juego();

        size_t cantidad_actual = 0;
        while (cantidad_actual < juego_poketest->cantidad_tarjetas) {
            mostrar_tarjetas(&cantidad_actual, juego_poketest->cantidad_tarjetas, tarjetas);
        }

        int carta_1 = -1, carta_2 = -1, indice_1 = 0, indice_2 = 0;
        seleccionar_opcion(juego_poketest, &carta_1, &carta_2);
        bool resultado = evaluar_cartas(juego_poketest, carta_1, carta_2, &indice_1, &indice_2);
        if (resultado && juego_poketest->turno_jugador_1 == true) {
            juego_poketest->jugador_1.puntaje++;
            tarjetas[carta_1].es_rojo = true;
            tarjetas[carta_2].es_rojo = true;
            juego_poketest->turno_jugador_1 = false;
            cantidad_rojos += 2;
        } else if (resultado && juego_poketest->turno_jugador_1 == false) {
            juego_poketest->jugador_2.puntaje++;
            tarjetas[carta_1].es_rojo = true;
            tarjetas[carta_2].es_rojo = true;
            juego_poketest->turno_jugador_1 = true;
            cantidad_rojos += 2;
        }
    }

    juego_poketest_mostrar_resultados(juego_poketest);

    return true;
}

void juego_poketest_destruir(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->pokemones_juego)
        return;

    if (juego_poketest->pokemones_juego != NULL)
        abb_destruir(juego_poketest->pokemones_juego);
    free(juego_poketest);
}