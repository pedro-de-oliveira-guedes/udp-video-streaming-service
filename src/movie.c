#include "movie.h"

#include <stdio.h>
#include <stdlib.h>

Movie *createMovie(char *title, char *script[5]) {
    Movie *movie = (Movie *) malloc(sizeof(Movie));

    movie->title = title;
    for (int i = 0; i < 5; i++) {
        movie->script[i] = script[i];
    }

    return movie;
}

void runMovie(Movie *movie) {
    for (int i = 0; i < 5; i++) {
        printf("Scene %d: %s\n", i, movie->script[i]);
    }
}
