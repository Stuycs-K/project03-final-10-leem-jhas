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

//takes in client guess, the codeword, and the current state, and returns the new state
char *check_guess(char guess, char *code_word, char* current){
  int len = strlen(code_word);
  char output[len];
  for(int i = 0; i <len;i++){
    if(code_word[i] == guess){
      output[i] = guess;
    }
    else{
      output[i] = current[i];
    }

  }
  return output;
}
 
int server_setup() {
  int from_client = 0;

  // printf("Server making the pipe\n");

  // printf("Server opening the pipe\n");
  mkfifo(WKP, 0666);
  from_client = open(WKP, O_RDONLY);

  // printf("Server removing the pipe\n");

  remove(WKP);

  return from_client;
}

int server_handshake(int *to_client) {
  int from_client = server_setup();

  // printf("Server reading SYN (the pid)\n");
  char private_name[50];
  read(from_client, private_name, 50);

  // printf("Server opening the Private Pipe\n");
  int pp = open(private_name, O_WRONLY);

  // printf("Server sending SYN_ACK\n");
  write(pp, SYN_ACK, 50);

  // printf("Server reading final ACK\n");
  char ack[50];
  read(from_client, ack, 50);

  if(strcmp(ack,ACK)==0){
    // printf("Server received ACK, handshake complete\n");
  }

  return from_client;
}

int client_handshake(int *to_server) {
  int from_server;

  char* private_name = calloc(50, sizeof(char));
  sprintf(private_name, "%d", getpid());

  // printf("Client making Private pipe\n");

  // printf("Client opening WKP\n");
  int wkp = open(WKP, O_WRONLY);

  // printf("Client Writing PP to WKP\n");
  write(wkp, private_name, 50);

  // printf("Client Opening PP\n");
  mkfifo(private_name, 0666);
  from_server = open(private_name, O_RDONLY);

  // printf("Client Deleting PP\n");
  remove(private_name);

  // printf("Client reading SYN_ACK\n");
  char syn_ack[50];
  read(from_server, syn_ack, 50);

  // printf("Client sending ACK\n");
  write(wkp, ACK, 50);
  
  //get code word from client
  char code_word[50];
  read(from_server, code_word, 50);


  //read last line and get current
  int r_file = open("hangman.txt", O_RDONLY , 0);   
      if(r_file == -1) err();


  char buff[256+1];
  buff[256]=0;

  int bytes;

  while((bytes = read(r_file, buff, 128))){
      
      if(bytes == -1)err();//all non 0 are true
      // printf("read\n");
  }  
  

  //shared memory
  int *data;
  int shmid;
  shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
  data = shmat(shmid, 0, 0);
  *data = * data + 1;
  printf("Round %d\n", *data);
  shmdt(data); //detach

  printf("Current:%s \n", buff);

  //ask client to guess a character
  printf("Guess a character:"); 
  char line_buff[256];
  fgets(line_buff,255,stdin);
  char guessed = line_buff[0];
  
  //process guess and get new state
  char after_guess[50];
  strcpy(after_guess, check_guess(guessed, code_word, buff));
  printf("After guessing: %s\n", check_guess(guessed, code_word, buff));

  //write new state 
  int w_file;

  w_file = open("hangman.txt", 
      O_TRUNC|O_WRONLY , 0611);
  if(w_file==-1)err();

  write(w_file, check_guess(guessed, code_word, buff), strlen(check_guess(guessed, code_word, buff)));//writing to file 
  // printf("wrote %s file after guess\n", check_guess(guessed, code_word, buff));
  close(from_server);
  return from_server;
}

int server_connect(int from_client) {   
  int to_client  = 0;

  // printf("Server reading SYN (the pid)\n");
  char private_name[50];
  read(from_client, private_name, 50);

  // printf("Server opening the Private Pipe\n");
  to_client = open(private_name, O_WRONLY);

  // printf("Server sending SYN_ACK\n");
  write(to_client, SYN_ACK, 50);

  // printf("Server reading final ACK\n");
  char ack[50];
  read(from_client, ack, 50);

  // printf("Server received ACK, handshake complete\n");
  
  char code_word[50] = "hello its me";
  

  //reads from file
  int r_file = open("hangman.txt", O_RDONLY , 0);   
      if(r_file == -1) err();


  char buff[256+1];
  buff[256]=0;

  int bytes;
  // char* current;

  while((bytes = read(r_file, buff, 128))){
      
      if(bytes == -1)err();//all non 0 are true
      // printf("read\n");
  }  

  //shared memory
  int *data;
  int shmid;
  shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
  data = shmat(shmid, 0, 0);
  printf("Result from round %d\n", *data);
  shmdt(data); //detach

  printf("Current:%s \n", buff);


  //writes back to file 
  int w_file;

  w_file = open("hangman.txt", 
      O_WRONLY | O_TRUNC, 0611);
  if(w_file==-1)err();
  write(w_file,buff, strlen(buff));

  //writes code word to client
  write(to_client, code_word, strlen(code_word));


  return to_client;
}

