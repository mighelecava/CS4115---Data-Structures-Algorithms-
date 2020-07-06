/**
 * @authors:  Sean Lynch (18245137),
 *       Nicole Berty (18246702),
 *       Michele Cavaliere (18219365),
 *       Matt Lucey (18247083)
**/
 
 
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>
#include <list>
#include <vector>
#include <math.h>
#include <algorithm>
#include <cstdio>
#include <utility>
#include <iomanip>

using std::cin;
using std::unordered_map;
using std::ifstream;
using std::string;
using std::endl;
using std::cout;
using std::list;
using std::vector;
using std::find;
using std::iterator;
 
 
int numOfNodes;
vector< list<int> > vertices;
int numOfEdges;
string basename;
unordered_map< int, vector<int> > coordinates;
unordered_map< int, int> pre;
 
bool contains(list<int> v, int t)
{
    auto it = find(v.begin(), v.end(), t);
    if(it != v.end())return true;
    return false;
}
 
int at(list<int> _list, int _i){
    list<int>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}
 
void displayGraph(vector< list<int> > vertices) {
    for (int i = 0; i < vertices.size(); i++)
    {
        for (int j = 0; j < vertices[i].size(); j++)
        {
            list<int>::iterator it = vertices[i].begin();
            std::advance(it, j);
            cout << *it;
        }
        cout << endl;
    }
}
 
void readInData() {
    string graphname, xyzname;
    cin >> basename;
 
    graphname = xyzname = basename;
    graphname += ".osm.graph";
 
    ifstream file(graphname);
    string line;
    while (getline(file, line) && line[0] == '%');  // sin Ã©
    std::istringstream lstream(line);
    //nodes = n, edges = m
    int n, m;
    lstream >> n >> m;
    numOfEdges = m;
    numOfNodes = n;
 
   
    list<int> vertex;
    int num;
    for (int i = 0; i < n; i++) {
        getline(file,line);
        int number_of_spaces = 0;
        for (auto& iter : line)
        {
            if (iter == ' ')
            {
                number_of_spaces++;
            }
        }
        std::istringstream iss(line);
        if (line.size() > 0)
        {
            for (int i = 0; i < number_of_spaces + 1; i++) {
                iss >> num;
                vertex.push_back(num);
            }
        }
        number_of_spaces = 0;
        vertices.push_back(vertex);
        vertex.clear();
    }
}
 
bool vectorContains(vector<int> vec, int node) {
    for (int i = 0; i < vec.size(); i++)
    {
        if(vec.at(i) == node) {
            return true;
        }
    }
    return false;
}
 
void Query5(int node, int k) {
    /*
    *loop then the nodes neighbours added to the working list
    *one element is chosen from the working list and that forms the current list
    * the current list elements are added to the working list while that node is deleted from the working list
    * nodes are added to the list of neighbours from current
    */
    vector<int> neighbours;
    list<int> list_of_neighbours;
    list<int> workinglist;
    list<int> templist;
    list<int> currentlist;
    list<int>::iterator it;
    list<int>::iterator it2;
    workinglist = vertices[node - 1];
    for (int i = 0; i < k; i++)
    {
        for (int j = 0; j < workinglist.size(); j++)
        {
            it = workinglist.begin();
            std::advance(it, j);
            currentlist = vertices[*it - 1];
            for (int x = 0; x < currentlist.size(); x++)
            {
                it2 = currentlist.begin();
                std::advance(it2, x);
                templist.push_back(*it2);
                if (i == k - 2)
                {
                    list_of_neighbours.push_back(*it2);
                }
            }
            currentlist.clear();
        }
        workinglist = templist;
        templist.clear();
    }
    for (int j = 0; j < list_of_neighbours.size(); j++)
    {
        it = list_of_neighbours.begin();
        std::advance(it, j);
        if (!vectorContains(neighbours, *it))
        {
            neighbours.push_back(*it);
        }
    }
    sort(neighbours.begin(), neighbours.end());
    cout << "N(" << node << "," << k << ")=";
    for (int i = 0; i < neighbours.size(); i++)
    {
        cout << " " <<neighbours.at(i);
    }
    cout << "." << endl;
}
 
void readInCoordinates() {
    string xyzname;
    xyzname = basename;
    xyzname +=".osm.xyz";
    ifstream file(xyzname);
    string line = " ";
    int n = 0;
    int num;
    vector<int> vertex;
    for (int i  = 0; i < numOfNodes; i++)
    {
        getline(file,line);
        std::istringstream iss(line);
        for (int i = 0; i < 3; i++) {
            iss >> num;
            vertex.push_back(num);
        }
        coordinates.insert(std::make_pair(n, vertex));
        vertex.clear();
        n++;
    }
}
 
