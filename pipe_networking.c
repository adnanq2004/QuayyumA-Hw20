#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  printf("server startup...\n");
  int from_client = 0;
  mkfifo(WKP, 0666);
  printf("server: made a well known pipe\n");
  from_client = open(WKP, O_RDONLY);
  char line[100];
  read(from_client, line, 100);
  printf("server: got client's secret pipe name; %s\n", line);
  remove(WKP);
  *to_client = open(line, O_WRONLY);
  strcpy(line, ACK);
  write(*to_client, line, 5);
  printf("server: sent %s to client", line);
  read(from_client, line, 5);
  if (!strcmp(line, "hola")) {
    printf("connection secured");
  }
  return from_client;
}


/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {
  printf("client startup...\n");
  int from_server = 0;
  char line[100] = "";
  // char *trial = getpid();
  // strcat(name, trial);
  sprintf(line, "%d", getpid());
  mkfifo(line, 0666);
  // mkfifo("spipe", 0666);
  // int sp = open("spipe", O_WRONLY);
  printf("client: made a secret pipe\n");
  *to_server = open(line, O_WRONLY);
  write(*to_server, line, 100);
  printf("client: sent name of secret pipe to server via wkp; %s\n", line);
  char line2[5];
  from_server = open(line, O_RDONLY);
  read(from_server, line2, 5);
  remove(line);
  printf("client: received %s from server\n", line2);
  int i;
  for (i = 0; i < strlen(line2); i++) {
    line2[i] -= 32;
  }
  write(*to_server, line2, 5);
  printf("client: sent %s in response to server\n", line2);
  return from_server;
}
