#include "catalog.h"

int main(int argc, char **argv) {
    Catalog *catalog = createCatalog();

    displayCatalog(catalog);

    destroyCatalog(catalog);

    return 0;
}
