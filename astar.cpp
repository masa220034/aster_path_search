#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

// グリッドのセルを表す構造体
struct Cell {
    int row, col;
    int f, g, h; // f = g + h
    Cell* parent;

    Cell(int r, int c, int gCost, int hCost, Cell* p = nullptr)
        : row(r), col(c), g(gCost), h(hCost), parent(p) {
        f = g + h;
    }
};

// グリッドのサイズと方向
const int ROW = 5;
const int COL = 5;
const int NUM_DIRECTIONS = 8;

const int dr[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
const int dc[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

// 通行可能なセルの割合を増やすための閾値
const double PASSABLE_THRESHOLD = 0.2;

// スタートとゴールの初期位置
const int INITIAL_START_ROW = 0;
const int INITIAL_START_COL = 0;
const int INITIAL_GOAL_ROW = ROW - 1;
const int INITIAL_GOAL_COL = COL - 1;

// ヒューリスティック関数（マンハッタン距離）
int heuristic(int r, int c, int goalR, int goalC) {
    return abs(goalR - r) + abs(goalC - c);
}

// A*アルゴリズム
void aStar(vector<vector<int>>& grid, int startR, int startC, int goalR, int goalC)
{
    vector<vector<bool>> closed(ROW, vector<bool>(COL, false));
    auto compare = [](Cell* a, Cell* b) { return a->f > b->f; };
    priority_queue<Cell*, vector<Cell*>, decltype(compare)> open(compare);

    Cell* start = new Cell(startR, startC, 0, heuristic(startR, startC, goalR, goalC));
    open.push(start);

    while (!open.empty()) 
    {
        Cell* current = open.top();
        open.pop();

        int r = current->row;
        int c = current->col;

        if (r == goalR && c == goalC) 
        {
            vector<pair<int, int>> path;
            while (current != nullptr) 
            {
                path.push_back({ current->row, current->col });
                current = current->parent;
            }
            reverse(path.begin(), path.end());

            // グリッド上で最短経路を表示
            for (int i = 0; i < ROW; ++i)
            {
                for (int j = 0; j < COL; ++j)
                {
                    if (i == startR && j == startC) 
                    {
                        cout << " S "; //スタート
                    }
                    else if (i == goalR && j == goalC)
                    {
                        cout << " G "; //ゴール
                    }
                    else if (grid[i][j] == 1)
                    {
                        cout << "■ "; //壁
                    }
                    else 
                    {
                        bool isPath = false;
                        for (const auto& p : path)
                        {
                            if (p.first == i && p.second == j)
                            {
                                cout << "○ "; //最短経路の道
                                isPath = true;
                                break;
                            }
                        }
                        if (!isPath) {
                            cout << "□ "; //通れる道
                        }
                    }
                }
                cout << endl;
            }
            return;
        }

        closed[r][c] = true;

        for (int i = 0; i < NUM_DIRECTIONS; ++i)
        {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < ROW && nc >= 0 && nc < COL && grid[nr][nc] == 0 && !closed[nr][nc]) 
            {
                int gNew = current->g + 1;
                int hNew = heuristic(nr, nc, goalR, goalC);
                int fNew = gNew + hNew;

                Cell* successor = new Cell(nr, nc, gNew, hNew, current);
                open.push(successor);
            }
        }
    }
}

// ランダムな迷路を生成する関数
void generateRandomMaze(vector<vector<int>>& grid, int rows, int cols) 
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);

    grid.clear();
    for (int i = 0; i < rows; ++i)
    {
        vector<int> row;
        for (int j = 0; j < cols; ++j) 
        {
            double val = dis(gen);
            if (val > PASSABLE_THRESHOLD)
            {
                row.push_back(0); // 通行可能
            }
            else 
            {
                row.push_back(1); // 通行不可能
            }
        }
        grid.push_back(row);
    }

    grid[INITIAL_START_ROW][INITIAL_START_COL] = 0; // スタート
    grid[rows - 1][cols - 1] = 0; // ゴール
}

int main() 
{
    vector<vector<int>> grid;

    generateRandomMaze(grid, ROW, COL);

    int startR = INITIAL_START_ROW;
    int startC = INITIAL_START_COL;
    int goalR = INITIAL_GOAL_ROW;
    int goalC = INITIAL_GOAL_COL;

    aStar(grid, startR, startC, goalR, goalC);

    return 0;
}