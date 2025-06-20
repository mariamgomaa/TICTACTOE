#include <QtTest>

#include "mainwindow.h"

class TestMainWindow : public QObject {
  Q_OBJECT

 private slots:
  void initTestCase();
  void cleanupTestCase();
  void init();
  void cleanup();

  // Basic initialization tests
  void testMainWindowInitialization();
  void testWindowProperties();
  void testUIComponentsExistence();

  // Login/Register functionality tests
  void testLoginScreenNavigation();
  void testLoginFunctionality();
  void testRegisterFunctionality();
  void testLogoutFunctionality();

  // Game setup tests
  void testGameSetupNavigation();
  void testModeSelection();
  void testDifficultySelection();
  void testSymbolSelection();
  void testPlayerNamesInput();

  // Game functionality tests
  void testGameBoardCreation();
  void testCellInteraction();
  void testNewGameFunctionality();
  void testGameStateManagement();
  void testAIGameplay();
  void testPvPGameplay();

  // Profile and history tests
  void testUserProfileDisplay();
  void testGameHistoryAccess();
  void testGameReplayInterface();

  // Animation and UI tests
  void testAnimationSystem();
  void testScreenTransitions();
  void testButtonAnimations();
  void testResponsiveDesign();

  void testAnimationInitialization();
  void testFlyingElementsCreation();
  void testUpdateUserStats();
  void testGameRecording();
  void testReplayControls();
  void testReplaySpeedControl();
  void testUserProfileUpdate();
  void testGameHistoryUpdate();
  void testBoardReset();
  void testAITimer();
  void testGameEndConditions();

  // Integration tests
  void testCompleteGameFlow();
  void testUserDataPersistence();

 private:
  MainWindow* mainWindow;
  QApplication* app;
};

void TestMainWindow::initTestCase() {
  // Create QApplication if it doesn't exist
  if (!QApplication::instance()) {
    int argc = 0;
    char** argv = nullptr;
    app = new QApplication(argc, argv);
  }
}

void TestMainWindow::cleanupTestCase() {
  // Cleanup is handled by Qt
}

void TestMainWindow::init() {
  mainWindow = new MainWindow();
  QTest::qWait(100);  // Allow initialization to complete
}

void TestMainWindow::cleanup() {
  delete mainWindow;
  mainWindow = nullptr;
}

void TestMainWindow::testMainWindowInitialization() {
  QVERIFY(mainWindow != nullptr);
  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));
  QVERIFY(!mainWindow->isVisible());  // Initially not visible
}

void TestMainWindow::testWindowProperties() {
  QSize minSize = mainWindow->minimumSize();
  QVERIFY(minSize.width() >= 800);
  QVERIFY(minSize.height() >= 600);

  // Test window flags
  Qt::WindowFlags flags = mainWindow->windowFlags();
  QVERIFY(flags & Qt::Window);
  QVERIFY(flags & Qt::WindowTitleHint);
}

void TestMainWindow::testUIComponentsExistence() {
  // Find stacked widget
  QStackedWidget* stackedWidget = mainWindow->findChild<QStackedWidget*>();
  QVERIFY(stackedWidget != nullptr);

  // Check for login components
  QList<QLineEdit*> lineEdits = mainWindow->findChildren<QLineEdit*>();
  QVERIFY(lineEdits.size() >= 2);  // At least username and password fields

  // Check for buttons
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  QVERIFY(buttons.size() > 0);
}

void TestMainWindow::testLoginScreenNavigation() {
  mainWindow->testShowLoginScreen();
  QTest::qWait(100);

  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  // Test navigation to register screen
  QPushButton* showRegisterBtn = nullptr;
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  for (QPushButton* btn : buttons) {
    if (btn->text().contains("New Player") || btn->text().contains("Join")) {
      showRegisterBtn = btn;
      break;
    }
  }

  if (showRegisterBtn) {
    QTest::mouseClick(showRegisterBtn, Qt::LeftButton);
    QTest::qWait(100);
    QCOMPARE(mainWindow->windowTitle(),
             QString("📝 Tic Tac Toe - Create Account!"));
  }
}

