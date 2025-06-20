#include "user.h"

// Default constructor that initializes win/loss/tie counters to zero
User::User() : gamesWon(0), gamesLost(0), gamesTied(0) {
}

// Constructor that sets user credentials and initializes statistics.
// Also sets the last login time to the current system time.
User::User(const QString &username, const QString &password, const QString &email)
    : username(username), password(password), email(email),
      gamesWon(0), gamesLost(0), gamesTied(0) {
  lastLogin = QDateTime::currentDateTime();
}

// Calculates and returns the user's win rate as a percentage.
// Returns 0.0 if the user has not played any games.
double User::getWinRate() const {
  int total = getTotalGames();
  if (total == 0) return 0.0;
  return (double)gamesWon / total * 100.0;
}

// Adds a completed game record to the user's game history.
// The new record is inserted at the beginning of the history list.
void User::addGameToHistory(const GameRecord &record) {
  gameHistory.prepend(record);
}
