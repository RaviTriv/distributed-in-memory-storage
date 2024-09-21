#include <cstdlib>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "../include/tcp-client.h"
using namespace std;

int main(int argc, char *argv[])
{
  // TAKE INPUT OF SERVER ADDRESSES
  // TAKE INPUT OF NODE IDS
  // char *serverIpAddress = argv[1];
  char *serverIpAddress = "127.0.0.1";

  char line[256];
  char message[256];
  int ports[5] = {4200, 4201};
  vector<int> ps;
  ps.push_back(4201);
  ps.push_back(4200);
  int port = 4200;
  int skip = 0;
  while (true)
  {
    string line;
    getline(cin, line);

    if (ps.size() > 1)
    {
      if (line.find("Read:") != string::npos)
      {
        port = ps.at(ps.size() - 2);
      }
      else
      {
        port = ps.at(ps.size() - 1);
      }
    }
    else
    {
      port = ps.at(0);
    }

    TCPClient *connector = new TCPClient();
    try
    {
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
    catch (const invalid_argument &e)
    {
      printf("FAILED to CONNECT\n");
      ps.pop_back();

      // Send message to update node id
      NetworkStream *stream = connector->connect(port, serverIpAddress);
      memset(&message, 0, sizeof(message));
      strcpy(message, "NEW MASTER");
      stream->send(message, sizeof(message));
      delete stream;
    }
  }
}