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

Donde la clave almacenada se corresponde con el cáracter que identifica a la opción, y luego el valor, se correponde a una estructura llamada `opcion`, la cual contiene además de este cáracter, la descripción, la funcionalidad de esa opción y un puntero que puede ser utilizado dentro de la opción.

Esta segunda estructura considere que era optimo que fuera publica para el usuario, para el momento de la muestra de las opciones

Lo cual me permitiría a la hora de acceder a la opción que se busca ejecutar hacerlo con complejidad de _O(1)_. A diferencia de una implementación con una _lista_ o un _ABB_ que hubiera tenido mayores complejidades temporales a la hora de buscar cierta opción.

Tambien decidí utilizar dentro de la implementación un `enum formato_muestra` para poder controlar la manera en que se dibuja el menú.

#### **Creación Menu**

Permite crear una estructura de menú, con el nombre dado. Si se recibe un nombre `NULL` se crea un menú sin nombre.

#### **Agregar Opción**

Esta función permite al usuario agregar las opciones que desee utilizar dentro de su menú.

Por lo que para poder realizar esta acción se requiere todas las cáracterísticas que refieran a esta opción.

#### **Quitar Opción**

Permite eliminar una opcion recibiendo el cáracter que la identifica.

#### **Obtener Nombre**

Permite al usuario acceder al nombre del menú, por si se desea realizar alguna operacion con el mismo.

#### **Mostrar Menu**

Perrmite aplicarle una función a cada una de las opciones del menú.

Se espera que la función realize una operaciones de muestra, de eso deriva el nombre.

#### **Ejecutar Opción**

Permite ejecutar una opción a través del cáracter que la identifica. Para ello se utiliza la función de `hash_buscar` la cual nos permite obtener el valor almacenado para esa opción. Y luego utilizar la funcionalidad almacenada y ejecutarla.

#### **Destrucción**

Permite liberar la memoria reservada para la estructura del menú.

---

### Juego Poketest


Este TDA, se creó con el objetivo de poder hacer uso del juego, para poder usar su implementación se debe tener en cuenta que se requiere de un archivo de pokemones previamente cargado en una estructura `tp1`.

Además se utiliza la implementación del TDA `lista` para poder crear los registros de `historial` de las ultimas 5 jugadas realizadas por los jugadores.

En donde cada uno de los valores almacenados dentro de la lista se va a corresponder con un TDA `registro`. El cual contiene como descripción al jugador que sele ccionó, las cartas seleccionadas por ese jugador, y un variable de tipo `bool` que indica si el resultado fue exitoso o no.

#### **Creación Juego**

Para la creación de un juego se requiere de un archivo previamente cargado con pokemones, de un valor utilizado como semilla, y de la cantidad de tarjetas que se deseen tener en total para el juego.

#### **Carga de Pokemones**

Esta función se encarga de cargar dentro de las tarjetas del juego los pokemones que se encuentran dentro del archivo de pokemones.

Para ello se utilizan las funciones `srand()` con la semilla dada o `time(NULL)` para utilizar la hora como semilla aleatoria.

Luego se busca dentro de los pokemones con `rand()` un pokemon con `ID` aleatorio, y se lo inserta dentro de las tarjetas tambien con un indice aleatorio.

#### **Jugada**

Permite ejecutar una jugada con las dos tarjetas seleccionadas pasadas por párametro, con el `enum JUGADOR` pasado por párametro.

La función se encarga de verificar la jugada con el módulo de `evaluar_tarjetas()` el cual determina si la jugada fue correcta, o no.

Se crea un nuevo registro para el historial de jugadas, con el resultado de la jugada y las cartas seleccionadas, y se lo agrega a la `lista` utilizada para el historial.

Se retorna el resultado de la jugada.

#### **Obtener Ultimo Registro**

Permite acceder al ultimo registro del historial de jugadas, por si se desea realizar alguna operacion con el mismo.

Retorna un puntero a una estructura `registro_historial_t`.

