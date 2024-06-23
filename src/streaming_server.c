#include "streaming_server.h"

#include <unistd.h>
#include <pthread.h>

#define MAX_TITLE_SIZE 100
#define MAX_SCRIPT_LINE_SIZE 200
#define BUFF_SIZE 1024

ClientThreadArgs* createClientThreadArgs(StreamingServer *streamingServer, int movieId) {
    ClientThreadArgs *clientThreadArgs = (ClientThreadArgs *) malloc(sizeof(ClientThreadArgs));
    clientThreadArgs->streamingServer = streamingServer;
    clientThreadArgs->movieId = movieId;

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
    if (handleClientRequests(clientThreadArgs->streamingServer, clientThreadArgs->movieId) != 0) {
        logError("Erro ao lidar com as requisições do cliente.");
    }

    free(clientThreadArgs);
    pthread_exit(0);
}

int handleClientRequests(StreamingServer *streamingServer, int movieId) {
    Movie *movie = streamingServer->catalog->movies[movieId - 1];

    if (sendStringToClient(streamingServer->server, (char *) movie->title, MAX_TITLE_SIZE) == -1) {
        return -1;
    }

    int validLine = 1;
    for (int i = 0; i < SCRIPT_LINES; i++) {
        if (sendIntegerToClient(streamingServer->server, validLine) == -1) {
            return -1;
        }

        if (sendStringToClient(streamingServer->server, (char *) movie->script[i], MAX_SCRIPT_LINE_SIZE) == -1) {
            return -1;
        }
        sleep(3);
    }

    validLine = 0;
    if (sendIntegerToClient(streamingServer->server, validLine) == -1) {
        return -1;
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
        int movieId;
        if (receiveIntegerFromClient(streamingServer->server, &movieId) == -1) {
            logError("Erro ao receber a conexão do cliente");
        }

        if (movieIdIsValid(movieId)) {
            printf("Novo cliente conectado.\n");
            ClientThreadArgs *clientThreadData = createClientThreadArgs(streamingServer, movieId);
            pthread_t clientThreadId;
            pthread_create(&clientThreadId, NULL, handleClientThread, clientThreadData);
        }
    }

    closeStreamingServer(streamingServer);
    return 0;
}
