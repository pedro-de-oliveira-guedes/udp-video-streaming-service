#include "streaming_server.h"

#include <unistd.h>
#include <pthread.h>

#define MAX_TITLE_SIZE 100
#define MAX_SCRIPT_LINE_SIZE 200
#define BUFF_SIZE 1024

int currentlyConnectedClients = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

ClientThreadArgs* createClientThreadArgs(Server server, int movieId, Catalog catalog) {
    ClientThreadArgs *clientThreadArgs = (ClientThreadArgs *) malloc(sizeof(ClientThreadArgs));
    clientThreadArgs->server = server;
    clientThreadArgs->movieId = movieId;
    clientThreadArgs->catalog = catalog;

    return clientThreadArgs;
}

StreamingServer* createStreamingServer(int argc, char **argv) {
    Server *server = parseArgumentsAndCreateServer(argc, argv);

    StreamingServer *streamingServer = (StreamingServer *) malloc(sizeof(StreamingServer));
    streamingServer->server = server;
    streamingServer->catalog = createCatalog(SHOULD_INCLUDE_SCRIPT);

    return streamingServer;
}

void *handleClientThread(void *data) {
    ClientThreadArgs *clientThreadArgs = (ClientThreadArgs *) data;
    if (handleClientRequests(clientThreadArgs) != 0) {
        logError("Erro ao lidar com as requisições do cliente.");
    }

    pthread_mutex_lock(&mutex);
    currentlyConnectedClients -= 1;
    pthread_mutex_unlock(&mutex);

    free(clientThreadArgs);
    pthread_exit(0);
}

int handleClientRequests(ClientThreadArgs *clientThreadArgs) {
    Movie *movie = clientThreadArgs->catalog.movies[clientThreadArgs->movieId - 1];

    if (sendStringToClient(&clientThreadArgs->server, (char *) movie->title, MAX_TITLE_SIZE) == -1) {
        return -1;
    }

    int validLine = 1;
    for (int i = 0; i < SCRIPT_LINES; i++) {
        if (sendIntegerToClient(&clientThreadArgs->server, validLine) == -1) {
            return -1;
        }

        if (sendStringToClient(&clientThreadArgs->server, (char *) movie->script[i], MAX_SCRIPT_LINE_SIZE) == -1) {
            return -1;
        }
        sleep(3);
    }

    validLine = 0;
    if (sendIntegerToClient(&clientThreadArgs->server, validLine) == -1) {
        return -1;
    }

    return 0;
}

void closeStreamingServer(StreamingServer *streamingServer) {
    closeServer(streamingServer->server);
    destroyCatalog(streamingServer->catalog);
    free(streamingServer);
}

void *clientCountSubroutine(void *data) {
    while (1) {
        printf("Número de clientes conectados: %d\n", currentlyConnectedClients);
        sleep(4);
    }
}

int main(int argc, char **argv) {
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        logError("Erro ao inicializar o mutex");
    }

    StreamingServer *streamingServer = createStreamingServer(argc, argv);

    if (setupServer(streamingServer->server) == -1) {
        logError("Erro ao configurar o servidor");
    }

    pthread_t clientCountThreadId;
    pthread_create(&clientCountThreadId, NULL, clientCountSubroutine, NULL);

    while (1) {
        ClientThreadArgs *clientThreadData = malloc(sizeof(ClientThreadArgs));

        int movieId;
        if (receiveIntegerFromClient(streamingServer->server, &movieId) == -1) {
            logError("Erro ao receber a conexão do cliente");
        }

        if (movieIdIsValid(movieId)) {
            clientThreadData->server = *streamingServer->server;
            clientThreadData->movieId = movieId;
            clientThreadData->catalog = *streamingServer->catalog;

            pthread_mutex_lock(&mutex);
            currentlyConnectedClients += 1;
            pthread_mutex_unlock(&mutex);
            
            pthread_t clientThreadId;
            pthread_create(&clientThreadId, NULL, handleClientThread, clientThreadData);
        }
    }

    pthread_mutex_destroy(&mutex);
    closeStreamingServer(streamingServer);
    return 0;
}
