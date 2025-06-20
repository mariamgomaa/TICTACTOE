#include "user.h"

/// @brief Default constructor.
/// Initializes game statistics to zero.
User::User() : gamesWon(0), gamesLost(0), gamesTied(0) {}

/// @brief Constructs a User with credentials and optional email.
/// Initializes statistics and sets last login to current time.
/// @param username The username for the account.
/// @param password The user's password (assumed hashed).
/// @param email Optional email address.
User::User(const QString& username, const QString& password,
           const QString& email)
    : username(username),
      password(password),
      email(email),
      gamesWon(0),
      gamesLost(0),
      gamesTied(0) {
  lastLogin = QDateTime::currentDateTime();
}

/// @brief Calculates the win rate as a percentage.
/// @return Percentage of games won out of total played.
/// If no games played, returns 0.0.
double User::getWinRate() const {
  int total = getTotalGames();
  if (total == 0) return 0.0;
  return (double)gamesWon / total * 100.0;
}

/// @brief Adds a completed game record to the user's history.
/// The game is added to the beginning of the list. All games
/// are preserved (database will handle any trimming or sync).
/// @param record The GameRecord to add.
void User::addGameToHistory(const GameRecord& record) {
<<<<<<< HEAD
  gameHistory.prepend(record);
  == == == =
<<<<<<< HEAD
               gameHistory.prepend(record);
  == == == = gameHistory.prepend(record);

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
>>>>>>> Merna
}
