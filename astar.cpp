//サンプルコード　ここから色々変えていく

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>

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
void aStar(int grid[ROW][COL], int startR, int startC, int goalR, int goalC) {
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

            cout << "Found Path: ";
            for (const auto& p : path) {
                cout << "(" << p.first << "," << p.second << ") ";
            }
            cout << endl;
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

    cout << "Path not found!" << endl;
}

int main() {
    int grid[ROW][COL] = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    int startR = 0, startC = 0;
    int goalR = 4, goalC = 4;

    aStar(grid, startR, startC, goalR, goalC);

    return 0;
}

/*#include <iostream>
#include <vector>
#include <cmath>

struct Node {
    Node* parent;
    std::pair<int, int> position;
    int g, h, f;

    Node(Node* p, std::pair<int, int> pos) : parent(p), position(pos), g(0), h(0), f(0) {}

    bool operator==(const Node& other) const {
        return position == other.position;
    }
};

bool isInClosedList(const std::vector<Node*>& closed_list, const Node* child) {
    for (const Node* closed_child : closed_list) {
        if (*closed_child == *child) {
            return true;
        }
    }
    return false;
}

std::vector<std::pair<int, int>> astar(std::vector<std::vector<int>>& maze, std::pair<int, int> start, std::pair<int, int> end) {
    Node* start_node = new Node(nullptr, start);
    start_node->g = start_node->h = start_node->f = 0;

    Node* end_node = new Node(nullptr, end);
    end_node->g = end_node->h = end_node->f = 0;

    std::vector<Node*> open_list;
    std::vector<Node*> closed_list;
    open_list.push_back(start_node);

    while (!open_list.empty()) {
        Node* current_node = open_list[0];
        int current_index = 0;
        for (size_t i = 0; i < open_list.size(); ++i) {
            if (open_list[i]->f < current_node->f) {
                current_node = open_list[i];
                current_index = static_cast<int>(i);
            }
        }

        open_list.erase(open_list.begin() + current_index);
        closed_list.push_back(current_node);

        if (*current_node == *end_node) {
            std::vector<std::pair<int, int>> path;
            Node* current = current_node;
            while (current != nullptr) {
                path.push_back(current->position);
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        std::vector<Node*> children;
        std::vector<std::pair<int, int>> new_positions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
        for (const auto& new_position : new_positions) {
            std::pair<int, int> node_position = { current_node->position.first + new_position.first, current_node->position.second + new_position.second };

            if (node_position.first > static_cast<int>(maze.size()) - 1 || node_position.first < 0 || node_position.second > static_cast<int>(maze[0].size()) - 1 || node_position.second < 0) {
                continue;
            }

            if (maze[node_position.first][node_position.second] != 0) {
                continue;
            }

            Node* new_node = new Node(current_node, node_position);
            children.push_back(new_node);
        }

        for (Node* child : children) {
            if (isInClosedList(closed_list, child)) {
                continue;
            }

            child->g = current_node->g + 1;
            child->h = ((child->position.first - end_node->position.first) * (child->position.first - end_node->position.first)) + ((child->position.second - end_node->position.second) * (child->position.second - end_node->position.second));
            child->f = child->g + child->h;

            bool in_open_list = false;
            for (Node* open_node : open_list) {
                if (child->position == open_node->position && child->g > open_node->g) {
                    in_open_list = true;
                    break;
                }
            }

            if (!in_open_list) {
                open_list.push_back(child);
            }
        }
    }

    return std::vector<std::pair<int, int>>(); // No path found
}

void example() {
    std::vector<std::vector<int>> maze = {
        {0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0},
        {0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0}
    };

    std::pair<int, int> start = { 0, 0 };
    std::pair<int, int> end = { 4, 4 };

    std::vector<std::pair<int, int>> path = astar(maze, start, end);
    for (const auto& position : path) {
        std::cout << "(" << position.first << ", " << position.second << ") ";
    }
    std::cout << std::endl;
}

int main() {
    example();
    return 0;
}*/