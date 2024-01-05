#include "pipe.h"

int err(){
  printf("errno %d\n",errno);
  printf("%s\n",strerror(errno));
  exit(1);
}

char *process(char *input){ 
  if(input == NULL){
      return NULL;
  }
  int len = strlen(input);
  char output[len];
  for(int i = 0; i < len; i++){
    char* curr = input;
    if(*curr != '\0'){
      output[i] = '-';
    }
  }
  //strcpy(output, input);
  /*if(output != NULL){
    char* curr = output;
    while(*curr != '\0'){
      curr='-';
    }
    curr++;
  }*/
  return output;
}
 
int server_setup() {
  int from_client = 0;

  printf("Server making the pipe\n");

  printf("Server opening the pipe\n");
  mkfifo(WKP, 0666);
  from_client = open(WKP, O_RDONLY);

  printf("Server removing the pipe\n");

  remove(WKP);

  return from_client;
}

int server_handshake(int *to_client) {
  int from_client = server_setup();

  printf("Server reading SYN (the pid)\n");
  char private_name[50];
  read(from_client, private_name, 50);

  printf("Server opening the Private Pipe\n");
  int pp = open(private_name, O_WRONLY);

  printf("Server sending SYN_ACK\n");
  write(pp, SYN_ACK, 50);

  printf("Server reading final ACK\n");
  char ack[50];
  read(from_client, ack, 50);

  if(strcmp(ack,ACK)==0){
    printf("Server received ACK, handshake complete\n");
  }

  return from_client;
}

int client_handshake(int *to_server) {
  int from_server;

  char* private_name = calloc(50, sizeof(char));
  sprintf(private_name, "%d", getpid());

  printf("Client making Private pipe\n");

  printf("Client opening WKP\n");
  int wkp = open(WKP, O_WRONLY);

  printf("Client Writing PP to WKP\n");
  write(wkp, private_name, 50);

  printf("Client Opening PP\n");
  mkfifo(private_name, 0666);
  from_server = open(private_name, O_RDONLY);

  printf("Client Deleting PP\n");
  remove(private_name);

  printf("Client reading SYN_ACK\n");
  char syn_ack[50];
  read(from_server, syn_ack, 50);

  printf("Client sending ACK\n");
  write(wkp, ACK, 50);
  

//user input to ask for a char
//read shared memory and find where if any, letter is right
//edit process function to cover all except that one letter
//use process function
//print result from process function and add to shared memory




  char code_word[50];
  char modified_word[50];
  read(from_server, code_word, 50);
  strcpy(modified_word,process(code_word));
  printf("%s\n",modified_word);
  //strcpy(modified_word, process(code_word));
  //write(wkp, modified_word, 50);

  close(from_server);
  return from_server;
}

int server_connect(int from_client) {   
  int to_client  = 0;

  printf("Server reading SYN (the pid)\n");
  char private_name[50];
  read(from_client, private_name, 50);

  printf("Server opening the Private Pipe\n");
  to_client = open(private_name, O_WRONLY);

  printf("Server sending SYN_ACK\n");
  write(to_client, SYN_ACK, 50);

  printf("Server reading final ACK\n");
  char ack[50];
  read(from_client, ack, 50);

  printf("Server received ACK, handshake complete\n");
  
  char* code_word = "hello its me";

//add word to shared memory
//create shared memory
int shmid;
shmid = shmget(2727980, sizeof(char*), IPC_CREAT | 0640);  

char *data;
data = shmat(shmid, 0, 0);//attach it to variable data
*data = *code_word;

//write dashes to client
write(to_client, process(code_word), 50);

//server print dashes
printf("Word: %s\n", process(code_word));

  return to_client;
}

