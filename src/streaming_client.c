#include "streaming_client.h"
#include "network_config.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_TITLE_SIZE 100
#define MAX_SCRIPT_LINE_SIZE 200

StreamingClient* createStreamingClient(int argc, char **argv) {
    Client *client = parseArgumentsAndCreateClient(argc, argv);

    StreamingClient *streamingClient = (StreamingClient *) malloc(sizeof(StreamingClient));
    streamingClient->client = client;
    streamingClient->cachedCatalog = NULL;

    return streamingClient;
}

void displayStreamingMenu(StreamingClient *streamingClient) {
    printf("######################################################################\n");
    printf("#          Serviço de Streaming de Filmes - Menu Principal           #\n");
    printf("######################################################################\n");
    printf("$ 0 - Sair\n");
    if (streamingClient->cachedCatalog != NULL) {
        displayCatalog(streamingClient->cachedCatalog);
    } else {
        requestAndDisplayCatalog(streamingClient);
    }
}

void requestAndDisplayCatalog(StreamingClient *streamingClient) {
    if (connectToServer(streamingClient->client) != 0) {
        logError("Erro ao conectar ao servidor.");
    }

    Catalog *currentCatalog = (Catalog *) malloc(sizeof(Catalog));
    while (1) {
        int movieId;
        if (recv(streamingClient->client->currentServerSocket, &movieId, sizeof(int), 0) == -1) {
            logError("Erro ao receber o ID do filme para exibição do catálogo.");
        }
        if (movieId == -1) {
            break;
        }

        char movieTitle[MAX_TITLE_SIZE];
        if (recv(streamingClient->client->currentServerSocket, &movieTitle, sizeof(movieTitle), 0) == -1) {
            logError("Erro ao receber o título do filme para exibição do catálogo.");
        }
        printf("$ %d - %s\n", movieId, movieTitle);

        Movie *newMovie = (Movie *) malloc(sizeof(Movie));
        newMovie->title = movieTitle;
        currentCatalog->movies[movieId - 1] = newMovie;
    }

    streamingClient->cachedCatalog = currentCatalog;
}

void handleUserMenuChoice(StreamingClient *streamingClient) {
    int choice;
    printf("Escolha uma opção: ");
    scanf("%d", &choice);

    if (choice == 0) {
        closeStreamingClient(streamingClient);
        exit(0);
    }

    handleMovieRequest(streamingClient, choice);
}

void closeStreamingClient(StreamingClient *streamingClient) {
    printf("Saindo do apliacativo de streaming...\n");

    int connectionEnd = -1;
    if (send(streamingClient->client->currentServerSocket, &connectionEnd, sizeof(int), 0) == -1) {
        logError("Erro ao enviar a mensagem de encerramento da conexão para o servidor.");
    }

    closeClient(streamingClient->client);
    free(streamingClient);

    exit(0);
}

void handleMovieRequest(StreamingClient *streamingClient, int movieId) {
    if (send(streamingClient->client->currentServerSocket, &movieId, sizeof(int), 0) == -1) {
        logError("Erro ao enviar o ID do filme para o servidor.");
    }

    char movieTitle[MAX_TITLE_SIZE];
    if (recv(streamingClient->client->currentServerSocket, &movieTitle, MAX_TITLE_SIZE, 0) == -1) {
        logError("Erro ao receber o título do filme.");
    }

    printf("Assistindo ao filme: %s\n", movieTitle);

    while (1) {
        int validScriptLine;
        if (recv(streamingClient->client->currentServerSocket, &validScriptLine, sizeof(int), 0) == -1) {
            logError("Erro ao receber a confirmação de linha de script válida.");
        }
        if (validScriptLine == -1) {
            break;
        }

        char scriptLine[MAX_SCRIPT_LINE_SIZE];
        if (recv(streamingClient->client->currentServerSocket, &scriptLine, MAX_SCRIPT_LINE_SIZE, 0) == -1) {
            logError("Erro ao receber a linha de script.");
        }
        printf("> %s\n", scriptLine);
    }
}
