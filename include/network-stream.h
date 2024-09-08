#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

using namespace std;

// A class to represent a data stream and send/receive data between TCP sockets
class NetworkStream
{
  int socketDescriptor;
  string socketIP;
  int socketPort;

public:
  friend class TCPServer;
  friend class TCPClient;

  ~NetworkStream();

  ssize_t send(char *buffer, size_t len);
  ssize_t recieve(char *buffer, size_t len);

  string getSocketIP();
  int getSocketPort();

private:
  NetworkStream(int sd, struct sockaddr_in *addy);
};