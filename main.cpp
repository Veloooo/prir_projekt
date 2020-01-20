#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>
#include <mpi.h>
#include <omp.h>

#define INFINITY2 9999999

int maxNode = 0;
using namespace std;

vector<int> loadFile(const string path) {
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
                if (ss.peek() == ';')
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
                //printf("%d %d\n", i, j);
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
    printf("%d\n", startnode);
    int **cost = new int *[maxNode];
    int distance[maxNode], pred[maxNode];
    int visited[maxNode], count, mindistance, nextnode, i, j;
    for (i = 0; i < n; i++) {
        cost[i] = new int[maxNode];
        for (j = 0; j < n; j++)
            cost[i][j] = 0;
    }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            if (G[i][j] == 0)
                cost[i][j] = INFINITY2;
            else
                cost[i][j] = G[i][j];
    for (i = 0; i < n; i++) {
        distance[i] = cost[startnode][i];
        pred[i] = startnode;
        visited[i] = 0;
    }
    distance[startnode] = 0;
    visited[startnode] = 1;

    #pragma omp parallel for schedule(runtime) num_threads(1) shared(distance,visited,pred)
    for(int count = 1;count < n - 1; count++){
		mindistance = INFINITY2;
        for (int i = 0; i < n; i++) {
            if (distance[i] < mindistance && !visited[i]) {
                mindistance = distance[i];
                nextnode = i;
            }
        }
        visited[nextnode] = 1;
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                if (mindistance + cost[nextnode][i] < distance[i]) {
                    distance[i] = mindistance + cost[nextnode][i];
                    pred[i] = nextnode;
                }
            }
        }
    }
    for (i = 0 ; i < maxNode; i++)
        delete[] cost[i];

    for (i = 0; i < n; i++)
        if (i != startnode) {
            j = i;
            do {
                j = pred[j];
            } while (j != startnode);
        }
}

int main(int argc, char **argv) {
    printf("starting...");
    int MPI_rank,MPI_size;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &MPI_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &MPI_size);
    vector<int> graph = loadFile("newFile.csv");
    int **graphConverted = formatGraph(graph);
    int i=0;
    MPI_Barrier(MPI_COMM_WORLD);
    double begin = MPI_Wtime();
    for (i = MPI_rank; i < maxNode; i+=MPI_size)
        dijkstra(graphConverted, maxNode, i);
    double end = MPI_Wtime();
    double elapsed_secs = (end - begin);
    printf("\nTime: %f", elapsed_secs);
    MPI_Finalize();
    return 0;
}
