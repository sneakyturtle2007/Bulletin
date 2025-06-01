#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <sqlite3.h>

#define PORT 8080
#define BUFFER 1024

typedef struct{
  int serverfd;
  int newSocket;
  struct sockaddr_in address;
  int opt;
  int addrlen;
  char buffer[BUFFER];
}Server;

void setupServer(Server *server);

int main(){
  //Setting up database
  /*sqlite3 *db;
  int db_status =  sqlite3_open("Database.db", &db);
  
  if(db_status){
    printf("Error opening or creating database\n");
    exit(EXIT_FAILURE);
  }else{
    printf("Great Success!\n");
    sqlite3_close(db);
  }*/
  // Setting up server
  Server server = {
    .serverfd = 0,
    .newSocket = 0,
    .opt = 1,
    .addrlen = sizeof(struct sockaddr_in),
    .buffer = {0}
  };

  setupServer(&server);
    

 
  while(true){
    if( (server.newSocket = accept(server.serverfd, (struct sockaddr*) &(server.address), (socklen_t*) &(server.addrlen) )) < 0 ){
      perror("Failed to accept new connection\n");
      exit(EXIT_FAILURE);
    }

    printf("Connection Accepted\n");
    bool quit = false;
    ssize_t valread = 0;
    while((valread == read(server.newSocket, server.buffer, sizeof(server.buffer))) > 0 || !quit){
      printf("Client: %s\n", server.buffer);
      switch (server.buffer) {
        case "test":
          printf("testing\n");
          char message[] = "terminate";
          write(server.newSocket, message, sizeof(message));
          break;
        case "terminate":
          printf("terminating connection....\n");
          memset(server.buffer, 0, sizeof(server.buffer));
          break;
        case "password":
          printf("shutting down server.....\n");
          close(server.newSocket);
          quit = true;
        default:
          break;
      }
      memset(server.buffer, 0, sizeof(server.buffer));
    }
    printf("closing connection\n\n");
    close(server.newSocket);
  }

  close(server.serverfd);

  return 0;
}

void setupServer(Server *server){
  server->opt = 1;
  server->addrlen = sizeof(server->address);
  
  if( (server->serverfd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    perror("Failed to create socket\n");
    exit(EXIT_FAILURE);
  }

  if(setsockopt(server->serverfd, SOL_SOCKET, SO_REUSEADDR, &(server->opt), sizeof(server->opt))){
    perror("Failed to setsockopt\n");
    exit(EXIT_FAILURE);
  }

  server->address.sin_family = AF_INET;
  server->address.sin_addr.s_addr = INADDR_ANY;
  server->address.sin_port = htons(PORT);
  
  if(bind(server->serverfd, (struct sockaddr*) &(server->address), sizeof(server->address)) < 0){
    perror("Binding failed\n");
    exit(EXIT_FAILURE);
  }
  
  if(listen(server->serverfd, 5) < 0){
    perror("Failed to listen\n");
    exit(EXIT_FAILURE);
  }
  
  printf("Server listening on port %d\n", PORT);
}
