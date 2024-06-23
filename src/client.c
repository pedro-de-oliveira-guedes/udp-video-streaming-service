#include "client.h"
#include "network_config.h"

#include <unistd.h>

#define BUFF_SIZE 1024

Client* createClient(char *ipVersion, char *ipAddress, int port) {
    Client *client = (Client *) malloc(sizeof(Client));

    // Initializes the client socket address with the specified IP version, IP address and port.
    int socketAddressResult = clientSocketInit(ipVersion, ipAddress, (uint16_t)port, &client->storage);
    if (socketAddressResult == -1) {
        logError("Erro ao inicializar o socket do cliente");
    }

    return client;
}

void printClientUsage() {
    printf("Usage: ./client <ipv4|ipv6> <ip_address> <port>\n");
    printf("Example: ./client ipv4 127.0.0.1 50501\n");
}

Client* parseArgumentsAndCreateClient(int argc, char **argv) {
    if (argc != 4) {
        printClientUsage();
        exit(1);
    }

    return createClient(argv[1], argv[2], atoi(argv[3]));
}

int connectToServer(Client *client) {
    // Creates a socket for UDP communication. The UDP is defined by SOCK_DGRAM.
    client->currentServerSocket = socket(client->storage.ss_family, SOCK_DGRAM, 0);
    if (client->currentServerSocket == -1) {
        logError("Erro ao criar o socket do cliente");
    }

    // Sends a message to the server to establish the connection.
    int startConnection = 0;
    if (sendIntegerToServer(client, startConnection) == -1) {
        logError("Erro ao enviar a mensagem de início de conexão para o servidor");
    }

    return 0;
}

int sendIntegerToServer(Client *client, int value) {
    return sendto(
        client->currentServerSocket,
        &value,
        sizeof(int),
        0,
        (struct sockaddr *)&client->storage,
        sizeof(client->storage));
}

int receiveIntegerFromServer(Client *client, int *value) {
    socklen_t storageSize = sizeof(client->storage);
    return recvfrom(
        client->currentServerSocket,
        value,
        sizeof(int),
        0,
        (struct sockaddr *)&client->storage,
        &storageSize);
}

int receiveStringFromServer(Client *client, char *buffer, int bufferSize) {
    socklen_t storageSize = sizeof(client->storage);
    return recvfrom(
        client->currentServerSocket,
        buffer,
        bufferSize,
        0,
        (struct sockaddr *)&client->storage,
        &storageSize);
}

void closeClient(Client *client) {
    close(client->currentServerSocket);
    free(client);
}
