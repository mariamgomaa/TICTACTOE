#include <gtest/gtest.h>

#include "tictactoegame.h"  //file need to be tested

// Test for makeMove function
// Test valid move
TEST(GameLogicTest, MakeMove_ValidCell_ReturnsTrue) {
  TicTacToeGame game;
  bool result = game.makeMove(1, 1, HUMAN);
  EXPECT_TRUE(result);
  EXPECT_EQ(game.getCell(1, 1), HUMAN);
}

// Test invalid move (already filled)
TEST(GameLogicTest, MakeMove_AlreadyFilled) {
  TicTacToeGame game;
  game.makeMove(1, 1, HUMAN);             // First move valid
  bool result = game.makeMove(1, 1, AI);  // Second move on same cell
  EXPECT_FALSE(result);
  EXPECT_EQ(game.getCell(1, 1), HUMAN);  // Should remain as HUMAN
}

// Test out-of-bounds move (negative index)
TEST(GameLogicTest, MakeMove_OutOfBoundsNegative) {
  TicTacToeGame game;
  bool result = game.makeMove(-1, 0, HUMAN);
  EXPECT_FALSE(result);
}

// Test out-of-bounds move (beyond board size)
TEST(GameLogicTest, MakeMove_OutOfBounds) {
  TicTacToeGame game;
  bool result =
      game.makeMove(3, 0, HUMAN);  // Board size is 3x3, so index 3 is out
  EXPECT_FALSE(result);
}

// Test for checkWin function
// Row win
TEST(GameLogicTest, CheckWin_RowWin_ReturnsTrue) {
  TicTacToeGame game;
  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, HUMAN);
  game.makeMove(0, 2, HUMAN);
  EXPECT_TRUE(game.checkWin(HUMAN));
}

// Column win
TEST(GameLogicTest, CheckWin_ColumnWin_ReturnsTrue) {
  TicTacToeGame game;
  game.makeMove(0, 1, AI);
  game.makeMove(1, 1, AI);
  game.makeMove(2, 1, AI);
  EXPECT_TRUE(game.checkWin(AI));
}

// Diagonal win
TEST(GameLogicTest, CheckWin_MainDiagonal_ReturnsTrue) {
  TicTacToeGame game;
  game.makeMove(0, 0, HUMAN);
  game.makeMove(1, 1, HUMAN);
  game.makeMove(2, 2, HUMAN);
  EXPECT_TRUE(game.checkWin(HUMAN));
}

// Anti-diagonal win
TEST(GameLogicTest, CheckWin_AntiDiagonal_ReturnsTrue) {
  TicTacToeGame game;
  game.makeMove(0, 2, AI);
  game.makeMove(1, 1, AI);
  game.makeMove(2, 0, AI);
  EXPECT_TRUE(game.checkWin(AI));
}

// No win
TEST(GameLogicTest, CheckWin_NoWin_ReturnsFalse) {
  TicTacToeGame game;
  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, HUMAN);
  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, HUMAN);
  game.makeMove(1, 2, AI);
  EXPECT_FALSE(game.checkWin(HUMAN));
  EXPECT_FALSE(game.checkWin(AI));
}

// Test for isBoardFull function
//  Full board returns true
TEST(GameLogicTest, IsBoardFull_FullBoard) {
  TicTacToeGame game;
  // Fill all cells
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) game.makeMove(i, j, HUMAN);

  EXPECT_TRUE(game.isBoardFull());
}

// One empty cell returns false
TEST(GameLogicTest, IsBoardFull_OneEmpty_ReturnsFalse) {
  TicTacToeGame game;

  // Fill all but one cell
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      if (!(i == 2 && j == 2))  // Leave bottom-right empty
        game.makeMove(i, j, AI);

  EXPECT_FALSE(game.isBoardFull());
}

// Empty board returns false
TEST(GameLogicTest, IsBoardFull_EmptyBoard_ReturnsFalse) {
  TicTacToeGame game;
  EXPECT_FALSE(game.isBoardFull());
}

// Test for getCell function
// Empty cell returns NONE
TEST(GameLogicTest, GetCell_EmptyCell_ReturnsNone) {
  TicTacToeGame game;
  EXPECT_EQ(game.getCell(1, 1), NONE);
}

