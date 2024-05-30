#include "movie.h"

#include <stdio.h>

int main(int argc, char **argv) {
    char *script[5] = {
        "Hello, world!",
        "I'm a movie script.",
        "I'm a movie script.",
        "I'm a movie script.",
        "I'm a movie script."
    };

    Movie *movie = create_movie("My Movie", script);
    run_movie(movie);

    return 0;
}
