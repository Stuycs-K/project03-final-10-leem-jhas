#include "pipe.h"

int main(int argc, char *argv[]) {
  
  int to_server;
  int from_server;
  
  if (strcmp(argv[1], "solo") == 0) {
    from_server = client_handshake( &to_server );
  }
  else if (strcmp(argv[1], "multi_create") == 0) {
    //from_server = multi_client_handshake( &to_server );
    multi_client_create();
  }
  else if (strcmp(argv[1], "multi_guess") == 0) {
    //from_server = multi_client_handshake( &to_server );
    multi_client_guess();
  }

}