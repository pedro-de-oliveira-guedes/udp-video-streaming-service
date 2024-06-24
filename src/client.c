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
    // Creates a server socket for UDP communication. The UDP is defined by "SOCK_DGRAM" option.
    client->currentServerSocket = socket(client->storage.ss_family, SOCK_DGRAM, 0);
    if (client->currentServerSocket == -1) {
        logError("Erro ao criar o socket do cliente");
    }

    return 0;
}

int sendIntegerToServer(Client *client, int value) {
    // Uses the sendto function to send the integer value to the server.
    return sendto(
        client->currentServerSocket,
        &value,
        sizeof(int),
        0,
        (struct sockaddr *)&client->storage,
        sizeof(client->storage));
}

int receiveIntegerFromServer(Client *client, int *value) {
    // Declares a variable to store the size of the storage structure.
    socklen_t storageSize = sizeof(client->storage);

    // Uses the recvfrom function to receive the integer value from the server.
    return recvfrom(
        client->currentServerSocket,
        value,
        sizeof(int),
        0,
        (struct sockaddr *)&client->storage,
        &storageSize);
}

int receiveStringFromServer(Client *client, char *buffer, int bufferSize) {
    // Declares a variable to store the size of the storage structure.
    socklen_t storageSize = sizeof(client->storage);

    // Uses the recvfrom function to receive the string from the server.
    return recvfrom(
        client->currentServerSocket,
        buffer,
        bufferSize,
        0,
        (struct sockaddr *)&client->storage,
        &storageSize);
}

void closeClient(Client *client) {
    close(client->currentServerSocket); // Closes the server socket
    free(client); // Frees the allocated memory for the client
}
