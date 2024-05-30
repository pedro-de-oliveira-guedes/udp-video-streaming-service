#include "catalog.h"

int main(int argc, char **argv) {
    Catalog *catalog = create_catalog();

    display_catalog(catalog);

    destroy_catalog(catalog);

    return 0;
}
