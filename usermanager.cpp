#include "usermanager.h"
#include <QSqlQuery>
<<<<<<< HEAD

<<<<<<< HEAD == == == =
=======
#include <QSqlError>
>>>>>>> Merna

>>>>>>> Merna
               /// @brief Constructs the UserManager object.
               /// Initializes the SQLite database and sets up necessary paths.
    UserManager::UserManager() : currentUser(nullptr) {
<<<<<<< HEAD
  // Set database path in user's documents folder
  QString documentsPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  QDir().mkpath(documentsPath + "/TicTacToe");
  dbPath = documentsPath + "/TicTacToe/tictactoe.db";
  == == == =
               // Set database path in user's documents folder
      QString documentsPath =
          QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  QDir().mkpath(documentsPath + "/TicTacToe");
  dbPath = documentsPath + "/TicTacToe/tictactoe.db";
>>>>>>> Merna

  initializeDatabase();
}

<<<<<<< HEAD
<<<<<<< HEAD
== == == =

>>>>>>> Merna
             /// @brief Destructor for UserManager.
             /// Saves user data and cleans up the database and memory.
    UserManager::~UserManager() {
<<<<<<< HEAD
=======
/// @brief Destructor for UserManager.
/// Saves user data and cleans up the database and memory.
UserManager::~UserManager()
{
>>>>>>> Merna
  if (currentUser) {
    saveUserData();
    delete currentUser;
  }

  if (db.isOpen()) {
    db.close();
  }
  == == == = if (currentUser) {
    saveUserData();
    delete currentUser;
  }

<<<<<<< HEAD
  == == == =

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
               if (db.isOpen()) {
    db.close();
  }
>>>>>>> Merna
}

/// @brief Initializes the SQLite database connection.
/// @return True if the database was opened and tables created successfully.
bool UserManager::initializeDatabase() {
<<<<<<< HEAD
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbPath);
  == == == = db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(dbPath);
>>>>>>> Merna

  if (!db.open()) {
    qDebug() << "Error opening database:" << db.lastError().text();
    return false;
  }

  return createTables();
}

<<<<<<< HEAD
<<<<<<< HEAD
== == == =

