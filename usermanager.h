#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QCryptographicHash>  // For hashing passwords
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>        // For database connection
#include <QSqlError>
#include <QSqlQuery>           // For executing SQL statements
#include <QStandardPaths>      // For locating standard directories
#include <QString>

#include "user.h"              // Include user model

// Manages user registration, authentication, and persistence
class UserManager {
 public:
  UserManager();   // Constructor: initializes database and setup
  ~UserManager();  // Destructor: closes database connection if open

          // Registers a new user with optional email
  bool registerUser(const QString& username, const QString& password,
                    const QString& email = "");

          // Authenticates a user using stored hashed password
  bool loginUser(const QString& username, const QString& password);

          // Logs out the current user
  void logoutUser();

          // Checks if a user is currently logged in
  bool isUserLoggedIn() const { return currentUser != nullptr; }

          // Returns pointer to the currently logged-in user
  User* getCurrentUser() const { return currentUser; }

          // Saves current user data to the database
  void saveUserData();

          // Loads data for the current user from the database
  void loadUserData();

          // Checks if a user already exists in the database
  bool userExists(const QString& username) const;

          // Retrieves a list of all registered usernames
  QStringList getAllUsernames() const;

          // Loads a user's game history for use in replays or stats
          // Allows full replay functionality
  QList<GameRecord> loadGameHistoryFromDatabase(const QString& username);

          // Loads all game records ever played by a user
  QList<GameRecord> loadAllGameHistoryFromDatabase(const QString& username);

 private:
  QSqlDatabase db;       // SQLite database object
  User* currentUser;     // Pointer to the current user
  QString dbPath;        // Path to the database file

          // Sets up the database connection and tables
  bool initializeDatabase();

          // Returns a SHA-256 hashed version of the input password
  QString hashPassword(const QString& password) const;

          // Creates required tables if they do not exist
  bool createTables();

          // Saves the user's profile and stats to the database
  bool saveUserToDatabase(const User& user);

          // Loads a user by username from the database
  User loadUserFromDatabase(const QString& username);

          // Saves the user's game history to the database
  bool saveGameHistoryToDatabase(const QString& username,
                                 const QList<GameRecord>& history);

          // Converts a list of moves to a single string for DB storage
  QString movesToString(const QVector<GameMove>& moves) const;

          // Converts a stored string back to a list of moves
  QVector<GameMove> movesFromString(const QString& movesStr) const;
};

#endif  // USERMANAGER_H
