#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include <utility>
#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <algorithm>

enum Player {
    NONE = 0,
    HUMAN = 1,
    AI = 2
};

class TicTacToeGame {
private:
    int board[3][3];
    int difficultyLevel;
    Player currentPlayer;
    bool gameMode; // true for PvP, false for PvAI

public:
    TicTacToeGame();
    void resetGame();
    bool makeMove(int row, int col, Player player);
    bool isBoardFull() const;
    bool checkWin(Player player) const;
    Player getCell(int row, int col) const;
    Player getCurrentPlayer() const { return currentPlayer; }
    void switchPlayer();

    // AI methods
    void setDifficulty(int level) { difficultyLevel = level; }
    void setGameMode(bool pvp) { gameMode = pvp; }
    std::pair<int, int> getAIMove();

private:
    std::pair<int, int> easyAI();
    std::pair<int, int> mediumAI();
    std::pair<int, int> getBestMove(Player aiPlayer);
    int minimax(int depth, bool isMaximizing, int alpha, int beta, Player aiPlayer, Player humanPlayer);
    int evaluateBoard(Player aiPlayer, Player humanPlayer);
};

#endif // TICTACTOEGAME_H
