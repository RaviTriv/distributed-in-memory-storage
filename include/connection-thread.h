#include "./thread.h"
#include "./work-queue.h"
#include "./persistence.h"
#include "./network-task.h"
#include "./key-value.h"
#include "./replication.h"

class ConnectionThread : public Thread
{
public:
  char message[256];
  WorkQueue<NetworkTask *> &queue;
  DataPersistence backup;
  KeyValueStore *store;
  Replication *replica;
  ConnectionThread(WorkQueue<NetworkTask *> &q, KeyValueStore *s, Replication *r);
  //void run();
};