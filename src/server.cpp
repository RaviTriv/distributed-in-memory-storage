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
#include <pthread.h>
#include "../include/key-value.h"

using namespace std;
int port = 2401;
char message[2100];
sockaddr_in serverAddress;
int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
sockaddr_in clientAddress;
socklen_t clientAddressLength = sizeof(clientAddress);
int clientDescriptor1;
int clientDescriptor2;
pthread_t t1;
pthread_t t2;
pthread_t t3;


void *serverResponse(void *)
{
  listen(serverSocket, 5);

  clientDescriptor1 = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLength);

  cout << "CONNECTED " << endl;
  while (true)
  {
    memset(&message, 0, sizeof(message));
    recv(clientDescriptor1, (char *)&message, sizeof(message), 0);

    cout << "Client: " << message << endl;

    if (strcmp(message, "QUIT") == 0)
    {
      cout << "ENDING SESSION" << endl;
      send(clientDescriptor1, (char *)"QUIT", strlen(message), 0);
      exit(0);
    }

    memset(&message, 0, sizeof(message));
    strcpy(message, "TEST RESPONSE");
    send(clientDescriptor1, (char *)&message, strlen(message), 0);
  }
  return 0;
}

void *serverResponse2(void *)
{
  listen(serverSocket, 5);

  clientDescriptor2 = accept(serverSocket, (sockaddr *)&clientAddress, &clientAddressLength);

  cout << "CONNECTED " << endl;
  while (true)
  {
    memset(&message, 0, sizeof(message));
    recv(clientDescriptor2, (char *)&message, sizeof(message), 0);

    cout << "Client: " << message << endl;

    if (strcmp(message, "QUIT") == 0)
    {
      cout << "ENDING SESSION" << endl;
      send(clientDescriptor2, (char *)"QUIT", strlen(message), 0);
      exit(0);
    }

    memset(&message, 0, sizeof(message));
    strcpy(message, "TEST RESPONSE");
    send(clientDescriptor2, (char *)&message, strlen(message), 0);
  }
  return 0;
}

int main(int argc, char *argv[])
{
  KeyValueStore store;
  store.set();
  memset(&serverAddress, '\0', sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(port);

  bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

  while (true)
  {
    pthread_create(&t1, NULL, serverResponse, NULL);
    pthread_create(&t2, NULL, serverResponse2, NULL);
  }
  close(clientDescriptor1);
  close(serverSocket);
}
