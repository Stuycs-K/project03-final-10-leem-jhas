#include "pipe.h"

static void sighandler(int signo) {
    if ( signo == SIGINT ){
      printf("Closing WKP...\n");
      remove(WKP);
      printf("WKP closed\n");
      exit(1);
    }
}

void write_random_code_word(){
    int w_file;
    w_file = open("codewords.txt", O_WRONLY | O_TRUNC | O_CREAT, 0611);
    if(w_file==-1)err();
    char list[5][50] = {"orange", "banana", "kiwi", "apple", "pineapple"};
    int length = sizeof(list) / sizeof(list[0]);
    for(int i = 0; i < length; i++){
        write(w_file, list[i], 50);
    }
    close(w_file);
}

char* get_code_word(){
    int r_file = open("codewords.txt", O_RDONLY , 0);
    char buff[50];
    ssize_t bytes;
    int total = 0;

    //get bytes of entire file
    while((bytes = read(r_file, buff, sizeof(buff))) > 0){
         // if(bytes == -1);//all non 0 are true
         // buff[bytes] = '\0';
         total += bytes;
       }
    // printf("bytes: %d\n", total);
    close(r_file); 
    //chooses random codeword
    srand(time(NULL));
    int num = (rand() % (total/50));

    int r_file1 = open("codewords.txt", O_RDONLY , 0);
    char* buff1 = (char*)malloc(50 * sizeof(char)); 
    lseek(r_file1, num*50, SEEK_SET);
    read(r_file1, buff1, sizeof(buff1));
    printf("code: %s\n", buff1);
    close(r_file1); 

    //wrote codeword to shared memory
    
    // int a;
    int w_file1;
    w_file1 = open("codewordfinal.txt", O_WRONLY | O_TRUNC | O_CREAT, 0611);
    if(w_file1==-1)err();
    write(w_file1, buff1, 50);
    close(w_file1);
                                // char *data3;
                                // int shmid3;
                                // shmid3 = shmget(125, sizeof(char*), IPC_CREAT | 0640);
                                // printf("hi\n");
                                // data3 = shmat(shmid3, 0, 0);
                                // printf("hi\n");
                                // // a = data3;
                                // // printf("data3 before: %s\n", a);
                                // for(int i =0; i<strlen(buff1); i++){
                                //     data3[i] = buff1[i];
                                // }
                                // printf("hi\n");
                                // int diff = strlen("pineapple") - strlen(buff1);  
                                // if(diff >0){ 
                                //     for(int j = strlen(buff1); j <diff+strlen(buff1) ; j++){
                                //         data3[j] = '\0';
                                //     }
                                // }
    // printf("data3 after: %s\n", data3);
    // printf("buff1: %s\n", buff1);

    // printf("Round: %d\n", *data);
    // shmdt(data3); //detach
    return buff1;
}
void add_to_bank(char * new_word){
    int w_file;
    w_file = open("codewords.txt", O_WRONLY|O_APPEND , 0611);
    if(w_file==-1)err();
    char add[50];
    strcpy(add, new_word);
    for(int i = strlen(new_word); i<50;i++){
        add[i]='\0';
    }
    write(w_file, add, 50);
    close(w_file);
}
int main(int argc, char *argv[]) {
    signal(SIGINT,sighandler);
    write_random_code_word();
    printf("Would you like to play or add a codeword to the bank?[play/add]: \n");
    char line_buff[50];
    fgets(line_buff, 50, stdin);
    
    line_buff[strlen(line_buff)-1] = '\0';

    if(strcmp(line_buff, "add")==0){
        printf("What word would you like to add?: \n");
        char line_buff1[50];
        fgets(line_buff1, 50, stdin);
        line_buff1[strlen(line_buff1)-1] = '\0';
        printf("adding %s\n", line_buff1);
        add_to_bank(line_buff1);
    }
  
    printf("ONTO THE GAME: WELCOME TO HANGMAN!!\n");


    //get code_word from function
    char code_word[51];
    printf("test\n");
    strcpy(code_word, get_code_word());
    // err();
    printf("test\n");
    printf("this is the code_word: %s\n", code_word);

    //shared memory for rounds
    int *data;
    int shmid;
    shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
    data = shmat(shmid, 0, 0);
    *data = 0;
    // printf("Round: %d\n", *data);
    shmdt(data); //detach

    //  shared memory for victory 
    int *data1;
    int shmid1;
    shmid1 = shmget(124, sizeof(int), IPC_CREAT | 0640);
    data1 = shmat(shmid1, 0, 0);  
    *data1 = 0;
    shmdt(data1); //detach
    
    int w_file;

    w_file = open("hangman.txt", O_WRONLY | O_TRUNC | O_CREAT, 0611);
    if(w_file==-1)err();
    // printf("created file\n");
    //write dashes to textfile
    char modified_word[50];
    strcpy(modified_word,process(code_word));
    write(w_file,modified_word, 50);
    close(w_file);
    // printf("wrote %s\n", modified_word);
    printf("once\n");
    while(1){

        int to_client;
        int from_client;
        //from_client = server_handshake(&to_client);
        from_client = server_setup(); // steps 1-3

        pid_t p = fork();
        if(p < 0){
            perror("Fork failed\n");
            exit(1);
        }
        else if(p == 0){ 
            to_client = server_connect(from_client);
            int *data2;
            int shmid2;
            int victory;
            shmid2 = shmget(124, sizeof(int), IPC_CREAT | 0640);
            data2 = shmat(shmid2, 0, 0);  
            victory = *data2;
            shmdt(data2); //detach
            printf("victory: %d\n", victory);
            if(victory>0){
                printf("You won!! Please do CONTROL C to start again.");
            } 
            // printf("done\n");
             //checks shared memory for victory 
            
            close(to_client);
            close(from_client);
            return p;
        }
    }
    //remove shared memory 
    shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
    shmctl(shmid, IPC_RMID, 0); //remove the segment
    shmid1 = shmget(124, sizeof(int), IPC_CREAT | 0640);
    shmctl(shmid1, IPC_RMID, 0); //remove the segment
    // int shmid3 = shmget(125, sizeof(int), IPC_CREAT | 0640);
    // shmctl(shmid3, IPC_RMID, 0); //remove the segment
} 