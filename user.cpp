#include "user.h"

// Default constructor initializes game counters to zero
User::User() : gamesWon(0), gamesLost(0), gamesTied(0) {}

// Parameterized constructor to initialize user credentials and email
// Also sets the last login time to the current time
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

// Computes and returns the win rate as a percentage
double User::getWinRate() const {
  int total = getTotalGames();
  if (total == 0) return 0.0;
  return (double)gamesWon / total * 100.0;
}

// Adds a game record to the beginning of the history list
// Keeps all games; database is responsible for managing storage
void User::addGameToHistory(const GameRecord& record) {
  // Add to beginning and keep all games (database will handle storage)
  gameHistory.prepend(record);
  // No longer limit to 5 games - let database handle the full history
}
