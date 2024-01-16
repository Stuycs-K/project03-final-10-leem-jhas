#include "pipe.h"

int main(int argc, char *argv[]) {
  
  int to_server;
  int from_server;
  
  if (strcmp(argv[1], "solo") == 0) {
    from_server = client_handshake( &to_server );
  }
  else if (strcmp(argv[1], "multi_create") == 0) {
    //from_server = multi_client_handshake( &to_server );
    char room_code[256];
    printf("Create a Room Code: ");
    fgets(room_code, sizeof(room_code), stdin);
    multi_client_create(room_code);
  }
  else if (strcmp(argv[1], "multi_guess") == 0) {
    //from_server = multi_client_handshake( &to_server );
    char join_code[256];
    printf("Room Code: ");
    fgets(join_code, sizeof(join_code), stdin);
    multi_client_guess(join_code);
  }

}