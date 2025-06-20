#include "usermanager.h"

#include <QSqlError>
#include <QSqlQuery>

// Constructor: sets up database path and initializes database
UserManager::UserManager() : currentUser(nullptr) {
  // Set database path in user's documents folder
  QString documentsPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  QDir().mkpath(documentsPath + "/TicTacToe");
  dbPath = documentsPath + "/TicTacToe/tictactoe.db";

  initializeDatabase();
}

// Destructor: saves user data and closes database connection
UserManager::~UserManager() {
  if (currentUser) {
    saveUserData();
    delete currentUser;
  }

  if (db.isOpen()) {
    db.close();
  }
}

// Initializes SQLite connection and creates required tables
bool UserManager::initializeDatabase() {
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbPath);

  if (!db.open()) {
    qDebug() << "Error opening database:" << db.lastError().text();
    return false;
  }

  return createTables();
}

// Creates 'users' and 'game_history' tables if not existing
bool UserManager::createTables() {
  QSqlQuery query(db);

          // Create users table
  QString createUsersTable = R"(
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password TEXT NOT NULL,
            email TEXT,
            last_login DATETIME,
            games_won INTEGER DEFAULT 0,
            games_lost INTEGER DEFAULT 0,
            games_tied INTEGER DEFAULT 0,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP
        )
    )";

  if (!query.exec(createUsersTable)) {
    qDebug() << "Error creating users table:" << query.lastError().text();
    return false;
  }

          // Create game history table
  QString createHistoryTable = R"(
        CREATE TABLE IF NOT EXISTS game_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT NOT NULL,
            timestamp DATETIME NOT NULL,
            opponent TEXT NOT NULL,
            result TEXT NOT NULL,
            game_mode TEXT NOT NULL,
            player_symbol TEXT NOT NULL,
            moves_data TEXT,
            FOREIGN KEY (username) REFERENCES users(username)
        )
    )";

  if (!query.exec(createHistoryTable)) {
    qDebug() << "Error creating game_history table:"
             << query.lastError().text();
    return false;
  }

          // Attempt to add moves_data column if missing
  QString alterTable = "ALTER TABLE game_history ADD COLUMN moves_data TEXT";
  query.exec(alterTable);  // Fails silently if already added

          // Create index for faster username lookups in game history
  QString createIndex =
      "CREATE INDEX IF NOT EXISTS idx_game_history_username ON "
      "game_history(username)";
  if (!query.exec(createIndex)) {
    qDebug() << "Error creating index:" << query.lastError().text();
  }

  return true;
}

// Registers a new user after validating input and uniqueness
bool UserManager::registerUser(const QString& username,
                               const QString& password,
                               const QString& email) {
  if (username.isEmpty() || password.isEmpty()) {
    return false;
  }

  if (userExists(username)) {
    return false;
  }

  QString hashedPassword = hashPassword(password);

  QSqlQuery query(db);
  query.prepare(
      "INSERT INTO users (username, password, email, last_login) VALUES (?, ?, "
      "?, CURRENT_TIMESTAMP)");
  query.addBindValue(username);
  query.addBindValue(hashedPassword);
  query.addBindValue(email);

  if (!query.exec()) {
    qDebug() << "Error registering user:" << query.lastError().text();
    return false;
  }

  return true;
}

// Logs in user by verifying credentials and loading profile
bool UserManager::loginUser(const QString& username,
                            const QString& password) {
  if (!userExists(username)) {
    return false;
  }

  QString hashedPassword = hashPassword(password);

  QSqlQuery query(db);
  query.prepare("SELECT password FROM users WHERE username = ?");
  query.addBindValue(username);

  if (!query.exec() || !query.next()) {
    return false;
  }

  QString storedPassword = query.value(0).toString();
  if (storedPassword != hashedPassword) {
    return false;
  }

          // Update login timestamp
  QSqlQuery updateQuery(db);
  updateQuery.prepare(
      "UPDATE users SET last_login = CURRENT_TIMESTAMP WHERE username = ?");
  updateQuery.addBindValue(username);
  updateQuery.exec();

          // Replace current user with loaded profile
  if (currentUser) {
    delete currentUser;
  }

  currentUser = new User();
  *currentUser = loadUserFromDatabase(username);

  return true;
}