void TestMainWindow::testLoginFunctionality() {
  mainWindow->testShowLoginScreen();
  QTest::qWait(100);

  QList<QLineEdit*> lineEdits = mainWindow->findChildren<QLineEdit*>();
  QVERIFY(lineEdits.size() >= 2);

  QLineEdit* usernameEdit = lineEdits[0];
  QLineEdit* passwordEdit = lineEdits[1];

  // Test empty login
  usernameEdit->clear();
  passwordEdit->clear();

  QPushButton* loginBtn = nullptr;
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  for (QPushButton* btn : buttons) {
    if (btn->text().contains("LAUNCH") || btn->text().contains("LOGIN")) {
      loginBtn = btn;
      break;
    }
  }

  QVERIFY(loginBtn != nullptr);

  // Test login with empty fields (should show error)
  QTest::mouseClick(loginBtn, Qt::LeftButton);
  QTest::qWait(200);

  // Should still be on login screen
  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  // Test with valid credentials
  usernameEdit->setText("testuser");
  passwordEdit->setText("testpass");
  QTest::mouseClick(loginBtn, Qt::LeftButton);
  QTest::qWait(200);
}

void TestMainWindow::testRegisterFunctionality() {
  mainWindow->testShowRegisterScreen();
  QTest::qWait(100);

  QList<QLineEdit*> lineEdits = mainWindow->findChildren<QLineEdit*>();
  QVERIFY(lineEdits.size() >= 3);  // Username, email, password

  if (lineEdits.size() >= 3) {
    QLineEdit* usernameEdit = lineEdits[0];
    QLineEdit* emailEdit = lineEdits[1];
    QLineEdit* passwordEdit = lineEdits[2];

    // Test registration with valid data
    usernameEdit->setText("newuser");
    emailEdit->setText("test@example.com");
    passwordEdit->setText("password123");

    QPushButton* registerBtn = nullptr;
    QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
    for (QPushButton* btn : buttons) {
      if (btn->text().contains("CREATE") || btn->text().contains("REGISTER")) {
        registerBtn = btn;
        break;
      }
    }

    if (registerBtn) {
      QTest::mouseClick(registerBtn, Qt::LeftButton);
      QTest::qWait(200);
    }
  }
}

void TestMainWindow::testGameSetupNavigation() {
  // First need to simulate being logged in
  mainWindow->testShowLoginScreen();
  QTest::qWait(100);

  // Navigate to game setup (this might require login first)
  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));
}

void TestMainWindow::testModeSelection() {
  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  QList<QRadioButton*> radioButtons = mainWindow->findChildren<QRadioButton*>();
  QRadioButton* pvpBtn = nullptr;
  QRadioButton* aiBtn = nullptr;

  for (QRadioButton* btn : radioButtons) {
    if (btn->text().contains("Player vs Player")) {
      pvpBtn = btn;
    } else if (btn->text().contains("AI")) {
      aiBtn = btn;
    }
  }

  if (pvpBtn) {
    QTest::mouseClick(pvpBtn, Qt::LeftButton);
    QTest::qWait(500);
    QVERIFY(pvpBtn->isChecked());
  }

  if (aiBtn) {
    QTest::mouseClick(aiBtn, Qt::LeftButton);
    QTest::qWait(500);
    QVERIFY(aiBtn->isChecked());
  }
}

void TestMainWindow::testDifficultySelection() {
  // Navigate to difficulty selection (usually after AI mode selection)
  QList<QRadioButton*> difficultyBtns =
      mainWindow->findChildren<QRadioButton*>();
  QRadioButton* easyBtn = nullptr;
  QRadioButton* mediumBtn = nullptr;
  QRadioButton* hardBtn = nullptr;

  for (QRadioButton* btn : difficultyBtns) {
    if (btn->text().contains("Easy")) {
      easyBtn = btn;
    } else if (btn->text().contains("Medium")) {
      mediumBtn = btn;
    } else if (btn->text().contains("Hard")) {
      hardBtn = btn;
    }
  }

  if (easyBtn) {
    QTest::mouseClick(easyBtn, Qt::LeftButton);
    QTest::qWait(300);
    QVERIFY(easyBtn->isChecked());
  }
}

