#include <stdio.h>
#include <stdlib.h>

class Task
{
public:
  int taskId;
  Task(int id)
  {
    taskId = id;
  };

  int getId() { return taskId; }
};