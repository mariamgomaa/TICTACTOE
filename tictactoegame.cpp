#include "tictactoegame.h"

#include <algorithm>
#include <random>

TicTacToeGame::TicTacToeGame() {
  std::srand(std::time(0));
  resetGame();
  difficultyLevel = 1;
  gameMode = true;  // Default to PvP
}

void TicTacToeGame::resetGame() {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) board[i][j] = NONE;
  currentPlayer = HUMAN;
}

bool TicTacToeGame::makeMove(int row, int col, Player player) {
  if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == NONE) {
    board[row][col] = player;
    return true;
  }
  return false;
}

bool TicTacToeGame::isBoardFull() const {
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (board[i][j] == NONE) return false;
  return true;
}

bool TicTacToeGame::checkWin(Player player) const {
  // Check rows and columns
  for (int i = 0; i < 3; ++i) {
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player)
      return true;
    if (board[0][i] == player && board[1][i] == player && board[2][i] == player)
      return true;
  }

  // Check diagonals
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player)
    return true;
  if (board[0][2] == player && board[1][1] == player && board[2][0] == player)
    return true;
  return false;
}

Player TicTacToeGame::getCell(int row, int col) const {
  if (row >= 0 && row < 3 && col >= 0 && col < 3)
    return static_cast<Player>(board[row][col]);
  return NONE;
}

void TicTacToeGame::switchPlayer() {
  currentPlayer = (currentPlayer == HUMAN) ? AI : HUMAN;
}

std::pair<int, int> TicTacToeGame::getAIMove() {
  switch (difficultyLevel) {
    case 1:
      return easyAI();
    case 2:
      return mediumAI();
    case 3:
      return getBestMove(AI);
    default:
      return easyAI();
  }
}

std::pair<int, int> TicTacToeGame::easyAI() {
  std::vector<std::pair<int, int>> emptyCells;
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (board[i][j] == NONE) emptyCells.push_back(std::make_pair(i, j));

  if (!emptyCells.empty()) {
    int randomIndex = std::rand() % emptyCells.size();
    return emptyCells[randomIndex];
  }
  return std::make_pair(-1, -1);
}

std::pair<int, int> TicTacToeGame::mediumAI() {
  // First check if AI can win
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == NONE) {
        board[i][j] = AI;
        if (checkWin(AI)) {
          board[i][j] = NONE;
          return std::make_pair(i, j);
        }
        board[i][j] = NONE;
      }
    }
  }

  // Then check if need to block human
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == NONE) {
        board[i][j] = HUMAN;
        if (checkWin(HUMAN)) {
          board[i][j] = NONE;
          return std::make_pair(i, j);
        }
        board[i][j] = NONE;
      }
    }
  }

  // Otherwise random move
  return easyAI();
}

std::pair<int, int> TicTacToeGame::getBestMove(Player aiPlayer) {
  int bestScore = INT_MIN;
  std::pair<int, int> bestMove = std::make_pair(-1, -1);
  Player humanPlayer = (aiPlayer == HUMAN) ? AI : HUMAN;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (board[i][j] == NONE) {
        board[i][j] = aiPlayer;
        int score = minimax(0, false, INT_MIN, INT_MAX, aiPlayer, humanPlayer);
        board[i][j] = NONE;
        if (score > bestScore) {
          bestScore = score;
          bestMove = std::make_pair(i, j);
        }
      }
    }
  }
  return bestMove;
}

int TicTacToeGame::evaluateBoard(Player aiPlayer, Player humanPlayer) {
  if (checkWin(aiPlayer)) return 10;
  if (checkWin(humanPlayer)) return -10;
  return 0;
}

int TicTacToeGame::minimax(int depth, bool isMaximizing, int alpha, int beta,
                           Player aiPlayer, Player humanPlayer) {
  int score = evaluateBoard(aiPlayer, humanPlayer);

  // Terminal states
  if (score == 10) return score - depth;   // AI wins (prefer faster wins)
  if (score == -10) return score + depth;  // Human wins (delay losses)
  if (isBoardFull()) return 0;             // Tie

  if (isMaximizing) {
    // AI's turn - maximize score
    int maxEval = INT_MIN;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (board[i][j] == NONE) {
          board[i][j] = aiPlayer;
          int eval =
              minimax(depth + 1, false, alpha, beta, aiPlayer, humanPlayer);
          board[i][j] = NONE;

          maxEval = std::max(maxEval, eval);
          alpha = std::max(alpha, eval);

          // Alpha-beta pruning
          if (beta <= alpha) {
            return maxEval;  // Beta cutoff
          }
        }
      }
    }
    return maxEval;
  } else {
    // Human's turn - minimize score
    int minEval = INT_MAX;
    for (int i = 0; i < 3; ++i) {
      for (int j = 0; j < 3; ++j) {
        if (board[i][j] == NONE) {
          board[i][j] = humanPlayer;
          int eval =
              minimax(depth + 1, true, alpha, beta, aiPlayer, humanPlayer);
          board[i][j] = NONE;

          minEval = std::min(minEval, eval);
          beta = std::min(beta, eval);

          // Alpha-beta pruning
          if (beta <= alpha) {
            return minEval;  // Alpha cutoff
          }
        }
      }
    }
    return minEval;
  }
}
