#include <cstdlib>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  char *serverIpAddress = "127.0.0.1";
  int port = 2401;
  char message[2100];

  struct hostent *host = gethostbyname(serverIpAddress);
  sockaddr_in serverSockAddress;
  memset(&serverSockAddress, '\0', sizeof(serverSockAddress));

  serverSockAddress.sin_family = AF_INET;
  serverSockAddress.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr *)*host->h_addr_list));
  serverSockAddress.sin_port = htons(port);

  int client = socket(AF_INET, SOCK_STREAM, 0);
  connect(client, (sockaddr *)&serverSockAddress, sizeof(serverSockAddress));
  while (true)
  {
    string input;
    getline(cin, input);
    memset(&message, 0, sizeof(message));
    strcpy(message, input.c_str());
    send(client, (char *)&message, strlen(message), 0);
    recv(client, (char *)&message, sizeof(message), 0);

    cout << "Server: " << message << endl;

    if (strcmp(message, "QUIT") == 0)
    {
      cout << "ENDING SESSION" << endl;
      break;
    }
  }
}