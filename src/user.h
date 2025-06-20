#ifndef USER_H
#define USER_H

#include <QDateTime>
#include <QList>
#include <QMetaType>
#include <QString>
#include <QVector>

/// @brief Represents a single move made during a Tic Tac Toe game.
struct GameMove {
  int row;     ///< The row index of the move (0-2).
  int col;     ///< The column index of the move (0-2).
  int player;  ///< The player who made the move: 1 for 'X', 2 for 'O'.

  GameMove() = default;

  /// @brief Constructs a GameMove with specified position and player.
  /// @param r Row index (0-2).
  /// @param c Column index (0-2).
  /// @param p Player identifier (1 = X, 2 = O).
  GameMove(int r, int c, int p) : row(r), col(c), player(p) {}
};

/// @brief Stores data about a completed game session.
struct GameRecord {
  QDateTime timestamp;      ///< Time when the game was played.
  QString opponent;         ///< Name of the opponent.
  QString result;           ///< Result of the game: Win/Loss/Tie.
  QString gameMode;         ///< Game mode (e.g., PvP or PvAI).
  QString playerSymbol;     ///< Player's symbol: 'X' or 'O'.
  QVector<GameMove> moves;  ///< All moves made in the game for replay.

  GameRecord() = default;

  /// @brief Constructs a GameRecord with given game summary info.
  /// @param opp Opponent's name.
  /// @param res Result of the game.
  /// @param mode Game mode (PvP, PvAI).
  /// @param symbol Player's symbol used in the game.
  GameRecord(const QString& opp, const QString& res, const QString& mode,
             const QString& symbol)
      : timestamp(QDateTime::currentDateTime()),
        opponent(opp),
        result(res),
        gameMode(mode),
        playerSymbol(symbol) {}
};

/// @brief Represents a user with authentication data and game stats.
class User {
 public:
  /// @brief Default constructor. Initializes stats to zero.
  User();

  /// @brief Constructs a User with basic credentials.
  /// @param username The user's username.
  /// @param password Hashed password for authentication.
  /// @param email Optional email address.
  User(const QString& username, const QString& password,
       const QString& email = "");

  /// @brief Gets the username.
  /// @return The username of the user.
  QString getUsername() const { return username; }

  /// @brief Gets the hashed password.
  /// @return The user's hashed password.
  QString getPassword() const { return password; }

  /// @brief Gets the user's email.
  /// @return The email associated with the user account.
  QString getEmail() const { return email; }

  /// @brief Gets the last login timestamp.
  /// @return The last login timestamp.
  QDateTime getLastLogin() const { return lastLogin; }

  /// @brief Gets number of games won.
  /// @return Number of games the user has won.
  int getGamesWon() const { return gamesWon; }

  /// @brief Gets number of games lost.
  /// @return Number of games the user has lost.
  int getGamesLost() const { return gamesLost; }

  /// @brief Gets number of tied games.
  /// @return Number of games that ended in a tie.
  int getGamesTied() const { return gamesTied; }

  /// @brief Retrieves the game history.
  /// @return Full list of past game records.
  QList<GameRecord> getGameHistory() const { return gameHistory; }

  /// @brief Sets the username.
  /// @param username The username to set.
  void setUsername(const QString& username) { this->username = username; }

  /// @brief Sets the password.
  /// @param password The password to set (assumed hashed).
  void setPassword(const QString& password) { this->password = password; }

  /// @brief Sets the user's email.
  /// @param email The email to associate with the user.
  void setEmail(const QString& email) { this->email = email; }

  /// @brief Sets the last login time.
  /// @param lastLogin The time of last successful login.
  void setLastLogin(const QDateTime& lastLogin) { this->lastLogin = lastLogin; }

  /// @brief Replaces the game history list.
  /// @param history The new list of GameRecords.
  void setGameHistory(const QList<GameRecord>& history) {
    this->gameHistory = history;
  }

  /// @brief Increments the win counter.
  void addWin() { gamesWon++; }

  /// @brief Increments the loss counter.
  void addLoss() { gamesLost++; }

  /// @brief Increments the tie counter.
  void addTie() { gamesTied++; }

  /// @brief Adds a game to the user's history.
  /// @param record GameRecord object containing the session data.
  void addGameToHistory(const GameRecord& record);

  /// @brief Computes total number of games played.
  /// @return Total games played by the user.
  int getTotalGames() const { return gamesWon + gamesLost + gamesTied; }

  /// @brief Calculates the win rate.
  /// @return Win rate as a float between 0.0 and 1.0.
  double getWinRate() const;

 private:
  QString username;               ///< Unique username.
  QString password;               ///< Encrypted or hashed password.
  QString email;                  ///< Optional email address.
  QDateTime lastLogin;            ///< Last login time.
  int gamesWon;                   ///< Number of wins.
  int gamesLost;                  ///< Number of losses.
  int gamesTied;                  ///< Number of ties.
  QList<GameRecord> gameHistory;  ///< List of all game records.
};

/// @brief Enables GameMove to be used in QVariant and Qt meta system.
Q_DECLARE_METATYPE(GameMove)

/// @brief Enables GameRecord to be used in QVariant and Qt meta system.
Q_DECLARE_METATYPE(GameRecord)

#endif  // USER_H
