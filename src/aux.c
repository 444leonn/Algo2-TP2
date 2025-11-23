#include "aux.h"

#include "constantes.h"
#include "ansi.h"
#include "aux_tp1.h"

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
