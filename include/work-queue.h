#include <pthread.h>
#include <list>

using namespace std;

class WorkQueue
{
public:
  WorkQueue();
  void add(void *);
  int remove();

private:
  list<void *> workQueue;
};