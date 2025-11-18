#include "juego_poketest.h"

#include <string.h>
#include <time.h>

void limpiar_pantalla_juego()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

void limpiar_buffer_juego() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void mostrar_tarjetas(size_t *cantidad_actual, size_t cantidad_total, tarjeta_t *tarjetas)
{
    size_t inicio = *cantidad_actual;
    size_t fin = (inicio + 6 < cantidad_total) ? inicio + 6 : cantidad_total;
    size_t tarjetas_a_mostrar = fin - inicio;

    printf(ANSI_COLOR_RESET);
    // TODO: Mejorar para que admita dos caracteres sin deformarse.
    for (size_t i = 0; i < tarjetas_a_mostrar; i++) {
        if (tarjetas[inicio + i].color_actual == COLOR_ROJO)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "┌———————————┐  " ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "┌———————————┐  " ANSI_COLOR_RESET);
    }
    printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);

    for (int j = 0; j < 2; j++) {
        for (size_t i = 0; i < tarjetas_a_mostrar; i++) {
            if (tarjetas[inicio + i].color_actual == COLOR_ROJO)
                printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
        }
        printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);
    }

    for (size_t i = 0; i < tarjetas_a_mostrar; i++) {
        if (tarjetas[inicio + i].color_actual == COLOR_ROJO)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|     %zu     |  " ANSI_COLOR_RESET, inicio + i);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|     %zu     |  " ANSI_COLOR_RESET, inicio + i);
    }
    printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);

    for (int j = 0; j < 2; j++) {
        for (size_t i = 0; i < tarjetas_a_mostrar; i++) {
            if (tarjetas[inicio + i].color_actual == COLOR_ROJO)
                printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
            else
                printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|           |  " ANSI_COLOR_RESET);
        }
        printf(ANSI_COLOR_RESET "\n" ANSI_COLOR_RESET);
    }

    for (size_t i = 0; i < tarjetas_a_mostrar; i++) {
        if (tarjetas[inicio + i].color_actual == COLOR_ROJO)
            printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "└———————————┘  " ANSI_COLOR_RESET);
        else
            printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "└———————————┘  " ANSI_COLOR_RESET);
    }
    printf(ANSI_COLOR_RESET "\n\n" ANSI_COLOR_RESET);

    *cantidad_actual = fin;
}

int comparador_pokemones(const void *_pokemon_a, const void *_pokemon_b)
{
    const struct pokemon *pokemon_a = _pokemon_a;
    const struct pokemon *pokemon_b = _pokemon_b;

    return pokemon_a->id - pokemon_b->id;
}

juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, size_t semilla)
{
    if (archivo_pokemones == NULL || tp1_cantidad(archivo_pokemones) == 0)
        return NULL;

    juego_poketest_t *juego_poketest = calloc(1, sizeof(juego_poketest_t));
    if (juego_poketest == NULL)
        return NULL;
    
    if (semilla < CANTIDAD_SEMILLA)
        semilla = CANTIDAD_SEMILLA;
    else if (semilla > tp1_cantidad(archivo_pokemones))
        semilla = tp1_cantidad(archivo_pokemones);
    
    juego_poketest->archivo_pokemones = archivo_pokemones;
    juego_poketest->semilla = semilla;
    juego_poketest->cantidad_cargada = 0;
    juego_poketest->cantidad_tarjetas = 2 * juego_poketest->semilla;
    juego_poketest->tarjetas = calloc(juego_poketest->cantidad_tarjetas, sizeof(tarjeta_t));
    juego_poketest->historial = lista_crear();
    if (juego_poketest->tarjetas == NULL || juego_poketest->historial == NULL) {
        free(juego_poketest->tarjetas);
        lista_destruir(juego_poketest->historial);
        free(juego_poketest);

        return NULL;
    }

    return juego_poketest;
}

bool cargar_pokemon(struct pokemon *pokemon, void *_juego_poketest)
{
    juego_poketest_t *juego_poketest = _juego_poketest;
    if (juego_poketest->cantidad_cargada < juego_poketest->semilla) {
        size_t pos_base = juego_poketest->cantidad_cargada * 2;
        
        juego_poketest->tarjetas[pos_base].id = pokemon->id;
        juego_poketest->tarjetas[pos_base].color_actual = COLOR_AZUL;
        juego_poketest->tarjetas[pos_base].valor = pokemon;
        
        juego_poketest->tarjetas[pos_base + 1].id = pokemon->id;
        juego_poketest->tarjetas[pos_base + 1].color_actual = COLOR_AZUL;
        juego_poketest->tarjetas[pos_base + 1].valor = pokemon;
        
        juego_poketest->cantidad_cargada++;

        return true;
    }

    return false;
}

void randomizar_tarjetas(tarjeta_t *tarjetas, size_t cantidad) {
    for (size_t i = cantidad - 1; i > 0; i--) {
        size_t j = (size_t) rand() % (i + 1);
        tarjeta_t aux = tarjetas[i];
        tarjetas[i] = tarjetas[j];
        tarjetas[j] = aux;
    }
}

bool juego_poketest_cargar(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->tarjetas)
        return false;

    srand((unsigned int)time(NULL));

    tp1_con_cada_pokemon(juego_poketest->archivo_pokemones, cargar_pokemon, juego_poketest);

    randomizar_tarjetas(juego_poketest->tarjetas, juego_poketest->cantidad_tarjetas);

    return true;
}

