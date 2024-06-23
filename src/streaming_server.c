#include "streaming_server.h"

#include <unistd.h>
#include <pthread.h>

#define MAX_TITLE_SIZE 100
#define MAX_SCRIPT_LINE_SIZE 200
#define BUFF_SIZE 1024

ClientThreadArgs* createClientThreadArgs(StreamingServer *streamingServer) {
    ClientThreadArgs *clientThreadArgs = (ClientThreadArgs *) malloc(sizeof(ClientThreadArgs));
    clientThreadArgs->streamingServer = streamingServer;

    return clientThreadArgs;
}

StreamingServer* createStreamingServer(int argc, char **argv) {
    Server *server = parseArgumentsAndCreateServer(argc, argv);

    StreamingServer *streamingServer = (StreamingServer *) malloc(sizeof(StreamingServer));
    streamingServer->server = server;
    streamingServer->catalog = createCatalog();

    return streamingServer;
}

void *handleClientThread(void *data) {
    ClientThreadArgs *clientThreadArgs = (ClientThreadArgs *) data;
    communicateWithClient(clientThreadArgs->streamingServer);

    free(clientThreadArgs);
    pthread_exit(0);
}

void communicateWithClient(StreamingServer *streamingServer) {
    if (provideCatalogToClient(streamingServer) != 0) {
        logError("Erro ao enviar o catálogo para o cliente.");
    }

    if (handleClientRequests(streamingServer) != 0) {
        logError("Erro ao lidar com as requisições do cliente.");
    }
}

int provideCatalogToClient(StreamingServer *streamingServer) {
    for (int i = 0; i < CATALOG_SIZE; i++) {
        int movieId = i + 1;
        if (sendIntegerToClient(streamingServer->server, movieId) == -1) {
            return -1;
        }

        char movieTitle[MAX_TITLE_SIZE];
        strcpy(movieTitle, streamingServer->catalog->movies[i]->title);
        if (sendStringToClient(streamingServer->server, *movieTitle, MAX_TITLE_SIZE) == -1) {
            return -1;
        }
    }

    int endOfCatalog = -1;
    if (sendIntegerToClient(streamingServer->server, endOfCatalog) == -1) {
        return -1;
    }

    return 0;
}

int handleClientRequests(StreamingServer *streamingServer) {
    while (1) {
        int movieId;
        if (receiveIntegerFromClient(streamingServer->server, &movieId) == -1) {
            return -1;
        }
        if (!movieIdIsValid(movieId)) {
            break;
        }

        Movie *movie = streamingServer->catalog->movies[movieId - 1];
        for (int i = 0; i < SCRIPT_LINES; i++) {
            if (sendStringToClient(streamingServer->server, *movie->script[i], MAX_SCRIPT_LINE_SIZE) == -1) {
                return -1;
            }
        }
    }

    return 0;
}

void closeStreamingServer(StreamingServer *streamingServer) {
    closeServer(streamingServer->server);
    destroyCatalog(streamingServer->catalog);
    free(streamingServer);
}


int main(int argc, char **argv) {
    StreamingServer *streamingServer = createStreamingServer(argc, argv);

    if (setupServer(streamingServer->server) == -1) {
        logError("Erro ao configurar o servidor");
    }

    while (1) {
        int newClientConnection;
        if (receiveIntegerFromClient(streamingServer->server, &newClientConnection) == -1) {
            logError("Erro ao receber a conexão do cliente");
        }

        if (newClientConnection == 0) {
            ClientThreadArgs *clientThreadData = createClientThreadArgs(streamingServer);
            pthread_t clientThreadId;
            pthread_create(&clientThreadId, NULL, handleClientThread, clientThreadData);
        }
    }

    closeStreamingServer(streamingServer);
    return 0;
}
