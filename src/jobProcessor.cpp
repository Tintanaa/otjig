#include "jobProcessor.h"

#define WARMING 1
#define COOLING 0
#define MAX_ATTEMPTS 1000000

JobProcessor::JobProcessor()
{
    this->machine_count = 0;
    this->job_count = 0;
    this->operations_number = 0;
    this->vertex_weights.push_back(0);
}

JobProcessor::~JobProcessor()
{
    this->jobs.clear();
}

void JobProcessor::setMachineCount(int machineCount)
{
    this->machine_count = machineCount;
}

void JobProcessor::setJobCount(int jobCount)
{
    this->job_count = jobCount;
}

int JobProcessor::getMachineCount()
{
    return this->machine_count;
}

int JobProcessor::getJobCount()
{
    return this->job_count;
}

void JobProcessor::addJob(Job *job)
{
    this->jobs.push_back(job);
}

Job *JobProcessor::getJob(int jobId)
{
    return this->jobs[jobId];
}

bool JobProcessor::importOrlib(std::string filename, int selected_jobs)
{
    std::ifstream input;                             //создать поток входного файла
    input.open(filename.c_str(), std::ifstream::in); //открыть файл с входным потоком

    if (input.is_open())
    {
        int temp = 0, i = 0, read_jobs = 0;

        std::string current_line;
        std::stringstream ss;

        std::getline(input, current_line); //читает первую строку с номером задания и машины

        ss << current_line;

        ss >> temp; //читает номер задания
        if (selected_jobs == (-1))
        {
            this->setJobCount(temp);
        }
        else
        {
            this->setJobCount(selected_jobs);
        }

        ss >> temp; //читает номер машины
        this->setMachineCount(temp);
        this->machines.resize(temp);

        ss.clear(); //очищает поток струн

        while (std::getline(input, current_line) && read_jobs < this->getJobCount()) //пока есть следующая строка
        {
            Job *j = new Job(); //создать новую работу (новая строка = новая работа)
            Task *t;            //создать заполнитель задачи
            temp = 0;           //очищает временную переменную

            ss << current_line; //читает строку

            while (ss >> temp) //пока строка не пуста, прочитайте токен
            {
                if (i % 2 == 0) //если четный элемент, то это машинный номер задачи
                {
                    t = new Task(); //создать задачу и установить известные на данный момент данные
                    t->setMachineId(temp);
                    operations_number++;
                    this->machines[temp].push_back(operations_number);
                }
                else //если нечетный элемент, то это длительность обработки задачи (задачу мы уже создали, так как номера машин всегда первые)
                {
                    t->setDuration(temp);
                    j->addTask(t);
                    vertex_weights.push_back(temp);
                }
                i++; //увеличить i, чтобы различать номер машины и продолжительность
            }
            ss.clear();      //очищает поток струн
            this->addJob(j); //добавляет задание чтения в вектор задания
            read_jobs++;
        }
        input.close(); //закрытие нашего входного файлового потока
        return true;
    }
    else
    {
        std::cout << "Error occurred while opening the file" << std::endl;
        return false;
    }
}

bool JobProcessor::importTaillard(std::string filename, int selected_jobs)
{
    std::ifstream input;                             //create an input file stream
    input.open(filename.c_str(), std::ifstream::in); //open file with input stream
    if (input.is_open())
    {
        int job_num = 0, task_id = 0, temp = 0;
        Task *t; //create a task placeholder
        Job *j;  //create a job placeholder

        std::string current_line;
        std::stringstream ss;

        std::getline(input, current_line); //reads first line with job and machine number and other not important(to us) data

        ss << current_line;

        ss >> temp; //reads job number
        if (selected_jobs == (-1))
        {
            this->setJobCount(temp);
        }
        else
        {
            this->setJobCount(selected_jobs);
        }

        ss >> temp; //читает номер машины
        this->setMachineCount(temp);
        this->machines.resize(temp);

        while (ss >> temp) //идет в конец строки
            ;

        ss.clear();

        bool times = true; //переменная, которая позволяет нам различать номер машины и строки продолжительности обработки

        while (std::getline(input, current_line)) //пока есть следующая строка
        {

            if (current_line.find("Times") != std::string::npos) //если мы встречаем строку «Время», мы устанавливаем для времени значение true, поэтому мы создаем только новые задания и задачи и заполняем задачи длительностью обработки.
            {
                times = true;
                continue;
            }

            if (current_line.find("Machines") != std::string::npos) //если мы встречаем строку «Машины», мы перебираем ранее созданные задания и заполняем номер машины задачи.
            {
                times = false;
                job_num = 0; //мы очищаем job_num, чтобы можно было перебирать ранее созданные задания
                continue;
            }

            if (times && job_num < this->getJobCount()) //если времена верны, то нам придется создать новую работу
            {
                j = new Job(); //создать новую работу (новая строка = новая работа)
            }

            ss << current_line;
            if (job_num < this->getJobCount())
            {
                while (ss >> temp) //пока строка не пуста, прочитайте токен
                {
                    if (times) //если время верно, создайте новые задачи и заполните известные на данный момент данные
                    {
                        t = new Task();
                        t->setDuration(temp);
                        j->addTask(t);
                        vertex_weights.push_back(temp);
                    }
                    else //если время неверно, просто введите идентификатор машины задачи
                    {
                        this->getJob(job_num)->getTask(task_id)->setMachineId(temp - 1);
                        operations_number++;
                        this->machines[temp - 1].push_back(operations_number);
                    }
                    task_id++;
                }
                if (times) //если времена верны, то нам придется добавить нашу работу
                {
                    this->addJob(j);
                }
            }
            while (ss >> temp) //идет в конец строки
                ;
            ss.clear();
            job_num++;
            task_id = 0;
        }

        return true;
    }
    else
        return false;
}

