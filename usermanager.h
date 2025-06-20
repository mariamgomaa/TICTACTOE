#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>
#include <QString>

// Class responsible for user management: registration, login,
// persistence to database, and handling game history.
class UserManager {
 public:
  // Constructor that sets up the database connection
  UserManager();

          // Destructor to clean up memory and save user data
  ~UserManager();

          // Registers a new user by hashing the password and saving info
  bool registerUser(const QString &username,
                    const QString &password,
                    const QString &email = "");

          // Authenticates a user by checking credentials
  bool loginUser(const QString &username, const QString &password);

          // Logs out the current user and triggers save
  void logoutUser();

          // Checks if a user is currently logged in
  bool isUserLoggedIn() const { return currentUser != nullptr; }

          // Returns a pointer to the logged-in user object
  User* getCurrentUser() const { return currentUser; }

          // Saves user profile and game history to the database
  void saveUserData();

          // Reserved for loading full user data if needed later
  void loadUserData();

          // Checks if the given username already exists
  bool userExists(const QString &username) const;

          // Returns a list of all usernames in the database
  QStringList getAllUsernames() const;

          // Loads the last 5 game records for the specified user
  QList<GameRecord> loadGameHistoryFromDatabase(const QString &username);

          // Loads the full game history for the specified user
  QList<GameRecord> loadAllGameHistoryFromDatabase(const QString &username);

 private:
  QSqlDatabase db;        // Connection to the SQLite database
  User* currentUser;      // Pointer to currently logged-in user
  QString dbPath;         // Filesystem path to the database file

          // Sets up the database connection and path
  bool initializeDatabase();

          // Hashes plain text password using SHA-256
  QString hashPassword(const QString &password) const;

          // Creates user and game tables if they do not exist
  bool createTables();

          // Saves user's statistics and profile to the database
  bool saveUserToDatabase(const User &user);

          // Loads user's profile and stats from the database
  User loadUserFromDatabase(const QString &username);

          // Saves list of game records to the database
  bool saveGameHistoryToDatabase(const QString &username,
                                 const QList<GameRecord> &history);

          // Serializes a list of GameMove objects into a string
  QString movesToString(const QVector<GameMove> &moves) const;

          // Deserializes a string into a list of GameMove objects
  QVector<GameMove> movesFromString(const QString &movesStr) const;
};

#endif  // USERMANAGER_H
