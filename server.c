#include "pipe.h"

static void sighandler(int signo) {
    if ( signo == SIGINT ){
      printf("Closing WKP...\n");
      remove(WKP);
      printf("WKP closed\n");
      exit(1);
    }
}

char* random_code_word(){
    char list[5][50] = {"orange", "banana", "kiwi", "apple", "pineapple"};
    srand(time(NULL));
    int num = (rand() % (5));
    char *output = list[num];
    return output;
}
int main() {
    signal(SIGINT,sighandler);

    char *code_word;
    strcpy(code_word, random_code_word());
    printf("code: %s\n", code_word);
    code_word = "pine apple";


    //shared memory for codeword
    // char *data3;
    // int shmid3;
    // shmid3 = shmget(125, sizeof(char*), IPC_CREAT | 0640);
    // data3 = shmat(shmid3, 0, 0);
    // for(int i =0; i<strlen(code_word); i++){
    //     data3[i] = code_word[i];
    // }
    // // printf("Round: %d\n", *data);
    // shmdt(data3); //detach

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
    printf("hello\n");
    char modified_word[50];
    printf("hello\n");
    strcpy(modified_word,process(code_word));
    printf("hey\n");
    write(w_file,modified_word, 50);
    // printf("wrote %s\n", modified_word);
    int victory;
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
            
            int *data2;
            int shmid2;
            shmid2 = shmget(124, sizeof(int), IPC_CREAT | 0640);
            data2 = shmat(shmid2, 0, 0);  
            victory = *data2;
            shmdt(data2); //detach

            to_client = server_connect(from_client);
            // printf("done\n");
             //checks shared memory for victory 
            if(victory>0){
                printf("You won!!");
            } 
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
} 