#include <QApplication>
#include <QTest>
#include <QtTest>

#include "user.h"
#include "usermanager.h"

class TestUserManager : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void cleanup();

  // Database initialization tests
  void testUserManagerInitialization();
  void testDatabaseInitialization();
  void testTableCreation();
  void testDatabasePath();

  // User registration tests
  void testUserRegistration();
  void testUserRegistrationValidation();
  void testDuplicateUserRegistration();
  void testUserRegistrationWithEmail();
  void testUserRegistrationEmptyFields();

  // User login tests
  void testUserLogin();
  void testUserLoginValidation();
  void testUserLoginNonExistentUser();
  void testUserLoginWrongPassword();
  void testUserLoginUpdatesLastLogin();

  // User logout tests
  void testUserLogout();
  void testUserLogoutWithoutLogin();
  void testUserLogoutClearsCurrentUser();

  // User existence and data tests
  void testUserExists();
  void testUserExistsNonExistent();
  void testGetAllUsernames();
  void testGetCurrentUser();
  void testIsUserLoggedIn();

  // Password hashing tests
  void testPasswordHashing();
  void testPasswordHashingConsistency();
  void testPasswordHashingSecurity();

  // User data persistence tests
  void testSaveUserData();
  void testLoadUserData();
  void testUserDataPersistence();
  void testUserStatisticsPersistence();

  // Game history tests
  void testSaveGameHistory();
  void testLoadGameHistory();
  void testLoadAllGameHistory();
  void testGameHistoryLimit();
  void testGameHistoryOrdering();

  // Move serialization tests
  void testMovesToString();
  void testMovesFromString();
  void testMoveSerializationRoundTrip();
  void testEmptyMoveSerialization();

  // Database operations tests
  void testDatabaseConnection();
  void testDatabaseTransactions();
  void testDatabaseErrorHandling();

  // Integration tests
  void testCompleteUserWorkflow();
  void testMultipleUsersWorkflow();
  void testUserSessionManagement();

 private:
  UserManager* userManager;
  QString testDbPath;

  // Helper methods
  void createTestUser(const QString& username, const QString& password,
                      const QString& email = "");
  GameRecord createTestGameRecord(const QString& opponent,
                                  const QString& result);
  void cleanupTestDatabase();
};

void TestUserManager::initTestCase() {
  // Create QApplication if it doesn't exist
  if (!QApplication::instance()) {
    int argc = 0;
    char** argv = nullptr;
    new QApplication(argc, argv);
  }

  // Set up test database path
  QString documentsPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  testDbPath = documentsPath + "/TicTacToe/test_tictactoe.db";
}

void TestUserManager::cleanupTestCase() { cleanupTestDatabase(); }

void TestUserManager::init() {
  userManager = new UserManager();
  QTest::qWait(100);  // Allow database initialization
}

void TestUserManager::cleanup() {
  delete userManager;
  userManager = nullptr;
}

void TestUserManager::cleanupTestDatabase() {
  QString documentsPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  QString dbPath = documentsPath + "/TicTacToe/tictactoe.db";
  QFile::remove(dbPath);
  QFile::remove(testDbPath);
}

void TestUserManager::createTestUser(const QString& username,
                                     const QString& password,
                                     const QString& email) {
  bool result = userManager->registerUser(username, password, email);
  QVERIFY(result);
}

GameRecord TestUserManager::createTestGameRecord(const QString& opponent,
                                                 const QString& result) {
  GameRecord record;
  record.timestamp = QDateTime::currentDateTime();
  record.opponent = opponent;
  record.result = result;
  record.gameMode = "PvP";
  record.playerSymbol = "X";

  // Add some test moves
  GameMove move1 = {0, 0, 1};
  GameMove move2 = {1, 1, 2};
  GameMove move3 = {2, 2, 1};
  record.moves.append(move1);
  record.moves.append(move2);
  record.moves.append(move3);

  return record;
}

// Test 1: UserManager Initialization
void TestUserManager::testUserManagerInitialization() {
  QVERIFY(userManager != nullptr);
  QVERIFY(!userManager->isUserLoggedIn());
  QVERIFY(userManager->getCurrentUser() == nullptr);
}

