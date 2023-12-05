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

// グリッドのサイズ
const int ROW = 5;
const int COL = 5;

// グリッドの移動方向（上下左右および斜め）
const int dr[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
const int dc[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

// ヒューリスティック関数（マンハッタン距離）
int heuristic(int r, int c, int goalR, int goalC) {
    return abs(goalR - r) + abs(goalC - c);
}

// A*アルゴリズム
void aStar(vector<vector<int>>& grid, int startR, int startC, int goalR, int goalC) {
    // OpenリストとClosedリストを初期化
    vector<vector<bool>> closed(ROW, vector<bool>(COL, false));
    auto compare = [](Cell* a, Cell* b) { return a->f > b->f; };
    priority_queue<Cell*, vector<Cell*>, decltype(compare)> open(compare);

    // スタートセルを作成してOpenリストに追加
    Cell* start = new Cell(startR, startC, 0, heuristic(startR, startC, goalR, goalC));
    open.push(start);

    while (!open.empty()) {
        // Openリストから最小のf値を持つセルを取得
        Cell* current = open.top();
        open.pop();

        int r = current->row;
        int c = current->col;

        // ゴールに到達したら経路を出力して終了
        if (r == goalR && c == goalC) {
            vector<pair<int, int>> path;
            while (current != nullptr) {
                path.push_back({ current->row, current->col });
                current = current->parent;
            }
            reverse(path.begin(), path.end());

            // グリッド上で最短経路を表示
            for (int i = 0; i < ROW; ++i) {
                for (int j = 0; j < COL; ++j) {
                    if (i == startR && j == startC) {
                        cout << " S "; //スタート
                    }
                    else if (i == goalR && j == goalC) {
                        cout << " G "; //ゴール
                    }
                    else if (grid[i][j] == 1) {
                        cout << "■ "; //壁
                    }
                    else {
                        bool isPath = false;
                        for (const auto& p : path) {
                            if (p.first == i && p.second == j) {
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

        // グリッド上の隣接セルを探索
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // グリッド範囲内かつ障害物でない場合
            if (nr >= 0 && nr < ROW && nc >= 0 && nc < COL && grid[nr][nc] == 0 && !closed[nr][nc]) {
                int gNew = current->g + 1;
                int hNew = heuristic(nr, nc, goalR, goalC);
                int fNew = gNew + hNew;

                // 新しいセルを作成してOpenリストに追加
                Cell* successor = new Cell(nr, nc, gNew, hNew, current);
                open.push(successor);
            }
        }
    }

    // スタートとゴールの位置をランダムに生成した迷路上で取得
    startR = rand() % ROW;
    startC = rand() % COL;

    do {
        goalR = rand() % ROW;
        goalC = rand() % COL;
    } while ((abs(startR - goalR) + abs(startC - goalC)) < ROW / 2);
    
    aStar(grid, startR, startC, goalR, goalC);
}

// ランダムな迷路を生成する関数
void generateRandomMaze(vector<vector<int>>& grid, int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0.0, 1.0);

    grid.clear();
    for (int i = 0; i < rows; ++i) {
        vector<int> row;
        for (int j = 0; j < cols; ++j) {
            double val = dis(gen);
            // 通行可能なセルの割合を増やす
            if (val > 0.2) { // 通行可能なセルの割合を調整するための値（0.2）を変更
                row.push_back(0); // 通行可能
            }
            else {
                row.push_back(1); // 通行不可能
            }
        }
        grid.push_back(row);
    }

    // スタートとゴールを通行可能なセルに設定する
    grid[0][0] = 0; // スタート
    grid[rows - 1][cols - 1] = 0; // ゴール
}

int main() {
    // ランダムな迷路を生成し、その迷路を使用する
    const int ROW = 5;
    const int COL = 5;
    vector<vector<int>> grid;

    generateRandomMaze(grid, ROW, COL); // ランダムな迷路を生成


    int startR = 0, startC = 0;
    int goalR = ROW - 1, goalC = COL - 1;


    aStar(grid, startR, startC, goalR, goalC);

    return 0;
}