#include <string>
#include <netinet/in.h>
#include "network-stream.h"

class TCPClient
{
public:
  TCPClient();
  NetworkStream *connect(int port, const char *s);
};