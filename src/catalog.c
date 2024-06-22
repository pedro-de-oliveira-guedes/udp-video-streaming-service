#include "catalog.h"

#include <stdio.h>
#include <stdlib.h>

Movie *get_lord_of_the_rings_movie() {
    Movie *lord_of_the_rings = createMovie("O Senhor dos Anéis", (char *[]) {
        "Um Anel para a todos governar",
        "Na Terra de Mordor onde as Sombras se deitam",
        "Não é o que temos, mas o que fazemos com o que temos",
        "Não há mal que sempre dure",
        "O mundo está mudando, senhor Frodo"
    });

    return lord_of_the_rings;
}

Movie *get_godfather_movie() {
    Movie *godfather = createMovie("O Poderoso Chefão", (char *[]) {
        "Vou fazer uma oferta que ele não pode recusar",
        "Mantenha seus amigos por perto, mas seus inimigos mais perto ainda",
        "É melhor ser temido que amado",
        "A vingança é um prato que se come frio",
        "Nunca deixe que ningúem saiba o que você está pensando"
    });

    return godfather;
}

Movie *get_fight_club_movie() {
    Movie *fight_club = createMovie("Clube da Luta", (char *[]) {
        "Primeira regra do Clube da Luta: você não fala sobre o Clube da Luta",
        "Segunda regra do Clube da Luta: você não fala sobre o Clube da Luta",
        "O que você possui acabará possuindo você",
        "É apenas depois de perder tudo que somos livres para fazer qualquer coisa",
        "Escolha suas lutas com sabedoria"
    });
    
    return fight_club;
}

Catalog* createCatalog() {
    Catalog *catalog = (Catalog *) malloc(sizeof(Catalog));

    catalog->movies[0] = get_lord_of_the_rings_movie();
    catalog->movies[1] = get_godfather_movie();
    catalog->movies[2] = get_fight_club_movie();

    return catalog;
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