#ifndef USER_H
#define USER_H

#include <QMetaType>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVector>

// Struct representing a move in the Tic Tac Toe game
struct GameMove {
  int row;     // Row index of the move (0-2)
  int col;     // Column index of the move (0-2)
  int player;  // 1 for 'X', 2 for 'O'

  GameMove() = default;

          // Initializes a move with given row, column, and player
  GameMove(int r, int c, int p) : row(r), col(c), player(p) {}
};

// Struct storing all information about a completed game
struct GameRecord {
  QDateTime timestamp;         // Date and time the game was played
  QString opponent;            // Name of the opponent
  QString result;              // Result: Win, Loss, or Tie
  QString gameMode;            // PvP or PvAI
  QString playerSymbol;        // Player's symbol in the game: 'X' or 'O'
  QVector<GameMove> moves;     // Sequence of moves made in the game

  GameRecord() = default;

          // Constructs a record with summary information
  GameRecord(const QString &opp, const QString &res,
             const QString &mode, const QString &symbol)
      : timestamp(QDateTime::currentDateTime()),
        opponent(opp),
        result(res),
        gameMode(mode),
        playerSymbol(symbol) {}
};

// Class representing a user account and game statistics
class User {
 public:
  // Default constructor initializing stats to zero
  User();

          // Constructor with required user info and optional email
  User(const QString &username, const QString &password,
       const QString &email = "");

          // Returns the username
  QString getUsername() const { return username; }

          // Returns the hashed/encrypted password
  QString getPassword() const { return password; }

          // Returns the associated email
  QString getEmail() const { return email; }

          // Returns the last login timestamp
  QDateTime getLastLogin() const { return lastLogin; }

          // Returns number of games won
  int getGamesWon() const { return gamesWon; }

          // Returns number of games lost
  int getGamesLost() const { return gamesLost; }

          // Returns number of tied games
  int getGamesTied() const { return gamesTied; }

          // Returns the full game history list
  QList<GameRecord> getGameHistory() const { return gameHistory; }

          // Sets the username
  void setUsername(const QString &username) { this->username = username; }

          // Sets the hashed/encrypted password
  void setPassword(const QString &password) { this->password = password; }

          // Sets the user's email
  void setEmail(const QString &email) { this->email = email; }

          // Sets the last login time
  void setLastLogin(const QDateTime &lastLogin) { this->lastLogin = lastLogin; }

          // Replaces game history with a new list
  void setGameHistory(const QList<GameRecord> &history) {
    this->gameHistory = history;
  }

          // Increments the win counter
  void addWin() { gamesWon++; }

          // Increments the loss counter
  void addLoss() { gamesLost++; }

          // Increments the tie counter
  void addTie() { gamesTied++; }

          // Adds a game to the history list
  void addGameToHistory(const GameRecord &record);

          // Returns the total number of games played
  int getTotalGames() const { return gamesWon + gamesLost + gamesTied; }

          // Calculates and returns the win rate (0.0 - 1.0)
  double getWinRate() const;

 private:
  QString username;              // Unique username of the user
  QString password;              // Hashed or encrypted password
  QString email;                 // Optional user email
  QDateTime lastLogin;           // Timestamp of the last login
  int gamesWon;                  // Total games won
  int gamesLost;                 // Total games lost
  int gamesTied;                 // Total games tied
  QList<GameRecord> gameHistory; // List of all game records
};

// Allows GameMove to be stored in QVariant and used with signals/slots
Q_DECLARE_METATYPE(GameMove)

// Allows GameRecord to be stored in QVariant and used with signals/slots
Q_DECLARE_METATYPE(GameRecord)

#endif // USER_H
