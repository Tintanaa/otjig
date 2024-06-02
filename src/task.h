#ifndef TASK_H
#define TASK_H

class Task
{
private:
  int machine_id; //идентификатор машины, на которой должна быть обработана задача
  int duration;   //продолжительность обработки задачи
public:
  void setMachineId(int machine_id);
  void setDuration(int duration);

  int getMachineId();
  int getDuration();

  Task();
};

#endif