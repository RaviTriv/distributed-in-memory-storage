#include <cstdlib>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include "../include/tcp-client.h"
using namespace std;

int main(int argc, char *argv[])
{
  // char *serverIpAddress = argv[1];
  char *serverIpAddress = "127.0.0.1";

  char line[256];
  TCPClient *connector = new TCPClient();
  NetworkStream *stream = connector->connect(4200, serverIpAddress);

  char message[256];
  strcpy(message, "yo");

  stream->send(message, sizeof(message));
  memset(&message, 0, sizeof(message));
  stream->recieve((char *)&message, sizeof(message));
  printf("RECIEVED: %s\n", message);
  delete stream;
}