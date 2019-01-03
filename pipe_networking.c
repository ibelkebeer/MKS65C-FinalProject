#include "pipe_networking.h"

/*=========================
  server_handshake
  args: int * to_client
  Performs the client side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe.
  returns the file descriptor for the upstream pipe.
  =========================*/
void server_handshake() {
  int clients = 0;
  int fifo;
  char name[10];
  char message[256];
  char newname[10];
  while(clients < 2){
    unlink(ACK);
    if(mkfifo(ACK, 0666) == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    printf("Server created\n");
    fifo = open(ACK, O_RDONLY);
    if(read(fifo, name, 256) == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    close(fifo);
    fifo = open(name, O_WRONLY);
    if(fifo == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    if(write(fifo, "I gotchu", strlen("I gotchu")) == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    close(fifo);
    fifo = open(ACK, O_RDONLY);
    if(read(fifo, message, 256) == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    close(fifo);
    printf("Handshake Complete\n");

    sprintf(newname, "%d", getpid() + clients);
    mkfifo(newname, 0666);
    fifo = open(name, O_WRONLY);
    if(fifo == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    if(write(fifo, newname, strlen(newname)) == -1){
      printf("ERROR: %s\n", strerror(errno));
      exit(1);
    }
    close(fifo);

  }
}



/*=========================
  client_handshake
  args: int * to_server
  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.
  returns the file descriptor for the downstream pipe.
  =========================*/
void client_handshake() {
  char name[10];
  sprintf(name, "%d", getpid());
  if(mkfifo(name, 0666) == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  int fifo = open(ACK, O_WRONLY);
  if(fifo == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  if(write(fifo, name, strlen(name)) == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  close(fifo);
  fifo = open(name, O_RDONLY);
  if(fifo == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  char message[256];
  if(read(fifo, message, 256) == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  close(fifo);
  fifo = open(ACK, O_WRONLY);
  if(write(fifo, "Ayo", strlen("Ayo")) == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  printf("Handshake Complete\n");
  fifo = open(name, O_RDONLY);
  if(fifo == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  char newname[10];
  if(read(fifo, newname, 10) == -1){
    printf("ERROR: %s\n", strerror(errno));
    exit(1);
  }
  close(fifo);
  int f = fork();
  if(!f){
    char* command[3];
    command[0] = "java";
    command[1] = "Tetris";
    command[2] = NULL;
    execvp(command[0], command);
    printf("%s\n", strerror(errno));
  }else{
    int status;
    wait(&status);
  }
  while(1){
    
  }
}