<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TP2

## Repositorio de Leon Acosta - 113246 - leacosta@fi.uba.ar

- Para la compilación y ejecución del programa se provee un **makefile**.

  - Compilar:

    ```bash
    # compilar el programa
    make tp2
    # compilar las pruebas
    make pruebas_alumno
    ```

  - Ejecutar:

    ```bash
    # correr el programa
    make ejecutar
    # correr con valgrind
    make valgrind
    # correr las pruebas
    make
    ```

> [!IMPORTANT]
> Para poder hacer uso de la funcionalidad del programa se requiere de un archivo en formato CSV el cual cargar.
>
> El mismo debe contener pokemones con el siguiente formato.
>
> ID,NOMBRE,TIPO,ATAQUE,DEFENSA,VELOCIDAD

---

## Funcionamiento

El programa funciona a partir de una interfaz que permite al usuario interactuar con la terminal a través de distintos menúes.

Para el desarrollo del juego decidí realizar los siguientes TDA.

---

### Menú

Este Tipo de Dato Abstracto busca contener las distintas opciones que pueda requerir el usuario, junto con su funcionalidad.

Para la implementación de este TDA decidí utilizar la implementación realizada para la Tabla de Hash.

Donde la clave almacenada se corresponde con el caracter que identifica a la opción, y luego el valor, se correponde a un TDA llamado `opcion`, el cual contiene ademas de este caracter, la descripción, la funcionalidad de esa opción y un puntero que puede ser utilizado dentro de la opción.

Lo cual me permitiría a la hora de acceder a la opción que se busca ejecutar hacerlo con complejidad de _O(1)_. A diferencia de una implementación con una _lista_ o un _ABB_ que hubiera tenido mayores complejidades temporales a la hora de buscar cierta opción.

Tambien decidí utilizar dentro de la implementación un `enum formato_muestra` para poder controlar la manera en que se dibuja el menu.

#### **Creación**

Permite crear una estructura de menú, con el nombre dado. Si se recibe un nombre `NULL` se crea un menú sin nombre.

#### **Agregar Opción**

Esta función permite al usuario agregar las opciones que desee utilizar dentro de su menu.

Por lo que para poder realizar esta acción se requiere todas las características que refieran a esta opción.

#### **Agregar Formato**

Permite al usuario agregar un formato de dibujado para las opciones del menú.

Para ello se requiere de una función que reciba el caracter que identifica a la opción y el string de descripción de esa opción.

#### **Seleccionar Formato**

Permite al usuario modificar el formato de dibujado para el menu, haciendo empleo del tipo enum de la estructura.

Si se elige el formato personalizado `FORMATO_CUSTOM` pero el mismo no ha sido agregado o no existe, se utiliza el formato por defecto que es el `FORMATO_1`.

#### **Mostrar Nombre**

Permite mostrar por pantalla el nombre del menu.
Si el menu no tiene nombre no muestra nada.

#### **Mostrar**

Perrmite mostrar todas las opciones cargadas en el menú, según el formato que se haya seleccionado anteriormente.

#### **Mostrar Completo**

Permite mostrar tanto el nombre del menú como tambien las opciones almacenadas dentro del mismo.

#### **Ejecutar Opción**

Permite ejecutar una opción a través del caracter que la identifica. Para ello se utiliza la función de `hash_buscar` la cual nos permite obtener el valor almacenado para esa opción. Y luego utilizar la funcionalidad almacenada y ejecutarla.

#### **Destrucción**

Permite liberar la memoria reservada para la estructura del menú.

---

### Juego Poketest


Este TDA, se creó con el objetivo de poder hacer uso del juego, para poder usar su implementación se debe tener en cuenta que se requiere de un archivo de pokemones previamente cargado en una estructura `tp1`.

Además se utiliza la implementación del TDA `lista` para poder crear los registros de `historial` de las ultimas 5 jugadas realizadas por los jugadores.

En donde cada uno de los valores almacenados dentro de la lista se va a corresponder con un TDA `registro`. El cual contiene como descripción al jugador que seleccionó, las cartas seleccionadas por ese jugador, y un variable de tipo `bool` que indica si el resultado fue exitoso o no.

#### **Creación**

Para la creación de un juego se requiere de un archivo previamente cargado con pokemon, de un valor utilizado como semilla, y de la cantidad de tarjetas que se deseen tener en total para el juego.

