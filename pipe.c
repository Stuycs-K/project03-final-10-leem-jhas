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

  char *output = malloc(sizeof(input)+1);
  strcpy(output, input);

  if(output != NULL){
    char* curr = output;
    while(*curr != '\0' && *curr != '\n'){
      *curr = '-'; 
      curr++;
    }
    *curr = '\0';
  }
  return output;
}

//takes in client guess, the codeword, and the current state, and returns the new state
char *check_guess(char *guess, char *code_word, char* current){
  if(guess == NULL){
    return current;
  }
  int code_len = strlen(code_word);
  int guess_len = strlen(guess)-1;

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
  else if(guess_len == 2 && (guess[1] == '\n' || guess[1] == '\0')){
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

  // Server making the pipe

  // Server opening the pipe
  mkfifo(WKP, 0666);
  from_client = open(WKP, O_RDONLY);

  // Server removing the pipe

  remove(WKP);

  return from_client;
}

int server_handshake(int *to_client) {
  int from_client = server_setup();

  // Server reading SYN (the pid)
  char private_name[50];
  read(from_client, private_name, 50);

  // Server opening the Private Pipe
  int pp = open(private_name, O_WRONLY);

  // Server sending SYN_ACK
  write(pp, SYN_ACK, 50);

  // Server reading final ACK;
  char ack[50];
  read(from_client, ack, 50);

  return from_client;
}

int client_handshake(int *to_server) {
  int from_server;

  // Client making Private pipe
  char* private_name = calloc(50, sizeof(char));
  sprintf(private_name, "%d", getpid());

  // Client opening WKP
  int wkp = open(WKP, O_WRONLY);

  // Client Writing PP to WKP
  write(wkp, private_name, 50);

  // Client Opening PP
  mkfifo(private_name, 0666);
  from_server = open(private_name, O_RDONLY);

  // Client Deleting PP
  remove(private_name);

  // Client reading SYN_ACK
  char syn_ack[50];
  read(from_server, syn_ack, 50);

  // Client sending ACK
  write(wkp, ACK, 50);
  
  //get code word from client
  char code_word[50];
  read(from_server, code_word, 50);

  //read last line and get current
  int r_file = open("hangman.txt", O_RDONLY , 0);   
  if(r_file == -1) err();

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
  int rounds = *data;
  shmdt(data); //detach
  
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
    printf("Congrats!! You won in %d rounds.\n", rounds);

    strcpy(is_victory, "done");
    //shared memory to say victory => victory means value 1 is stored in shared memory 
    int *data1;
    int shmid1;
    shmid1 = shmget(124, sizeof(int), IPC_CREAT | 0640);
    data1 = shmat(shmid1, 0, 0);
    *data1 = 1;
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
  
  close(from_server);
  return from_server;
}


int server_connect(int from_client) {   
  int to_client  = 0;
  //Server reading SYN
  char private_name[50];
  read(from_client, private_name, 50);

  //Server opening the Private Pipe
  to_client = open(private_name, O_WRONLY);

  //Server sending SYN_ACK
  write(to_client, SYN_ACK, 50);

  //Server reading final ACK
  char ack[50];
  read(from_client, ack, 50);

  //Server received ACK, handshake complete

  //gets code word from shared memory
  char code_word[50];
  int r_file1 = open("codewordfinal.txt", O_RDONLY , 0);   
  if(r_file1 == -1) err();
  read(r_file1, code_word, 50);
  close(r_file1);
  printf("code word length: %d\n", strlen(code_word));

  // char *data3;
  // int shmid3;
  // shmid3 = shmget(125, sizeof(char*), IPC_CREAT | 0640);
  // data3 = shmat(shmid3, 0, 0);
  // for(int i =0; i<strlen(data3); i++){
  //     code_word[i] = data3[i];
  // }
  // shmdt(data3); //detach

  printf("server read code_word: %s", code_word);

  int r_file = open("hangman.txt", O_RDONLY , 0);   
  if(r_file == -1) err();

  char buff[256];
  ssize_t bytes;
  //gets current state
  while((bytes = read(r_file, buff, sizeof(buff) - 1)) > 0){ 
    if(bytes == -1)err();//all non 0 are true
    buff[bytes] = '\0';
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

/////////// MULTI-CLIENTS

int multi_client_create(char *room_code) {
  int semd = semget(KEY, 1, IPC_CREAT | 0644);
  if (semd == -1) {
    perror("Error: Cannot create semaphore.\n");
    exit(1);
  }
  union semun us;
  us.val = 1;
  semctl(semd, 0, SETVAL, us.val);

  int shmid = shmget(SHMEM, sizeof(off_t), IPC_CREAT | 0644);
  if (shmid == -1) {
    perror("Error: Cannot create shared memory\n");
    exit(1);
  }

  room_code[strlen(room_code)-1] = '\0'; // removing '\n' from filename
  char filename[50];
  strcpy(filename, room_code);
  strcat(room_code,".txt");
  strcat(filename,"_hidden.txt");

  int w_story = open(room_code, O_CREAT | O_TRUNC, 0644);
  if (w_story == -1){
    perror("Error: Cannot open room\n");
    exit(1);
  }

  close(w_story);

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  semd = semget(KEY, 1, 0);
  shmid = shmget(SHMEM, sizeof(int), 0);
  semop(semd, &sb, 1);
    
  printf("Attempting to open room...\n");

  int r_story = open(room_code, O_RDONLY);
  if (r_story == -1){
    perror("Error: Cannot open file\n");
    exit(1);
  }

  int *file_size = shmat(shmid, 0, 0);
  if (*file_size == (off_t)-1){
    perror("Error: Cannot add shared memory\n");
    exit(1);
  }

  int pos = lseek(r_story, -(*file_size), SEEK_END);

  char buffer[256];
  ssize_t bytes = read(r_story, buffer, sizeof(buffer) - 1);
  if (bytes == -1){
    perror("Error: Cannot read file\n");
    exit(1);
  }
  buffer[bytes] = '\0';

  printf("Write a phrase: ");
  fgets(buffer, sizeof(buffer)-1, stdin);
  buffer[strlen(buffer)-1] = '\0';
  char modified_word[50];
  strcpy(modified_word,process(buffer));
  printf("%d\n",strlen(buffer));

  *file_size = strlen(buffer);

  close(r_story);

  w_story = open(room_code, O_WRONLY | O_APPEND);
  if (w_story == -1){
    perror("Error: Cannot open file\n");
    exit(1);
  }

  if (write(w_story, modified_word, strlen(modified_word)) == -1){
    perror("Error: Cannot write to file\n");
    exit(1);
  }

  sb.sem_op = 1;
  if (semop(semd, &sb, 1) == -1){
    perror("Error: Cannot release semaphore\n");
    exit(1);
  }

  close(w_story);

  int code = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (w_story == -1){
    perror("Error: Cannot open file\n");
    exit(1);
  }
  printf("buufer: %s\n",buffer);
  if (write(code, buffer, strlen(buffer)) == -1){
    perror("Error: Cannot write to file\n");
    exit(1);
  }

  return 0;
}

int code_len;
int multi_client_guess(char *join_code) {
  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;

  int semd = semget(KEY, 1, 0);
  int shmid = shmget(SHMEM, sizeof(int), 0);
  semop(semd, &sb, 1);
    
  printf("Attempting to join room...\n");

  join_code[strlen(join_code)-1] = '\0'; // removing '\n' from filename
  char filename[50];
  strcpy(filename, join_code);
  strcat(join_code,".txt");
  strcat(filename,"_hidden.txt");


  //retrieve code first
  int code = open(filename, O_RDONLY);
  if (code == -1){
    perror("This room does not exist\n");
    exit(1);
  }

  char hidden[256];
  ssize_t num = read(code, hidden, sizeof(hidden) - 1);
  if (num == -1){
    perror("Error: Cannot read file\n");
    exit(1);
  }
  hidden[num] = '\0';
  //printf("code: %s\n",hidden);
  
  close(code);

  //open file with guesses
  int r_story = open(join_code, O_RDONLY);
  if (r_story == -1){
    perror("This room does not exist\n");
    exit(1);
  }

  int *file_size = shmat(shmid, 0, 0);
  if (*file_size == (off_t)-1){
    perror("Error: Cannot add shared memory\n");
    exit(1);
  }

  int pos = lseek(r_story, -(*file_size), SEEK_END);

  char buffer[256];
  ssize_t bytes = read(r_story, buffer, sizeof(buffer) - 1);
  if (bytes == -1){
    perror("Error: Cannot read file\n");
    exit(1);
  }

  buffer[bytes] = '\0';
  printf("Current State: %s \n", buffer);

  printf("Your Guess: ");
  char guessed[256];
  fgets(guessed,sizeof(guessed),stdin);

  *file_size = strlen(buffer);

  close(r_story);

  //char *code_word = "pineapple";
  char after_guess[50];
  strcpy(after_guess, check_guess(guessed, hidden, buffer));

  //printf("code: %s, guess: %s, prev line: %s, new line: %s\n", code_word, guessed, buffer, after_guess);
  //printf("size of guess: %d\n", strlen(guessed));

  int w_story = open(join_code, O_WRONLY | O_APPEND);
  if (w_story == -1){
    perror("Error: Cannot open file\n");
    exit(1);
  }
  if (write(w_story, after_guess, strlen(after_guess)) == -1){
    perror("Error: Cannot write to file\n");
    exit(1);
  }

  close(w_story);

  sb.sem_op = 1;
  if (semop(semd, &sb, 1) == -1){
    perror("Error: Cannot release semaphore\n");
    exit(1);
  }

  if(strcmp(after_guess, hidden) == 0){
    printf("Congrats! You've guessed the hidden phrase!\n");
    printf("Closing room...\n");
    remove(filename);
    remove(join_code);

    int semd = semget(KEY, 1, 0);
    int shmid = shmget(SHMEM, sizeof(int), 0);

    if (semctl(semd, IPC_RMID, 0) == -1){
        perror("Error: Cannot remove semaphore\n");
        exit(1);
    }

    if (shmctl(shmid, IPC_RMID, 0) == -1){
        perror("Error: Cannot remove shared memory\n");
        exit(1);
    }
  }

  return 0;
}