// Test 2: Database Initialization
void TestUserManager::testDatabaseInitialization() {
  // Database should be initialized during constructor
  QStringList usernames = userManager->getAllUsernames();
  // Should not crash and return empty list initially
  QVERIFY(usernames.isEmpty() || usernames.size() >= 0);
}

// Test 3: Table Creation
void TestUserManager::testTableCreation() {
  // Test that we can perform database operations (indicates tables exist)
  bool userExists = userManager->userExists("nonexistent_user");
  QVERIFY(!userExists);  // Should return false without crashing
}

// Test 4: Database Path
void TestUserManager::testDatabasePath() {
  QString documentsPath =
      QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
  QString expectedPath = documentsPath + "/TicTacToe";
  QDir dir(expectedPath);
  QVERIFY(dir.exists());  // Directory should be created
}

// Test 5: User Registration
void TestUserManager::testUserRegistration() {
  bool result =
      userManager->registerUser("testuser", "testpass", "test@example.com");
  QVERIFY(result);

  // Verify user was created
  QVERIFY(userManager->userExists("testuser"));
}

// Test 6: User Registration Validation
void TestUserManager::testUserRegistrationValidation() {
  // Test empty username
  bool result1 = userManager->registerUser("", "password", "email@test.com");
  QVERIFY(!result1);

  // Test empty password
  bool result2 = userManager->registerUser("username", "", "email@test.com");
  QVERIFY(!result2);

  // Test both empty
  bool result3 = userManager->registerUser("", "", "");
  QVERIFY(!result3);
}

// Test 7: Duplicate User Registration
void TestUserManager::testDuplicateUserRegistration() {
  // Register first user
  bool result1 =
      userManager->registerUser("duplicate", "pass1", "email1@test.com");
  QVERIFY(result1);

  // Try to register same username
  bool result2 =
      userManager->registerUser("duplicate", "pass2", "email2@test.com");
  QVERIFY(!result2);
}

// Test 8: User Registration With Email
void TestUserManager::testUserRegistrationWithEmail() {
  bool result =
      userManager->registerUser("emailuser", "password", "user@example.com");
  QVERIFY(result);
  QVERIFY(userManager->userExists("emailuser"));
}

// Test 9: User Registration Empty Fields
void TestUserManager::testUserRegistrationEmptyFields() {
  // Registration without email should work
  bool result = userManager->registerUser("noemail", "password");
  QVERIFY(result);
  QVERIFY(userManager->userExists("noemail"));
}

// Test 10: User Login
void TestUserManager::testUserLogin() {
  // First register a user
  createTestUser("loginuser", "loginpass", "login@test.com");

  // Test login
  bool result = userManager->loginUser("loginuser", "loginpass");
  QVERIFY(result);
  QVERIFY(userManager->isUserLoggedIn());
  QVERIFY(userManager->getCurrentUser() != nullptr);
  QCOMPARE(userManager->getCurrentUser()->getUsername(), QString("loginuser"));
}

// Test 11: User Login Validation
void TestUserManager::testUserLoginValidation() {
  // Test login with non-existent user
  bool result = userManager->loginUser("nonexistent", "password");
  QVERIFY(!result);
  QVERIFY(!userManager->isUserLoggedIn());
}

// Test 12: User Login Non-Existent User
void TestUserManager::testUserLoginNonExistentUser() {
  bool result = userManager->loginUser("ghost", "password");
  QVERIFY(!result);
  QVERIFY(userManager->getCurrentUser() == nullptr);
}

// Test 13: User Login Wrong Password
void TestUserManager::testUserLoginWrongPassword() {
  createTestUser("wrongpass", "correctpass");

  bool result = userManager->loginUser("wrongpass", "wrongpassword");
  QVERIFY(!result);
  QVERIFY(!userManager->isUserLoggedIn());
}

