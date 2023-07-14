#include <iostream>
#include <vector>
#include <ctime>

using namespace std;

// Define the direction array, used to judge the position of the surrounding grid
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// gamestate
enum GameState {
    PLAYING,
    WON,
    LOST
};

class Minesweeper {
private:
    int rows;
    int cols;
    int numMines;
    vector<vector<char>> board;
    vector<vector<bool>> revealed;
    vector<vector<bool>> marked;
    GameState state;

public:
    Minesweeper(int numRows, int numCols, int numMines) : rows(numRows), cols(numCols), numMines(numMines) {
        board.resize(rows, vector<char>(cols, '0'));
        revealed.resize(rows, vector<bool>(cols, false));
        marked.resize(rows, vector<bool>(cols, false));
        state = GameState::PLAYING;
    }

    void placeMines() {
        int count = 0;
        while (count < numMines) {
            int x = rand() % rows;
            int y = rand() % cols;
            if (board[x][y] != '*') {
                board[x][y] = '*';
                count++;
            }
        }
    }

    void calculateNumbers() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] != '*') {
                    int count = 0;
                    for (int k = 0; k < 8; k++) {
                        int x = i + dx[k];
                        int y = j + dy[k];
                        if (isValid(x, y) && board[x][y] == '*') {
                            count++;
                        }
                    }
                    board[i][j] = count + '0';
                }
            }
        }
    }

    void revealCell(int row, int col) {
        if (!isValid(row, col) || revealed[row][col] || marked[row][col]) {
            return;
        }

        if (board[row][col] == '*') {
            state = GameState::LOST;
            return;
        }

        revealed[row][col] = true;

        if (board[row][col] == '0') {
            for (int k = 0; k < 8; k++) {
                int x = row + dx[k];
                int y = col + dy[k];
                revealCell(x, y);
            }
        }

        if (checkWin()) {
            state = GameState::WON;
        }
    }

    void toggleMark(int row, int col) {
        if (isValid(row, col)) {
            marked[row][col] = !marked[row][col];
        }
    }

    bool checkWin() {
        int count = 0;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (board[i][j] != '*' && revealed[i][j]) {
                    count++;
                }
            }
        }
        return count == rows * cols - numMines;
    }

    void displayBoard() {
        cout << "  ";
        for (int j = 0; j < cols; j++) {
            cout << j << " ";
        }
        cout << endl;
        for (int i = 0; i < rows; i++) {
            cout << i << " ";
            for (int j = 0; j < cols; j++) {
                if (revealed[i][j]) {
                    cout << board[i][j] << " ";
                } else if (marked[i][j]) {
                    cout << "M ";
                } else {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }

    bool isValid(int row, int col) {
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    GameState getState() const {
        return state;
    }
};

int main() {
    srand(time(nullptr));

    Minesweeper game(9, 9, 10);
    game.placeMines();
    game.calculateNumbers();

    while (game.getState() == GameState::PLAYING) {
        game.displayBoard();

        int row, col;
        cout << "Enter row and column (separated by space): ";
        cin >> row >> col;

        game.revealCell(row, col);
        game.toggleMark(row, col);
    }

    game.displayBoard();

    if (game.getState() == GameState::WON) {
        cout << "Congratulations! You won the game." << endl;
    } else {
        cout << "Game over! You lost." << endl;
    }

    return 0;
}
