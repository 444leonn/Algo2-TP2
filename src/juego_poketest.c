#include "juego_poketest.h"

#include <stdio.h>
#include <unistd.h>
#include <stdio_ext.h>

typedef struct aux_pokemones {
	struct pokemon *pokemones;
	size_t cantidad;
} aux_pokemones_t;

