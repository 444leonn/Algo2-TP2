#ifndef CONSTANTES_H_
#define CONSTANTES_H_

// MODOS DE APERTURA DE ARCHIVO
#define MODO_LECTURA "r"
#define MODO_ESCRITURA "w"

// VALORES PARA INICIALIZACIONES
#define TAMAÑO_INICIAL 15
#define CAPACIDAD_INICIAL 4
#define CAPACIDAD_MINIMA 3
#define CAPACIDAD_TABLA 10

#define CANT_INVALIDA 1
#define ERROR -1
#define TIPO_INVALIDO -1
#define POSICION_INVALIDA -1

// PARA VALIDAR PARAMETROS
#define OPCION_ID "idIDIdiD"
#define OPCION_NOMBRE "nombrenombrEnombREnomBREnoMBREnOMBRENOMBRE"
#define OPCIONES_JUEGO "bBcCeEjJmMqQsS"
#define OPCION_VOLVER "zZ"
#define OPCION_BUSQUEDA_MUESTRA "iInN"

// MENSAJES DE ERROR
#define CANT_PARAMETROS \
	"Error, debe ingresar una cantidad de parametros valida."
#define PARAMETRO_INVALIDO "Error, parametro ingresado invalido."
#define FALLO_LECTURA_ARCHIVO "Error, ha fallado la lectura del archivo."
#define FALLO_CREACION_ABB "Error, ha fallado creacion de ABB."
#define FALLO_CREACION_HASH "Error, ha fallado creacion de HASH."
#define NO_ENCONTRADO "El Pokemon no ha sido encontrado."
#define OPCION_INVALIDA "Error, opcion invalida."
#define OPCION_NO_ENCONTRADA "Error, la opcion no ha sido encontrada."
#define FALTA_ARCHIVO "Error, debe cargar un archivo primero."
#define FALLO "Error, algo ha fallado."

// MENU
#define CANTIDAD_MENUS 3
// Nombre Menu
#define MENU_PRUEBA "Menu Prueba"
#define NOMBRE_JUEGO "TP2 PokeTest"
// Mensajes
#define INTENTE_NUEVAMENTE "Intente ingresar una opcion valida."
#define MENSAJE_SELECCION "Seleccione una Opcion ...  "
#define MENSAJE_INSTRUCCION_ARCHIVO "Cargar Archivo: Ingrese el path de un archivo en formato .csv.\nEl cual contenga pokemones que desee cargar para el juego."
#define MENSAJE_ARCHIVO "Ingrese el nombre de un archivo (path):  "
#define MENSAJE_CARGADO "Archivo Cargado Correctamente."
#define MENSAJE_CONTINUAR "Presione ENTER para continuar ...  "
#define MENSAJE_VOLVER "Presione Z para volver atras."
#define MENSAJE_VOLVIENDO "Volviendo al Menu de Inicio ..."
#define MENSAJE_ESTILO "Cambiando al siguiente estilo del Menu ..."
#define MENSAJE_INGRESAR_ID "Ingrese el ID de un pokemon:  "
#define MENSAJE_INGRESAR_NOMBRE "Ingrese el NOMBRE de un pokemon:  "
#define MENSAJE_POKEMON_ENCONTRADO "Pokemon Encontrado: "

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
#define CANTIDAD_SEMILLA 5

#define CHAR_E 'E'
#define ESTILO "Cambiar Estilo"

#define CHAR_Q 'Q'
#define SALIR "Salir"


#endif // CONSTANTES_H_