void JobProcessor::printData()
{
    /*
        Печатает данные, импортированные любым из методов.
    */
    std::cout << "Imported data with " << this->getJobCount() << " jobs and " << this->getMachineCount() << " machines" << std::endl;
    for (int i = 0; i < job_count; i++)
    {
        for (int j = 0; j < (this->getJob(0)->getTaskCount()); j++)
        {
            Task *task = this->getJob(i)->getTask(j);
            std::cout << task->getMachineId() << " " << task->getDuration() << " ";
        }
        std::cout << std::endl;
    }
}

void JobProcessor::createGraph(void)
{
    /*
        Создает дизъюнктивный граф, используя импортированные данные.
	*/
    Graph g(operations_number + 2); // количество операций + источник + приемник

    int source = 0;
    int sink = operations_number + 1; // последняя вершина

    int current_operation_vertex = 1; // первая вершина = первая задача

    int jobs_number = this->getJobCount(); // Количество рабочих мест
    for (int i = 0; i < jobs_number; i++)  // Для каждой работы
    {
        int job_operations = this->getJob(i)->getTaskCount(); // Посчитать количество операций/задач
        g.addArc(0, current_operation_vertex, 0);            // добавить дугу от источника к первой задаче задания (расстояние == 0)
        for (int j = 0; j < job_operations - 1; j++)          // Для каждой задачи (работы i), кроме последней
        {
            // Добавить дугу от задачи j к задаче j+1 с расстоянием, равным времени задачи.
            g.addArc(current_operation_vertex, current_operation_vertex + 1, this->getJob(i)->getTask(j)->getDuration());
            current_operation_vertex++;
        }
        // Добавить дугу от последней задачи до опускания на расстоянии, равном времени последней задачи.
        g.addArc(current_operation_vertex, sink, this->getJob(i)->getTask(job_operations - 1)->getDuration());

        current_operation_vertex++;
    }

    for (int i = 0; i < machines.size(); i++) // Для каждой машины создайте клику
    {
        Machine m = machines[i]; // m — вектор вершин операций, выполняемых на машине i

        int no_ops_machine = m.size(); // Количество операций на машине i

        vector<int> l; // Вектор, содержащий длины операций на машине i

        for (int j = 0; j < no_ops_machine; j++) // за каждую операцию
        {
            l.push_back(vertex_weights[m[j]]); // получить длину выходящих дуг
        }

        g.createAcyclicClique(m, l); // создать клику (она должна быть без циклов!)
    }
    this->graph = g;
}

int JobProcessor::getCmax(void)
{
    /*
        Возвращает общую продолжительность, которая просто означает путь от источника до приемника максимальной длины.
	*/
    return graph.maxDistances().back();
}

vector<int> JobProcessor::selectArc(deque<int> criticalPath)
{
    /*
        Случайным образом выбирает дугу, соединяющую две вершины критического пути.
    */
    int random_vector = (rand() % (criticalPath.size() - 2)) + 1;
    vector<int> selected_arc;
    selected_arc.resize(2);
    selected_arc[0] = criticalPath[random_vector];
    selected_arc[1] = criticalPath[random_vector + 1];
    return selected_arc;
}

