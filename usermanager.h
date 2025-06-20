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

/// @brief Handles all user-related operations including authentication,
/// data persistence, and game history management.
class UserManager {
 public:
  /// @brief Constructs a new UserManager and initializes the database.
  UserManager();

          /// @brief Destructor that ensures user data is saved and memory is freed.
  ~UserManager();

          /// @brief Registers a new user and stores their credentials securely.
          /// @param username Desired username.
          /// @param password Plain-text password.
          /// @param email Optional email address.
          /// @return True if registration succeeded.
  bool registerUser(const QString &username,
                    const QString &password,
                    const QString &email = "");

          /// @brief Logs in an existing user after verifying credentials.
          /// @param username Username to authenticate.
          /// @param password Plain-text password to validate.
          /// @return True if login is successful.
  bool loginUser(const QString &username, const QString &password);

          /// @brief Logs out the current user and saves their data.
  void logoutUser();

          /// @brief Checks if a user is currently logged in.
          /// @return True if a user is logged in.
  bool isUserLoggedIn() const { return currentUser != nullptr; }

          /// @brief Returns the pointer to the currently logged-in user.
          /// @return Pointer to User object, or nullptr if no user is logged in.
  User* getCurrentUser() const { return currentUser; }

          /// @brief Saves the current user's data and game history.
  void saveUserData();

          /// @brief Reserved for future use: Loads user data on demand.
  void loadUserData();

          /// @brief Checks if a username already exists in the database.
          /// @param username Username to check.
          /// @return True if the username is taken.
  bool userExists(const QString &username) const;

          /// @brief Retrieves all registered usernames.
          /// @return A QStringList of all usernames.
  QStringList getAllUsernames() const;

          /// @brief Loads the last 5 game records for a specific user.
          /// @param username Target user.
          /// @return A list of GameRecord objects.
  QList<GameRecord> loadGameHistoryFromDatabase(const QString &username);

          /// @brief Loads the complete game history for a specific user.
          /// @param username Target user.
          /// @return A list of all GameRecord entries.
  QList<GameRecord> loadAllGameHistoryFromDatabase(const QString &username);

 private:
  QSqlDatabase db;              ///< SQLite database connection.
  User* currentUser;            ///< Currently logged-in user.
  QString dbPath;               ///< Path to the SQLite database file.

          /// @brief Initializes and connects to the SQLite database.
          /// @return True if initialization was successful.
  bool initializeDatabase();

          /// @brief Hashes a plain-text password using SHA-256.
          /// @param password The password to hash.
          /// @return A hex-encoded hash string.
  QString hashPassword(const QString &password) const;

          /// @brief Creates necessary database tables.
          /// @return True if all tables were created successfully.
  bool createTables();

          /// @brief Saves the given user's stats and metadata to the database.
          /// @param user The user whose data is being saved.
          /// @return True if the operation succeeded.
  bool saveUserToDatabase(const User &user);

          /// @brief Loads a user's data and statistics from the database.
          /// @param username The username to load.
          /// @return A User object with loaded attributes.
  User loadUserFromDatabase(const QString &username);

          /// @brief Saves a list of game records to the database.
          /// @param username Username of the player.
          /// @param history Game history to save.
          /// @return True if all records were saved successfully.
  bool saveGameHistoryToDatabase(const QString &username,
                                 const QList<GameRecord> &history);

          /// @brief Converts a vector of GameMove objects into a string.
          /// @param moves The moves to serialize.
          /// @return A compact string format for storage.
  QString movesToString(const QVector<GameMove> &moves) const;

          /// @brief Parses a move string back into a vector of GameMove.
          /// @param movesStr The serialized move data.
          /// @return Deserialized list of moves.
  QVector<GameMove> movesFromString(const QString &movesStr) const;
};

#endif  // USERMANAGER_H
