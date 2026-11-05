#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <stdexcept>
#include <algorithm>

using namespace std;

/**
 * @class HexBoard
 * @brief Represents a Hex board of size N x N and manages the game state.
 *
 * Players: Blue (1) connects Top → Bottom; Red (2) connects Left → Right.
 * The board is stored as a 2D vector of colours (0 = empty, 1 = Blue, 2 = Red).
 * A graph (adjacency list) is built for each cell to perform BFS win detection.
 */
class HexBoard {
private:
    int N;                              ///< Board size (N x N)
    vector<vector<int>> board;          ///< board[r][c] = 0,1,2
    vector<vector<int>> adj;            ///< adjacency list for each node (r*N+c)
    int turn;                           ///< 1 = Blue, 2 = Red
    bool gameOver;
    int winner;

    /**
     * @brief Checks if the given cell is within the board.
     */
    bool inBounds(int r, int c) const {
        return r >= 0 && r < N && c >= 0 && c < N;
    }

    /**
     * @brief Builds the graph: each cell has up to 6 neighbours.
     *
     * For a hex grid with pointy-top orientation, neighbours are:
     *   (r-1,c),   (r-1,c+1),
     *   (r,c-1),   (r,c+1),
     *   (r+1,c-1), (r+1,c)
     */
    void buildGraph() {
        adj.assign(N*N, {});
        int dr[6] = {-1, -1, 0, 0, 1, 1};
        int dc[6] = {0, 1, -1, 1, -1, 0};
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                int id = r*N + c;
                for (int k = 0; k < 6; ++k) {
                    int nr = r + dr[k];
                    int nc = c + dc[k];
                    if (inBounds(nr, nc)) {
                        adj[id].push_back(nr*N + nc);
                    }
                }
            }
        }
    }

    /**
     * @brief BFS to check if the current player has connected their two sides.
     * @param player 1=Blue (Top→Bottom), 2=Red (Left→Right)
     * @return true if connected, false otherwise.
     */
    bool checkWin(int player) {
        vector<bool> visited(N*N, false);
        queue<int> q;

        // Seed the queue with all cells on the starting side
        if (player == 1) {  // Blue: top row
            for (int c = 0; c < N; ++c) {
                int id = 0*N + c;   // row 0
                if (board[0][c] == player) {
                    visited[id] = true;
                    q.push(id);
                }
            }
        } else {  // Red: left column
            for (int r = 0; r < N; ++r) {
                int id = r*N + 0;   // column 0
                if (board[r][0] == player) {
                    visited[id] = true;
                    q.push(id);
                }
            }
        }

        while (!q.empty()) {
            int id = q.front(); q.pop();
            int r = id / N;
            int c = id % N;

            // Check if we've reached the opposite side
            if (player == 1 && r == N-1) return true;
            if (player == 2 && c == N-1) return true;

            for (int nb : adj[id]) {
                int nr = nb / N;
                int nc = nb % N;
                if (!visited[nb] && board[nr][nc] == player) {
                    visited[nb] = true;
                    q.push(nb);
                }
            }
        }
        return false;
    }

public:
    /**
     * @brief Constructs a board of size N x N.
     */
    HexBoard(int size) : N(size), board(N, vector<int>(N, 0)), turn(1), gameOver(false), winner(0) {
        buildGraph();
    }

    /**
     * @brief Places a stone for the current player at (r, c) if legal.
     * @return true if move was made, false if illegal (cell occupied or game over).
     */
    bool placeMove(int r, int c) {
        if (gameOver) {
            cout << "Game is already over.\n";
            return false;
        }
        if (!inBounds(r, c)) {
            cout << "Coordinates out of bounds.\n";
            return false;
        }
        if (board[r][c] != 0) {
            cout << "Cell already occupied.\n";
            return false;
        }

        board[r][c] = turn;
        if (checkWin(turn)) {
            gameOver = true;
            winner = turn;
        } else {
            // Switch turn
            turn = (turn == 1) ? 2 : 1;
        }
        return true;
    }

    /**
     * @brief Displays the board with ASCII symbols.
     *   . = empty, B = Blue, R = Red.
     * Uses a hex-like offset for odd rows.
     */
    void display() const {
        cout << "\n  ";
        for (int c = 0; c < N; ++c)
            cout << " " << c << " ";
        cout << "\n";

        for (int r = 0; r < N; ++r) {
            // Indentation for odd rows (hex offset)
            if (r % 2 == 1) cout << "  ";
            cout << r << " ";
            for (int c = 0; c < N; ++c) {
                char ch = '.';
                if (board[r][c] == 1) ch = 'B';
                else if (board[r][c] == 2) ch = 'R';
                cout << ch << "  ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    /**
     * @brief Returns whether the game is over.
     */
    bool isGameOver() const { return gameOver; }

    /**
     * @brief Returns the winner (1=Blue, 2=Red) or 0 if none.
     */
    int getWinner() const { return winner; }

    /**
     * @brief Returns the current player's turn (1=Blue, 2=Red).
     */
    int getTurn() const { return turn; }
};

// ----------------------------------------------------------------------------
//  Main program
// ----------------------------------------------------------------------------

int main() {
    int size;
    cout << "Enter board size (e.g., 7 or 11): ";
    cin >> size;
    if (size < 2) size = 7;

    HexBoard game(size);

    cout << "\n--- HEX GAME ---\n";
    cout << "Blue (B) connects Top → Bottom.\n";
    cout << "Red  (R) connects Left → Right.\n";
    cout << "Coordinates are (row, column) starting from 0.\n";
    cout << "Enter your move as two integers: row col\n\n";

    while (!game.isGameOver()) {
        game.display();

        string playerName = (game.getTurn() == 1) ? "Blue (B)" : "Red (R)";
        cout << playerName << "'s turn.\n";

        int r, c;
        cout << "Enter row and column: ";
        cin >> r >> c;

        if (!game.placeMove(r, c)) {
            cout << "Illegal move. Try again.\n";
            continue;
        }
    }

    // Game over
    game.display();
    int w = game.getWinner();
    if (w == 1) cout << "Blue (B) wins! Top–Bottom connected.\n";
    else if (w == 2) cout << "Red (R) wins! Left–Right connected.\n";
    else cout << "Unexpected end.\n";

    return 0;
}