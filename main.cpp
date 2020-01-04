#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <sstream>
#include <ctime>


#define INFINITY 9999999

int maxNode = 0;
using namespace std;

vector<int> loadFile(const string &path) {
    vector<int> graph;
    ifstream file;
    file.open(path);
    if (!file.fail()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            for (int i; ss >> i;) {
                if (i + 1 > maxNode)
                    maxNode = i + 1;
                graph.push_back(i);
                if (ss.peek() == ',')
                    ss.ignore();
            }
        }
        file.close();
    }
    return graph;
}

int **formatGraph(vector<int> inData) {
    int **graph = 0;
    int currentIndex = 0;
    int srcNode = inData[0];
    int dstNode = inData[1];
    int cost = inData[2];
    graph = new int *[maxNode];
    for (int i = 0; i < maxNode; i++) {
        graph[i] = new int[maxNode];
        for (int j = 0; j < maxNode; j++) {
            if (srcNode == i && dstNode == j) {
                graph[i][j] = cost;
                currentIndex++;
                srcNode = inData[currentIndex * 3];
                dstNode = inData[currentIndex * 3 + 1];
                cost = inData[currentIndex * 3 + 2];
                printf("%d %d\n", i, j);
            } else
                graph[i][j] = 0;
        }
    }
    for (int i = 0; i < maxNode; i++) {
        for (int j = 0; j < maxNode; j++) {
            graph[j][i] = graph[i][j];
        }
    }
    return graph;
}

void dijkstra(int **G, int n, int startnode) {
    printf("\nStart node : %d \n", startnode);
    int cost[maxNode][maxNode], distance[maxNode], pred[maxNode];
    int visited[maxNode], count, mindistance, nextnode, i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (G[i][j] == 0)
                cost[i][j] = INFINITY;
            else
                cost[i][j] = G[i][j];
    for (i = 0; i < n; i++) {
        distance[i] = cost[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;
    count = 1;
    while (count < n - 1) {
        mindistance = INFINITY;
        for (i = 0; i < n; i++) {
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }
        }
        visited[nextnode] = 1;
        for (i = 0; i < n; i++) {
            if (!visited[i]) {
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
            }
        }
        count++;
    }
    for (i = 0; i < n; i++)
        if (i != startnode) {
            cout << "\nDistance of node " << i << "=" << distance[i];
            cout << "\nPath=" << i;
            j = i;
            do {
                j = pred[j];
                cout << "<-" << j;
            } while (j != startnode);
        }
}

void printMatrix(int **matrix) {
    for (int i = 0; i < maxNode; i++) {
        for (int j = 0; j < maxNode; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n ");
    }
}

int main() {
    vector<int> graph = loadFile("..\\data\\data.csv");
    int **graphConverted = formatGraph(graph);
    printMatrix(graphConverted);
    clock_t begin = clock();
    for (int i = 0; i < maxNode; i++)
        dijkstra(graphConverted, maxNode, i);
    clock_t end = clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
    printf("\nTime: %f", elapsed_secs);
    return 0;
}
