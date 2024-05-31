#include "client.h"
#include "network_config.h"

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
    // Creates a socket for TCP communication. The TCP is defined by SOCK_STREAM.
    client->currentServerSocket = socket(client->storage.ss_family, SOCK_STREAM, 0);
    if (client->currentServerSocket == -1) {
        logError("Erro ao criar o socket do cliente");
    }

    // Connects the client socket to the server socket.
    if (0 != connect(client->currentServerSocket, (struct sockaddr *)&client->storage, sizeof(client->storage))) {
        return -1;
    }

    // Formats the connection address to string and prints it.
    char connectedAddress[BUFF_SIZE];
    if (0 != convertAddressToString((struct sockaddr *)&client->storage, connectedAddress, BUFF_SIZE)) {
        return -1;
    }
    printf("Conectado ao servidor %s\n", connectedAddress);

    return 0;
}

void closeClient(Client *client) {
    close(client->currentServerSocket);
    free(client);
}