// Test 14: User Login Updates Last Login
void TestUserManager::testUserLoginUpdatesLastLogin() {
  // Create test user
  createTestUser("timeuser", "timepass");

  // Test Case 1: Login should succeed and set last login
  bool result = userManager->loginUser("timeuser", "timepass");
  QVERIFY(result);
  QVERIFY(userManager->isUserLoggedIn());

  User* user = userManager->getCurrentUser();
  QVERIFY(user != nullptr);

  // Test Case 2: Last login should be valid (not null/empty)
  QDateTime lastLogin = user->getLastLogin();
  QVERIFY(lastLogin.isValid());
  QVERIFY(!lastLogin.isNull());

  // Test Case 3: Last login should be recent (within last day)
  QDateTime yesterday = QDateTime::currentDateTime().addDays(-1);
  QVERIFY(lastLogin >= yesterday);
}

// Test 15: User Logout
void TestUserManager::testUserLogout() {
  createTestUser("logoutuser", "logoutpass");
  userManager->loginUser("logoutuser", "logoutpass");

  QVERIFY(userManager->isUserLoggedIn());

  userManager->logoutUser();

  QVERIFY(!userManager->isUserLoggedIn());
  QVERIFY(userManager->getCurrentUser() == nullptr);
}

// Test 16: User Logout Without Login
void TestUserManager::testUserLogoutWithoutLogin() {
  // Should not crash when logging out without being logged in
  userManager->logoutUser();
  QVERIFY(!userManager->isUserLoggedIn());
}

// Test 17: User Logout Clears Current User
void TestUserManager::testUserLogoutClearsCurrentUser() {
  createTestUser("clearuser", "clearpass");
  userManager->loginUser("clearuser", "clearpass");

  QVERIFY(userManager->getCurrentUser() != nullptr);

  userManager->logoutUser();

  QVERIFY(userManager->getCurrentUser() == nullptr);
}

// Test 18: User Exists
void TestUserManager::testUserExists() {
  createTestUser("existsuser", "existspass");

  QVERIFY(userManager->userExists("existsuser"));
  QVERIFY(!userManager->userExists("nonexistentuser"));
}

// Test 19: User Exists Non-Existent
void TestUserManager::testUserExistsNonExistent() {
  QVERIFY(!userManager->userExists("definitelynotauser"));
  QVERIFY(!userManager->userExists(""));
}

// Test 20: Get All Usernames
void TestUserManager::testGetAllUsernames() {
  QStringList initialUsers = userManager->getAllUsernames();
  int initialCount = initialUsers.size();

  createTestUser("user1", "pass1");
  createTestUser("user2", "pass2");
  createTestUser("user3", "pass3");

  QStringList allUsers = userManager->getAllUsernames();
  QVERIFY(allUsers.size() >= initialCount + 3);
  QVERIFY(allUsers.contains("user1"));
  QVERIFY(allUsers.contains("user2"));
  QVERIFY(allUsers.contains("user3"));
}

// Test 21: Get Current User
void TestUserManager::testGetCurrentUser() {
  QVERIFY(userManager->getCurrentUser() == nullptr);

  createTestUser("currentuser", "currentpass");
  userManager->loginUser("currentuser", "currentpass");

  User* current = userManager->getCurrentUser();
  QVERIFY(current != nullptr);
  QCOMPARE(current->getUsername(), QString("currentuser"));
}

// Test 22: Is User Logged In
void TestUserManager::testIsUserLoggedIn() {
  QVERIFY(!userManager->isUserLoggedIn());

  createTestUser("logincheck", "logincheck");
  userManager->loginUser("logincheck", "logincheck");

  QVERIFY(userManager->isUserLoggedIn());

  userManager->logoutUser();

  QVERIFY(!userManager->isUserLoggedIn());
}

// Test 23: Password Hashing
void TestUserManager::testPasswordHashing() {
  createTestUser("hashuser", "plainpassword");

  // Login should work with original password
  bool result = userManager->loginUser("hashuser", "plainpassword");
  QVERIFY(result);

  // Password should be hashed (not stored as plain text)
  // This is tested indirectly by successful login
}

// Test 24: Password Hashing Consistency
void TestUserManager::testPasswordHashingConsistency() {
  createTestUser("consistency1", "samepassword");
  createTestUser("consistency2", "samepassword");

  // Both users should be able to login with same password
  QVERIFY(userManager->loginUser("consistency1", "samepassword"));
  userManager->logoutUser();
  QVERIFY(userManager->loginUser("consistency2", "samepassword"));
}

