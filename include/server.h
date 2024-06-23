//---------------------------------------------------------------------
// File	    : server.h
// Content	: Server functions definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef SERVER_H
#define SERVER_H

#include "network_config.h"

typedef struct {
    struct sockaddr_storage storage;

    int socket;
} Server;

/**
 * @brief Creates a new Server instance.
 *
 * @param ipVersion The IP version to use for the server (e.g., "IPv4" or "IPv6").
 * @param port The port number to bind the server to.
 * @return A pointer to the newly created Server instance. The program is terminated if an error occurs.
 */
Server* createServer(char *ipVersion, int port);

/**
 * @brief Prints the usage information for the server.
 */
void printServerUsage();

/**
 * Parses the command line arguments and creates a Server object.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 * @return A pointer to the created Server object.
 */
Server* parseArgumentsAndCreateServer(int argc, char **argv);

/**
 * @brief Sets up the server by binding it to a port, listening for incoming connections 
 *        and showing the connection address.
 *
 * @param server The Server instance.
 * @return 0 if the server was successfully set up, -1 otherwise.
 */
int setupServer(Server *server);

/**
 * @brief Receives a client connection request, accepts it, prints the client address 
 *        and returns the client socket file descriptor.
 *
 * @param server The Server instance.
 * @return The client socket file descriptor or -1 if an error occurred.
 */
int connectToClient(Server *server);

/**
 * @brief Closes the server connection and frees the allocated resources.
 * 
 * @param server A pointer to the Server object.
 */
void closeServer(Server *server);

#endif