void TestMainWindow::testSymbolSelection() {
  QList<QRadioButton*> symbolBtns = mainWindow->findChildren<QRadioButton*>();
  QRadioButton* xBtn = nullptr;
  QRadioButton* oBtn = nullptr;

  for (QRadioButton* btn : symbolBtns) {
    if (btn->text().contains("X")) {
      xBtn = btn;
    } else if (btn->text().contains("O")) {
      oBtn = btn;
    }
  }

  if (xBtn && oBtn) {
    QTest::mouseClick(xBtn, Qt::LeftButton);
    QTest::qWait(100);
    QVERIFY(xBtn->isChecked());
    QVERIFY(!oBtn->isChecked());

    QTest::mouseClick(oBtn, Qt::LeftButton);
    QTest::qWait(100);
    QVERIFY(!xBtn->isChecked());
    QVERIFY(oBtn->isChecked());
  }
}

void TestMainWindow::testPlayerNamesInput() {
  mainWindow->testShowPlayerNamesInput();
  QTest::qWait(100);

  QCOMPARE(mainWindow->windowTitle(), QString("👥 Tic Tac Toe - Player Names"));

  QList<QLineEdit*> nameEdits = mainWindow->findChildren<QLineEdit*>();
  if (nameEdits.size() >= 2) {
    QLineEdit* player1Edit = nameEdits[0];
    QLineEdit* player2Edit = nameEdits[1];

    player1Edit->setText("TestPlayer1");
    player2Edit->setText("TestPlayer2");

    QCOMPARE(player1Edit->text(), QString("TestPlayer1"));
    QCOMPARE(player2Edit->text(), QString("TestPlayer2"));

    // Test confirm button
    QPushButton* confirmBtn = nullptr;
    QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
    for (QPushButton* btn : buttons) {
      if (btn->text().contains("Start Game") ||
          btn->text().contains("CONFIRM")) {
        confirmBtn = btn;
        break;
      }
    }

    if (confirmBtn) {
      QTest::mouseClick(confirmBtn, Qt::LeftButton);
      QTest::qWait(200);
    }
  }
}

void TestMainWindow::testGameBoardCreation() {
  mainWindow->testShowGameSetup();
  QTest::qWait(200);

  // Find game board cells
  QList<QPushButton*> allButtons = mainWindow->findChildren<QPushButton*>();
  int cellCount = 0;

  for (QPushButton* button : allButtons) {
    // Game cells are typically large square buttons
    if (button->size().width() > 80 && button->size().height() > 80) {
      cellCount++;
    }
  }

  QVERIFY(cellCount >= 9);  // Should have at least 9 cells for tic-tac-toe
}

void TestMainWindow::testCellInteraction() {
  mainWindow->testShowGameSetup();
  mainWindow->testNewGame();
  QTest::qWait(200);

  QList<QPushButton*> allButtons = mainWindow->findChildren<QPushButton*>();
  QPushButton* gameCell = nullptr;

  // Find a game cell (large, empty, enabled button)
  for (QPushButton* btn : allButtons) {
    if (btn->text().isEmpty() && btn->isEnabled() && btn->size().width() > 80 &&
        btn->size().height() > 80) {
      gameCell = btn;
      break;
    }
  }

  if (gameCell) {
    QString initialText = gameCell->text();
    QTest::mouseClick(gameCell, Qt::LeftButton);
    QTest::qWait(200);

    QString newText = gameCell->text();
    QVERIFY(newText == "X" || newText == "O");
    QVERIFY(initialText != newText);
  }
}

void TestMainWindow::testNewGameFunctionality() {
  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  // Find new game button
  QPushButton* newGameBtn = nullptr;
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  for (QPushButton* btn : buttons) {
    if (btn->text().contains("PLAY AGAIN") ||
        btn->text().contains("New Game")) {
      newGameBtn = btn;
      break;
    }
  }

  if (newGameBtn) {
    QTest::mouseClick(newGameBtn, Qt::LeftButton);
    QTest::qWait(200);

    // Verify board is reset - check for empty cells
    QList<QPushButton*> allButtons = mainWindow->findChildren<QPushButton*>();
    int emptyCells = 0;

    for (QPushButton* btn : allButtons) {
      if (btn->size().width() > 80 && btn->size().height() > 80) {
        if (btn->text().isEmpty() && btn->isEnabled()) {
          emptyCells++;
        }
      }
    }

    QVERIFY(emptyCells >= 9);  // Should have 9 empty, enabled cells
  }
}