#### **Progreso**

Permite acceder al progreso actual en el que se encuentra el juego.

Devuelve un numero flotante, el cual varia de 0.00 a 100.00, el cual representa el porcentaje del progreso.

#### **Puntaje**

Permite acceder al puntaje de alguno de los jugadores pertenecientes al `enum JUGADORES`

#### **Mostrar Juego**

Muestra el set de tarjetas completo con un diseño por pantalla, junto con los registros del historial.

#### **Destructor**

Libera toda la memoria empleada para el juego.

---

### Programa Principal

Para el desarrollo del programa principal utilicé un TDA `menu_poketest` el cual almacena todos los menú utilizados a lo largo de todo el programa, como también la semilla a utilizar, la cantidad de tarjetas que se desea utilizar en el juego (en este caso 18), y el archivo de pokemones de la estructura `tp1`. Además se utiliza una variable de tipo bool para controlar si se debe salir o no del programa.

Se utilizan multiples funciones auxiliares, las cuales nos permiten mostrar mensajes por pantalla, limpiar el buffer de entrada, utilizar la funcion `sleep()` para esperar una cantidad determinadas de segundos para mostrar un mensaje:

#### **Creación**

Esta función se encarga de reservar la memoria necesaria para todos los menús que se utilizaran a lo largo de la ejecución del programa, creando los tres menús con sus respectivos nombres.

#### **Construir**

Este módulo recibe la estructura previamente creada y se encarga de cargar todos los menú, agregando todas las opciones y funcionalidad necesaria para poder tener el programa con toda su funcionalidad.

#### **Comenzar**

Muestra el menú principal y pide el ingreso de una opción dentro de las disponibles, se utiliza la variable de salida como condición de corte, lo cual permite ir mostrando el menú luego de cada opción.

Utiliza la funcion `menú_ejecutar` que permite mostrar el menú y solicitar el reingreso de la opcion si la misma es invalida. Esta funcion tambien es utilizada en las operaciones que implican mostrar el menú de busqueda o el menú de muestra de los pokemones.

#### **Destruir**

Se encarga de liberar la memoria utilizada por las estructuras.

#### **Opciones**

Si bien al utilizar el menú almacenamos las opciones, considero que tiene relevancia mencionar como cada una de estas opciones fue diseñada.

- **Cargar Archivo**: Esta opción recibe la estructura de `menú_poketest` y solicita al usuario el nombre de un archivo, lo lee y carga los pokemones en una estructura `tp1`, guardandolo
- **Buscar**: La opción de buscar muestra dentro de la estructura de `menú_poketest` el menú que se corresponde con la busqueda, y solicita el ingreso de una de las opciones (buscar por nombre, por id o volver).
- **Mostrar**: Igual a la opcion anterior, pero muestra el menú relacionado con la muestra de los pokemones.
- **Jugar**: Crea y carga una estructura del juego con la semilla almacenada y comienza el juego. Utilizando el progreso como control del flujo del juego. Una vez se alcanza el 100% el juego finaliza y se muestran los resultados.
- **Jugar con Semilla**: Solicita el ingreso de una semilla y luego invoca a la opcion de jugar, pero con la semilla modificada.
- **Cambiar Estilo**: Modifica el estilo de dibujado del menú, intercambiandose entre los distintos formato ya establecidos.
- **Salir**: Modifica la variable de control de salida almacenada dentro de la estructura.

### Funciones Auxiliares

Las funciones que se encuentran dentro de `aux.h` fueron utilizadas en la implementacion tanto de los TDA, como para el programa principal.

Estos módulos nos facilitan ciertas operaciones principalmente relacionadas con la muestra por pantalla, ya que ciertos mensajes son mostrados de manera repetitiva en varias ocasiones a lo largo de los programas.

Tambien tenemos una funcion `limpia_buffer()` para el buffer de entrada, y dos comparadores de pokemones, uno por numero de ID y otro por nombre.