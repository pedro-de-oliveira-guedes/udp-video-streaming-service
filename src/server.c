#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFF_SIZE 1024

Server* createServer(char *ipVersion, int port) {
    Server *server = (Server *) malloc(sizeof(Server));

    // Initializes the server socket address with the specified IP version and port.
    int socketAddressResult = serverSocketInit(ipVersion, (uint16_t)port, &server->storage);
    if (socketAddressResult == -1) {
        logError("Erro ao inicializar o socket do servidor");
    }

    // Creates a socket for UDP communication. The UDP is defined by SOCK_DGRAM.
    server->socket = socket(server->storage.ss_family, SOCK_DGRAM, 0);
    if (server->socket == -1) {
        logError("Erro ao criar o socket do servidor");
    }

    // Allows the server to reuse the port number immediately after it is terminated.
    if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1) {
        logError("Erro ao configurar a reutilização do endereço e porta do servidor");
    }

    return server;
}

void printServerUsage() {
    printf("Usage: ./server <ipv4|ipv6> <port>\n");
    printf("Example: ./server ipv4 50501\n");
}

Server* parseArgumentsAndCreateServer(int argc, char **argv) {
    if (argc != 3) {
        printServerUsage();
        exit(1);
    }

    return createServer(argv[1], atoi(argv[2]));
}

int setupServer(Server *server) {
    // Binds the server socket to the specified address.
    if (bind(server->socket, (struct sockaddr *)&server->storage, sizeof(server->storage)) == -1) {
        return -1;
    }

    // Formats the connection address to string and prints it.
    char serverAddress[BUFF_SIZE];
    if (0 != convertAddressToString((struct sockaddr *)&server->storage, serverAddress, BUFF_SIZE)) {
        return -1;
    }
    printf("Servidor escutando em %s\n", serverAddress);

    return 0;
}

int receiveIntegerFromClient(Server *server, int *value) {
    socklen_t storageSize = sizeof(server->storage);
    return recvfrom(
            server->socket,
            value,
            sizeof(int),
            0,
            (struct sockaddr *) &server->storage,
            &storageSize);
}

int sendIntegerToClient(Server *server, int value) {
    return sendto(
            server->socket,
            &value,
            sizeof(int),
            0,
            (struct sockaddr *) &server->storage,
            sizeof(server->storage));
}

int sendStringToClient(Server *server, char *buffer, size_t bufferSize) {
    return sendto(
            server->socket,
            buffer,
            bufferSize,
            0,
            (struct sockaddr *) &server->storage,
            sizeof(server->storage));
}

void closeServer(Server *server) {
    close(server->socket);
    free(server);
}
