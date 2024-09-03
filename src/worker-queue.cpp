#include <pthread.h>
#include <list>
#include "../include/work-queue.h"

WorkQueue::WorkQueue() {};

void WorkQueue::add(void * work)
{
  workQueue.push_back(work);
}

int WorkQueue::remove()
{
  int temp = workQueue.front();
  workQueue.pop_front();
  return temp;
}