>>>>>>> Merna
             /// @brief Creates required database tables if they do not exist.
             /// @return True if tables were created or already exist.
    bool UserManager::createTables() {
<<<<<<< HEAD
=======
/// @brief Creates required database tables if they do not exist.
/// @return True if tables were created or already exist.
bool UserManager::createTables()
{
>>>>>>> Merna
  QSqlQuery query(db);

  // Create users table
  QString createUsersTable = R"(...)";  // See code for full definition

  if (!query.exec(createUsersTable)) {
    qDebug() << "Error creating users table:" << query.lastError().text();
    return false;
  }
<<<<<<< HEAD

  // Create game_history table with moves_data column
  QString createHistoryTable = R"(...)";
  == == == = QSqlQuery query(db);

  // Create users table
  QString createUsersTable = R"(...)";  // See code for full definition

<<<<<<< HEAD
  == == == =

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
               if (!query.exec(createUsersTable)) {
    qDebug() << "Error creating users table:" << query.lastError().text();
    return false;
  }
<<<<<<< HEAD
  == == == =
=======
>>>>>>> Merna

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
>>>>>>> Merna

               // Create game_history table with moves_data column
      QString createHistoryTable = R"(...)";

  if (!query.exec(createHistoryTable)) {
    qDebug() << "Error creating game_history table:"
             << query.lastError().text();
    return false;
  }

  // Try to add moves_data column if missing
  QString alterTable = "ALTER TABLE game_history ADD COLUMN moves_data TEXT";
  query.exec(alterTable);  // Silent fail if column exists

  // Add index for performance
  QString createIndex =
      "CREATE INDEX IF NOT EXISTS idx_game_history_username "
      "ON game_history(username)";
  if (!query.exec(createIndex)) {
    qDebug() << "Error creating index:" << query.lastError().text();
  }

<<<<<<< HEAD
<<<<<<< HEAD
  == == == =

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
               return true;
=======
  return true;
>>>>>>> Merna
}

/// @brief Registers a new user by inserting into the database.
/// @param username The new username.
/// @param password The new plain-text password.
/// @param email Optional email address.
/// @return True if registration was successful.
bool UserManager::registerUser(const QString& username, const QString& password,
                               const QString& email) {
  if (username.isEmpty() || password.isEmpty()) {
    return false;
  }

  if (userExists(username)) {
    return false;
  }

<<<<<<< HEAD
  == == == =

               if (userExists(username)) {
    return false;
  }

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
=======
>>>>>>> Merna
  QString hashedPassword = hashPassword(password);

  QSqlQuery query(db);
  query.prepare("INSERT INTO users (...)");
  query.addBindValue(username);
  query.addBindValue(hashedPassword);
  query.addBindValue(email);

  if (!query.exec()) {
    qDebug() << "Error registering user:" << query.lastError().text();
    return false;
  }

<<<<<<< HEAD
  == == == =

               if (!query.exec()) {
    qDebug() << "Error registering user:" << query.lastError().text();
    return false;
  }

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
=======
>>>>>>> Merna
  return true;
}

/// @brief Logs in a user if credentials are valid.
/// @param username The username to log in.
/// @param password The plain-text password to verify.
/// @return True if login was successful.
bool UserManager::loginUser(const QString& username, const QString& password) {
<<<<<<< HEAD
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

<<<<<<< HEAD
  // Update last login timestamp
=======
          // Update last login timestamp
>>>>>>> Merna
  QSqlQuery updateQuery(db);
  updateQuery.prepare(
      "UPDATE users SET last_login = CURRENT_TIMESTAMP "
      "WHERE username = ?");
  updateQuery.addBindValue(username);
  updateQuery.exec();

  // Load full user data
  if (currentUser) {
    delete currentUser;
  }

  currentUser = new User();
  *currentUser = loadUserFromDatabase(username);

  return true;
}

/// @brief Logs out the current user and saves their data.
void UserManager::logoutUser() {
  if (currentUser) {
    saveUserData();
    delete currentUser;
    currentUser = nullptr;
  }
}

/// @brief Checks if a username already exists in the database.
/// @param username The username to check.
/// @return True if user exists, false otherwise.
bool UserManager::userExists(const QString& username) const {
  QSqlQuery query(db);
  query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
  query.addBindValue(username);

  if (query.exec() && query.next()) {
    return query.value(0).toInt() > 0;
  }

  return false;
}

/// @brief Retrieves all registered usernames from the database.
/// @return A list of all usernames.
QStringList UserManager::getAllUsernames() const {
  QStringList usernames;
  QSqlQuery query("SELECT username FROM users ORDER BY username", db);

  while (query.next()) {
    usernames << query.value(0).toString();
  }

  return usernames;
}

/// @brief Hashes a plaintext password using SHA-256.
/// @param password The input plaintext password.
/// @return A hexadecimal hash string.
QString UserManager::hashPassword(const QString& password) const {
  QCryptographicHash hash(QCryptographicHash::Sha256);
  hash.addData(password.toUtf8());
  return hash.result().toHex();
}

/// @brief Saves the current user's data and game history to the database.
void UserManager::saveUserData() {
  if (!currentUser) return;

  saveUserToDatabase(*currentUser);
  saveGameHistoryToDatabase(currentUser->getUsername(),
                            currentUser->getGameHistory());
}

/// @brief Dummy compatibility method for future use.
/// Loads user data — currently a no-op.
void UserManager::loadUserData() {
  // This method is kept for interface compatibility
  // Data is loaded on-demand when user logs in
}

/// @brief Updates a user's statistics in the database.
/// @param user The User object containing updated data.
/// @return True if update was successful.
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
  == == == = if (!userExists(username)) { return false; }

<<<<<<< HEAD
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
  == == == =

               QString hashedPassword = hashPassword(password);
>>>>>>> Merna

  if (!query.exec()) {
    qDebug() << "Error saving user data:" << query.lastError().text();
    return false;
  }

<<<<<<< HEAD
  return true;
}

== == == = if (!query.exec() || !query.next()) { return false; }

QString storedPassword = query.value(0).toString();
if (storedPassword != hashedPassword) {
  return false;
}

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37