// After move by HUMAN, getCell returns HUMAN
TEST(GameLogicTest, GetCell_HumanMove_ReturnsHuman) {
  TicTacToeGame game;
  game.makeMove(0, 2, HUMAN);
  EXPECT_EQ(game.getCell(0, 2), HUMAN);
}

// After move by AI, getCell returns AI
TEST(GameLogicTest, GetCell_AIMove_ReturnsAI) {
  TicTacToeGame game;
  game.makeMove(2, 2, AI);
  EXPECT_EQ(game.getCell(2, 2), AI);
}

// Out-of-bounds row returns NONE
TEST(GameLogicTest, GetCell_RowOutOfBounds_ReturnsNone) {
  TicTacToeGame game;
  EXPECT_EQ(game.getCell(-1, 1), NONE);
  EXPECT_EQ(game.getCell(3, 0), NONE);
}

// Out-of-bounds column returns NONE
TEST(GameLogicTest, GetCell_ColOutOfBounds_ReturnsNone) {
  TicTacToeGame game;
  EXPECT_EQ(game.getCell(1, -1), NONE);
  EXPECT_EQ(game.getCell(1, 3), NONE);
}

// Test for resetGame function
// Test that the board is cleared after resetGame
TEST(GameLogicTest, ResetGame_ClearsBoard) {
  TicTacToeGame game;

  // Make some moves to fill the board
  game.makeMove(0, 0, HUMAN);
  game.makeMove(1, 1, AI);
  game.makeMove(2, 2, HUMAN);

  game.resetGame();  // Clear the board

  // Check every cell is NONE
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      EXPECT_EQ(game.getCell(i, j), NONE);
    }
  }
}

// Test that current player is set to HUMAN after reset
TEST(GameLogicTest, ResetGame_SetsCurrentPlayerToHuman) {
  TicTacToeGame game;

  game.switchPlayer();  // Switch to AI
  game.resetGame();     // switch back to HUMAN

  EXPECT_TRUE(game.makeMove(0, 0, HUMAN));  // If HUMAN starts, this is allowed
  EXPECT_EQ(game.getCell(0, 0), HUMAN);
}

// Test for SwitchPlayer Function
TEST(GameLogicTest, SwitchPlayer_TogglesCorrectly) {
  TicTacToeGame game;

  // Initially HUMAN
  EXPECT_EQ(game.getCurrentPlayer(), HUMAN);

  // After 1st switch → should be AI
  game.switchPlayer();
  EXPECT_EQ(game.getCurrentPlayer(), AI);

  // After 2nd switch → back to HUMAN
  game.switchPlayer();
  EXPECT_EQ(game.getCurrentPlayer(), HUMAN);
}

// Test for easyAI function
// Test 1: AI returns a valid move on an empty board
TEST(AIEasyTest, EasyAI_EmptyBoard_ReturnsValidMove) {
  TicTacToeGame game;
  std::pair<int, int> move = game.test_easyAI();

  EXPECT_GE(move.first, 0);
  EXPECT_LT(move.first, 3);
  EXPECT_GE(move.second, 0);
  EXPECT_LT(move.second, 3);
  EXPECT_EQ(game.getCell(move.first, move.second), NONE);
}

// Test 2: AI avoids filled cells
TEST(AIEasyTest, EasyAI_AvoidsFilledCells) {
  TicTacToeGame game;

  // Fill some cells
  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, HUMAN);

  std::pair<int, int> move = game.test_easyAI();

  // Make sure the AI did not return a filled position
  EXPECT_NE(move, std::make_pair(0, 0));
  EXPECT_NE(move, std::make_pair(0, 1));
  EXPECT_NE(move, std::make_pair(0, 2));
  EXPECT_EQ(game.getCell(move.first, move.second), NONE);
}

// Test 3: AI returns (-1, -1) when board is full
TEST(AIEasyTest, EasyAI_FullBoard_ReturnsInvalidMove) {
  TicTacToeGame game;

  // Fill entire board
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j) game.makeMove(i, j, HUMAN);

  std::pair<int, int> move = game.test_easyAI();
  EXPECT_EQ(move.first, -1);
  EXPECT_EQ(move.second, -1);
}

