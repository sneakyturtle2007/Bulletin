//
// Created by luis on 5/28/25
//


#include "director.h"

#define BUFFER 1024
#define PORT 8080

#if defined(__linux__) || defined(__APPLE__)
  #include <unistd.h>
  #include <arpa/inet.h>
  #include <stdbool.h>

  typedef struct{
    int serverSocket;
    int newSocket;
    struct sockaddr_in address;
    int opt;
    int addrlen;
    char buffer[BUFFER];
  }Server;

  void setupServerL(Server *server);

#elif defined(_WIN32) || defined(_WIN64)
  #include <winsock2.h>
  #include <ws2tcpip.h>
  #include <windows.h>
  #include <stdbool.h>

  #pragma comment(lib, "Ws2_32.lib")

  typedef struct {
    SOCKET sSocket;
    SOCKET cSocket;
    int opt;
    int addrlen;
    struct sockaddr_in address;
    char buffer[BUFFER];
  }Server;
  
  void setupServerW(Server *server);

#endif


int main(){
  //Setting up database
    sqlite3 *db;
    Error status = open_database(&db);
    if(status.code != OK){
      fprintf(stderr, "%s\n", status.message);
      return 1;
    }

  // Setting up server

  #ifdef __linux__
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
            .data = malloc(256 * sizeof(char)),
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

  #else

    WSADATA wsaData;
    const int status = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (status != 0) {
      printf("Failed to initialize Winsock\n");
      WSACleanup();
      exit(EXIT_FAILURE);

    }

    Server server = {
      .opt = 1,
      .addrlen = sizeof(struct sockaddr_in),
      .buffer = {0}
    };

    setupServerW(&server);

    while(true){
      if( (server.cSocket = connect(server.sSocket, 
          (struct sockaddr*) (&server.address), server.addrlen )) < 0 ){
        perror("Failed to accept new connection\n");
        exit(EXIT_FAILURE);
      }

      printf("Connection Accepted\n");

      bool quit = false;
      int readStat = 0;
      while( (readStat = recv(server.cSocket, server.buffer, 
              sizeof(server.buffer), 0)) == 0 || !quit){
        printf("Client: %s\n", server.buffer);

        if(strcmp(server.buffer, "test") != 0){
          printf("testing\n");
          char message[] = "terminate";
          send(server.cSocket, message, sizeof(message), 0);
          break;
        }else if(strcmp(server.buffer, "terminate") != 0){
          printf("terminating connection....\n");
          memset(server.buffer, 0, sizeof(server.buffer));
          break;
        }else if(strcmp(server.buffer, "password") != 0){
          printf("shutting down server.....\n");
          shutdown(server.cSocket, SD_BOTH);
          quit = true;
        }
        memset(server.buffer, 0, sizeof(server.buffer));
      }
      printf("closing connection\n\n");
      shutdown(server.cSocket, SD_BOTH);
    }

    shutdown(server.sSocket, SD_BOTH);
  #endif

  return 0;
}


#if defined(__linux__) || defined(__APPLE__)
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
#else

  void setupServerW(Server *server) {

    if( (server->sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
        == INVALID_SOCKET){
      perror("Failed to create socket\n");
      exit(EXIT_FAILURE);
    }

    if(setsockopt(server->sSocket, SOL_SOCKET, SO_REUSEADDR, (char*)(&server->opt), 
          sizeof(server->opt)) != 0){
      perror("Failed to setsockopt\n");
      exit(EXIT_FAILURE);
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);

    printf("Server listening on port %d\n", PORT);
  }
#endif

