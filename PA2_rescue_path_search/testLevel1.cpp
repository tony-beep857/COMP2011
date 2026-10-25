#include "pa2.cpp"
int main()
{
    int map[MAX_MAP_SIZE][MAX_MAP_SIZE];
    int rows, cols, startR = 0, startC =0, targetR = 5, targetC = 5;

    readMapFromFile("map01", map, rows, cols);
    cout << rows << "\t" << cols << endl;
    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int path[MAX_PATH_LENGTH][2];
    int pathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];
    int currentPathLength = 0;

    bool found = shortestPath(map, rows, cols, path, pathLength, targetR, targetC,
                                startR, startC, visited,
                                currentPath, currentPathLength);
    cout << found << endl;
    printPath(path, pathLength, 0);

}