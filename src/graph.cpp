#include "graph.h"
#include <cstdio>

Graph::Graph()
{
}

Graph::Graph(int n)
{
    /*
        Создает граф, имеющий n вершин
	    Так же создаём списки входящих и исходящих дуг, они нам нужны для DFS и поиска критических секций
	*/
    vertices_number = n;
    arcs_exiting_vertex.resize(n);
    arcs_entering_vertex.resize(n);
}

Graph::~Graph()
{
}

void Graph::addArc(int from, int to, int length)
{
    /*
        Добавление дуги из вершины «от» в вершину «до» определенной длины
	*/
    Arc outgoing_arc = {to, length};
    Arc incoming_arc = {from, length};
    arcs_exiting_vertex[from].push_back(outgoing_arc);
    arcs_entering_vertex[to].push_back(incoming_arc);
}

void Graph::setArcLength(int from, int to, int length)
{
    /*
        Устанавливает разную длину дуги от вершины «от» до другой вершины «до».
	*/
    bool exists = false;
    for (Vertex::iterator it = arcs_exiting_vertex[from].begin(); it != arcs_exiting_vertex[from].end(); it++) //для каждой дуги, выходящей из вершины
    {
        if (it->vertex_id == to)    //если найдена дуга, входящая в вершину "tp"
        {
            it->length = length;    //изменяем длину
            exists = true;
        }
    }
    for (Vertex::iterator it = arcs_entering_vertex[to].begin(); it != arcs_entering_vertex[to].end(); it++) //для каждой дуги, входящей в вершину
    {
        if (it->vertex_id == from)  //если найдена дуга исходящая из вершины
        {
            it->length = length;    //изменяем длину
            if (exists)
                return;
        }
    }
    if (!exists)
    {
        printf("Arc form %d to %d does not exist\n", from, to);
    }
}

Arc Graph::getArc(int from, int to)
{
    /*
        Возвращает дугу от вершины «от» до вершины «до»
	*/
    for (Vertex::iterator it = arcs_exiting_vertex[from].begin(); it != arcs_exiting_vertex[from].end(); it++)  //for every outgoing arc
    {
        if (it->vertex_id == to)    //если найдена дуга, которая входит в вершину
        {
            return *it;     //возвращаем эту дугу
        }
    }
    printf("Arc form %d to %d does not exist\n", from, to);

}

int Graph::getArcLength(int from, int to)
{
    /*
		Возвращает длину дуги от вершины до вершины
	*/
    Arc a = getArc(from, to);
    return a.length;
}

void Graph::deleteArc(int from, int to)
{
    /*
		Удаляет дугу от вершины до вершины
	*/
    bool erased = false;
    for (Vertex::iterator it = arcs_exiting_vertex[from].begin(); it != arcs_exiting_vertex[from].end(); it++) //для каждой дуги из вершины
    {
        if (it->vertex_id == to)    //если дуга входит в вершину
        {
            arcs_exiting_vertex[from].erase(it);    //удаляем
            erased = true;
            break;
        }
    }
    for (Vertex::iterator it = arcs_entering_vertex[to].begin(); it != arcs_entering_vertex[to].end(); it++) //для каждой дуги входящей в вершину
    {
        if (it->vertex_id == from)  //если дуга выходит из вершины
        {
            arcs_entering_vertex[to].erase(it); //удаляем дугу
            break;
        }
    }
    if (erased)
    {
        return;
    }
    printf("Arc form %d to %d does not exist\n", from, to);
}

void Graph::invertArc(int from, int to)
{
    /*
        меняем направление дуги
	*/
    int length = getArc(from, to).length;
    deleteArc(from, to);
    addArc(to, from, length);
}

vector<int> Graph::maxDistances()
{
    /*
        Возвращает вектор максимальных расстояний от источника до каждой вершины.
	*/
    vector<int> distances;  //создание вектора расстояний для каждой вершины
    int n = vertices_number;

    distances.resize(n);

    deque<int> topo = topologicalSort();   //топологическая сортировка

    for (int j = 0; j < n; j++) //установка всех расстояний на минимум, который в нашем случае равен 0
    {
        distances[topo[j]] = 0;
    }

    for (int j = 1; j < n; j++) //для каждой вершины, кроме источника, расстояние которого в любом случае минимально
    {
        for (int i = 0; i < arcs_entering_vertex[topo[j]].size(); i++)  //для каждой вершины, которая соединена с нашей вершиной j (дуга должна входить в вершину j)
        {
            int a = distances[topo[j]];

            int b = distances[arcs_entering_vertex[topo[j]][i].vertex_id];

            Arc arc = getArc(arcs_entering_vertex[topo[j]][i].vertex_id, topo[j]);
            int c = arc.length;

            distances[topo[j]] = max(a, b + c); //расстояние максимально превышает текущее расстояние и расстояние до соседней вершины плюс длина дуги
        }
    }

    return distances;
}