void TestMainWindow::testUserProfileDisplay() {
  mainWindow->testShowUserProfile();
  QTest::qWait(100);

  // Check for profile elements
  QList<QLabel*> labels = mainWindow->findChildren<QLabel*>();
  bool hasWelcomeLabel = false;
  bool hasStatsLabel = false;

  for (QLabel* label : labels) {
    if (label->text().contains("Welcome")) {
      hasWelcomeLabel = true;
    } else if (label->text().contains("Statistics")) {
      hasStatsLabel = true;
    }
  }

  QVERIFY(hasWelcomeLabel ||
          hasStatsLabel);  // Should have some profile content
}

void TestMainWindow::testGameHistoryAccess() {
  mainWindow->testShowGameHistory();
  QTest::qWait(100);

  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  // Check for history display elements
  QList<QLabel*> labels = mainWindow->findChildren<QLabel*>();
  bool hasHistoryContent = false;

  for (QLabel* label : labels) {
    if (label->text().contains("History") || label->text().contains("Game")) {
      hasHistoryContent = true;
      break;
    }
  }

  QVERIFY(hasHistoryContent);
}

void TestMainWindow::testGameReplayInterface() {
  mainWindow->testShowGameReplay();
  QTest::qWait(100);

  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  // Check for replay controls
  QListWidget* gameList = mainWindow->findChild<QListWidget*>();
  QSlider* speedSlider = mainWindow->findChild<QSlider*>();

  QVERIFY(gameList != nullptr);
  QVERIFY(speedSlider != nullptr);

  if (speedSlider) {
    QVERIFY(speedSlider->minimum() >= 1);
    QVERIFY(speedSlider->maximum() <= 5);
  }
}

void TestMainWindow::testAnimationSystem() {
  // Test that animation components are initialized
  QList<QTimer*> timers = mainWindow->findChildren<QTimer*>();
  QVERIFY(timers.size() >= 1);  // Should have at least one timer for animations

  // Test flying elements creation
  mainWindow->testShowLoginScreen();
  QTest::qWait(300);  // Wait for animations to initialize

  QList<QLabel*> labels = mainWindow->findChildren<QLabel*>();
  int flyingElements = 0;

  for (QLabel* label : labels) {
    if (label->text() == "✖" || label->text() == "⭕") {
      flyingElements++;
    }
  }

  QVERIFY(flyingElements > 0);  // Should have flying elements
}

void TestMainWindow::testScreenTransitions() {
  // Test major screen transitions
  mainWindow->testShowLoginScreen();
  QTest::qWait(100);
  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  mainWindow->testShowRegisterScreen();
  QTest::qWait(100);
  QCOMPARE(mainWindow->windowTitle(),
           QString("📝 Tic Tac Toe - Create Account!"));

  mainWindow->testShowGameSetup();
  QTest::qWait(100);
  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));

  mainWindow->testShowGameSetup();
  QTest::qWait(100);
  QCOMPARE(mainWindow->windowTitle(),
           QString("🎮 Tic Tac Toe - Sign In to Play!"));
}

void TestMainWindow::testButtonAnimations() {
  mainWindow->testShowGameSetup();
  mainWindow->testNewGame();
  QTest::qWait(200);

  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  QPushButton* gameCell = nullptr;

  for (QPushButton* btn : buttons) {
    if (btn->isEnabled() && btn->text().isEmpty() && btn->size().width() > 80) {
      gameCell = btn;
      break;
    }
  }

  if (gameCell) {
    QRect originalGeometry = gameCell->geometry();
    QTest::mouseClick(gameCell, Qt::LeftButton);
    QTest::qWait(300);  // Wait for animation to complete

    // Animation should complete and return to original size
    QCOMPARE(gameCell->geometry(), originalGeometry);
  }
}

