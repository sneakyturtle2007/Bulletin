//
// Created by luis on 5/28/25
//

#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "director.h"
#include "error_handling.h"
#include "string_utils.h"

#define BUFFER 1024
#define PORT 8080

typedef struct{
  int serverSocket;
  int newSocket;
  struct sockaddr_in address;
  int opt;
  int addrlen;
  char buffer[BUFFER];
}Server;

void setupServerL(Server *server);

int main(){
  //Setting up database

    sqlite3 *db;
    Error status = open_database(&db);
    if(status.code != OK){
      fprintf(stderr, "%s\n", status.message);
      return 1;
    }
  // Setting up server

  Server server = {
    .serverSocket = 0,
    .newSocket = 0,
    .opt = 1,
    .addrlen = sizeof(struct sockaddr_in),
    .buffer = {0}
  };

  setupServerL(&server);

  while(true){
    if( (server.newSocket = accept(server.serverSocket,
        (struct sockaddr*) &(server.address),
        (socklen_t*) &(server.addrlen))) < 0 ){
      perror("Failed to accept new connection\n");
      exit(EXIT_FAILURE);
    }

    printf("Connection Accepted\n");

    bool quit = false;
    ssize_t valread = 0;
    while( (valread == read(server.newSocket, server.buffer, 
            sizeof(server.buffer))) != 0 || !quit){
      
      printf("Client: %s\n", server.buffer);
      if(strcmp(server.buffer, "terminate") == 0){
        printf("terminating connection....\n");
        memset(server.buffer, 0, sizeof(server.buffer));
        close(server.newSocket);
        break;
      }else if(strcmp(server.buffer, "password") == 0){
        memset(server.buffer, 0, sizeof(server.buffer));
        printf("shutting down server.....\n");
        quit = true;
        break;
      }else{
        String response = {
          .data = calloc(256, sizeof(char)),
          .length = 0,
          .capacity = 256 * sizeof(char)
        };
        response.data[response.length] = '\0'; // zero initializing the string
        status = input_handler(&db, server.buffer, &response);

        if(status.code != OK){
          fprintf(stderr, 
            "%s\n", status.message);
          free(response.data);
          memset(server.buffer, 0, sizeof(server.buffer));
          printf("server response: %s\n", status.message);
          write(server.newSocket, status.message, strlen(status.message));
        }else{
          printf("server response: %s\n", response.data);
          write(server.newSocket, response.data, response.length);
          free(response.data);
        }
      }
      memset(server.buffer, 0, sizeof(server.buffer));

    }
    server.newSocket = 0;
    printf("closing connection\n\n");
  }
  close(server.newSocket);
  close(server.serverSocket);

  return 0;
}

void setupServerL(Server *server){

  if( (server->serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("Failed to create socket\n");
    exit(EXIT_FAILURE);
  }

  if(setsockopt(server->serverSocket, SOL_SOCKET, SO_REUSEADDR, &(server->opt), 
        sizeof(server->opt))){
    perror("Failed to setsockopt\n");
    exit(EXIT_FAILURE);
  }

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(PORT);

  if(bind(server->serverSocket, (struct sockaddr*) &(server->address), 
        sizeof(server->address)) < 0){
    perror("Binding failed\n");
    exit(EXIT_FAILURE);
  }

  if(listen(server->serverSocket, 5) < 0){
    perror("Failed to listen\n");
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d\n", PORT);
}
