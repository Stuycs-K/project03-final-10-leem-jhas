#include "pipe.h"

static void sighandler(int signo) {
    if ( signo == SIGINT ){
      printf("Closing WKP...\n");
      remove(WKP);
      printf("WKP closed\n");
      exit(1);
    }
}
int main() {
    signal(SIGINT,sighandler);

    char code_word[50] = "hello its me";
    //shared memory 

    int *data;
    int shmid;
    shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
    data = shmat(shmid, 0, 0);
    *data = 0;
    printf("Round: %d\n", *data);
    shmdt(data); //detach


    int w_file;

    w_file = open("hangman.txt", 
        O_WRONLY | O_TRUNC | O_CREAT, 0611);
    if(w_file==-1)err();
    // printf("created file\n");
    //write dashes to textfile
    char modified_word[50];
    strcpy(modified_word,process(code_word));
    write(w_file,modified_word, strlen(modified_word));
    printf("wrote %s\n", modified_word);
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
            close(to_client);
            close(from_client);
            return p;
        }
    }
    // //remove shared memory 
    // shmid = shmget(123, sizeof(int), IPC_CREAT | 0640);
    // shmctl(shmid, IPC_RMID, 0); //remove the segment
} 