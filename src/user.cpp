#include "user.h"

User::User() : gamesWon(0), gamesLost(0), gamesTied(0) {}

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

double User::getWinRate() const {
  int total = getTotalGames();
  if (total == 0) return 0.0;
  return (double)gamesWon / total * 100.0;
}

void User::addGameToHistory(const GameRecord& record) {
  // Add to beginning and keep all games (database will handle storage)
  gameHistory.prepend(record);
  // No longer limit to 5 games - let database handle the full history
}
