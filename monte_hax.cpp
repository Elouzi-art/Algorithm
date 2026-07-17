/**
 * @file hex_ai.cpp
 * @brief Hex game with Monte Carlo AI (Homework 5)
 *
 * Compile: g++ -std=c++11 -O2 -o hex_ai hex_ai.cpp
 * Run:     ./hex_ai
 */

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;

// ------------------------------------------------------------------------
//  HexBoard class – manages the game state and win detection
// ------------------------------------------------------------------------

class HexBoard {
private:
    int N;                          // board size (N x N)
    vector<int> board;              // flat array: board[r*N+c] = 0,1,2
    vector<vector<int>> adj;        // adjacency list for each cell
    int turn;                       // 1=Blue, 2=Red
    bool gameOver;
    int winner;

    // Neighbour offsets for a pointy‑top hex grid
    const int dr[6] = {-1, -1, 0, 0, 1, 1};
    const int dc[6] = {0, 1, -1, 1, -1, 0};

    bool inBounds(int r, int c) const {
        return r >= 0 && r < N && c >= 0 && c < N;
    }

    void buildGraph() {
        adj.assign(N*N, {});
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

    // BFS from the starting side(s) of the given player
    bool checkWin(int player) const {
        vector<bool> visited(N*N, false);
        queue<int> q;

        if (player == 1) {      // Blue: connect top (row 0) to bottom (row N-1)
            for (int c = 0; c < N; ++c) {
                int id = 0*N + c;
                if (board[id] == player) {
                    visited[id] = true;
                    q.push(id);
                }
            }
        } else {                // Red: connect left (col 0) to right (col N-1)
            for (int r = 0; r < N; ++r) {
                int id = r*N + 0;
                if (board[id] == player) {
                    visited[id] = true;
                    q.push(id);
                }
            }
        }

        while (!q.empty()) {
            int id = q.front(); q.pop();
            int r = id / N;
            int c = id % N;

            // Reached opposite side?
            if (player == 1 && r == N-1) return true;
            if (player == 2 && c == N-1) return true;

            for (int nb : adj[id]) {
                if (!visited[nb] && board[nb] == player) {
                    visited[nb] = true;
                    q.push(nb);
                }
            }
        }
        return false;
    }

public:
    HexBoard(int size) : N(size), board(N*N, 0), turn(1), gameOver(false), winner(0) {
        buildGraph();
    }

    // Copy constructor (used for simulations)
    HexBoard(const HexBoard& other)
        : N(other.N), board(other.board), adj(other.adj),
          turn(other.turn), gameOver(other.gameOver), winner(other.winner) {}

    // Accessors
    int getTurn() const { return turn; }
    bool isGameOver() const { return gameOver; }
    int getWinner() const { return winner; }
    int getSize() const { return N; }
    int getCell(int r, int c) const { return board[r*N + c]; }

    // Place a stone for the current player; returns true if legal
    bool placeMove(int r, int c) {
        if (gameOver) return false;
        if (!inBounds(r, c)) return false;
        int id = r*N + c;
        if (board[id] != 0) return false;

        board[id] = turn;
        if (checkWin(turn)) {
            gameOver = true;
            winner = turn;
        } else {
            turn = (turn == 1) ? 2 : 1;
        }
        return true;
    }

    // Force a move (used for rollouts) – no turn switch, just fill
    void forceMove(int r, int c, int player) {
        if (inBounds(r, c)) {
            int id = r*N + c;
            if (board[id] == 0) board[id] = player;
        }
    }

    // Get list of empty cells (as flat indices)
    vector<int> getEmptyCells() const {
        vector<int> empty;
        for (int i = 0; i < N*N; ++i) {
            if (board[i] == 0) empty.push_back(i);
        }
        return empty;
    }

    // Return the winner (1 or 2) after a full board; assumes board is full
    int evaluateFullBoard() const {
        // If the board is full, exactly one player must have won.
        if (checkWin(1)) return 1;
        if (checkWin(2)) return 2;
        return 0; // Should not happen
    }

    // Display board with ASCII hex layout
    void display() const {
        cout << "\n  ";
        for (int c = 0; c < N; ++c)
            cout << " " << c << " ";
        cout << "\n";
        for (int r = 0; r < N; ++r) {
            if (r % 2 == 1) cout << "  ";
            cout << r << " ";
            for (int c = 0; c < N; ++c) {
                char ch = '.';
                int val = board[r*N + c];
                if (val == 1) ch = 'B';
                else if (val == 2) ch = 'R';
                cout << ch << "  ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
};

// ------------------------------------------------------------------------
//  Monte Carlo AI
// ------------------------------------------------------------------------

class MonteCarloAI {
private:
    int N;
    int player;             // AI's player number (1=Blue, 2=Red)
    int opponent;
    int numSimulations;
    mt19937 rng;

    // Perform one random playout from a given board state.
    // The board is assumed to have all moves already made for the current turn?
    // Actually we fill all empty cells randomly, then evaluate winner.
    int simulate(HexBoard board) {
        // Get list of empty cells
        vector<int> empty = board.getEmptyCells();
        // Shuffle empty cells
        shuffle(empty.begin(), empty.end(), rng);

        // Fill them with random (but equal chance) players.
        // We need to simulate alternating moves, but for win detection
        // it's enough to fill randomly because eventually both players will
        // have placed stones; the winner is determined by connectivity.
        // However, we must ensure that the number of stones of each player
        // is consistent with the game rules (alternating). But since we only
        // care about the final winner and we have a board with some stones already,
        // we can just assign each empty cell randomly to player 1 or 2 with equal probability.
        // This is a common simplification for Monte Carlo in Hex.
        for (int id : empty) {
            int r = id / N;
            int c = id % N;
            // Randomly assign to either player (but keep total counts roughly equal)
            // We can just pick random 1/2.
            int playerToPlace = (rng() % 2) + 1;
            board.forceMove(r, c, playerToPlace);
        }
        // Now evaluate who won
        return board.evaluateFullBoard();
    }

public:
    MonteCarloAI(int size, int aiPlayer, int sims = 1000)
        : N(size), player(aiPlayer), opponent((aiPlayer == 1) ? 2 : 1),
          numSimulations(sims), rng(chrono::steady_clock::now().time_since_epoch().count()) {}

    // Given the current board and the player to move (which should be the AI's turn),
    // return the best move as a pair (row, col).
    pair<int,int> getBestMove(const HexBoard& currentBoard) {
        vector<int> empty = currentBoard.getEmptyCells();
        if (empty.empty()) return {-1,-1}; // no moves

        // For each legal move, simulate many games and count wins
        vector<double> winRates(empty.size(), 0.0);

        // We'll perform simulations for each candidate move.
        // To avoid re-allocating board repeatedly, we create a copy for each.
        #pragma omp parallel for  // optional if you have OpenMP; remove if not
        for (size_t i = 0; i < empty.size(); ++i) {
            int id = empty[i];
            int r = id / N;
            int c = id % N;

            // Make a copy of the board and place AI's move
            HexBoard simBoard(currentBoard);
            if (!simBoard.placeMove(r, c)) {
                // Should not happen because cell is empty
                winRates[i] = 0.0;
                continue;
            }

            // Now it's opponent's turn in simBoard, but we don't care about turns;
            // we just fill the rest randomly.
            // Also note: we might want to flip the turn after placing, but simulate()
            // will fill randomly anyway.

            int wins = 0;
            for (int s = 0; s < numSimulations; ++s) {
                int winner = simulate(simBoard);
                if (winner == player) wins++;
            }
            winRates[i] = (double)wins / numSimulations;
        }

        // Choose the move with the highest win rate
        size_t bestIdx = 0;
        double bestRate = -1.0;
        for (size_t i = 0; i < empty.size(); ++i) {
            if (winRates[i] > bestRate) {
                bestRate = winRates[i];
                bestIdx = i;
            }
        }

        int bestId = empty[bestIdx];
        return {bestId / N, bestId % N};
    }
};

// ------------------------------------------------------------------------
//  Main game loop
// ------------------------------------------------------------------------

int main() {
    int size;
    cout << "Enter board size (e.g., 7 or 11): ";
    cin >> size;
    if (size < 2) size = 7;

    HexBoard board(size);

    // Let human choose color
    int humanColor;
    cout << "Choose your color: 1 = Blue (top-bottom), 2 = Red (left-right): ";
    cin >> humanColor;
    if (humanColor != 1 && humanColor != 2) humanColor = 1;

    int aiColor = (humanColor == 1) ? 2 : 1;
    cout << "You are " << (humanColor==1?"Blue (B)":"Red (R)") << endl;
    cout << "AI is " << (aiColor==1?"Blue (B)":"Red (R)") << endl;

    MonteCarloAI ai(size, aiColor, 1000);  // 1000 simulations per move

    cout << "\n--- HEX GAME ---\n";
    cout << "Blue (B) connects Top → Bottom.\n";
    cout << "Red  (R) connects Left → Right.\n";
    cout << "Coordinates are (row, column) starting from 0.\n";
    cout << "Enter your move as two integers: row col\n\n";

    bool humanTurn = true;
    if (humanColor == 2) humanTurn = false; // if human is Red, AI (Blue) goes first

    while (!board.isGameOver()) {
        board.display();

        if ((humanTurn && humanColor == 1) || (!humanTurn && aiColor == 1)) {
            // Blue's turn (either human or AI)
        } else {
            // Red's turn
        }

        if (humanTurn) {
            // Human move
            int r, c;
            cout << "Your move (row col): ";
            cin >> r >> c;
            if (!board.placeMove(r, c)) {
                cout << "Illegal move. Try again.\n";
                continue;
            }
        } else {
            // AI move
            cout << "AI is thinking...\n";
            auto start = chrono::steady_clock::now();
            pair<int,int> move = ai.getBestMove(board);
            auto end = chrono::steady_clock::now();
            double elapsed = chrono::duration<double>(end - start).count();
            cout << "AI move: " << move.first << " " << move.second
                 << " (took " << elapsed << " seconds)" << endl;
            if (move.first == -1) {
                cout << "No legal moves.\n";
                break;
            }
            board.placeMove(move.first, move.second);
        }

        // Switch turn
        humanTurn = !humanTurn;
    }

    board.display();
    int w = board.getWinner();
    if (w == humanColor) {
        cout << "Congratulations! You win!\n";
    } else if (w == aiColor) {
        cout << "AI wins! Better luck next time.\n";
    } else {
        cout << "Game ended with no winner? (should not happen)\n";
    }

    return 0;
}