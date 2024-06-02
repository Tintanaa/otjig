#ifndef JOBPROCESSOR_H
#define JOBPROCESSOR_H

#include <cmath>
#include "job.h"
#include "graph.h"
#include <sstream>
#include <fstream>
#include <list>
#include <iostream>
#include <string>

typedef std::vector<int> Machine;
typedef std::vector<Machine> Machines;
typedef std::vector<Job *> Jobs;

class JobProcessor
{
  private:
    int machine_count;
    int job_count;
    int operations_number; // Общее количество операций/задач

    Jobs jobs;
    Machines machines;

    vector<int> vertex_weights; // Содержит веса дуг, выходящих из вершин.

  public:
    Graph graph; // Дизъюнктивный граф

    void setMachineCount(int machine_count);
    void setJobCount(int job_count);

    int getMachineCount();
    int getJobCount();

    void addJob(Job *job);
    Job *getJob(int job_id);

    bool importOrlib(std::string file_name, int selected_jobs);   //импортирует данные из формата Orlib
    bool importTaillard(std::string file_name, int selected_jobs); //импортирует данные из формата Tailard
    void printData();

    void createGraph();
    int getCmax();
    vector<int> selectArc(deque<int> criticalPath);
    bool successChance(int cmax, int new_cmax, double temperature);
    vector<int> getStartTimes();
    vector<int> getJobLenghts();
    void printStartTimes();
    double solveUsingSA(double initial_temperature, double alpha_warming, double alpha_cooling, int cooling_length, double warming_threshold, int max_moves_without_improvement);

    JobProcessor();
    ~JobProcessor();
};
#endif