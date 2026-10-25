#include"pa2.cpp"
using namespace std;

int main()
{
    int resourceMap[MAX_MAP_SIZE][MAX_MAP_SIZE] = {};
    int rows = 0; int cols = 0;
    readMapFromFile("map02", resourceMap, rows, cols);
    int startR = 0, startC = 0;
    int targetR = 7, targetC = 9;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == startR && j == startC) {
                cout << " S ";
            }
            else if (i == targetR && j == targetC) {
                cout << " D ";
            }
            else if (resourceMap[i][j] == WALL_CELL) {
                cout << " # ";
            }
            else if (resourceMap[i][j] < 0) {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
            else {
                cout << setw(2) << resourceMap[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << rows << "\t" << cols << endl;
    bool visited[MAX_MAP_SIZE][MAX_MAP_SIZE] = {false};
    int bestPath[MAX_PATH_LENGTH][2];
    int bestPathLength = 0;
    int currentPath[MAX_PATH_LENGTH][2];
    int currentPathLength = 0;
    int timeLeft = 25; int unlockThreshold = 10;
    rescueSurvivorsWithPenalty(resourceMap, rows, cols, targetR, targetC,
                               startR, startC, visited,
                               currentPath, currentPathLength,
                               timeLeft,
                               bestPath, bestPathLength,
                               0,
                               unlockThreshold,
                               bestPathLength);
    printPath(bestPath, bestPathLength);
    cout << bestPathLength << endl;
}