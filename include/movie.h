//---------------------------------------------------------------------
// File	    : movie.h
// Content	: Movie TAD definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef MOVIE_H
#define MOVIE_H

typedef struct {
    char *title;
    char *script[5];
} Movie;

/**
 * @brief Creates a new Movie object with the given title and script.
 *
 * @param title The title of the movie.
 * @param script An array of strings representing the script of the movie.
 *               The array must have a size of 5.
 * @return A pointer to the newly created Movie object.
 */
Movie *create_movie(char *title, char *script[5]);

/**
 * @brief Displays the script of the given movie.
 *
 * @param movie The movie to be run.
 */
void run_movie(Movie *movie);

#endif