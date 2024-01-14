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

  //char* output = calloc(sizeof(input),sizeof(char));
  char *output = malloc(sizeof(input)+1);
  //int len = strlen(input);
  strcpy(output, input);
  printf("input: %s, output: %s, size: \n",input, output);
  if(output != NULL){
    char* curr = output;
    //int i = 0;
    while(*curr != '\0'){
      *curr = '-'; 
      curr++;
      //i++;
    }
    //*curr = '\0';
  }
  return output;
  /*
  //printf("hi\n");
  int len = strlen(input);
  //printf("hi\n");
  char output[50];
  //printf("hi\n");
  strcpy(output, input);
  printf("input: %s, output: %s, size: %d\n",input, output, len);

  //char* curr = output;
  int i = 0;
  while(i < len){
    output[i] = '-';
    i++; 
  }
  output[i] = '\0';
  
  printf("hi\n");
  return output;*/
}

//takes in client guess, the codeword, and the current state, and returns the new state
char *check_guess(char *guess, char *code_word, char* current){
  if(guess == NULL){
    return current;
  }
  int code_len = strlen(code_word);
  int guess_len = strlen(guess)-1;
  //char output[code_len];
  char* output = malloc(sizeof(code_word)+1);
  if(guess_len == 1){
    for(int i = 0; i < code_len; i++){
      if(code_word[i] == guess[0]){
        output[i] = guess[0];
      }
      else{
        output[i] = current[i];
      }
    }
  }
  else{
    for(int i = 0; i < guess_len; i++){
      if(code_word[i] == guess[i]){
        output[i] = guess[i];
      }
      else{
        output[i] = current[i];
      }
    }
    for(int i = guess_len; i < code_len; i++){
      output[i] = current[i];
    }
  }
  output[code_len]='\0';
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


  //char buff[256+1];
  //buff[50]=0;

  //int bytes;
  char buff[50];
  ssize_t bytes;

  while((bytes = read(r_file, buff, sizeof(buff)-1)) > 0){ 
    if(bytes == -1)err();//all non 0 are true
    buff[bytes] = '\0';
    // printf("read\n");
  } 
  if(strcmp(code_word, buff)==0){
    printf("NO STOP YOU HAVE ALREADY WON!!!!!\n");
    exit(1);
  }
  //shared memory
  int *data;
  int shmid;
  shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
  data = shmat(shmid, 0, 0);
  *data = * data + 1;
  printf("Round %d\n", *data);
  
  
  
 printf("Current:%s \nlength: %lu\n", buff, strlen(buff));
 

  //ask client to guess a character
  printf("Guess a character:"); 
  char line_buff[256];
  fgets(line_buff,255,stdin);
  char *guessed = line_buff;
  
  //process guess and get new state
  char after_guess[50];
  char is_victory[50];
  strcpy(after_guess, check_guess(guessed, code_word, buff));
  if(strcmp(code_word, check_guess(guessed, code_word, buff)) ==0){
    printf("Congrats!! You won in %d rounds.\n", *data);
    strcpy(is_victory, "done");
     //shared memory to say victory => victory means value 1 is stored in shared memory 
    int *data1;
    int shmid1;
    shmid1 = shmget(124, sizeof(int), IPC_CREAT | 0640);
    data = shmat(shmid1, 0, 0);
    *data1 = *data1 +1;
    shmdt(data1); //detach
    write(from_server, is_victory, 50);//added to slow down server
  }else{
    printf("After guessing: %s\n", check_guess(guessed, code_word, buff));
    strcpy(is_victory, "not done");
  }

  
  printf("status %s\n", is_victory);
  //write new state 
  int w_file;

  w_file = open("hangman.txt", O_TRUNC|O_WRONLY , 0611);
  if(w_file==-1)err();

  write(w_file, check_guess(guessed, code_word, buff), strlen(check_guess(guessed, code_word, buff)));//writing to file 
  // printf("wrote %s file after guess\nlength: %lu\n", check_guess(guessed, code_word, buff), strlen(check_guess(guessed, code_word, buff)));
  shmdt(data); //detach
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

  //gets code word from shared memory
  char code_word[50];

  char *data3;
  int shmid3;
  shmid3 = shmget(125, sizeof(char*), IPC_CREAT | 0640);
  data3 = shmat(shmid3, 0, 0);
  for(int i =0; i<strlen(data3); i++){
      code_word[i] = data3[i];
  }
  shmdt(data3); //detach

  printf("server read code_word: %s", code_word);

  

  int r_file = open("hangman.txt", O_RDONLY , 0);   
  if(r_file == -1) err();

  //char buff[256+1];
  //buff[50]=0;

  //int bytes;
  char buff[256];
  ssize_t bytes;
  //gets current state
  while((bytes = read(r_file, buff, sizeof(buff) - 1)) > 0){ 
    if(bytes == -1)err();//all non 0 are true
    buff[bytes] = '\0';
    // printf("read\n");
  } 


  //shared memory for rounds
  int *data;
  int shmid;
  shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
  data = shmat(shmid, 0, 0);
  printf("\n-----\nResult from round %d\n", *data);
  shmdt(data); //detach

  printf("Current:%s \nlength: %lu\n", buff, strlen(buff));


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