// Update last login timestamp
QSqlQuery updateQuery(db);
updateQuery.prepare(
    "UPDATE users SET last_login = CURRENT_TIMESTAMP "
    "WHERE username = ?");
updateQuery.addBindValue(username);
updateQuery.exec();

// Load full user data
if (currentUser) {
  delete currentUser;
}

<<<<<<< HEAD
currentUser = new User();
*currentUser = loadUserFromDatabase(username);

== == == =

             currentUser = new User();
*currentUser = loadUserFromDatabase(username);

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
return true;
}

/// @brief Logs out the current user and saves their data.
void UserManager::logoutUser() {
  if (currentUser) {
    saveUserData();
    delete currentUser;
    currentUser = nullptr;
  }
}

/// @brief Checks if a username already exists in the database.
/// @param username The username to check.
/// @return True if user exists, false otherwise.
bool UserManager::userExists(const QString& username) const {
  QSqlQuery query(db);
  query.prepare("SELECT COUNT(*) FROM users WHERE username = ?");
  query.addBindValue(username);

<<<<<<< HEAD
  if (query.exec() && query.next()) {
    return query.value(0).toInt() > 0;
  }

  == == == =

               if (query.exec() && query.next()) {
    return query.value(0).toInt() > 0;
  }

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
  return false;
}

/// @brief Retrieves all registered usernames from the database.
/// @return A list of all usernames.
QStringList UserManager::getAllUsernames() const {
  QStringList usernames;
  QSqlQuery query("SELECT username FROM users ORDER BY username", db);

<<<<<<< HEAD
  while (query.next()) {
    usernames << query.value(0).toString();
  }

  == == == =

               while (query.next()) {
    usernames << query.value(0).toString();
  }

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
  return usernames;
}

/// @brief Hashes a plaintext password using SHA-256.
/// @param password The input plaintext password.
/// @return A hexadecimal hash string.
QString UserManager::hashPassword(const QString& password) const {
  QCryptographicHash hash(QCryptographicHash::Sha256);
  hash.addData(password.toUtf8());
  return hash.result().toHex();
}

/// @brief Saves the current user's data and game history to the database.
void UserManager::saveUserData() {
  if (!currentUser) return;

  saveUserToDatabase(*currentUser);
  saveGameHistoryToDatabase(currentUser->getUsername(),
                            currentUser->getGameHistory());
}

/// @brief Dummy compatibility method for future use.
/// Loads user data — currently a no-op.
void UserManager::loadUserData() {
  // This method is kept for interface compatibility
  // Data is loaded on-demand when user logs in
}

/// @brief Updates a user's statistics in the database.
/// @param user The User object containing updated data.
/// @return True if update was successful.
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

<<<<<<< HEAD
  == == == =

               query.addBindValue(user.getEmail());
  query.addBindValue(user.getGamesWon());
  query.addBindValue(user.getGamesLost());
  query.addBindValue(user.getGamesTied());
  query.addBindValue(user.getUsername());

  if (!query.exec()) {
    qDebug() << "Error saving user data:" << query.lastError().text();
    return false;
  }

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
  return true;
}


>>>>>>> Merna
=======
  return true;
}

>>>>>>> Merna
/// @brief Loads a User object from the database using the username.
/// @param username The target username.
/// @return A populated User object.
User UserManager::loadUserFromDatabase(const QString& username) {
<<<<<<< HEAD
  User user;

  QSqlQuery query(db);
  query.prepare(R"(
      SELECT username, password, email, last_login,
             games_won, games_lost, games_tied
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
    == == == = User user;

    QSqlQuery query(db);
    query.prepare(R"(
      SELECT username, password, email, last_login,
             games_won, games_lost, games_tied
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
>>>>>>> Merna

      for (int i = 0; i < gamesWon; i++) user.addWin();
      for (int i = 0; i < gamesLost; i++) user.addLoss();
      for (int i = 0; i < gamesTied; i++) user.addTie();

<<<<<<< HEAD
<<<<<<< HEAD
      QList<GameRecord> history = loadGameHistoryFromDatabase(username);
      user.setGameHistory(history);
    }

    return user;
  }

  == == == = QList<GameRecord> history = loadGameHistoryFromDatabase(username);
  user.setGameHistory(history);
}

