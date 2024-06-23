//---------------------------------------------------------------------
// File	    : streaming_server.h
// Content	: Streaming Server definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef STREAMING_SERVER_H
#define STREAMING_SERVER_H

#include "server.h"
#include "catalog.h"

typedef struct {
    Server *server;

    Catalog *catalog;
} StreamingServer;

/**
 * @brief Creates a new StreamingServer instance.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 * 
 * @return A pointer to the newly created StreamingServer instance. The program is terminated if an error occurs.
 */
StreamingServer* createStreamingServer(int argc, char **argv);

/**
 * @brief Connects to client, sends catalog and handles client requests.
 * 
 * @param streamingServer The StreamingServer instance.
 * @param clientSocket The client socket file descriptor.
 */
void communicateWithClient(StreamingServer *streamingServer, int clientSocket);

/**
 * @brief Sends the catalog to the client right after a connection is established.
 * 
 * @param streamingServer The StreamingServer instance.
 * @param clientSocket The client socket file descriptor.
 * 
 * @return 0 if the catalog was successfully sent, -1 otherwise.
 */
int provideCatalogToClient(StreamingServer *streamingServer, int clientSocket);

/**
 * @brief Handles the client requests for movie streaming.
 * 
 * @param streamingServer The StreamingServer instance.
 * @param clientSocket The client socket file descriptor.
 * 
 * @return 0 if the client requests were successfully handled, -1 otherwise.
 */
int handleClientRequests(StreamingServer *streamingServer, int clientSocket);

/**
 * @brief Awaits a client connection request and accepts it.
 * 
 * @param server The Server instance.
 * 
 * @return The client socket file descriptor or -1 if an error occurred.
 */
int connectToClient(Server *server);

/**
 * @brief Closes the streaming server connection and frees the allocated resources.
 *
 * This function closes the streaming server connection and releases any resources
 * allocated during the streaming server initialization. It should be called when the
 * server is stopped to prevent resource leaks.
 *
 * @param streamingServer A pointer to the streaming server object.
 */
void closeStreamingServer(StreamingServer *streamingServer);

#endif