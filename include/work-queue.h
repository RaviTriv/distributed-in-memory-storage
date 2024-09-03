#include <pthread.h>
#include <list>

using namespace std;

template <typename T>
class WorkQueue
{
  list<T> queue;
  pthread_mutex_t mutex;
  pthread_cond_t cond;

public:
  WorkQueue()
  {
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);
  }
  ~WorkQueue()
  {
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
  }
  void add(T task)
  {
    pthread_mutex_lock(&mutex);
    queue.push_back(task);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
  }
  T remove()
  {
    pthread_mutex_lock(&mutex);
    while (queue.size() == 0)
    {
      pthread_cond_wait(&cond, &mutex);
    }
    T task = queue.front();
    queue.pop_front();
    pthread_mutex_unlock(&mutex);
    return task;
  }
  int size()
  {
    pthread_mutex_lock(&mutex);
    int size = queue.size();
    pthread_mutex_unlock(&mutex);
    return size;
  }
};