<<<<<<< HEAD
== == == =

             if (query.exec() && query.next()) {
  user.setUsername(query.value("username").toString());
  user.setPassword(query.value("password").toString());
  user.setEmail(query.value("email").toString());
  user.setLastLogin(query.value("last_login").toDateTime());

  // Load game statistics
  int gamesWon = query.value("games_won").toInt();
  int gamesLost = query.value("games_lost").toInt();
  int gamesTied = query.value("games_tied").toInt();

  for (int i = 0; i < gamesWon; i++) user.addWin();
  for (int i = 0; i < gamesLost; i++) user.addLoss();
  for (int i = 0; i < gamesTied; i++) user.addTie();

  // Load only last 5 games for display, but keep reference to all
  QList<GameRecord> history = loadGameHistoryFromDatabase(username);
  user.setGameHistory(history);
}

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
return user;
}


>>>>>>> Merna
=======
  return user;
}

>>>>>>> Merna
/// @brief Saves all game records for a user to the database.
/// @param username The user's username.
/// @param history List of GameRecord objects.
/// @return True if all records were saved successfully.
bool UserManager::saveGameHistoryToDatabase(const QString& username,
                                            const QList<GameRecord>& history) {
<<<<<<< HEAD
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
      INSERT INTO game_history (username, timestamp, opponent,
                                result, game_mode, player_symbol,
                                moves_data)
      VALUES (?, ?, ?, ?, ?, ?, ?)
  )");
  == == == = QSqlQuery deleteQuery(db);
  deleteQuery.prepare("DELETE FROM game_history WHERE username = ?");
  deleteQuery.addBindValue(username);
>>>>>>> Merna

  for (const GameRecord& record : history) {
    insertQuery.addBindValue(username);
    insertQuery.addBindValue(record.timestamp);
    insertQuery.addBindValue(record.opponent);
    insertQuery.addBindValue(record.result);
    insertQuery.addBindValue(record.gameMode);
    insertQuery.addBindValue(record.playerSymbol);
    insertQuery.addBindValue(movesToString(record.moves));

<<<<<<< HEAD
<<<<<<< HEAD
    == == == = QSqlQuery insertQuery(db);
    insertQuery.prepare(R"(
      INSERT INTO game_history (username, timestamp, opponent,
                                result, game_mode, player_symbol,
                                moves_data)
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

<<<<<<< HEAD
      if (!insertQuery.exec()) {
        qDebug() << "Error saving game record:"
                 << insertQuery.lastError().text();
        return false;
      }
    }

    == == == =
>>>>>>> Merna
                 if (!insertQuery.exec()) {
      qDebug() << "Error saving game record:" << insertQuery.lastError().text();
=======
    if (!insertQuery.exec()) {
      qDebug() << "Error saving game record:"
               << insertQuery.lastError().text();
>>>>>>> Merna
      return false;
    }
<<<<<<< HEAD
    == == == =

>>>>>>> Merna
  }

<<<<<<< HEAD
<<<<<<< HEAD
  return true;
}

== == == =
>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
             return true;
}


>>>>>>> Merna
=======
  return true;
}

>>>>>>> Merna
/// @brief Loads the last 5 game records from the database.
/// @param username The target username.
/// @return A list of GameRecord objects.
QList<GameRecord> UserManager::loadGameHistoryFromDatabase(
    const QString& username) {
  QList<GameRecord> history;

  QSqlQuery query(db);
  query.prepare(R"(
      SELECT timestamp, opponent, result,
             game_mode, player_symbol, moves_data
      FROM game_history
      WHERE username = ?
      ORDER BY timestamp DESC
      LIMIT 5
  )");
<<<<<<< HEAD
  query.addBindValue(username);

  if (query.exec()) {
    while (query.next()) {
      GameRecord record;
      record.timestamp = query.value("timestamp").toDateTime();
      record.opponent = query.value("opponent").toString();
      record.result = query.value("result").toString();
      record.gameMode = query.value("game_mode").toString();
      record.playerSymbol = query.value("player_symbol").toString();
      == == == = query.addBindValue(username);
>>>>>>> Merna

      QString movesData = query.value("moves_data").toString();
      if (!movesData.isEmpty()) {
        record.moves = movesFromString(movesData);
      }

      history.append(record);
    }
  } else {
    qDebug() << "Error loading game history:" << query.lastError().text();
<<<<<<< HEAD
    == == == = QString movesData = query.value("moves_data").toString();
    if (!movesData.isEmpty()) {
      record.moves = movesFromString(movesData);
    }

<<<<<<< HEAD
    history.append(record);
>>>>>>> Merna
  }

<<<<<<< HEAD
  return history;
  == == == = == == == = history.append(record);
}
}
else {
  qDebug() << "Error loading game history:" << query.lastError().text();
}
}
else {
  qDebug() << "Error loading game history:" << query.lastError().text();
}

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
return history;
>>>>>>> Merna
=======
  }

  return history;