deque<int> Graph::criticalPath(int sink)
{
    /*
        Возвращает критический путь от источника (0) до стока, который представляет собой самый длинный возможный путь прохождения через все вершины.
	*/
    vector<int> d = maxDistances();

    deque<int> path;       //критический путь
    path.push_front(sink); // перемещение последнего элемента на критический путь
    int v = sink;          // текущая вершина
    bool overflow = false;
    while (v != 0) // пока мы не достигнем источника
    {
        int u;
        for (int i = 0; i < arcs_entering_vertex[v].size(); i++)    //для каждой дуги, входящей в текущую вершину
        {
            if (d[v] == d[arcs_entering_vertex[v][i].vertex_id] + getArcLength(arcs_entering_vertex[v][i].vertex_id, v)) // найди вершину, для которой путь самый большой
            {
                u = arcs_entering_vertex[v][i].vertex_id; // поставь тебя в эту вершину
                break;
            }
        }
        path.push_front(u); // толкни тебя до кучи
        v = u;              // установить текущую вершину на u
        if (path.size() > 2000 && !overflow)
        {
            printf("Path size is too big!\n");
            overflow = true;
        }
    } // end

    return path;
}

void Graph::createAcyclicClique(vector<int> vertices, vector<int> lengths)
{
    /*
        Создает дизъюнктивные дуги между каждой парой вершин из «вершин».
        Дуги имеют длину = lengths[i], то есть длину дуги, выходящей из вершины i.
        Направление дуг — от вершины к соседним с ней вершинам, с которыми еще не связано.
        Это гарантирует, что мы получим ациклический граф.
	*/

    for (int i = 0; i < vertices.size(); i++) //для каждой вершины в вершинах
    {
        int from = vertices[i];
        for (int j = i + 1; j < vertices.size(); j++)   //для остальных вершин
        {
            addArc(from, vertices[j], lengths[i]);     //добавить дугу от текущей i-й вершины к остальным вершинам с длиной i-й задачи.
        }
    }
}

deque<int> Graph::topologicalSort() // result[i] = i-я вершина в топологическом расположении
{
    /*
        Возвращает топологический порядок графа (или пустую deque, если в графе есть цикл, чего быть не должно). 
        Мы используем наши векторы уже посещенных и посещенных в данный момент вершин.
	*/

    topological_order.clear(); //чистим deque

    dfs_already_visited.resize(vertices_number);
    dfs_currently_visited.resize(vertices_number);

    fill(dfs_already_visited.begin(), dfs_already_visited.end(), false);
    fill(dfs_currently_visited.begin(), dfs_currently_visited.end(), false);

    cycle_exists = false;

    for (int i = 0; i < vertices_number; i++) //для каждой вершины, которая еще не посещена
    {
        if (!dfs_already_visited[i])
        {
            dfsVisitTopo(i); //выполняем топологический поиск
        }
    }
    return (!cycle_exists) ? topological_order : deque<int>();  //если цикл не найден возвращаем topological_order
}

void Graph::dfsVisitTopo(int node)
{
    /*
        Часть алгоритма топологической сортировки. Мы принимаем меры предосторожности на случай возникновения цикла, 
        но этого не должно произойти, поскольку мы работаем над ориентированным ациклическим графом.
        Мы посещаем вершину и ее преемников (за исключением случаев, когда мы уже посетили ее), пока не найдем вершину, у которой нет преемников. 
        Затем мы добавляем его в deque topological_order.
	*/
    if (cycle_exists)
    {
        return;
    }
    if (dfs_currently_visited[node])
    {
        cycle_exists = true;
        return;
    }
    else if (!dfs_already_visited[node]) //если вершина еще не была посещена
    {
        dfs_currently_visited[node] = true;
        for (int i = 0; i < arcs_exiting_vertex[node].size(); i++) //для вершин-преемников
        {
            dfsVisitTopo(arcs_exiting_vertex[node][i].vertex_id); //выполняем топологический поиск
        }
        dfs_already_visited[node] = true;
        dfs_currently_visited[node] = false;
        topological_order.push_front(node); //после достижения вершины, у которой не осталось непосещенных преемников, добавляем вершину в deque topological_order
    }
}

bool Graph::getCycleExists(){
    return cycle_exists;
}