// Test for mediumAI function
// Test 1: AI should take winning move
TEST(AIMediumTest, MediumAI_WinsWhenPossible) {
  TicTacToeGame game;

  // Set board so AI can win at (0, 2)
  game.makeMove(0, 0, AI);
  game.makeMove(0, 1, AI);

  std::pair<int, int> move = game.test_mediumAI();

  EXPECT_EQ(move.first, 0);
  EXPECT_EQ(move.second, 2);
}

// Test 2: AI should block human from winning
TEST(AIMediumTest, MediumAI_BlocksHumanWin) {
  TicTacToeGame game;

  // HUMAN can win at (2, 2)
  game.makeMove(2, 0, HUMAN);
  game.makeMove(2, 1, HUMAN);

  std::pair<int, int> move = game.test_mediumAI();

  EXPECT_EQ(move.first, 2);
  EXPECT_EQ(move.second, 2);
}

// Test 3: No threat or win → random move
TEST(AIMediumTest, MediumAI_RandomWhenNoThreat) {
  TicTacToeGame game;

  // No one is winning yet
  game.makeMove(0, 0, HUMAN);
  game.makeMove(1, 1, AI);

  std::pair<int, int> move = game.test_mediumAI();

  EXPECT_GE(move.first, 0);
  EXPECT_LT(move.first, 3);
  EXPECT_GE(move.second, 0);
  EXPECT_LT(move.second, 3);
  EXPECT_EQ(game.getCell(move.first, move.second), NONE);
}

// Test for getBestMove function
//  AI should take winning move
TEST(AIHardTest, GetBestMove_AIWinsWhenPossible) {
  TicTacToeGame game;

  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, AI);
  // Empty (1,2) lets AI win

  std::pair<int, int> move = game.test_getBestMove(AI);
  EXPECT_EQ(move.first, 1);
  EXPECT_EQ(move.second, 2);
}

// AI should block human from winning
TEST(AIHardTest, GetBestMove_AIBlocksHuman) {
  TicTacToeGame game;

  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, HUMAN);
  // Human can win at (0,2) unless AI blocks

  std::pair<int, int> move = game.test_getBestMove(AI);
  EXPECT_EQ(move.first, 0);
  EXPECT_EQ(move.second, 2);
}

TEST(AIHardTest, GetBestMove_FirstMoveIsValid) {
  TicTacToeGame game;
  std::pair<int, int> move = game.test_getBestMove(AI);
  // Ensure the move is within bounds
  EXPECT_GE(move.first, 0);   // Row ≥ 0
  EXPECT_LT(move.first, 3);   // Row < 3
  EXPECT_GE(move.second, 0);  // Col ≥ 0
  EXPECT_LT(move.second, 3);  // Col < 3

  // Ensure the chosen cell is actually empty
  EXPECT_EQ(game.getCell(move.first, move.second), NONE);
}

// AI finds draw when no win possible
TEST(AIHardTest, GetBestMove_ForcedDraw_ReturnsValidMove) {
  TicTacToeGame game;

  // Set up a nearly full board
  // HUMAN: X, AI: O
  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, HUMAN);

  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, HUMAN);
  game.makeMove(1, 2, AI);

  game.makeMove(2, 0, HUMAN);
  game.makeMove(2, 1, NONE);  // Only move left
  game.makeMove(2, 2, AI);

  std::pair<int, int> move = game.test_getBestMove(AI);
  EXPECT_EQ(move.first, 2);
  EXPECT_EQ(move.second, 1);
}

// Test for minimax function
// Test1: AI has already won (score = 10 - depth)
TEST(MinimaxTest, AIWinScore) {
  TicTacToeGame game;
  game.makeMove(0, 0, AI);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, AI);  // AI wins top row

  int score = game.test_minimax(0, false, INT_MIN, INT_MAX, AI, HUMAN);
  EXPECT_EQ(score, 10);  // 10 - depth (depth = 0)
}

// Test2: Human has already won (score = -10 + depth)
TEST(MinimaxTest, HumanWinScore) {
  TicTacToeGame game;
  game.makeMove(1, 0, HUMAN);
  game.makeMove(1, 1, HUMAN);
  game.makeMove(1, 2, HUMAN);  // Human wins middle row

  int score = game.test_minimax(2, true, INT_MIN, INT_MAX, AI, HUMAN);
  EXPECT_EQ(score, -8);  // -10 + depth (depth = 2)
}