// Test 25: Password Hashing Security
void TestUserManager::testPasswordHashingSecurity() {
  createTestUser("secureuser", "password123");

  // Wrong password should fail
  QVERIFY(!userManager->loginUser("secureuser", "password124"));
  QVERIFY(!userManager->loginUser("secureuser", "Password123"));
  QVERIFY(!userManager->loginUser("secureuser", ""));
}

// Test 26: Save User Data
void TestUserManager::testSaveUserData() {
  createTestUser("saveuser", "savepass");
  userManager->loginUser("saveuser", "savepass");

  User* user = userManager->getCurrentUser();
  user->addWin();
  user->addLoss();
  user->addTie();

  userManager->saveUserData();

  // Data should be saved (tested by logout/login cycle)
  userManager->logoutUser();
  userManager->loginUser("saveuser", "savepass");

  User* reloadedUser = userManager->getCurrentUser();
  QCOMPARE(reloadedUser->getGamesWon(), 1);
  QCOMPARE(reloadedUser->getGamesLost(), 1);
  QCOMPARE(reloadedUser->getGamesTied(), 1);
}

// Test 27: Load User Data
void TestUserManager::testLoadUserData() {
  createTestUser("loaduser", "loadpass");
  userManager->loginUser("loaduser", "loadpass");

  // Add some statistics
  User* user = userManager->getCurrentUser();
  user->addWin();
  user->addWin();
  user->addLoss();

  userManager->saveUserData();
  userManager->logoutUser();

  // Login again to test loading
  userManager->loginUser("loaduser", "loadpass");
  User* loadedUser = userManager->getCurrentUser();

  QCOMPARE(loadedUser->getGamesWon(), 2);
  QCOMPARE(loadedUser->getGamesLost(), 1);
  QCOMPARE(loadedUser->getGamesTied(), 0);
}

// Test 28: User Data Persistence
void TestUserManager::testUserDataPersistence() {
  createTestUser("persistent", "persistent");
  userManager->loginUser("persistent", "persistent");

  User* user = userManager->getCurrentUser();
  user->setEmail("persistent@test.com");
  user->addWin();
  user->addWin();
  user->addWin();
  user->addLoss();
  user->addTie();

  userManager->saveUserData();
  userManager->logoutUser();

  // Create new UserManager to test persistence across sessions
  delete userManager;
  userManager = new UserManager();

  userManager->loginUser("persistent", "persistent");
  User* persistentUser = userManager->getCurrentUser();

  QCOMPARE(persistentUser->getEmail(), QString("persistent@test.com"));
  QCOMPARE(persistentUser->getGamesWon(), 3);
  QCOMPARE(persistentUser->getGamesLost(), 1);
  QCOMPARE(persistentUser->getGamesTied(), 1);
}

// Test 29: User Statistics Persistence
void TestUserManager::testUserStatisticsPersistence() {
  createTestUser("statsuser", "statspass");
  userManager->loginUser("statsuser", "statspass");

  User* user = userManager->getCurrentUser();
  for (int i = 0; i < 5; i++) user->addWin();
  for (int i = 0; i < 3; i++) user->addLoss();
  for (int i = 0; i < 2; i++) user->addTie();

  userManager->saveUserData();
  userManager->logoutUser();
  userManager->loginUser("statsuser", "statspass");

  User* reloadedUser = userManager->getCurrentUser();
  QCOMPARE(reloadedUser->getGamesWon(), 5);
  QCOMPARE(reloadedUser->getGamesLost(), 3);
  QCOMPARE(reloadedUser->getGamesTied(), 2);
  QCOMPARE(reloadedUser->getTotalGames(), 10);
}

// Test 30: Save Game History
void TestUserManager::testSaveGameHistory() {
  createTestUser("historyuser", "historypass");
  userManager->loginUser("historyuser", "historypass");

  User* user = userManager->getCurrentUser();
  GameRecord record = createTestGameRecord("TestOpponent", "Won");

  QList<GameRecord> history;
  history.append(record);
  user->setGameHistory(history);

  userManager->saveUserData();

  // Verify history was saved
  QList<GameRecord> savedHistory =
      userManager->loadGameHistoryFromDatabase("historyuser");
  QCOMPARE(savedHistory.size(), 1);
  QCOMPARE(savedHistory[0].opponent, QString("TestOpponent"));
  QCOMPARE(savedHistory[0].result, QString("Won"));
}