#### **Carga de Pokemones**

Esta función se encarga de cargar dentro de las tarjetas del juego los pokemones que se encuentran dentro del archivo de pokemones.

Para ello se utilizan las funciones `srand()` con la semilla dada o `time(NULL)` para utilizar la hora como semilla aleatoria.

Luego se busca dentro de los pokemones con `rand()` un pokemon con `ID` aleatorio, y se lo inserta dentro de las tarjetas tambien con un indice aleatorio.

#### **Jugar**

Con este módulo podemos comenzar el juego. Para ellos se utilizan ciertos módulos auxiliares para poder mostrar las tarjetas, pedir la selección de cartas por parte de los jugadores y evaluar la coincidencia entre ambas.

Para ello se hacen uso de ciertas funciones auxiliares que nos lo permiten.

- `limpiar_pantalla()`: nos permite limpiar la terminal, dejándola preparada para cualquier cosa que se quiera mostrar.
- `mostrar_juego()`: muestra la totalidad de las tarjetas por pantalla con hasta un máximo de 6 tarjetas consecutivas.
- `seleccionar_tarjetas()`: lee por entrada las tarjetas que se seleccionen.
- `evaluar_resultado()`: evalúa las cartas seleccionadas y si son válidas y coinciden devuelve `true`.
- `crear_registro_historial()`: crea el registro de la jugada ejecutada oara guardar dentro del historial.

Si el resultado de la jugada ejectuada es exitoso, se aumenta el puntaje del jugador actual.

Se vuelve a ejecutar toda la secuencia del juego hasta que se terminen las tarjetas a seleccionar disponibles.

Cuando dos tarjetas fueron seleccionadas y coinciden, son marcadas como rojas. Y la cantidad de tarjetas rojas almacenada dentro de la estructura aumenta.

#### **Destructor**

Libera toda la memoria empleada para el juego.

---

### Programa Principal

Para el desarrollo del programa principal utilicé un TDA `menu_poketest` el cual almacena todos los menú utilizados a lo largo de todo el programa, como también la semilla a utilizar, la cantidad de tarjetas que se desea utilizar en el juego (en este caso 18), y el archivo de pokemones de la estructura `tp1`. Además se utiliza una variable de tipo bool para controlar si se debe salir o no del programa.

Se utilizan multiples funciones auxiliares, las cuales nos permiten mostrar mensajes por pantalla, limpiar el buffer de entrada, utilizar la funcion `sleep()` para esperar una cantidad determinadas de segundos para mostrar un mensaje:

#### **Creación**

Esta función se encarga de reservar la memoria necesaria para todos los menús que se utilizaran a lo largo de la ejecución del programa, creando los tres menus con sus respectivos nombres.

#### **Construir**

Este módulo recibe la estructura previamente creada y se encarga de cargar todos los menu, agregando todas las opciones y funcionalidad necesaria para poder tener el programa con toda su funcionalidad.

#### **Comenzar**

Muestra el menú principal y pide el ingreso de una opción dentro de las disponibles, se utiliza la variable de salida como condición de corte, lo cual permite ir mostrando el menu luego de cada opción.

#### **Destruir**

Se encarga de liberar la memoria utilizada por las estructuras.

#### **Opciones**

Si bien al utilizar el menú almacenamos las opciones, considero que tiene relevancia mencionar como cada una de estas opciones fue diseñada.

- **Cargar Archivo**: Esta opción recibe la estructura de `menu_poketest` y solicita al usuario el nombre de un archivo, lo lee y carga los pokemones en una estructura `tp1`, guardandolo
- **Buscar**: La opción de buscar muestra dentro de la estructura de `menu_poketest` el menu que se corresponde con la busqueda, y solicita el ingreso de una de las opciones (buscar por nombre, por id o volver).
- **Mostrar**: Igual a la opcion anterior, pero muestra el menu relacionado con la muestra de los pokemones.
- **Jugar**: Crea y carga una estructura del juego con la semilla almacenada y comienza el juego.
- **Jugar con Semilla**: Solicita el ingreso de una semilla y luego invoca a la opcion de jugar, pero con la semilla modificada.
- **Cambiar Estilo**: Modifica el estilo de dibujado del menu, intercambiandose entre los distintos formato ya establecidos.
- **Salir**: Modifica la variable de control de salida almacenada dentro de la estructura.
