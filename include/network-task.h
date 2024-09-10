#include <stdio.h>
#include <stdlib.h>
#include "./network-stream.h"

#pragma once

class NetworkTask
{
  NetworkStream *stream;

public:
  NetworkTask(NetworkStream *s)
  {
    stream = s;
  }

  ~NetworkTask()
  {
    delete stream;
  }

  NetworkStream *getStream()
  {
    return stream;
  }
};