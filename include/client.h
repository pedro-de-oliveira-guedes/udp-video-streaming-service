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
 * @brief Creates an UDP socket to communicate with the server.
 * 
 * @param client A pointer to the Client object.
 * @return 0 if the connection was successful, -1 otherwise.
 
 */
int connectToServer(Client *client);

/**
 * @brief Sends an integer value to the server.
 *
 * @param client A pointer to the Client object.
 * @param value The integer value to send.
 * @return 0 if the value was sent successfully, -1 otherwise.
 */
int sendIntegerToServer(Client *client, int value);

/**
 * @brief Receives an integer value from the server.
 *
 * @param client A pointer to the Client object.
 * @param value A pointer to the integer variable that will receive the value.
 * @return 0 if the value was received successfully, -1 otherwise.
 */
int receiveIntegerFromServer(Client *client, int *value);

/**
 * @brief Receives a string from the server.
 * 
 * @param client A pointer to the Client object.
 * @param buffer A pointer to the buffer that will receive the string.
 * @param bufferSize The size of the buffer.
 * @return The number of bytes received, or -1 if an error occurred.
 */
int receiveStringFromServer(Client *client, char *buffer, int bufferSize);

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
