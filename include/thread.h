#include <pthread.h>

class Thread
{
public:
  Thread();
  virtual ~Thread();
  int start();
  int join();
  int detach();
  pthread_t getThreadId();

  virtual void *run() = 0;

private:
  pthread_t threadId;
  int running;
  int detached;
};