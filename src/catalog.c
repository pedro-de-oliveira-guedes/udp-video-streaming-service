#include "catalog.h"

#include <stdio.h>
#include <stdlib.h>

Movie *get_lord_of_the_rings_movie(int shouldIncludeScripts) {
    if (shouldIncludeScripts) {
        return createMovie("O Senhor dos Anéis", (char *[]) {
            "Um anel para a todos governar",
            "Não é o que temos, mas o que fazemos com o que temos",
            "Não é o que somos, mas o que fazemos que define quem somos",
            "A jornada é mais importante que o destino",
            "A esperança é a última que morre"
        });
    } else {
        return createMovie("O Senhor dos Anéis", NULL);
    }
}

Movie *get_godfather_movie(int shouldIncludeScripts) {
    if (shouldIncludeScripts) {
        return createMovie("O Poderoso Chefão", (char *[]) {
            "Vou fazer uma oferta que ele não pode recusar",
            "Mantenha seus amigos por perto, mas seus inimigos mais perto ainda",
            "É melhor ser temido que amado",
            "A vingança é um prato que se come frio",
            "Nunca deixe que ningúem saiba o que você está pensando"
        });
    } else {
        return createMovie("O Poderoso Chefão", NULL);
    }
}

Movie *get_fight_club_movie(int shouldIncludeScripts) {
    if (shouldIncludeScripts) {
        return createMovie("Clube da Luta", (char *[]) {
        "Primeira regra do Clube da Luta: você não fala sobre o Clube da Luta",
        "Segunda regra do Clube da Luta: você não fala sobre o Clube da Luta",
        "O que você possui acabará possuindo você",
        "É apenas depois de perder tudo que somos livres para fazer qualquer coisa",
        "Escolha suas lutas com sabedoria"
    });
    } else {
        return createMovie("Clube da Luta", NULL);
    }
}

Catalog* createCatalog(int shouldIncludeScripts) {
    Catalog *catalog = (Catalog *) malloc(sizeof(Catalog));

    catalog->movies[0] = get_lord_of_the_rings_movie(shouldIncludeScripts);
    catalog->movies[1] = get_godfather_movie(shouldIncludeScripts);
    catalog->movies[2] = get_fight_club_movie(shouldIncludeScripts);

    return catalog;
}

int movieIdIsValid(int movieId) {
    return movieId > 0 && movieId <= CATALOG_SIZE;
}

void displayCatalog(Catalog *catalog) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        printf("$ %d - %s\n", i + 1, catalog->movies[i]->title);
    }
}

void destroyCatalog(Catalog *catalog) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        free(catalog->movies[i]);
    }

    free(catalog);
}