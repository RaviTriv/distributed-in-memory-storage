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
  vector<int> ports;
  for (int i = 0; i < argc; i++)
  {
    ports.push_back(atoi(argv[i]));
  }

  for (int i = 1; i < ports.size(); i++)
  {
    printf("INIT PORTS: %d\n", ports.at(i));
  }
  char line[256];
  char message[256];
  // 0 4202 4201

  // ports.push_back(4201);
  // ports.push_back(4200);
  int port = 4200;
  while (true)
  {
    string line;
    getline(cin, line);

    if (ports.size() > 2)
    {
      if (line.find("Read:") != string::npos)
      {
        port = ports.at(ports.size() - 2);
      }
      else
      {
        port = ports.at(ports.size() - 1);
      }
    }
    else
    {
      port = ports.at(1);
    }
    printf("SIZE: %lu\n", ports.size());
    printf("USING PORT: %d\n", port);

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
      // Remove connection that is down
      ports.pop_back();
      port = ports.at(ports.size() - 1);
      string sendPorts = "NEW MASTER ";
      for (int i = 1; i < ports.size(); i++)
      {
        sendPorts += to_string(ports.at(i));
        sendPorts += " ";
        // printf("PORT: %d\n", ports.at(i));
      }
      printf("SEND MESSAGE: %s\n", sendPorts.c_str());
      // Send message to update node id
      NetworkStream *stream = connector->connect(port, serverIpAddress);
      memset(&message, 0, sizeof(message));
      // SEND SLAVE PORTS FROM HERE AS WELL
      strcpy(message, sendPorts.c_str());
      stream->send(message, sizeof(message));
      delete stream;
    }
  }
}