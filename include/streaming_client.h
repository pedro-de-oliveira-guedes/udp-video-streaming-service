//---------------------------------------------------------------------
// File	    : streaming_client.h
// Content	: Streaming Client definitions
// Author	: Pedro de Oliveira Guedes
//---------------------------------------------------------------------

#ifndef STREAMING_CLIENT_H
#define STREAMING_CLIENT_H

#include "client.h"

typedef struct {
    Client *client;
} StreamingClient;

/**
 * @brief Creates a new StreamingClient instance.
 *
 * @param argc The number of command line arguments.
 * @param argv An array of strings containing the command line arguments.
 * @return A pointer to the newly created StreamingClient instance. The program is terminated if an error occurs.
 */
StreamingClient* createStreamingClient(int argc, char **argv);

/**
 * @brief Displays the streaming menu options along with the catalog.
 *
 * @param streamingClient The StreamingClient instance.
 */
void displayStreamingMenu(StreamingClient *streamingClient);

/**
 * @brief Requests the catalog from the server and displays it.
 *
 * @param streamingClient The StreamingClient instance.
 */
void requestAndDisplayCatalog(StreamingClient *streamingClient);

/**
 * @brief Handles the user's menu choice.
 *
 * @param streamingClient The StreamingClient instance.
 */
void handleUserMenuChoice(StreamingClient *streamingClient);

/**
 * @brief Closes the streaming client connection and frees the allocated resources.
 *
 * This function closes the streaming client connection and releases any resources
 * allocated during the streaming client initialization. It should be called when the
 * user chooses to exit the streaming client to prevent resource leaks.
 *
 * @param streamingClient A pointer to the streaming client object.
 */
void closeStreamingClient(StreamingClient *streamingClient);

/**
 * @brief Handles the movie request by requesting the movie from the server.
 *
 * @param streamingClient The StreamingClient instance.
 * @param movieId The ID of the movie to request.
 */
void handleMovieRequest(StreamingClient *streamingClient, int movieId);

#endif