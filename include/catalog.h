//---------------------------------------------------------------------
// File	    : catalog.h
// Content	: Catalog TAD definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef CATALOG_H
#define CATALOG_H

#include "movie.h"

typedef struct Catalog {
    Movie *movies[3];
} Catalog;

/**
 * @brief Creates a new Catalog object, instantiating three movies.
 *        Currently, the three movies are "The Lord of the Rings", "The Godfather" and "Fight Club".
 * 
 * @return A pointer to the newly created Catalog object.
 */
Catalog* create_catalog();

/**
 * @brief This function takes a pointer to a Catalog object and displays its information.
 *
 * @param catalog A pointer to the Catalog object to be displayed.
 */
void display_catalog(Catalog *catalog);

/**
 * @brief This function releases the memory allocated for the catalog object and all its associated resources.
 *
 * @param catalog The catalog object to be destroyed.
 */
void destroy_catalog(Catalog *catalog);

#endif
