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
    streamingClient->catalogTitles = createCatalog(SHOULD_NOT_INCLUDE_SCRIPT);

    return streamingClient;
}

void displayStreamingMenu(StreamingClient *streamingClient) {
    printf("######################################################################\n");
    printf("#          Serviço de Streaming de Filmes - Menu Principal           #\n");
    printf("######################################################################\n");
    printf("$ 0 - Sair\n");
    displayCatalog(streamingClient->catalogTitles);
}

void handleUserMenuChoice(StreamingClient *streamingClient) {
    int choice;
    printf("Escolha uma opção: ");
    scanf("%d", &choice);

    if (choice == 0) {
        closeStreamingClient(streamingClient);
        exit(0);
    }

    if (movieIdIsValid(choice)) {
        handleMovieRequest(streamingClient, choice);
    } else {
        printf("A opção %d não é valida. Escolha somente um número presente no menu.\n\n\n", choice);
        return;
    }
}

void closeStreamingClient(StreamingClient *streamingClient) {
    int connectionEnd = -1;
    if (sendIntegerToServer(streamingClient->client, connectionEnd) == -1) {
        logError("Erro ao enviar a mensagem de encerramento da conexão para o servidor.");
    }

    closeClient(streamingClient->client);
    free(streamingClient);
}

void handleMovieRequest(StreamingClient *streamingClient, int movieId) {
    if (connectToServer(streamingClient->client) == -1) {
        logError("Erro ao conectar ao servidor.");
    }

    if (sendIntegerToServer(streamingClient->client, movieId) == -1) {
        logError("Erro ao enviar o ID do filme para o servidor.");
    }

    char movieTitle[MAX_TITLE_SIZE];
    if (receiveStringFromServer(streamingClient->client, movieTitle, MAX_TITLE_SIZE) == -1) {
        logError("Erro ao receber o título do filme.");
    }

    printf("Assistindo ao filme: %s\n", movieTitle);

    while (1) {
        int validScriptLine;
        if (receiveIntegerFromServer(streamingClient->client, &validScriptLine) == -1) {
            logError("Erro ao receber a confirmação de linha de script válida.");
        }
        if (!validScriptLine) {
            printf("Fim do filme.\n\n\n\n");
            break;
        }

        char scriptLine[MAX_SCRIPT_LINE_SIZE];
        if (receiveStringFromServer(streamingClient->client, scriptLine, MAX_SCRIPT_LINE_SIZE) == -1) {
            logError("Erro ao receber a linha de script.");
        }
        printf("> %s\n", scriptLine);
    }
}

int main(int argc, char **argv) {
    StreamingClient *streamingClient = createStreamingClient(argc, argv);

    while (1) {
        displayStreamingMenu(streamingClient);
        handleUserMenuChoice(streamingClient);
    }

    return 0;
}
