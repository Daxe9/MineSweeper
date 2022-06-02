#include <iostream>
#include <vector>
#include <random>

using std::vector;
using std::cout;
using std::cin;
using std::endl;

typedef struct Single_Cell {
    // if the count is -1 then it's a bomb
    int count;
    bool visible;
} Cell;

int direction[8][2] = {
        // top left corner
        {-1, -1},
        // top middle
        {-1, 0},
        // top right corner
        {-1, 1},
        // middle left
        {0,  -1},
        // middle right
        {0,  1},
        // bottom left corner
        {1,  -1},
        // bottom middle
        {1,  0},
        // bottom right corner
        {1,  1}};

void bombRandomizer(vector<vector<Cell>> &matrix, int discriminant) {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> randomWidth(0, (int) matrix.size() - 1);
    std::uniform_int_distribution<int> randomHeight(0, (int) matrix.at(0).size() - 1);
    int i = 0;
    int newWidth, newHeight;
    while (i < discriminant) {
        do {
            newWidth = randomWidth(mt);
            newHeight = randomHeight(mt);
        } while (matrix.at(newWidth).at(newHeight).count != 0);
        matrix.at(newWidth).at(newHeight).count = -1;
        ++i;
    }
}

int countBomb(vector<vector<Cell>> &matrix, int x, int y) {
    int counter = 0;

    for (auto [directionX, directionY]: direction) {
        try {
            if (matrix.at(x + directionX).at(y + directionY).count == -1) {
                counter++;
            } else {
                throw std::exception();
            }
        } catch (std::exception &e) {
            continue;
        }
    }

    return counter;
}

vector<vector<Cell>> createMatrix(int width, int height, int discriminant) {
    vector<vector<Cell>> matrix;
    // initialized the boarrd
    for (int i = 0; i < width; ++i) {
        vector<Cell> temp;
        for (int j = 0; j < height; ++j) {
            Cell newCell;
            newCell.count = 0;
            newCell.visible = false;
            temp.push_back(newCell);
        }
        matrix.push_back(temp);
    }

    // randomize bombs
    bombRandomizer(matrix, discriminant);

    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.at(i).size(); ++j) {
            if (matrix.at(i).at(j).count != -1) {
                matrix.at(i).at(j).count = countBomb(matrix, (int) i, (int) j);
            }
        }
    }
    return matrix;
}


void printMatrix(vector<vector<Cell>> &matrix, bool reveal) {
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix.at(i).size(); ++j) {
            if (!reveal) {
                if (matrix.at(i).at(j).visible) {
                    if (matrix.at(i).at(j).count == -1) {
                        cout << "X";
                    } else {
                        cout << matrix.at(i).at(j).count;
                    }
                } else {
                    cout << "O";
                }
            } else {
                if (matrix.at(i).at(j).count == -1) {
                    cout << "X";
                } else {
                    cout << matrix.at(i).at(j).count;
                }
            }
            cout << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

void controlNeighbours(vector<vector<Cell>> &matrix, int x, int y, int &neededCellToExplore) {
    matrix.at(x).at(y).visible = true;
    --neededCellToExplore;

    for (auto [directionX, directionY]: direction) {
        try {
            if (matrix.at(x + directionX).at(y + directionY).count == 0 &&
                !matrix.at(x + directionX).at(y + directionY).visible) {
                matrix.at(x + directionX).at(y + directionY).visible = true;

                controlNeighbours(matrix, x + directionX, y + directionY, neededCellToExplore);
            } else {
                throw std::exception();
            }
        } catch (std::exception &e) {
            continue;
        }
    }
}

int main() {
    int width = 5;
    int height = width;

    // bomb generator disciminant
    const int discriminant = width * height / 8;
    int neededCellToExplore = width * height - discriminant;

    // create a matrix
    vector<vector<Cell>> matrix = createMatrix(width, height, discriminant);

    // main loop
    int userX = 0, userY = 0;
    while (true) {
        if (neededCellToExplore == 0) {
            cout << "YOU WIN!!!\n";
            break;
        }
        printMatrix(matrix, false);
        do {
            cout << "Enter X coordinate: ";
            cin >> userY;
            cout << "Enter Y coordinate: ";
            cin >> userX;
        } while (userX >= width || userX < 0 || userY >= height || userY < 0);

        // process input
        if (matrix.at(userX).at(userY).count == -1) {
            cout << "THERE IS A BOMB AT (" << userY << ", " << userX << ")\nFINE\n";
            printMatrix(matrix, true);
            break;
        } else if (!matrix.at(userX).at(userY).visible) {
            cout << "THE NUMBER AT (" << userY << ", " << userX << ") is " << matrix.at(userX).at(userY).count << "\n";
            controlNeighbours(matrix, userX, userY, neededCellToExplore);
        } else {
            cout << "Already Explored, Try again!\n";
        }
    }

    return 0;
}

