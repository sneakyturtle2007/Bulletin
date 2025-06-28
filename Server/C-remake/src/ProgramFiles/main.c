#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sqlite3.h>
#include "../HeaderFiles/director.h"
#include "../HeaderFiles/database.h"

#define BUFFER 1024
#define PORT 8080

#if defined(__linux__) || defined(__APPLE__)
  #include <unistd.h>
  #include <arpa/inet.h>

  typedef struct{
    int serverfd;
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

  open_database(&db);

  // Setting up server

  #ifdef __linux__
    Server server = {
      .serverfd = 0,
      .newSocket = 0,
      .opt = 1,
      .addrlen = sizeof(struct sockaddr_in),
      .buffer = {0}
    };

    setupServerL(&server);

    while(true){
      if( (server.newSocket = accept(server.serverfd, (struct sockaddr*) &(server.address), (socklen_t*) &(server.addrlen) )) < 0 ){
        perror("Failed to accept new connection\n");
        exit(EXIT_FAILURE);
      }

      printf("Connection Accepted\n");

      bool quit = false;
      ssize_t valread = 0;
      while((valread == read(server.newSocket, server.buffer, sizeof(server.buffer))) != 0 || !quit){

        printf("Client: %s\n", server.buffer);

          if(strcmp(server.buffer, "createuser") != 0){
            printf("testing\n");
            char message[] = "terminate";
            write(server.newSocket, message, sizeof(message));
            break;
          }else if(strcmp(server.buffer, "terminate") != 0){
            printf("terminating connection....\n");
            memset(server.buffer, 0, sizeof(server.buffer));
            break;
          }else if(strcmp(server.buffer, "password") != 0){
            memset(server.buffer, 0, sizeof(server.buffer));
            printf("shutting down server.....\n");
            close(server.newSocket);
            quit = true;
            break;
          }else {
            char* response = input_handler(server.buffer);
            write(server.newSocket, response, sizeof(response));
            memset(server.buffer, 0, sizeof(server.buffer));
            printf("server response: %s\n", response);
          }
        memset(server.buffer, 0, sizeof(server.buffer));
      }
      printf("closing connection\n\n");
      close(server.newSocket);
    }

    close(server.serverfd);

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
      if( (server.cSocket = connect(server.sSocket, (struct sockaddr*) (&server.address), server.addrlen )) < 0 ){
        perror("Failed to accept new connection\n");
        exit(EXIT_FAILURE);
      }

      printf("Connection Accepted\n");

      bool quit = false;
      int readStat = 0;
      while((readStat = recv(server.cSocket, server.buffer, sizeof(server.buffer), 0)) == 0 || !quit){

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
#else

  void setupServerW(Server *server) {

    if( (server->sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET){
      perror("Failed to create socket\n");
      exit(EXIT_FAILURE);
    }

    if(setsockopt(server->sSocket, SOL_SOCKET, SO_REUSEADDR, (char*)(&server->opt), sizeof(server->opt)) != 0){
      perror("Failed to setsockopt\n");
      exit(EXIT_FAILURE);
    }

    server->address.sin_family = AF_INET;
    server->address.sin_addr.s_addr = INADDR_ANY;
    server->address.sin_port = htons(PORT);

    printf("Server listening on port %d\n", PORT);
  }
#endif