// Logs out current user and saves their data
void UserManager::logoutUser() {
  if (currentUser) {
    saveUserData();
    delete currentUser;
    currentUser = nullptr;
  }
}

// Checks if a username already exists in the database
bool UserManager::userExists(const QString& username) const {
  QSqlQuery query(db);
  query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
  query.addBindValue(username);

  if (query.exec() && query.next()) {
    return query.value(0).toInt() > 0;
  }

  return false;
}

// Returns a list of all registered usernames
QStringList UserManager::getAllUsernames() const {
  QStringList usernames;
  QSqlQuery query("SELECT username FROM users ORDER BY username", db);

  while (query.next()) {
    usernames << query.value(0).toString();
  }

  return usernames;
}

// Returns SHA-256 hash of a given password
QString UserManager::hashPassword(const QString& password) const {
  QCryptographicHash hash(QCryptographicHash::Sha256);
  hash.addData(password.toUtf8());
  return hash.result().toHex();
}

// Saves current user data and game history to the database
void UserManager::saveUserData() {
  if (!currentUser) return;

  saveUserToDatabase(*currentUser);
  saveGameHistoryToDatabase(currentUser->getUsername(),
                            currentUser->getGameHistory());
}

// Placeholder method for future expansion
void UserManager::loadUserData() {
  // This method is kept for interface compatibility
  // Data is loaded on-demand when user logs in
}

// Updates user statistics and email in database
bool UserManager::saveUserToDatabase(const User& user) {
  QSqlQuery query(db);
  query.prepare(R"(
        UPDATE users
        SET email = ?, games_won = ?, games_lost = ?, games_tied = ?
        WHERE username = ?
    )");

  query.addBindValue(user.getEmail());
  query.addBindValue(user.getGamesWon());
  query.addBindValue(user.getGamesLost());
  query.addBindValue(user.getGamesTied());
  query.addBindValue(user.getUsername());

  if (!query.exec()) {
    qDebug() << "Error saving user data:" << query.lastError().text();
    return false;
  }

  return true;
}

// Loads user profile, stats, and game history from database
User UserManager::loadUserFromDatabase(const QString& username) {
  User user;

  QSqlQuery query(db);
  query.prepare(R"(
        SELECT username, password, email, last_login, games_won, games_lost, games_tied
        FROM users WHERE username = ?
    )");
  query.addBindValue(username);

  if (query.exec() && query.next()) {
    user.setUsername(query.value("username").toString());
    user.setPassword(query.value("password").toString());
    user.setEmail(query.value("email").toString());
    user.setLastLogin(query.value("last_login").toDateTime());

    int gamesWon = query.value("games_won").toInt();
    int gamesLost = query.value("games_lost").toInt();
    int gamesTied = query.value("games_tied").toInt();

    for (int i = 0; i < gamesWon; i++) user.addWin();
    for (int i = 0; i < gamesLost; i++) user.addLoss();
    for (int i = 0; i < gamesTied; i++) user.addTie();

            // Load recent game history for display
    QList<GameRecord> history = loadGameHistoryFromDatabase(username);
    user.setGameHistory(history);
  }

  return user;
}

