#include "streaming_server.h"

#include <unistd.h>

#define MAX_TITLE_SIZE 100
#define MAX_SCRIPT_LINE_SIZE 200
#define BUFF_SIZE 1024

StreamingServer* createStreamingServer(int argc, char **argv) {
    Server *server = parseArgumentsAndCreateServer(argc, argv);

    StreamingServer *streamingServer = (StreamingServer *) malloc(sizeof(StreamingServer));
    streamingServer->server = server;
    streamingServer->catalog = createCatalog();

    return streamingServer;
}

void communicateWithClient(StreamingServer *streamingServer, int clientSocket) {
    if (provideCatalogToClient(streamingServer, clientSocket) != 0) {
        logError("Erro ao enviar o catálogo para o cliente.");
    }

    if (handleClientRequests(streamingServer, clientSocket) != 0) {
        logError("Erro ao lidar com as requisições do cliente.");
    }
}

int provideCatalogToClient(StreamingServer *streamingServer, int clientSocket) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        int movieId = i + 1;
        if (send(clientSocket, &movieId, sizeof(int), 0) == -1) {
            return -1;
        }

        char movieTitle[MAX_TITLE_SIZE];
        strcpy(movieTitle, streamingServer->catalog->movies[i]->title);
        if (send(clientSocket, &movieTitle, sizeof(movieTitle), 0) == -1) {
            return -1;
        }
    }

    int endOfCatalog = -1;
    if (send(clientSocket, &endOfCatalog, sizeof(int), 0) == -1) {
        return -1;
    }

    return 0;
}

int movieIdIsValid(int movieId) {
    return movieId >= 1 && movieId <= CATALOG_SIZE;
}

int handleClientRequests(StreamingServer *streamingServer, int clientSocket) {
    while (1) {
        int movieId;
        if (recv(clientSocket, &movieId, sizeof(int), 0) == -1) {
            return -1;
        }
        if (!movieIdIsValid(movieId)) {
            break;
        }

        Movie *movie = streamingServer->catalog->movies[movieId - 1];
        for (int i = 0; i < SCRIPT_LINES; i++) {
            if (send(clientSocket, movie->script[i], MAX_SCRIPT_LINE_SIZE, 0) != 0) {
                return -1;
            }
        }
    }

    return 0;
}

int connectToClient(Server *server) {
    struct sockaddr_storage clientStorage;
    socklen_t clientStorageSize = sizeof(clientStorage);

    // Accept a new connection request from a client
    int clientSocket = accept(server->socket, (struct sockaddr *) &clientStorage, &clientStorageSize);

    // Formats the connection address and prints it
    char clientAddress[BUFF_SIZE];
    if (0 != convertAddressToString((struct sockaddr *) &clientStorage, clientAddress, BUFF_SIZE)) {
        return -1;
    }
    printf("Conexão aceita de %s\n", clientAddress);

    return clientSocket;
}

void closeStreamingServer(StreamingServer *streamingServer) {
    closeServer(streamingServer->server);
    freeCatalog(streamingServer->catalog);
    free(streamingServer);
}
