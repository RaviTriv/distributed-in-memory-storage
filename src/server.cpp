#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

using namespace std;

int main(int argc, char *argv[])
{
  int port = 2401;
  char message[2100];

  sockaddr_in serverAddress;
  memset(&serverAddress, '\0', sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);

  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  listen(serverSocket, 1);

  sockaddr_in clientAddress;
  socklen_t clientAddressLength = sizeof(clientAddress);
  int clientDescriptor = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLength);
  cout << "CONNECTED " << endl;

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

    // string input;
    // getline(cin, input);
    memset(&message, 0, sizeof(message));
    strcpy(message, "TEST RESPONSE");
    send(clientDescriptor, (char *)&message, strlen(message), 0);
  }
  close(clientDescriptor);
  close(serverSocket);
}