// Test3: Full board with no winner = draw (score = 0)
TEST(MinimaxTest, DrawScore) {
  TicTacToeGame game;

  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, HUMAN);
  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, HUMAN);
  game.makeMove(1, 2, AI);
  game.makeMove(2, 0, AI);
  game.makeMove(2, 1, HUMAN);
  game.makeMove(2, 2, AI);

  int score = game.test_minimax(4, true, INT_MIN, INT_MAX, AI, HUMAN);
  EXPECT_EQ(score, 0);
}

// Test for evaluateBoard function
// AI has a winning line → score should be 10
TEST(EvaluateBoardTest, AIWins_Returns10) {
  TicTacToeGame game;

  game.makeMove(0, 0, AI);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, AI);  // AI wins top row

  int score = game.test_evaluateBoard(AI, HUMAN);
  EXPECT_EQ(score, 10);
}

// Human has a winning line → score should be -10
TEST(EvaluateBoardTest, HumanWins_ReturnsMinus10) {
  TicTacToeGame game;

  game.makeMove(2, 0, HUMAN);
  game.makeMove(2, 1, HUMAN);
  game.makeMove(2, 2, HUMAN);  // Human wins bottom row

  int score = game.test_evaluateBoard(AI, HUMAN);
  EXPECT_EQ(score, -10);
}

// No winner → score should be 0
TEST(EvaluateBoardTest, NoWinner_Returns0) {
  TicTacToeGame game;

  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, AI);
  game.makeMove(0, 2, HUMAN);
  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, HUMAN);

  int score = game.test_evaluateBoard(AI, HUMAN);
  EXPECT_EQ(score, 0);
}

// Test for getAIMove function
// Easy Mode: Should return a valid move, but NOT guarantee a win or block
TEST(GetAIMoveTest, Difficulty1_EasyMode_ValidRandomMove) {
  TicTacToeGame game;
  game.setDifficulty(1);  // Easy

  game.makeMove(0, 0, HUMAN);
  game.makeMove(0, 1, HUMAN);

  std::pair<int, int> move = game.getAIMove();

  // Just check the move is in bounds and empty
  EXPECT_GE(move.first, 0);
  EXPECT_LT(move.first, 3);
  EXPECT_GE(move.second, 0);
  EXPECT_LT(move.second, 3);
  EXPECT_EQ(game.getCell(move.first, move.second), NONE);
}

// Medium Mode: Should block human if human is about to win
TEST(GetAIMoveTest, Difficulty2_MediumMode_BlocksHuman) {
  TicTacToeGame game;
  game.setDifficulty(2);  // Medium

  // Human is about to win
  game.makeMove(2, 0, HUMAN);
  game.makeMove(2, 1, HUMAN);

  std::pair<int, int> move = game.getAIMove();

  // Expect AI to block at (2,2)
  EXPECT_EQ(move.first, 2);
  EXPECT_EQ(move.second, 2);
}

// Medium Mode: Should win if AI has two in a row
TEST(GetAIMoveTest, Difficulty2_MediumMode_WinsIfPossible) {
  TicTacToeGame game;
  game.setDifficulty(2);  // Medium

  // AI can win
  game.makeMove(0, 0, AI);
  game.makeMove(0, 1, AI);

  std::pair<int, int> move = game.getAIMove();

  // Expect AI to win at (0,2)
  EXPECT_EQ(move.first, 0);
  EXPECT_EQ(move.second, 2);
}

// Hard Mode: Should always find winning move using minimax
TEST(GetAIMoveTest, Difficulty3_HardMode_WinsIfPossible) {
  TicTacToeGame game;
  game.setDifficulty(3);  // Hard

  game.makeMove(1, 0, AI);
  game.makeMove(1, 1, AI);

  std::pair<int, int> move = game.getAIMove();

  // Expect AI to win at (1,2)
  EXPECT_EQ(move.first, 1);
  EXPECT_EQ(move.second, 2);
}

int main() {
  ::testing::InitGoogleTest();  // initialze googletest
  return RUN_ALL_TESTS();       // excute all tests above main
}
