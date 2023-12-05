#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;

// �O���b�h�̃Z����\���\����
struct Cell {
    int row, col;
    int f, g, h; // f = g + h
    Cell* parent;

    Cell(int r, int c, int gCost, int hCost, Cell* p = nullptr)
        : row(r), col(c), g(gCost), h(hCost), parent(p) {
        f = g + h;
    }
};

// �O���b�h�̃T�C�Y
const int ROW = 5;
const int COL = 5;

// �O���b�h�̈ړ������i�㉺���E����ю΂߁j
const int dr[] = { -1, 1, 0, 0, -1, -1, 1, 1 };
const int dc[] = { 0, 0, -1, 1, -1, 1, -1, 1 };

// �q���[���X�e�B�b�N�֐��i�}���n�b�^�������j
int heuristic(int r, int c, int goalR, int goalC) {
    return abs(goalR - r) + abs(goalC - c);
}

// A*�A���S���Y��
void aStar(vector<vector<int>>& grid, int startR, int startC, int goalR, int goalC) {
    // Open���X�g��Closed���X�g��������
    vector<vector<bool>> closed(ROW, vector<bool>(COL, false));
    auto compare = [](Cell* a, Cell* b) { return a->f > b->f; };
    priority_queue<Cell*, vector<Cell*>, decltype(compare)> open(compare);

    // �X�^�[�g�Z�����쐬����Open���X�g�ɒǉ�
    Cell* start = new Cell(startR, startC, 0, heuristic(startR, startC, goalR, goalC));
    open.push(start);

    while (!open.empty()) {
        // Open���X�g����ŏ���f�l�����Z�����擾
        Cell* current = open.top();
        open.pop();

        int r = current->row;
        int c = current->col;

        // �S�[���ɓ��B������o�H���o�͂��ďI��
        if (r == goalR && c == goalC) {
            vector<pair<int, int>> path;
            while (current != nullptr) {
                path.push_back({ current->row, current->col });
                current = current->parent;
            }
            reverse(path.begin(), path.end());

            // �O���b�h��ōŒZ�o�H��\��
            for (int i = 0; i < ROW; ++i) {
                for (int j = 0; j < COL; ++j) {
                    if (i == startR && j == startC) {
                        cout << " S "; //�X�^�[�g
                    }
                    else if (i == goalR && j == goalC) {
                        cout << " G "; //�S�[��
                    }
                    else if (grid[i][j] == 1) {
                        cout << "�� "; //��
                    }
                    else {
                        bool isPath = false;
                        for (const auto& p : path) {
                            if (p.first == i && p.second == j) {
                                cout << "�� "; //�ŒZ�o�H�̓�
                                isPath = true;
                                break;
                            }
                        }
                        if (!isPath) {
                            cout << "�� "; //�ʂ�铹
                        }
                    }
                }
                cout << endl;
            }

            return;
        }

        closed[r][c] = true;

        // �O���b�h��̗אڃZ����T��
        for (int i = 0; i < 8; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // �O���b�h�͈͓�����Q���łȂ��ꍇ
            if (nr >= 0 && nr < ROW && nc >= 0 && nc < COL && grid[nr][nc] == 0 && !closed[nr][nc]) {
                int gNew = current->g + 1;
                int hNew = heuristic(nr, nc, goalR, goalC);
                int fNew = gNew + hNew;

                // �V�����Z�����쐬����Open���X�g�ɒǉ�
                Cell* successor = new Cell(nr, nc, gNew, hNew, current);
                open.push(successor);
            }
        }
    }

    // �X�^�[�g�ƃS�[���̈ʒu�������_���ɐ����������H��Ŏ擾
    startR = rand() % ROW;
    startC = rand() % COL;

    do {
        goalR = rand() % ROW;
        goalC = rand() % COL;
    } while ((abs(startR - goalR) + abs(startC - goalC)) < ROW / 2);
    
    aStar(grid, startR, startC, goalR, goalC);
}

// �����_���Ȗ��H�𐶐�����֐�
void generateRandomMaze(vector<vector<int>>& grid, int rows, int cols) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0.0, 1.0);

    grid.clear();
    for (int i = 0; i < rows; ++i) {
        vector<int> row;
        for (int j = 0; j < cols; ++j) {
            double val = dis(gen);
            // �ʍs�\�ȃZ���̊����𑝂₷
            if (val > 0.2) { // �ʍs�\�ȃZ���̊����𒲐����邽�߂̒l�i0.2�j��ύX
                row.push_back(0); // �ʍs�\
            }
            else {
                row.push_back(1); // �ʍs�s�\
            }
        }
        grid.push_back(row);
    }

    // �X�^�[�g�ƃS�[����ʍs�\�ȃZ���ɐݒ肷��
    grid[0][0] = 0; // �X�^�[�g
    grid[rows - 1][cols - 1] = 0; // �S�[��
}

int main() {
    // �����_���Ȗ��H�𐶐����A���̖��H���g�p����
    const int ROW = 5;
    const int COL = 5;
    vector<vector<int>> grid;

    generateRandomMaze(grid, ROW, COL); // �����_���Ȗ��H�𐶐�


    int startR = 0, startC = 0;
    int goalR = ROW - 1, goalC = COL - 1;


    aStar(grid, startR, startC, goalR, goalC);

    return 0;
}