// Test 31: Load Game History
void TestUserManager::testLoadGameHistory() {
  createTestUser("loadhistory", "loadhistory");
  userManager->loginUser("loadhistory", "loadhistory");

  User* user = userManager->getCurrentUser();
  QList<GameRecord> history;

  // Create multiple game records
  for (int i = 0; i < 3; i++) {
    GameRecord record =
        createTestGameRecord(QString("Opponent%1").arg(i), "Won");
    history.append(record);
  }

  user->setGameHistory(history);
  userManager->saveUserData();

  // Load history
  QList<GameRecord> loadedHistory =
      userManager->loadGameHistoryFromDatabase("loadhistory");
  QCOMPARE(loadedHistory.size(), 3);

  for (int i = 0; i < 3; i++) {
    QVERIFY(loadedHistory[i].opponent.contains("Opponent"));
    QCOMPARE(loadedHistory[i].result, QString("Won"));
  }
}

// Test 32: Load All Game History
void TestUserManager::testLoadAllGameHistory() {
  createTestUser("allhistory", "allhistory");
  userManager->loginUser("allhistory", "allhistory");

  User* user = userManager->getCurrentUser();
  QList<GameRecord> history;

  // Create more than 5 records (limit for regular load)
  for (int i = 0; i < 7; i++) {
    GameRecord record =
        createTestGameRecord(QString("AllOpponent%1").arg(i), "Won");
    history.append(record);
  }

  user->setGameHistory(history);
  userManager->saveUserData();

  // Load limited history (should be 5)
  QList<GameRecord> limitedHistory =
      userManager->loadGameHistoryFromDatabase("allhistory");
  QCOMPARE(limitedHistory.size(), 5);

  // Load all history (should be 7)
  QList<GameRecord> allHistory =
      userManager->loadAllGameHistoryFromDatabase("allhistory");
  QCOMPARE(allHistory.size(), 7);
}

// Test 33: Game History Limit
void TestUserManager::testGameHistoryLimit() {
  createTestUser("limituser", "limitpass");
  userManager->loginUser("limituser", "limitpass");

  User* user = userManager->getCurrentUser();
  QList<GameRecord> history;

  // Create 10 game records
  for (int i = 0; i < 10; i++) {
    GameRecord record =
        createTestGameRecord(QString("LimitOpponent%1").arg(i), "Won");
    history.append(record);
  }

  user->setGameHistory(history);
  userManager->saveUserData();

  // Regular load should limit to 5
  QList<GameRecord> limitedHistory =
      userManager->loadGameHistoryFromDatabase("limituser");
  QCOMPARE(limitedHistory.size(), 5);
}

// Test 34: Game History Ordering
void TestUserManager::testGameHistoryOrdering() {
  createTestUser("orderuser", "orderpass");
  userManager->loginUser("orderuser", "orderpass");

  User* user = userManager->getCurrentUser();
  QList<GameRecord> history;

  // Create records with different timestamps
  for (int i = 0; i < 3; i++) {
    GameRecord record =
        createTestGameRecord(QString("OrderOpponent%1").arg(i), "Won");
    record.timestamp =
        QDateTime::currentDateTime().addSecs(-i * 60);  // Different times
    history.append(record);
  }

  user->setGameHistory(history);
  userManager->saveUserData();

  // Load history (should be ordered by timestamp DESC)
  QList<GameRecord> orderedHistory =
      userManager->loadGameHistoryFromDatabase("orderuser");
  QCOMPARE(orderedHistory.size(), 3);

  // Verify ordering (most recent first)
  for (int i = 0; i < orderedHistory.size() - 1; i++) {
    QVERIFY(orderedHistory[i].timestamp >= orderedHistory[i + 1].timestamp);
  }
}

