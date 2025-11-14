#ifndef CONSTANTES_H_
#define CONSTANTES_H_

// MODOS DE APERTURA DE ARCHIVO
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

// VALORES PARA INICIALIZACIONES
#define TAMAÑO_INICIAL 15
#define CAPACIDAD_INICIAL 4
#define CAPACIDAD_MINIMA 3
#define CAPACIDAD_TABLA 20

#define CANT_INVALIDA 1
#define ERROR -1
#define TIPO_INVALIDO -1
#define POSICION_INVALIDA -1

// PARA VALIDAR PARAMETROS
#define OPCION_ID "idIDIdiD"
#define OPCION_NOMBRE "nombrenombrEnombREnomBREnoMBREnOMBRENOMBRE"

// NOMBRES MENU
#define MENSAJE_SELECCION "Seleccione una Opcion ...  "
#define MENU_PRUEBA "Menu Prueba"
#define NOMBRE_JUEGO "TP2 PokeTest"

// MENSAJES DE ERROR
#define CANT_PARAMETROS \
	"Error, debe ingresar una cantidad de parametros valida."
#define PARAMETRO_INVALIDO "Error, parametro ingresado invalido."
#define FALLO_LECTURA_ARCHIVO "Error, ha falla la lectura del archivo."
#define FALLO_CREACION_ABB "Error, ha fallado creacion de ABB."
#define FALLO_CREACION_HASH "Error, ha fallado creacion de HASH."
#define NO_ENCONTRADO "El Pokemon no ha sido encontrado."
#define OPCION_INVALIDA "Error, opcion de busqueda invalida."
#define OPCION_NO_ENCONTRADA "Error, la opcion no ha sido encontrada"

// OPCIONES JUEGO
#define MENU_ANTERIOR "Volver al Menu Anterior"

#define CHAR_C 'C'
#define CARGAR_ARCHIVO "Cargar Archivo"

#define CHAR_B 'B'
#define BUSCAR "Buscar Pokemon"
#define CHAR_N 'N'
#define BUSCAR_NOMBRE "Buscar por Nombre"
#define CHAR_I 'I'
#define BUSCAR_ID "Buscar por ID"

#define CHAR_M 'M'
#define MOSTRAR "Mostrar Pokemones"
#define MOSTRAR_NOMBRE "Mostrar por Nombre"
#define MOSTRAR_ID "Mostrar por ID"

#define CHAR_J 'J'
#define JUGAR "Jugar"

#define CHAR_S 'S'
#define JUGAR_SEMILLA "Jugar con Semilla"

#define CHAR_E 'E'
#define ESTILO "Cambiar Estilo"

#define CHAR_Q 'Q'
#define SALIR "Salir"


#endif // CONSTANTES_H_
