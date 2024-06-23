//---------------------------------------------------------------------
// File	    : catalog.h
// Content	: Catalog TAD definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef CATALOG_H
#define CATALOG_H

#include "movie.h"

#define CATALOG_SIZE 3

typedef struct Catalog {
    Movie *movies[CATALOG_SIZE];
} Catalog;

/**
 * @brief Creates a new Catalog object, instantiating three movies.
 *        Currently, the three movies are "The Lord of the Rings", "The Godfather" and "Fight Club".
 * 
 * @return A pointer to the newly created Catalog object.
 */
Catalog* createCatalog();

/**
 * @brief Checks if a movie ID is valid.
 * 
 * @param movieId The movie ID to be checked.
 * 
 * @return 1 if the movie ID is valid, 0 otherwise.
 */
int movieIdIsValid(int movieId);

/**
 * @brief This function takes a pointer to a Catalog object and displays its information.
 *
 * @param catalog A pointer to the Catalog object to be displayed.
 */
void displayCatalog(Catalog *catalog);

/**
 * @brief This function releases the memory allocated for the catalog object and all its associated resources.
 *
 * @param catalog The catalog object to be destroyed.
 */
void destroyCatalog(Catalog *catalog);

#endif