// Saves all games in user's history to the database
bool UserManager::saveGameHistoryToDatabase(
    const QString& username, const QList<GameRecord>& history) {
  QSqlQuery deleteQuery(db);
  deleteQuery.prepare("DELETE FROM game_history WHERE username = ?");
  deleteQuery.addBindValue(username);

  if (!deleteQuery.exec()) {
    qDebug() << "Error deleting old game history:"
             << deleteQuery.lastError().text();
    return false;
  }

  QSqlQuery insertQuery(db);
  insertQuery.prepare(R"(
        INSERT INTO game_history (username, timestamp, opponent, result, game_mode, player_symbol, moves_data)
        VALUES (?, ?, ?, ?, ?, ?, ?)
    )");

  for (const GameRecord& record : history) {
    insertQuery.addBindValue(username);
    insertQuery.addBindValue(record.timestamp);
    insertQuery.addBindValue(record.opponent);
    insertQuery.addBindValue(record.result);
    insertQuery.addBindValue(record.gameMode);
    insertQuery.addBindValue(record.playerSymbol);
    insertQuery.addBindValue(movesToString(record.moves));

    if (!insertQuery.exec()) {
      qDebug() << "Error saving game record:" << insertQuery.lastError().text();
      return false;
    }
  }

  return true;
}

// Loads the last 5 game records for a user
QList<GameRecord> UserManager::loadGameHistoryFromDatabase(
    const QString& username) {
  QList<GameRecord> history;

  QSqlQuery query(db);
  query.prepare(R"(
        SELECT timestamp, opponent, result, game_mode, player_symbol, moves_data
        FROM game_history
        WHERE username = ?
        ORDER BY timestamp DESC
        LIMIT 5
    )");
  query.addBindValue(username);

  if (query.exec()) {
    while (query.next()) {
      GameRecord record;
      record.timestamp = query.value("timestamp").toDateTime();
      record.opponent = query.value("opponent").toString();
      record.result = query.value("result").toString();
      record.gameMode = query.value("game_mode").toString();
      record.playerSymbol = query.value("player_symbol").toString();

      QString movesData = query.value("moves_data").toString();
      if (!movesData.isEmpty()) {
        record.moves = movesFromString(movesData);
      }

      history.append(record);
    }
  } else {
    qDebug() << "Error loading game history:" << query.lastError().text();
  }

  return history;
}

// Loads all game records ever played by a user
QList<GameRecord> UserManager::loadAllGameHistoryFromDatabase(
    const QString& username) {
  QList<GameRecord> history;

  QSqlQuery query(db);
  query.prepare(R"(
        SELECT timestamp, opponent, result, game_mode, player_symbol, moves_data
        FROM game_history
        WHERE username = ?
        ORDER BY timestamp DESC
    )");
  query.addBindValue(username);

  if (query.exec()) {
    while (query.next()) {
      GameRecord record;
      record.timestamp = query.value("timestamp").toDateTime();
      record.opponent = query.value("opponent").toString();
      record.result = query.value("result").toString();
      record.gameMode = query.value("game_mode").toString();
      record.playerSymbol = query.value("player_symbol").toString();

      QString movesData = query.value("moves_data").toString();
      if (!movesData.isEmpty()) {
        record.moves = movesFromString(movesData);
      }

      history.append(record);
    }
  } else {
    qDebug() << "Error loading all game history:" << query.lastError().text();
  }

  return history;
}

// Converts move list to semicolon-separated string
QString UserManager::movesToString(const QVector<GameMove>& moves) const {
  QStringList moveStrings;
  for (const GameMove& move : moves) {
    moveStrings
        << QString("%1,%2,%3").arg(move.row).arg(move.col).arg(move.player);
  }
  return moveStrings.join(";");
}

// Converts serialized move string back to move list
QVector<GameMove> UserManager::movesFromString(
    const QString& movesStr) const {
  QVector<GameMove> moves;
  if (movesStr.isEmpty()) return moves;

  QStringList moveStrings = movesStr.split(";");
  for (const QString& moveStr : moveStrings) {
    QStringList parts = moveStr.split(",");
    if (parts.size() == 3) {
      GameMove move;
      move.row = parts[0].toInt();
      move.col = parts[1].toInt();
      move.player = parts[2].toInt();
      moves.append(move);
    }
  }
  return moves;
}
