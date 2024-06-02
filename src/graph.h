#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <deque>
#include <algorithm>
#include <cstdio>

using namespace std;

typedef struct Arc
{				   // дуговая структура
	int vertex_id; // номер предшественника или преемника
	int length;	// длина дуги
} Arc;

typedef vector<Arc> Vertex;		 // Элемент вектора вершин. Он содержит входящие или выходящие дуги.
typedef vector<Vertex> Vertices; // Вектор предшественников или преемников

class Graph
{
  public:
	Graph();
	Graph(int n); // n = количество вершин
	~Graph();

	void addArc(int from, int to, int length);		   // Добавление дуги из вершины «от» в вершину «до» определенной длины
	void setArcLength(int from, int to, int length); // Устанавливает разную длину дуги от вершины «от» до другой вершины «до».
	Arc getArc(int from, int to);					   // Возвращает дугу от вершины «от» до вершины «до»
	int getArcLength(int from, int to);			   // Возвращает длину дуги от вершины «форма» до вершины «до».
	void deleteArc(int from, int to);				   // Удаляет дугу от вершины «от» до вершины «до».
	void invertArc(int from, int to);				   // Инвертирует направление дуги
	bool getCycleExists(); 

	void createAcyclicClique(vector<int> vertices, vector<int> lengths); // Создает дизъюнктивные дуги между каждой парой вершин из «вершин».

	vector<int> maxDistances();			// Возвращает вектор максимальных расстояний от источника до каждой вершины.
	deque<int> criticalPath(int sink); // Возвращает критический путь от источника к приемнику, который является просто самым длинным из возможных путей.


  private:
	int vertices_number;	// Количество вершин в графе
	Vertices arcs_exiting_vertex; // Список преемников, необходимых для DFS
	Vertices arcs_entering_vertex; // Список предшественников, необходимых для поиска самого длинного пути

	vector<bool> dfs_already_visited; // Содержит информацию о том, посещалась ли вершина во время DFS.
	vector<bool> dfs_currently_visited;		  // Содержит информацию о том, посещает ли вершину DFS в данный момент.
	bool cycle_exists;				  // Информация о том, обнаружен ли цикл во время DFS

	deque<int> topological_order; // Топологический порядок, установленный в методе topologicalSort().

	deque<int> topologicalSort(); // Возвращает топологический порядок вершин графа
	void dfsVisitTopo(int node); // Вызывает процедуру посещения DFS для топологической сортировки
};

#endif