#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QString>


/// @brief Manages user authentication, session control, and data persistence.
class UserManager {
public:
    /// @brief Constructor: initializes database connection and path.
    UserManager();

    /// @brief Destructor: cleans up resources if needed.
    ~UserManager();

    /// @brief Registers a new user with given credentials.
    /// @param username New user's name.
    /// @param password New user's password (to be hashed).
    /// @param email Optional user email.
    /// @return True on successful registration.
    bool registerUser(const QString &username, const QString &password,
                      const QString &email = "");

    /// @brief Logs in a user if credentials match database records.
    /// @param username Username to authenticate.
    /// @param password Raw password to verify.
    /// @return True if login is successful.
    bool loginUser(const QString &username, const QString &password);

    /// @brief Logs out the current user (clears session).
    void logoutUser();

    /// @brief Checks if a user is currently logged in.
    /// @return True if a user session is active.
    bool isUserLoggedIn() const { return currentUser != nullptr; }

    /// @brief Gets a pointer to the currently logged-in user.
    /// @return Pointer to the current User object.
    User* getCurrentUser() const { return currentUser; }

    /// @brief Saves current user data and game history to the database.
    void saveUserData();

    /// @brief Loads user data from the database into memory.
    void loadUserData();

    /// @brief Checks if a username already exists in the database.
    /// @param username Username to check.
    /// @return True if the user exists.
    bool userExists(const QString &username) const;

    /// @brief Retrieves a list of all registered usernames.
    /// @return A list of usernames.
    QStringList getAllUsernames() const;

    /// @brief Loads the last few games played by the given user.
    /// @param username Target user.
    /// @return List of GameRecord objects.
    QList<GameRecord> loadGameHistoryFromDatabase(const QString &username);

    /// @brief Loads all games played by the given user.
    /// Useful for full replay or statistics.
    /// @param username Target user.
    /// @return Full game history list.
    QList<GameRecord> loadAllGameHistoryFromDatabase(const QString &username);

private:
    QSqlDatabase db;                  ///< SQLite database connection.
    User* currentUser;               ///< Pointer to currently logged-in user.
    QString dbPath;                  ///< Path to the database file.

    /// @brief Initializes database connection and file location.
    /// @return True if successful.
    bool initializeDatabase();

    /// @brief Hashes a plain-text password using SHA-256.
    /// @param password Raw password.
    /// @return Hashed password as hex string.
    QString hashPassword(const QString &password) const;

    /// @brief Creates required tables in the database (users, history).
    /// @return True if successful.
    bool createTables();

    /// @brief Saves a user's profile and statistics to the database.
    /// @param user User object to store.
    /// @return True if the operation succeeds.
    bool saveUserToDatabase(const User &user);

    /// @brief Loads a user and statistics from the database.
    /// @param username Username to load.
    /// @return User object with populated data.
    User loadUserFromDatabase(const QString &username);

    /// @brief Saves the user's game history to the database.
    /// @param username Owner of the history.
    /// @param history List of GameRecord entries.
    /// @return True if successful.
    bool saveGameHistoryToDatabase(const QString &username,
                                   const QList<GameRecord> &history);

    /// @brief Serializes a list of moves to a string for DB storage.
    /// @param moves Vector of GameMove.
    /// @return Encoded move string.
    QString movesToString(const QVector<GameMove> &moves) const;

    /// @brief Deserializes a string of moves from the database.
    /// @param movesStr Encoded move string.
    /// @return Vector of GameMove.
    QVector<GameMove> movesFromString(const QString &movesStr) const;


#endif  // USERMANAGER_H