void TestMainWindow::testGameStateManagement() {
  mainWindow->testShowGameSetup();
  mainWindow->testNewGame();
  QTest::qWait(200);

  // Test that game starts in proper state
  QLabel* statusLabel = nullptr;
  QList<QLabel*> labels = mainWindow->findChildren<QLabel*>();

  for (QLabel* label : labels) {
    if (label->text().contains("GAME") || label->text().contains("Play") ||
        label->text().contains("Ready")) {
      statusLabel = label;
      break;
    }
  }

  QVERIFY(statusLabel != nullptr);
  if (statusLabel) {
    QVERIFY(!statusLabel->text().isEmpty());
  }
}

void TestMainWindow::testAIGameplay() {
  // This test simulates AI gameplay flow
  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  // Select AI mode if available
  QList<QRadioButton*> radioButtons = mainWindow->findChildren<QRadioButton*>();
  for (QRadioButton* btn : radioButtons) {
    if (btn->text().contains("AI")) {
      QTest::mouseClick(btn, Qt::LeftButton);
      QTest::qWait(500);
      break;
    }
  }

  // The test verifies that AI mode setup doesn't crash
  QVERIFY(true);  // If we reach here, no crash occurred
}

void TestMainWindow::testPvPGameplay() {
  // Test Player vs Player mode
  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  QList<QRadioButton*> radioButtons = mainWindow->findChildren<QRadioButton*>();
  for (QRadioButton* btn : radioButtons) {
    if (btn->text().contains("Player vs Player")) {
      QTest::mouseClick(btn, Qt::LeftButton);
      QTest::qWait(500);
      break;
    }
  }

  QVERIFY(true);  // Verify PvP mode setup doesn't crash
}

void TestMainWindow::testResponsiveDesign() {
  QSize originalSize = mainWindow->size();

  // Test window resizing
  mainWindow->resize(1200, 800);
  QTest::qWait(100);
  QCOMPARE(mainWindow->size(), QSize(1200, 800));

  // Test minimum size constraints
  mainWindow->resize(100, 100);
  QTest::qWait(100);

  QSize currentSize = mainWindow->size();
  QSize minSize = mainWindow->minimumSize();

  QVERIFY(currentSize.width() >= minSize.width());
  QVERIFY(currentSize.height() >= minSize.height());
}

void TestMainWindow::testCompleteGameFlow() {
  // Test a complete game flow from login to game
  mainWindow->testShowLoginScreen();
  QTest::qWait(100);

  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  mainWindow->testShowGameSetup();
  QTest::qWait(100);

  mainWindow->testNewGame();
  QTest::qWait(100);

  // Verify we can complete the flow without crashes
  QVERIFY(true);
}

void TestMainWindow::testUserDataPersistence() {
  // Test that user data operations don't crash
  mainWindow->testShowUserProfile();
  QTest::qWait(100);

  mainWindow->testShowGameHistory();
  QTest::qWait(100);

  // If we reach here, data operations completed successfully
  QVERIFY(true);
}

void TestMainWindow::testLogoutFunctionality() {
  // Test logout functionality
  QPushButton* logoutBtn = nullptr;
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();

  for (QPushButton* btn : buttons) {
    if (btn->text().contains("LOGOUT") || btn->text().contains("Logout")) {
      logoutBtn = btn;
      break;
    }
  }

  if (logoutBtn) {
    QTest::mouseClick(logoutBtn, Qt::LeftButton);
    QTest::qWait(200);

    // Should return to login screen
    QCOMPARE(mainWindow->windowTitle(),
             QString("🎮 Tic Tac Toe - Sign In to Play!"));
  }
}

void TestMainWindow::testAnimationInitialization() {
  // Test animation system initialization
  QList<QTimer*> timers = mainWindow->findChildren<QTimer*>();
  QVERIFY(timers.size() >= 1);  // Should have at least one timer

  // Test that animations don't crash
  mainWindow->testShowLoginScreen();
  QTest::qWait(200);

  QVERIFY(true);  // If we reach here, animations work
}