>>>>>>> Merna
}

/// @brief Loads all game records (not limited) for a user.
/// @param username The target username.
/// @return A list of all GameRecord objects.
QList<GameRecord> UserManager::loadAllGameHistoryFromDatabase(
    const QString& username) {
  QList<GameRecord> history;

  QSqlQuery query(db);
  query.prepare(R"(
      SELECT timestamp, opponent, result,
             game_mode, player_symbol, moves_data
      FROM game_history
      WHERE username = ?
      ORDER BY timestamp DESC
  )");
<<<<<<< HEAD
  query.addBindValue(username);

  if (query.exec()) {
    while (query.next()) {
      GameRecord record;
      record.timestamp = query.value("timestamp").toDateTime();
      record.opponent = query.value("opponent").toString();
      record.result = query.value("result").toString();
      record.gameMode = query.value("game_mode").toString();
      record.playerSymbol = query.value("player_symbol").toString();
      == == == = query.addBindValue(username);
>>>>>>> Merna

      QString movesData = query.value("moves_data").toString();
      if (!movesData.isEmpty()) {
        record.moves = movesFromString(movesData);
      }

      history.append(record);
    }
  } else {
    qDebug() << "Error loading all game history:" << query.lastError().text();
<<<<<<< HEAD
    == == == = QString movesData = query.value("moves_data").toString();
    if (!movesData.isEmpty()) {
      record.moves = movesFromString(movesData);
    }

<<<<<<< HEAD
    history.append(record);
>>>>>>> Merna
  }

<<<<<<< HEAD
  return history;
}

== == == = == == == = history.append(record);
}
}
else {
  qDebug() << "Error loading all game history:" << query.lastError().text();
}
}
else {
  qDebug() << "Error loading all game history:" << query.lastError().text();
}

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
return history;
}


>>>>>>> Merna
=======
  }

  return history;
}

>>>>>>> Merna
/// @brief Serializes a vector of moves into a string for database storage.
/// @param moves A vector of GameMove.
/// @return A semicolon-separated move string.
QString UserManager::movesToString(const QVector<GameMove>& moves) const {
  QStringList moveStrings;
  for (const GameMove& move : moves) {
    moveStrings
        << QString("%1,%2,%3").arg(move.row).arg(move.col).arg(move.player);
  }
  return moveStrings.join(";");
}

/// @brief Deserializes a string of moves from the database into a vector.
/// @param movesStr The move string from the database.
/// @return A QVector of GameMove objects.
QVector<GameMove> UserManager::movesFromString(const QString& movesStr) const {
<<<<<<< HEAD
  QVector<GameMove> moves;
  if (movesStr.isEmpty()) return moves;

<<<<<<< HEAD
  == == == = QVector<GameMove> moves;
  if (movesStr.isEmpty()) return moves;

<<<<<<< HEAD
=======
>>>>>>> Merna
  QStringList moveStrings = movesStr.split(";");
  for (const QString& moveStr : moveStrings) {
    QStringList parts = moveStr.split(",");
    if (parts.size() == 3) {
      GameMove move;
      move.row = parts[0].toInt();
      move.col = parts[1].toInt();
      move.player = parts[2].toInt();
      moves.append(move);
<<<<<<< HEAD
      == == == =
>>>>>>> Merna
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
<<<<<<< HEAD
        == == == =

>>>>>>> 2cb39a158de34bd848a6f2e8c8e460bfe63ddc37
>>>>>>> Merna
      }
      return moves;
=======
>>>>>>> Merna
    }
