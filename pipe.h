#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h> 
#include <unistd.h>
#include <signal.h>
#include <time.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define WKP "hangman"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 50
#define MAX_CLIENTS 100

#define SYN 0
#define SYN_ACK "Beware the Ides of March"
#define ACK "Et tu, Brute?"
#define MESSAGE 3
#define EXIT 4

#define KEY 35782
#define SHMEM 78001

int server_handshake(int *to_client);
int client_handshake(int *to_server);
int multi_client_create(char *room_code);
int multi_client_guess(char *join_code);

//for basic & persistent servers
int server_connect(int from_client);

//for forking server
int server_setup();
int server_handshake_half(int *to_client, int from_client);

char *process(char *input);
char *check_guess(char *guess, char *code_word, char* current);
int victory(int to_client);

int err();

/*union semun {
    int val;                  //used for SETVAL
    struct semid_ds *buf;     //used for IPC_STAT and IPC_SET
    unsigned short  *array;   //used for SETALL
    struct seminfo  *__buf;
};*/

#endif
