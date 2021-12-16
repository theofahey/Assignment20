#include "pipe_networking.h"


/*=========================
  server_handshake
  args: int * to_client

  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_handshake(int *to_client) {
  mkfifo("Mario", 0644);
  printf("Created well known pipe \n");

  int from_client = open("Mario", O_RDONLY);
  char text[1000];
  read(from_client,text,1000);
  printf("Read sp name\n");
  remove ("Mario");
  printf("removed Well known pipe\n");
  *to_client = open(text, O_WRONLY);
  printf("opened secretpipe\n");
  char * line = "test";
  write(*to_client,line,sizeof(line));
  printf("wrote message through secret pipe");
  char output[BUFFER_SIZE];
  read(from_client,output,BUFFER_SIZE);
  printf("read final output %s", output);
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
  char secret[BUFFER_SIZE];
  mkfifo(secret,0644);
  printf("Created Secret client\n");
  *to_server = open("Mario", O_WRONLY);
  write(*to_server, "Secret", 7);
  remove("Mario");
  printf("Wrote and Removed from Well known Pipe\n");
  char text[1000];
  int from_server = open(secret, O_RDONLY);
  read(from_server,text,1000);
  remove(secret);
  printf("read and removed from secret");
  int i,hold,length;
  length = strlen(text);
  for (i = 0; i < length/2;i++){
   hold = text[i];
   text[i] = text[length - i - 1];
   text[length - i - 1] = hold;
    }
   write(*to_server, text, sizeof(text));
   printf("returned reversed text\n");

  return from_server;
}
