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

void mostrar_tarjeta_roja(tarjeta_t tarjeta)
{
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "┌————————————┐  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    if (tarjeta.numero_tarjeta < 10)
        printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|     %zu      |  " ANSI_COLOR_RESET, tarjeta.numero_tarjeta);
    else
        printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|     %zu     |  " ANSI_COLOR_RESET, tarjeta.numero_tarjeta);
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "└————————————┘  " ANSI_COLOR_RESET);
}

void mostrar_tarjeta_azul(tarjeta_t tarjeta)
{
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "┌————————————┐  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    if (tarjeta.numero_tarjeta < 10)
        printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|     %zu      |  " ANSI_COLOR_RESET, tarjeta.numero_tarjeta);
    else
        printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|     %zu     |  " ANSI_COLOR_RESET, tarjeta.numero_tarjeta);
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "|            |  " ANSI_COLOR_RESET);
    printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD "└————————————┘  " ANSI_COLOR_RESET);
}

void mostrar_tarjeta(tarjeta_t tarjeta)
{
    if (tarjeta.color_actual == COLOR_ROJO)
        mostrar_tarjeta_roja(tarjeta);
    else
        mostrar_tarjeta_azul(tarjeta);
}

void mostrar_tarjetas(tarjeta_t *tarjetas, size_t cantidad_total)
{
    if (tarjetas == NULL)
        return;

    printf(ANSI_COLOR_RESET);
    size_t i = 0;
    while (i < cantidad_total) {
        for (int j = 0; j < 6; j++) {
            mostrar_tarjeta[tarjetas[i]];
            i++;
        }
        printf("\n");
    }
    printf(ANSI_COLOR_RESET "\n\n" ANSI_COLOR_RESET);
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
    for (int i = 0; i < cantidad; i++)
        tarjetas[i].numero_tarjeta = i;
}

juego_poketest_t *juego_poketest_crear(tp1_t *archivo_pokemones, int semilla)
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
    juego_poketest->cantidad_tarjetas = CANTIDAD_TARJETAS;
    juego_poketest->tarjetas = crear_tarjetas(juego_poketest->cantidad_tarjetas);
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
    
    if (juego_poketest->cantidad_cargada + 2 > juego_poketest->cantidad_tarjetas)
        return false;
    
    size_t posicion_1 = rand() % juego_poketest->cantidad_tarjetas;
    while (juego_poketest->tarjetas[posicion_1].valor != NULL)
        posicion_1 = rand() % juego_poketest->cantidad_tarjetas;
    juego_poketest->tarjetas[posicion_1].id_pokemon = pokemon->id;
    juego_poketest->tarjetas[posicion_1].color_actual = COLOR_AZUL;
    juego_poketest->tarjetas[posicion_1].valor = pokemon;
    juego_poketest->cantidad_cargada++;
    
    size_t posicion_2 = rand() % juego_poketest->cantidad_tarjetas;
    while (juego_poketest->tarjetas[posicion_2].valor != NULL)
        posicion_2 = rand() % juego_poketest->cantidad_tarjetas;
    juego_poketest->tarjetas[posicion_2].id_pokemon = pokemon->id;
    juego_poketest->tarjetas[posicion_2].color_actual = COLOR_AZUL;
    juego_poketest->tarjetas[posicion_2].valor = pokemon;
    juego_poketest->cantidad_cargada++;

    return true;
}

bool juego_poketest_cargar(juego_poketest_t *juego_poketest)
{
    if (!juego_poketest || !juego_poketest->archivo_pokemones || !juego_poketest->tarjetas)
        return false;

    if (juego_poketest->semilla != 0)
        srand((unsigned int)juego_poketest->semilla);
    else
        srand((unsigned int)time(NULL));

    size_t cantidad_cargada = tp1_con_cada_pokemon(juego_poketest->archivo_pokemones, cargar_pokemon, juego_poketest);
    while (cantidad_cargada < juego_poketest->cantidad_tarjetas)
        cantidad_cargada += tp1_con_cada_pokemon(juego_poketest->archivo_pokemones, cargar_pokemon, juego_poketest);

    return true;
}

void seleccionar_tarjeta(juego_poketest_t *juego_poketest, int *carta_1, int *carta_2)
{
    if (juego_poketest == NULL || carta_1 == NULL || carta_2 == NULL)
        return;
    
    if (juego_poketest->turno_jugador_1 == true)
        printf("\n" ANSI_COLOR_BOLD JUGADOR_1 ANSI_COLOR_RESET "\n");
    else
        printf("\n" ANSI_COLOR_BOLD JUGADOR_2 ANSI_COLOR_RESET "\n");

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

    if (comparador_pokemones(juego_poketest->tarjetas[carta_1].valor, juego_poketest->tarjetas[carta_2].valor) == 0)
        if (juego_poketest->tarjetas[carta_1].color_actual == COLOR_AZUL && juego_poketest->tarjetas[carta_2].color_actual == COLOR_AZUL) {
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
        while (cantidad_actual < juego_poketest->cantidad_tarjetas)
            mostrar_tarjetas(juego_poketest->tarjetas, juego_poketest->cantidad_tarjetas);
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