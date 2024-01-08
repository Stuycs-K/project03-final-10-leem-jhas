#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h> 

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


int server_handshake(int *to_client);
int client_handshake(int *to_server);

//for basic & persistent servers
int server_connect(int from_client);

//for forking server
int server_setup();
int server_handshake_half(int *to_client, int from_client);

//multi_server
//int multi_server_setup();
//int multi_server_connect(int from_client, struct message m);



char *process(char *input);
char *check_guess(char *guess, char *code_word, char* current);

int err();

#endif
