#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QList>
#include <QMetaType>
#include <QString>
#include <QVector>

// Represents a single move in the game
struct GameMove {
  int row;       // Row index of the move
  int col;       // Column index of the move
  int player;    // Player number: 1 for X, 2 for O

  GameMove() = default;
  GameMove(int r, int c, int p) : row(r), col(c), player(p) {}
};

// Represents a completed game record for history
struct GameRecord {
  QDateTime timestamp;       // Date and time of the game
  QString opponent;          // Opponent username
  QString result;            // Result: Win, Loss, or Tie
  QString gameMode;          // Game mode: Singleplayer or Multiplayer
  QString playerSymbol;      // Symbol used by this user (X or O)
  QVector<GameMove> moves;   // List of all moves for replay

  GameRecord() = default;
  GameRecord(const QString& opp, const QString& res, const QString& mode,
             const QString& symbol)
      : timestamp(QDateTime::currentDateTime()),
        opponent(opp),
        result(res),
        gameMode(mode),
        playerSymbol(symbol) {}
};

// Manages user information and game statistics
class User {
 public:
  User();
  User(const QString& username, const QString& password,
       const QString& email = "");

          // Getters
  QString getUsername() const { return username; }
  QString getPassword() const { return password; }
  QString getEmail() const { return email; }
  QDateTime getLastLogin() const { return lastLogin; }
  int getGamesWon() const { return gamesWon; }
  int getGamesLost() const { return gamesLost; }
  int getGamesTied() const { return gamesTied; }
  QList<GameRecord> getGameHistory() const { return gameHistory; }

          // Setters
  void setUsername(const QString& username) { this->username = username; }
  void setPassword(const QString& password) { this->password = password; }
  void setEmail(const QString& email) { this->email = email; }
  void setLastLogin(const QDateTime& lastLogin) { this->lastLogin = lastLogin; }
  void setGameHistory(const QList<GameRecord>& history) {
    this->gameHistory = history;
  }

          // Game result counters
  void addWin() { gamesWon++; }
  void addLoss() { gamesLost++; }
  void addTie() { gamesTied++; }

          // Add a game record to history
          // Stores all games for full database synchronization
  void addGameToHistory(const GameRecord& record);

          // Returns total number of games played
  int getTotalGames() const { return gamesWon + gamesLost + gamesTied; }

          // Calculates win rate as a fraction of total games
  double getWinRate() const;

 private:
  QString username;                // User's name
  QString password;                // User's hashed password
  QString email;                   // Optional user email
  QDateTime lastLogin;             // Timestamp of last successful login
  int gamesWon;                    // Number of games won
  int gamesLost;                   // Number of games lost
  int gamesTied;                   // Number of games tied
  QList<GameRecord> gameHistory;  // Full history of played games
};

// Enable GameMove and GameRecord types for use with Qt's meta-object system
Q_DECLARE_METATYPE(GameMove)
Q_DECLARE_METATYPE(GameRecord)

#endif  // USER_H
