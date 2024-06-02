#ifndef JOB_H
#define JOB_H
#include <vector>
#include "task.h"

class Job
{
private:
  std::vector<Task *> tasks;

public:
  void addTask(Task *Task);   //добавляет Task в конец вектора задач задания
  Task *getTask(int task_id); //возвращает задачу с заданным идентификатором
  int getTaskCount();

  Job();
  ~Job();
};

#endif