void seleccionar_tarjeta(juego_poketest_t *juego_poketest, int *carta_1, int *carta_2)
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
    limpiar_buffer_juego();

    printf(MENSAJE_SELECCION_CARTA_2);
    scanf("%d", carta_2);
    limpiar_buffer_juego();
}

bool evaluar_tarjetas(juego_poketest_t *juego_poketest, int carta_1, int carta_2)
{
    
    if (carta_1 < 0 || carta_1 >= juego_poketest->cantidad_tarjetas || 
        carta_2 < 0 || carta_2 >= juego_poketest->cantidad_tarjetas)
        return false;
    else if (carta_1 == carta_2)
        return false;
    
    sleep(1);
    printf(ANSI_COLOR_BOLD "\n        Cartas Seleccionadas:\n" ANSI_COLOR_RESET);
    sleep(1);
    printf(ANSI_COLOR_CYAN"            %d: %s" ANSI_COLOR_RESET "\n", carta_1, juego_poketest->tarjetas[carta_1].valor->nombre);
    sleep(1);
    printf(ANSI_COLOR_CYAN"            %d: %s" ANSI_COLOR_RESET "\n", carta_2, juego_poketest->tarjetas[carta_2].valor->nombre);
    sleep(1);

    if (comparador_pokemones(juego_poketest->tarjetas[carta_1].valor, juego_poketest->tarjetas[carta_2].valor) == 0) {
        juego_poketest->tarjetas[carta_1].color_actual = COLOR_ROJO;
        juego_poketest->tarjetas[carta_2].color_actual = COLOR_ROJO;
        return true;
    }
    return false;
}

void juego_poketest_mostrar_resultados(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest)
        return;

    printf(ANSI_COLOR_BOLD FIN_JUEGO ANSI_COLOR_RESET"\n\n");
    printf(ANSI_COLOR_BOLD "Resultados:" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_BOLD JUGADOR_1": %ld" ANSI_COLOR_RESET "\n", juego_poketest->jugador_1.puntaje);
    printf(ANSI_COLOR_BOLD JUGADOR_2": %ld" ANSI_COLOR_RESET "\n", juego_poketest->jugador_2.puntaje);

    if (juego_poketest->jugador_1.puntaje > juego_poketest->jugador_2.puntaje)
        printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "\n" MENSAJE_GANADOR JUGADOR_1 ANSI_COLOR_RESET"\n\n\n");
    else if (juego_poketest->jugador_1.puntaje < juego_poketest->jugador_2.puntaje)
        printf(ANSI_COLOR_GREEN ANSI_COLOR_BOLD "\n" MENSAJE_GANADOR JUGADOR_2 ANSI_COLOR_RESET "\n\n\n");
    else
        printf(ANSI_COLOR_MAGENTA ANSI_COLOR_BOLD EMPATE ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
    getchar();
}

registro_historial_t *crear_registro_historial(char *jugador, int primer_tarjeta, int tarjeta_2, bool resultado)
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
        printf(" %s: %d - %d \n", registro->registro, registro->primer_carta, registro->segunda_carta);
    } else {
        printf(ANSI_COLOR_RED CRUZ ANSI_COLOR_RESET);
        printf(" %s: %d - %d \n", registro->registro, registro->primer_carta, registro->segunda_carta);
    }

    return true;
}

bool juego_poketest_jugar(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->tarjetas || !juego_poketest->historial)
        return false;
    juego_poketest->turno_jugador_1 = true;

    size_t cantidad_rojas = 0;
    while (cantidad_rojas < juego_poketest->cantidad_tarjetas) {
        limpiar_pantalla_juego();
        printf("\n"ANSI_COLOR_BOLD INICIO_JUEGO ANSI_COLOR_RESET"\n\n");
        size_t cantidad_actual = 0;
        while (cantidad_actual < juego_poketest->cantidad_tarjetas) {
            mostrar_tarjetas(&cantidad_actual, juego_poketest->cantidad_tarjetas, juego_poketest->tarjetas);
        }
        lista_con_cada_elemento(juego_poketest->historial, mostrar_registro, NULL);

        int carta_1 = POSICION_INVALIDA, carta_2 = POSICION_INVALIDA;
        seleccionar_tarjeta(juego_poketest, &carta_1, &carta_2);
        bool resultado = evaluar_tarjetas(juego_poketest, carta_1, carta_2);
        
        registro_historial_t *registro_actual = NULL;
        if (juego_poketest->turno_jugador_1 == true)
            registro_actual = crear_registro_historial(JUGADOR_1, carta_1, carta_2, resultado);
        else
            registro_actual = crear_registro_historial(JUGADOR_2, carta_1, carta_2, resultado);
        
        if (registro_actual != NULL) {
            if (lista_cantidad(juego_poketest->historial) >= MAX_HISTORIAL) {
                registro_historial_t *eliminado = lista_eliminar_elemento(juego_poketest->historial, 0);
                destructor_registro_historial(eliminado);
            }
            lista_agregar(juego_poketest->historial, registro_actual);
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

    juego_poketest_mostrar_resultados(juego_poketest);

    return true;
}

void juego_poketest_destruir(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest)
        return;

    if (juego_poketest->tarjetas != NULL)
        free(juego_poketest->tarjetas);
    if (juego_poketest->historial != NULL)
        lista_destruir_todo(juego_poketest->historial, destructor_registro_historial);
    free(juego_poketest);
}