#include "aux.h"

#include "menu.h"
#include "constantes.h"
#include "ansi.h"
#include "aux_tp1.h"

#include <ctype.h>

void limpiar_buffer()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF)
		;
}

void limpiar_pantalla()
{
	printf(ANSI_CLEAR_SCREEN);
	printf(ANSI_RESET_SCREEN);
}

bool evaluar_opciones(char *opcion_evaluada, char *opciones_disponibles)
{
	return strstr(opciones_disponibles, opcion_evaluada) != NULL;
}

void esperar_segundos(size_t cantidad)
{
	for (int i = 0; i < cantidad; i++)
		sleep(1);
}

bool evaluar_volver_menu(char *opcion_evaluada)
{
	if (evaluar_opciones(opcion_evaluada, OPCION_VOLVER) == true) {
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	return false;
}

char seleccionar_opcion()
{
	printf("\n\n" ANSI_COLOR_BOLD MENSAJE_SELECCION ANSI_COLOR_RESET);
	int aux = fgetc(stdin);
	int siguiente = fgetc(stdin);
	if ((char)siguiente != '\n' && (char)siguiente != EOF) {
		ungetc(siguiente, stdin);
	}
	char c = (char)aux;
	printf("\n");

	return c;
}

bool validar_archivo_pokemones(tp1_t *archivo_pokemones)
{
	if (archivo_pokemones == NULL) {
		printf(ANSI_COLOR_RED FALTA_ARCHIVO ANSI_COLOR_RESET "\n");
		printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
		esperar_segundos(2);
		return true;
	}
	return false;
}

void mostrar_mensaje_cargar()
{
	printf(ANSI_COLOR_BLUE MENSAJE_CARGAR_ARCHIVO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_INSTRUCCION_ARCHIVO ANSI_COLOR_RESET
	       "\n");
	printf(ANSI_COLOR_MAGENTA MENSAJE_VOLVER ANSI_COLOR_RESET "\n\n");
	esperar_segundos(2);
}

void mostrar_mensaje_fallo_lectura()
{
	printf(ANSI_COLOR_RED FALLO_LECTURA_ARCHIVO ANSI_COLOR_RESET "\n\n");
	printf(ANSI_COLOR_BLUE MENSAJE_VOLVIENDO ANSI_COLOR_RESET "\n");
	esperar_segundos(2);
}

void mostrar_mensaje_opcion_invalida()
{
	printf(ANSI_COLOR_RED ANSI_COLOR_BOLD OPCION_INVALIDA ANSI_COLOR_RESET
	       "\n");
	printf(ANSI_COLOR_BLUE ANSI_COLOR_BOLD
		       INTENTE_NUEVAMENTE ANSI_COLOR_RESET "\n\n");
	sleep(2);
}

void mostrar_mensaje_continuar()
{
	printf("\n" ANSI_COLOR_BLUE MENSAJE_CONTINUAR ANSI_COLOR_RESET);
	getchar();
	limpiar_buffer();
}

char *leer_nombre_archivo()
{
	char *nombre = leer_linea(stdin);
	if (nombre == NULL) {
		mostrar_mensaje_fallo_lectura();
		return NULL;
	}
	return nombre;
}

void convertir_palabra(char *palabra)
{
	int i = 0;
	if (palabra[i] != '\0') {
		palabra[i] = (char)toupper((int)palabra[i]);
		i++;
	}

	while (palabra[i] != '\0') {
		palabra[i] = (char)tolower((int)palabra[i]);
		i++;
	}
}

bool mostrar_formato_1(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return false;

	opcion_t *opcion = _opcion;
	printf(ANSI_COLOR_BOLD "%s) " ANSI_COLOR_RESET "%s", clave,
	       opcion->descripcion);
	printf("\n");

	return true;
}

bool mostrar_formato_2(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return false;

	opcion_t *opcion = _opcion;

	printf(ANSI_COLOR_RED ANSI_COLOR_BOLD "%s" ANSI_COLOR_RESET ". "
					      "%s",
	       clave, opcion->descripcion);
	printf("\n");

	return true;
}

bool mostrar_formato_3(char *clave, void *_opcion, void *aux)
{
	if (clave == NULL || _opcion == NULL)
		return false;

	opcion_t *opcion = _opcion;

	printf(ANSI_COLOR_GREEN "┌—————┐\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_GREEN "|" ANSI_COLOR_RESET ANSI_COLOR_BOLD
				"  %s  " ANSI_COLOR_RESET ANSI_COLOR_GREEN
				"|" ANSI_COLOR_RESET,
	       clave);
	printf(" %s\n", opcion->descripcion);
	printf(ANSI_COLOR_GREEN "└—————┘\n\n" ANSI_COLOR_RESET);

	return true;
}

void mostrar_nombre_menu(char *nombre)
{
	if (nombre == NULL)
		return;

	size_t largo_nombre = strlen(nombre);
	for (int i = 0; i < largo_nombre * 4; i++)
		printf("=");
	printf("\n");
	printf("|");
	for (int i = 0; i < largo_nombre * 4 / 3; i++)
		printf(" ");
	printf("%s", nombre);
	for (int i = 0; i < largo_nombre * 4 / 3 + 1; i++)
		printf(" ");
	printf("|");
	printf("\n");
	for (int i = 0; i < largo_nombre * 4; i++)
		printf("=");

	printf("\n");
}

void mostrar_progreso_juego(float progreso)
{
	printf(ANSI_BG_GREEN "\n\nProgreso: [ %.0f%% ]" ANSI_BG_RESET,
	       progreso);
	printf(ANSI_COLOR_BOLD "  [" ANSI_COLOR_RESET);

	int i = 0;
	for (i = 0; i < (int)progreso / 5; i++)
		printf(ANSI_COLOR_BOLD "#" ANSI_COLOR_RESET);
	if (i < 20)
		for (int j = i; j < 20; j++)
			printf(ANSI_COLOR_BOLD "." ANSI_COLOR_RESET);
	printf(ANSI_COLOR_BOLD "]" ANSI_COLOR_RESET);

	printf("\n\n");
}

void seleccionar_tarjetas(int *carta_1, int *carta_2)
{
	if (carta_1 == NULL || carta_2 == NULL)
		return;

	printf(MENSAJE_SELECCION_CARTA_1);
	if (scanf("%d", carta_1) != 1)
		limpiar_buffer();

	printf(MENSAJE_SELECCION_CARTA_2);
	if (scanf("%d", carta_2) != 1)
		limpiar_buffer();
}

int comparador_pokemones_id(const void *_pokemon_a, const void *_pokemon_b)
{
	const struct pokemon *pokemon_a = _pokemon_a;
	const struct pokemon *pokemon_b = _pokemon_b;
	return pokemon_a->id - pokemon_b->id;
}

int comparador_pokemones_nombre(const void *_pokemon_a, const void *_pokemon_b)
{
	const struct pokemon *pokemon_a = _pokemon_a;
	const struct pokemon *pokemon_b = _pokemon_b;

	return strcmp(pokemon_a->nombre, pokemon_b->nombre);
}