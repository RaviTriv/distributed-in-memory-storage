#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
using namespace std;

int main()
{
  int port = 5201;
  char message[2100];

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (serverSocket == -1)
  {
    printf("Error creating socket \n");
  }

  sockaddr_in serverAddress;
  memset(&serverAddress, '\0', sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);

  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 1);

  sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  int clientDescriptor = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLength);
  // struct sockaddr_in serverAddress = {.sin_family = AF_INET, .sin_port = htons()};
  return 0;
  while (true)
  {
    memset(&message, 0, sizeof(message));
    recv(clientDescriptor, (char *)&message, sizeof(message), 0);
    cout << "Client: " << message << endl;
    if (strcmp(message, "QUIT") == 0)
    {
      cout << "ENDING SESSION" << endl;
      send(clientDescriptor, (char *)"QUIT", strlen(message), 0);
      break;
    }
    send(clientDescriptor, "TEST", strlen(message), 0);
    // do stuff
  }
  close(clientDescriptor);
  close(serverSocket);
}