void Query7BFS(int startVertex, int endVertex) {
     bool *visited = new bool[numOfNodes];
    for(int i = 0; i < numOfNodes; i++)
        visited[i] = false;
 
    // Create a queue for BFS
    list<int> queue;
 
    // Mark the current node as visited and enqueue it
    visited[startVertex] = true;
    queue.push_back(startVertex);
 
    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;
    bool found = false;
    int path = 0;
 
    while(!queue.empty() && !found)
    {
        // Dequeue a vertex from queue and print it
        startVertex = queue.front();
        queue.pop_front();
 
        for (i = vertices[startVertex - 1].begin(); i != vertices[startVertex - 1].end(); ++i)
        {
            if (!visited[*i])
            {
                pre.insert(std::make_pair(*i, startVertex));
                visited[*i] = true;
                queue.push_back(*i);
            }
            if (*i == endVertex)
            {
                found = true;
            }
        }
    }
}
 
double calculateDistance(int node1, int node2) {
    vector<int> node1XY = coordinates.at(node1);
    vector<int> node2XY = coordinates.at(node2);
    double distance = sqrt(pow(node1XY.at(0)  - node2XY.at(0), 2) + pow(node1XY.at(1) - node2XY.at(1), 2));
    return distance;
}
 
int minDistance(vector<double> dist, vector<bool> sptSet)
{
    double min = 10000000, min_index;
 
    for (int v = 0; v < numOfNodes; v++) {
        if (sptSet.at(v) == false && dist.at(v) < min) {
            min = dist.at(v), min_index = v;
        }
    }
    return min_index;
}
 
//xyz are in coordinates
//vertexs are in vertices
double ShortestPath(int startNode, int endNode) {
    startNode--, endNode--;
    vector<double> dist;
    vector<bool> sptSet;
 
    for (int i = 0; i < numOfNodes; i++)
    {
        dist.push_back(10000000);
        sptSet.push_back(false);
    }
 
    dist.at(startNode) = 0;
    for (int i = 0; i < numOfNodes - 1; i++)
    {
        int u = minDistance(dist, sptSet);
        for (int j = 0; j < vertices[u].size(); j++)
        {
            list<int>::iterator it = vertices[u].begin();
            advance(it, j);
            if (dist.at(u) != 10000000 && !sptSet.at(u) && dist.at(u) + calculateDistance(u, *it - 1) < dist.at(*it - 1))
            {
                dist.at(*it - 1) = dist.at(u) + calculateDistance(u, *it - 1);
            }
            /*if(u == endNode && sptSet.at(u) == true) {
                return dist.at(endNode);
            }*/
        }
        sptSet.at(u) = true;
    }
    return dist.at(endNode);
}
 
int main(int argc, char *argv[])
{
    readInData();
    readInCoordinates();
    int x = 8;
    int largest = 0;
    double average = 0;
    int nodeNum = 0;
    list<int>::iterator it;
    int source, end;
 
    std::cout << std::fixed;
    std::cout << std::setprecision(6);

    float temp;
 
    cin >> x;
    switch (x)
    {
    case 1:
        cout << "n= " << numOfNodes << "; m= " << numOfEdges << "." << endl;
        break;
    case 2:
        largest = 0;
        for (int i = 0; i < vertices.size(); i++)
        {
            if (vertices[largest].size() < vertices[i].size())
            {
                largest = i;
            }
        }
        cout << "v= " << largest+1 << "; |N(v)|= " << vertices[largest].size() << "." << endl;
        break;
    case 3:
        average = 0;
        for (int i = 0; i < vertices.size(); i++)
        {
            average += vertices[i].size();
        }
        average /= vertices.size();
        cout << "avg |N(v)|= " << average << "." << endl;
        break;
    case 4:
        cin >> nodeNum;
        cout << "N(" << nodeNum << ")=";
        for (int i : vertices[nodeNum-1])
        {
            cout << " " << i;
        }
        cout << "." << endl;
        break;
    case 5:
        cin >> largest >> average;
        Query5(largest, average);
        break;
    case 6:
        cin >> source >> end;
        temp = ShortestPath(source, end);
        cout << "Path: " << source << " - " << end << " // optional output" << endl;
        cout << "d(" << source << "," << end << ")=  " << temp << "." << endl;
        break;
    case 7:
        cin >> source >> end;
        Query7BFS(source, end);
        vector<int> path;
        int crawl = end;
        int num = 0;
        path.push_back(crawl);
        while (crawl != source)
        {
            path.push_back(pre.at(crawl));
            crawl = pre.at(crawl);
            num++;
        }
        cout << "Path: ";
        for (int i = path.size() - 1; i >= 0; i--) {
            if(i == 0) {
                cout << path[i] << " // optional output" << endl;
            } else {
                cout << path[i] << " - ";
            }
        }
        cout << "ed(" << source << "," << end << ")= " << num << ".";
        cout << endl;
        break;
    }
    return 0;
}