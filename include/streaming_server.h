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

typedef struct {
    Server server;
    int movieId;
    Catalog catalog;
} ClientThreadArgs;

/**
 * @brief Creates a new ClientThreadArgs instance.
 * 
 * @param serverSocket The server socket.
 * @param movieId The movie ID to be streamed.
 * 
 * @return A pointer to the newly created ClientThreadArgs instance.
 */
ClientThreadArgs* createClientThreadArgs(Server server, int movieId, Catalog catalog);

/**
 * @brief Handles the client requests in a separate thread.
 * 
 * @param data A pointer to a ClientThreadArgs instance.
 * 
 * @return NULL.
 */
void *handleClientThread(void *data);

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
 * @brief Handles the client requests for movie streaming.
 * 
 * @param serverSocket The server socket.
 * @param movieId The movie ID to be streamed.
 * 
 * @return 0 if the client requests were successfully handled, -1 otherwise.
 */
int handleClientRequests(ClientThreadArgs *clientThreadArgs);

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