bool JobProcessor::successChance(int cmax, int new_cmax, double temperature)
{
    double chance = ((double)rand() / (RAND_MAX));
    double result = exp((cmax - new_cmax) / (temperature));
    if (chance <= result){
        return true;
    }
    else
        return false;
}

vector<int> JobProcessor::getStartTimes(void)
{
    /*
        Возвращает время начала всех операций, что означает расчет максимальных расстояний от источника до каждой вершины. 
        Мы исключаем первое расстояние, которое всегда равно 0.
	*/
    vector<int> v = graph.maxDistances();
    v.erase(v.begin(), v.begin() + 1);
    v.pop_back();
    return v;
}

vector<int> JobProcessor::getJobLenghts(void)
{
    /*
        Возвращает вектор, содержащий продолжительность каждого задания.
    */
    vector<int> v;
    for (int i = 0; i < this->jobs.size(); i++)
    {
        v.push_back(this->getJob(i)->getTaskCount());
    }
    return v;
}

void JobProcessor::printStartTimes(void)
{
    /*
        Печатает время начала каждой задачи, где каждая строка представляет собой задание.
    */
    vector<int> times = getStartTimes();
    vector<int> job_lenghts = getJobLenghts();
    int current_job_pos = 0;
    int current_job = 0;
    for (int i = 0; i < times.size(); i++)
    {
        if (current_job_pos == job_lenghts[current_job])
        {
            current_job++;
            current_job_pos = 0;
            printf("\n");
        }
        printf("%d ", times[i]);
        current_job_pos++;
    }
}