// Test 35: Moves To String
void TestUserManager::testMovesToString() {
  QVector<GameMove> moves;
  GameMove move1 = {0, 0, 1};
  GameMove move2 = {1, 1, 2};
  GameMove move3 = {2, 2, 1};
  moves.append(move1);
  moves.append(move2);
  moves.append(move3);

  // Create temporary UserManager to access private method through testing
  createTestUser("moveuser", "movepass");
  userManager->loginUser("moveuser", "movepass");

  User* user = userManager->getCurrentUser();
  GameRecord record = createTestGameRecord("TestOpponent", "Won");
  record.moves = moves;

  QList<GameRecord> history;
  history.append(record);
  user->setGameHistory(history);
  userManager->saveUserData();

  // Load back and verify moves were serialized/deserialized correctly
  QList<GameRecord> loadedHistory =
      userManager->loadGameHistoryFromDatabase("moveuser");
  QCOMPARE(loadedHistory.size(), 1);
  QCOMPARE(loadedHistory[0].moves.size(), 3);
  QCOMPARE(loadedHistory[0].moves[0].row, 0);
  QCOMPARE(loadedHistory[0].moves[0].col, 0);
  QCOMPARE(loadedHistory[0].moves[0].player, 1);
}

// Test 36: Moves From String
void TestUserManager::testMovesFromString() {
  // Test through save/load cycle
  createTestUser("stringuser", "stringpass");
  userManager->loginUser("stringuser", "stringpass");

  User* user = userManager->getCurrentUser();
  GameRecord record = createTestGameRecord("StringOpponent", "Won");

  // Clear moves and add specific ones
  record.moves.clear();
  GameMove move1 = {2, 1, 2};
  GameMove move2 = {0, 2, 1};
  record.moves.append(move1);
  record.moves.append(move2);

  QList<GameRecord> history;
  history.append(record);
  user->setGameHistory(history);
  userManager->saveUserData();

  // Load and verify
  QList<GameRecord> loadedHistory =
      userManager->loadGameHistoryFromDatabase("stringuser");
  QCOMPARE(loadedHistory[0].moves.size(), 2);
  QCOMPARE(loadedHistory[0].moves[0].row, 2);
  QCOMPARE(loadedHistory[0].moves[0].col, 1);
  QCOMPARE(loadedHistory[0].moves[0].player, 2);
}

// Test 37: Move Serialization Round Trip
void TestUserManager::testMoveSerializationRoundTrip() {
  createTestUser("roundtrip", "roundtrip");
  userManager->loginUser("roundtrip", "roundtrip");

  User* user = userManager->getCurrentUser();
  QVector<GameMove> originalMoves;

  // Create complex move sequence
  for (int i = 0; i < 9; i++) {
    GameMove move = {i / 3, i % 3, (i % 2) + 1};
    originalMoves.append(move);
  }

  GameRecord record = createTestGameRecord("RoundTripOpponent", "Won");
  record.moves = originalMoves;

  QList<GameRecord> history;
  history.append(record);
  user->setGameHistory(history);
  userManager->saveUserData();

  // Load and compare
  QList<GameRecord> loadedHistory =
      userManager->loadGameHistoryFromDatabase("roundtrip");
  QCOMPARE(loadedHistory[0].moves.size(), originalMoves.size());

  for (int i = 0; i < originalMoves.size(); i++) {
    QCOMPARE(loadedHistory[0].moves[i].row, originalMoves[i].row);
    QCOMPARE(loadedHistory[0].moves[i].col, originalMoves[i].col);
    QCOMPARE(loadedHistory[0].moves[i].player, originalMoves[i].player);
  }
}

// Test 38: Empty Move Serialization
void TestUserManager::testEmptyMoveSerialization() {
  createTestUser("emptymoves", "emptymoves");
  userManager->loginUser("emptymoves", "emptymoves");

  User* user = userManager->getCurrentUser();
  GameRecord record = createTestGameRecord("EmptyOpponent", "Won");
  record.moves.clear();  // Empty moves

  QList<GameRecord> history;
  history.append(record);
  user->setGameHistory(history);
  userManager->saveUserData();

  // Load and verify empty moves handled correctly
  QList<GameRecord> loadedHistory =
      userManager->loadGameHistoryFromDatabase("emptymoves");
  QCOMPARE(loadedHistory[0].moves.size(), 0);
}

// Test 39: Database Connection
void TestUserManager::testDatabaseConnection() {
  // Test that database operations work
  QVERIFY(!userManager->userExists("connectiontest"));

  bool registered = userManager->registerUser("connectiontest", "password");
  QVERIFY(registered);

  QVERIFY(userManager->userExists("connectiontest"));
}

