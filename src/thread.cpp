#include <pthread.h>
#include "../include/thread.h"

Thread::Thread()
{
  threadId = 0;
  running = 0;
  detached = 0;
}

Thread::~Thread()
{
  if (running == 1 && detached == 0)
  {
    pthread_detach(threadId);
  }
  if (running == 1)
  {
    pthread_cancel(threadId);
  }
}

static void *runThread(void *arg)
{
  return ((Thread *)arg)->run();
}

int Thread::start()
{
  int res = pthread_create(&threadId, NULL, runThread, this);
  if (res == 0)
  {
    running = 1;
  }
  return res;
}

int Thread::join()
{
  int res = -1;
  if (running == 1)
  {
    res = pthread_join(threadId, NULL);
    if (res == 0)
    {
      detached = 1;
    }
  }
  return res;
}

int Thread::detach()
{
  int res = -1;
  if (running == 1 && detached == 0)
  {
    res = pthread_detach(threadId);
    if (res == 0)
    {
      detached = 1;
    }
  }
  return res;
}

pthread_t Thread::getThreadId(){
  return threadId;
}