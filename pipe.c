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

char *check_guess(char guess, char *code_word){
  int len = strlen(code_word);
  char output[len];
  for(int i = 0; i <len;i++){
    if(code_word[i] == guess){
      output[i] = guess;
    }
    else{
      output[i] = '-';
    }

  }
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
  
  char code_word[50];
  // char modified_word[50];
  read(from_server, code_word, 50);
  // strcpy(modified_word,process(code_word));
  // printf("%s\n",modified_word);

  int length_of_code_word = strlen(code_word);
  //read last line
                  // int r_file = open("hangman.txt", O_RDONLY , 0);   
                  //    if(r_file == -1) err();

                // lseek(r_file, -1*length_of_code_word, SEEK_END );
                // char buff[256+1];
                // buff[256]=0;

                // int bytes;
                // char* current;


                // while((bytes = read(r_file, buff, BUFFER_SIZE))){
                    
                //     if(bytes == -1)err();//all non 0 are true
                //     current=buff;
                //     // printf("%s\n",buff); 
                //     // printf("h\n");
                // }  
                // printf("Current:%s \n", current);

  //ask client to guess a character
  printf("Guess a character:"); 
  char line_buff[256];
  fgets(line_buff,255,stdin);
  char guessed = line_buff[0];
  
  char after_guess[50];
  // strcpy(after_guess, check_guess(guessed, code_word));
  printf("After guessing: %s\n", check_guess(guessed, code_word));


              // int w_file;

              // w_file = open("hangman.txt", 
              //     O_WRONLY | O_TRUNC | O_CREAT, 0611);
              // if(w_file==-1)err();


              // write(w_file, after_guess, strlen(after_guess));//writing to file 
              // printf("wrote to file after guess");
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
  
  char code_word[50] = "hello its me";


  // int w_file;

  // w_file = open("hangman.txt", 
  //     O_WRONLY | O_TRUNC | O_CREAT, 0611);
  // if(w_file==-1)err();
  // // printf("created file\n");
  // //write dashes to textfile
  // char modified_word[50];
  // strcpy(modified_word,process(code_word));
  // write(w_file,modified_word, strlen(modified_word));
//create text file

  int length_of_code_word = strlen(code_word);
  //read last line
  int r_file = open("hangman.txt", O_RDONLY , 0);   
     if(r_file == -1) err();

  lseek(r_file, -1*length_of_code_word, SEEK_END );
  char buff[256+1];
  buff[256]=0;

  int bytes;
  char* current;

  while((bytes = read(r_file, buff, BUFFER_SIZE))){
      
      if(bytes == -1)err();//all non 0 are true
      current=buff;
      // printf("%s\n",buff); 
      // printf("h\n");
  }  
  printf("Word: %s\n", buff);

          // int w_file;

          // w_file = open("hangman.txt", 
          //     O_WRONLY | O_TRUNC | O_CREAT, 0611);
          // if(w_file==-1)err();
          // write(w_file,buff, strlen(buff));
          // printf("wrote to file\n");
  
  write(to_client, code_word, strlen(code_word));
  printf("wrote code to client\n");
  //server print dashes
  


  //write code word to client
  

  return to_client;
}