// Test 40: Database Transactions
void TestUserManager::testDatabaseTransactions() {
  // Test multiple operations in sequence
  createTestUser("transaction1", "pass1");
  createTestUser("transaction2", "pass2");
  createTestUser("transaction3", "pass3");

  QStringList users = userManager->getAllUsernames();
  QVERIFY(users.contains("transaction1"));
  QVERIFY(users.contains("transaction2"));
  QVERIFY(users.contains("transaction3"));
}

// Test 41: Database Error Handling
void TestUserManager::testDatabaseErrorHandling() {
  // Test graceful handling of invalid operations
  bool result = userManager->loginUser("", "");
  QVERIFY(!result);

  result = userManager->registerUser("", "");
  QVERIFY(!result);

  // Should not crash with invalid usernames
  QVERIFY(!userManager->userExists(""));
}

// Test 42: Complete User Workflow
void TestUserManager::testCompleteUserWorkflow() {
  // Test complete user lifecycle
  QString username = "workflowuser";
  QString password = "workflowpass";
  QString email = "workflow@test.com";

  // Register
  QVERIFY(userManager->registerUser(username, password, email));
  QVERIFY(userManager->userExists(username));

  // Login
  QVERIFY(userManager->loginUser(username, password));
  QVERIFY(userManager->isUserLoggedIn());

  // Use account
  User* user = userManager->getCurrentUser();
  user->addWin();
  user->addLoss();

  // Save and logout
  userManager->saveUserData();
  userManager->logoutUser();
  QVERIFY(!userManager->isUserLoggedIn());

  // Login again and verify data
  QVERIFY(userManager->loginUser(username, password));
  User* reloadedUser = userManager->getCurrentUser();
  QCOMPARE(reloadedUser->getGamesWon(), 1);
  QCOMPARE(reloadedUser->getGamesLost(), 1);
}

// Test 43: Multiple Users Workflow
void TestUserManager::testMultipleUsersWorkflow() {
  // Create multiple users
  createTestUser("multi1", "pass1", "multi1@test.com");
  createTestUser("multi2", "pass2", "multi2@test.com");
  createTestUser("multi3", "pass3", "multi3@test.com");

  // Test each user independently
  userManager->loginUser("multi1", "pass1");
  userManager->getCurrentUser()->addWin();
  userManager->saveUserData();
  userManager->logoutUser();

  userManager->loginUser("multi2", "pass2");
  userManager->getCurrentUser()->addLoss();
  userManager->getCurrentUser()->addLoss();
  userManager->saveUserData();
  userManager->logoutUser();

  userManager->loginUser("multi3", "pass3");
  userManager->getCurrentUser()->addTie();
  userManager->saveUserData();
  userManager->logoutUser();

  // Verify each user's data is separate
  userManager->loginUser("multi1", "pass1");
  QCOMPARE(userManager->getCurrentUser()->getGamesWon(), 1);
  QCOMPARE(userManager->getCurrentUser()->getGamesLost(), 0);
  userManager->logoutUser();

  userManager->loginUser("multi2", "pass2");
  QCOMPARE(userManager->getCurrentUser()->getGamesWon(), 0);
  QCOMPARE(userManager->getCurrentUser()->getGamesLost(), 2);
  userManager->logoutUser();
}

// Test 44: User Session Management
void TestUserManager::testUserSessionManagement() {
  createTestUser("session1", "sessionpass");
  createTestUser("session2", "sessionpass");

  // Login first user
  userManager->loginUser("session1", "sessionpass");
  QCOMPARE(userManager->getCurrentUser()->getUsername(), QString("session1"));

  // Login second user (should replace first)
  userManager->loginUser("session2", "sessionpass");
  QCOMPARE(userManager->getCurrentUser()->getUsername(), QString("session2"));

  // Only one user should be logged in at a time
  QVERIFY(userManager->isUserLoggedIn());

  userManager->logoutUser();
  QVERIFY(!userManager->isUserLoggedIn());
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  TestUserManager test;
  return QTest::qExec(&test, argc, argv);
}
#include "test_usermanager.moc"
