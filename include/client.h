//---------------------------------------------------------------------
// File	    : client.h
// Content	: Client functions definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>

typedef struct {
    struct sockaddr_storage storage;

    int currentServerSocket;
} Client;

/**
 * @brief Creates a new client with the specified IP version, IP address and port.
 *
 * @param ipVersion The IP version to use (e.g., "IPv4" or "IPv6").
 * @param ipAddress The IP address to connect to.
 * @param port The port number to connect to.
 * @return A pointer to the newly created Client object. The program is terminated if an error occurs.
 */
Client* createClient(char *ipVersion, char *ipAddress, int port);

/**
 * @brief Prints the usage instructions for the client program.
 */
void printClientUsage();

/**
 * @brief Parses the command line arguments and creates a Client object.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 * @return A pointer to the created Client object.
 */
Client* parseArgumentsAndCreateClient(int argc, char **argv);

/**
 * @brief Connects the client to the server.
 *
 * @param client A pointer to the Client object.
 * @return 0 if the connection was successful, -1 otherwise.
 */
int connectToServer(Client *client);

/**
 * @brief Closes the client connection and frees the allocated resources.
 *
 * This function closes the client connection and releases any resources
 * allocated during the client initialization. It should be called when the
 * client is no longer needed to prevent resource leaks.
 *
 * @param client A pointer to the client object.
 */
void closeClient(Client *client);

#endif
