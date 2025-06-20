#include <QApplication>
#include <QtTest>

#include "user.h"

class TestUser : public QObject {
  Q_OBJECT

 private slots:
  void init();
  void cleanup();

  // Test the actual implemented methods
  void testDefaultConstructor();
  void testParameterizedConstructor();
  void testGetWinRate();
  void testGetWinRateZeroGames();
  void testAddGameToHistory();
  void testInlineGettersSetters();

 private:
  User* user;
};

void TestUser::init() { user = new User(); }

void TestUser::cleanup() { delete user; }

void TestUser::testDefaultConstructor() {
  QCOMPARE(user->getGamesWon(), 0);
  QCOMPARE(user->getGamesLost(), 0);
  QCOMPARE(user->getGamesTied(), 0);
  QVERIFY(user->getUsername().isEmpty());
}

void TestUser::testParameterizedConstructor() {
  User paramUser("testuser", "testpass", "test@example.com");
  QCOMPARE(paramUser.getUsername(), QString("testuser"));
  QCOMPARE(paramUser.getPassword(), QString("testpass"));
  QCOMPARE(paramUser.getEmail(), QString("test@example.com"));
  QVERIFY(paramUser.getLastLogin().isValid());
}

void TestUser::testGetWinRate() {
  user->addWin();
  user->addWin();
  user->addLoss();
  double winRate = user->getWinRate();
  QVERIFY(winRate > 66.6 && winRate < 66.7);  // More readable and robust
}

void TestUser::testGetWinRateZeroGames() { QCOMPARE(user->getWinRate(), 0.0); }

void TestUser::testAddGameToHistory() {
  GameRecord record("TestOpponent", "Won", "PvP", "X");
  user->addGameToHistory(record);
  QCOMPARE(user->getGameHistory().size(), 1);
}

void TestUser::testInlineGettersSetters() {
  user->setUsername("testuser");
  QCOMPARE(user->getUsername(), QString("testuser"));

  user->addWin();
  QCOMPARE(user->getGamesWon(), 1);
  QCOMPARE(user->getTotalGames(), 1);
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  TestUser test;
  return QTest::qExec(&test, argc, argv);
}
#include "test_user.moc"
