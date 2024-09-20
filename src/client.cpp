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
  char message[256];
  int ports[5] = {4200, 4201};
  int port = 4200;
  while (true)
  {
    string line;
    getline(cin, line);

    if (line.find("Read:") != string::npos)
    {
      port = 4201;
    }
    else
    {
      port = 4200;
    }

    TCPClient *connector = new TCPClient();
    NetworkStream *stream = connector->connect(port, serverIpAddress);
    memset(&message, 0, sizeof(message));
    // string line;
    // getline(cin, line);
    strcpy(message, line.c_str());
    stream->send(message, sizeof(message));
    memset(&message, 0, sizeof(message));
    stream->recieve((char *)&message, sizeof(message));
    printf("RECIEVED: %s\n", message);
    delete stream;
  }
}