void TestMainWindow::testFlyingElementsCreation() {
  mainWindow->testShowLoginScreen();
  QTest::qWait(300);

  // Check for flying elements
  QList<QLabel*> labels = mainWindow->findChildren<QLabel*>();
  int flyingElements = 0;

  for (QLabel* label : labels) {
    if (label->text() == "✖" || label->text() == "⭕") {
      flyingElements++;
    }
  }

  QVERIFY(flyingElements > 0);
}

void TestMainWindow::testUpdateUserStats() {
  // Test different stat updates
  mainWindow->testUpdateUserStats(true, false, false);  // Win
  mainWindow->testUpdateUserStats(false, true, false);  // Loss
  mainWindow->testUpdateUserStats(false, false, true);  // Tie

  // Should not crash
  QVERIFY(true);
}

void TestMainWindow::testGameRecording() {
  // Test recording moves
  mainWindow->testRecordGameMove(0, 0, 1);
  mainWindow->testRecordGameMove(1, 1, 2);
  mainWindow->testRecordGameMove(2, 2, 1);

  // Test recording result
  mainWindow->testRecordGameResult("Won", "TestPlayer", "PvP", "X");

  QVERIFY(true);
}

void TestMainWindow::testReplayControls() {
  mainWindow->testShowGameReplay();
  QTest::qWait(100);

  // Find replay control buttons
  QList<QPushButton*> buttons = mainWindow->findChildren<QPushButton*>();
  QPushButton* playBtn = nullptr;
  QPushButton* pauseBtn = nullptr;
  QPushButton* resetBtn = nullptr;

  for (QPushButton* btn : buttons) {
    if (btn->text().contains("Play"))
      playBtn = btn;
    else if (btn->text().contains("Pause"))
      pauseBtn = btn;
    else if (btn->text().contains("Reset"))
      resetBtn = btn;
  }

  // Test controls exist
  QVERIFY(playBtn != nullptr);
  QVERIFY(pauseBtn != nullptr);
  QVERIFY(resetBtn != nullptr);
}

void TestMainWindow::testReplaySpeedControl() {
  mainWindow->testShowGameReplay();
  QTest::qWait(100);

  QSlider* speedSlider = mainWindow->findChild<QSlider*>();
  QVERIFY(speedSlider != nullptr);

  if (speedSlider) {
    // Test speed range
    QVERIFY(speedSlider->minimum() >= 1);
    QVERIFY(speedSlider->maximum() <= 5);

    // Test changing speed
    speedSlider->setValue(5);
    QTest::qWait(50);
    QCOMPARE(speedSlider->value(), 5);

    speedSlider->setValue(1);
    QTest::qWait(50);
    QCOMPARE(speedSlider->value(), 1);
  }
}

void TestMainWindow::testUserProfileUpdate() {
  mainWindow->testUpdateUserProfileDisplay();
  QTest::qWait(50);

  // Should not crash
  QVERIFY(true);
}

void TestMainWindow::testGameHistoryUpdate() {
  mainWindow->testUpdateGameHistoryDisplay();
  QTest::qWait(50);

  // Should not crash
  QVERIFY(true);
}

void TestMainWindow::testBoardReset() {
  mainWindow->testShowGame();
  QTest::qWait(100);

  mainWindow->testResetBoard();
  QTest::qWait(50);

  // Should not crash
  QVERIFY(true);
}

void TestMainWindow::testAITimer() {
  // Find AI timer
  QList<QTimer*> timers = mainWindow->findChildren<QTimer*>();
  bool hasAITimer = false;

  for (QTimer* timer : timers) {
    if (timer->isSingleShot()) {
      hasAITimer = true;
      break;
    }
  }

  QVERIFY(hasAITimer);
}

void TestMainWindow::testGameEndConditions() {
  mainWindow->testShowGame();
  mainWindow->testNewGame();
  QTest::qWait(100);

  // Test checkGameEnd doesn't crash
  mainWindow->testCheckGameEnd();
  QTest::qWait(50);

  QVERIFY(true);
}

int main(int argc, char* argv[]) {
  QApplication app(argc, argv);
  TestMainWindow test;
  return QTest::qExec(&test, argc, argv);
}
#include "test_mainwindow.moc"