double JobProcessor::solveUsingSA(double initial_temperature, double alpha_warming, double alpha_cooling, int cooling_length, double warming_threshold, int max_moves_without_improvement)
{
    /*
        Решает проблему Job-Shop с использованием метаэвристики моделирования отжига.
    */

    srand(time(NULL));
    int mode = WARMING; //Warming / cooling
    double temperature = initial_temperature;

    list<bool> last_moves;
    const int last_moves_size = 2000;
    last_moves.resize(last_moves_size);
    std::fill(last_moves.begin(), last_moves.end(), false);

    int accepted_moves = 0;
    int accepted_moves_out_of_last_moves = 0;
    int moves_without_improvement = 0;

    int arc_select_attempts = 0;
    int no_cycle_attempts = 0;

    int task_count_in_job = ((operations_number) / (this->getJobCount()));
    bool cant_change_path = false;
    bool cycle_created = false;
    createGraph();

    int cmax;
    int new_cmax;

    deque<int> crit_path;
    vector<int> random_arc;
    random_arc.resize(2);

    bool time_exceeded = false;

    struct timespec start, stop;
    double totaltime = 0.0;
    clock_gettime(CLOCK_REALTIME, &start);
    while (moves_without_improvement < max_moves_without_improvement && !time_exceeded) // пока время не истекло и мы все еще немного улучшаемся
    {
        crit_path = graph.criticalPath(operations_number + 1); //получить критический путь созданного графа
        cmax = getCmax();                                      //получить Cmax
        if (graph.getCycleExists())                           //определить, нет ли цикла в начале
        {
            cout << "Wrong data: cycle detected at the beginning!" << endl;
            return -1;
        }

        no_cycle_attempts = 0;
        do //делаем пока не создадим цикл инвертированием дуги
        {
            arc_select_attempts = 0;
            cycle_created = false;
            do //выбрать случайную дугу, которая не начинается в источнике и не заканчивается в приемнике и не находится между двумя соседними задачами
            {
                random_arc = selectArc(crit_path);
                ++arc_select_attempts;
                if (arc_select_attempts > MAX_ATTEMPTS)
                {
                    cant_change_path = true;
                    break;
                }
            } while (random_arc[0] == 0 || random_arc[1] == operations_number + 1 || ((random_arc[1] - random_arc[0] == 1) && (random_arc[0] % task_count_in_job != 0)));
            if (cant_change_path)
            { //если на критическом пути нет дуги, которую мы могли бы изменить, то закончим
                break;
            }

            graph.invertArc(random_arc[0], random_arc[1]); //инвертировать направление найденной дуги
            graph.setArcLength(random_arc[1], random_arc[0], vertex_weights[random_arc[1]]);

            getCmax(); // вычислить cmax, поскольку он использует топологическую сортировку, которая определяет цикл, который позже можно проверить с помощью "getCycleExists()"
            ++no_cycle_attempts;

            if (graph.getCycleExists()) //если мы создали цикл путем инвертирования, верните дугу и повторите попытку
            {
                cycle_created = true;
                graph.invertArc(random_arc[1], random_arc[0]); //вернуть дугу обратно
                graph.setArcLength(random_arc[0], random_arc[1], vertex_weights[random_arc[0]]);
            }
            if (no_cycle_attempts > MAX_ATTEMPTS)
            {
                cant_change_path = true;
                break;
            }
        } while (cycle_created);
        if (cant_change_path) //если после многих попыток мы не сможем изменить критический путь, то закончим
        {
            break;
        }

        new_cmax = getCmax(); //получить Cmax после инвертирования дуги

        if (new_cmax < cmax) //если новый CMAX лучше
        {
            if (mode == WARMING) //если потепление, добавьте информацию о последних принятых ходах
            {
                last_moves.push_back(true);
                if (last_moves.front() == false)
                    accepted_moves_out_of_last_moves++;
                last_moves.pop_front();
            }
            accepted_moves++;
            cmax = new_cmax; //установите cmax на новый
        }
        else if (new_cmax == cmax) //если новый Cmax такой же, примите переход (отсутствие улучшения)
        {
            accepted_moves++;
            moves_without_improvement++;
            if (mode == WARMING) //если потепление, добавьте информацию о последних принятых ходах
            {
                last_moves.push_back(true);
                if (last_moves.front() == false)
                    accepted_moves_out_of_last_moves++;
                last_moves.pop_front();
            }
        }
        else //если новый Cmax будет хуже
        {
            if (successChance(cmax, new_cmax, temperature) == true) //если вероятность успеха больше случайной, тогда примите
            {
                accepted_moves++;
                cmax = new_cmax;
                moves_without_improvement++;
                if (mode == WARMING) //если потепление, добавьте информацию о последних принятых ходах
                {
                    last_moves.push_back(true);
                    if (last_moves.front() == false)
                    {
                        accepted_moves_out_of_last_moves++;
                    }
                    last_moves.pop_front();
                }
            }
            else // иначе не принимай переезд
            {
                moves_without_improvement++;
                if (mode == WARMING) //если потепление, добавьте информацию о принятых ходах
                {
                    if (accepted_moves > cooling_length / 10) //отрегулировать температуру (увеличить), если после 10% Cooling_length мы не принимаем ход
                    {
                        accepted_moves = 0;
                        temperature += alpha_warming * initial_temperature;
                    }
                    last_moves.push_back(false); // добавить информацию о последних (не)принятых ходах
                    if (last_moves.front() == true)
                        accepted_moves_out_of_last_moves--;
                    last_moves.pop_front();
                }
                graph.invertArc(random_arc[1], random_arc[0]); //вернуть дугу обратно
                graph.setArcLength(random_arc[0], random_arc[1], vertex_weights[random_arc[0]]);
            }
        }
        if (mode == WARMING && (double)accepted_moves_out_of_last_moves / last_moves_size >= warming_threshold) //если потепление и последние принятые ходы/последние ходы превышают пороговое значение
        {
            accepted_moves = 0;
            mode = COOLING; //измените режим на охлаждение
        }
        else if (mode == COOLING && accepted_moves >= cooling_length) //если охлаждение и мы превышаем допустимую длину охлаждения
        {
            temperature *= alpha_cooling; //уменьшить температуру
            accepted_moves = 0;
        }

        clock_gettime(CLOCK_REALTIME, &stop);
        totaltime += (double)(stop.tv_sec - start.tv_sec) + 1.e-9 * (stop.tv_nsec - start.tv_nsec);

        //Если время превысило 5 минут, завершить
        if (totaltime > 300.0)
            time_exceeded = true;

        clock_gettime(CLOCK_REALTIME, &start);
    }
    std::cout << "Program ended, because : " << std::endl;
    if (!(moves_without_improvement < max_moves_without_improvement))
        printf("Exceeded number of moves without improvement\n");
    else if (time_exceeded)
        printf("Exceeded time\n");
    else if (cycle_created)
        printf("Cycle in the graph created!.\n");
    else if (cant_change_path)
        printf("Cannot alter the critical path anymore\n");

    printf("\nStart times: \n");
    printStartTimes();
    printf("\n \n");
    printf("Cmax: %d, Time in sec: %f\n", getCmax(), totaltime);   // FOR CALC

    return totaltime;
}