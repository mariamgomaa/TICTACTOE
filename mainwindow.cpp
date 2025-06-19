#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isPlayerX(true), isPvP(true), selectedDifficulty(1),
    humanPlayer(HUMAN), aiPlayer(AI), player1Name("Player 1"), player2Name("Player 2"),
    currentReplayMoveIndex(0), isReplaying(false)
{
    game = new TicTacToeGame();
    userManager = new UserManager();

    aiTimer = new QTimer(this);
    aiTimer->setSingleShot(true);
    connect(aiTimer, &QTimer::timeout, this, &MainWindow::makeAIMove);


    // Initialize animations
    celebrationAnimation = new QPropertyAnimation(this);
    winAnimationGroup = new QSequentialAnimationGroup(this);
    statusOpacityEffect = new QGraphicsOpacityEffect(this);

    // Initialize replay board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            replayBoard[i][j] = 0;
        }
    }

    setupUI();
    showLoginScreen();
}


MainWindow::~MainWindow()
{
    delete game;
    delete userManager;
}

void MainWindow::setupUI()
{
    // Create scroll area but configure it to prevent scrollbars
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setCentralWidget(scrollArea);

    stackedWidget = new QStackedWidget();
    scrollArea->setWidget(stackedWidget);

    // Setup all UI components with modern gaming aesthetics
    setupLoginUI();
    setupRegisterUI();
    setupUserProfileUI();
    setupGameHistoryUI();
    setupGameReplayUI();
    setupPlayerNamesUI();
    setupGameSetupUI();
    setupDifficultyWidget();
    setupSymbolWidget();
    setupGameUI();

    setWindowTitle("🎮 Tic Tac Toe - Epic Gaming Experience!");

    // Professional window sizing and centering
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Use 90% of screen size for immersive gaming experience
    int windowWidth = static_cast<int>(screenGeometry.width() * 0.90);
    int windowHeight = static_cast<int>(screenGeometry.height() * 0.90);

    // Ensure minimum size for very small screens
    int minWidth = qMin(900, screenGeometry.width() - 50);
    int minHeight = qMin(700, screenGeometry.height() - 50);

    // Use the larger of calculated or minimum
    windowWidth = qMax(windowWidth, minWidth);
    windowHeight = qMax(windowHeight, minHeight);

    // Set window size
    setMinimumSize(minWidth, minHeight);
    resize(windowWidth, windowHeight);

    // Center window perfectly on screen
    int x = (screenGeometry.width() - windowWidth) / 2;
    int y = (screenGeometry.height() - windowHeight) / 2;
    move(qMax(0, x), qMax(0, y));

    // Modern window flags for professional appearance
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint |
                   Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Epic gaming gradient background with depth
    setStyleSheet(
        "QMainWindow, QWidget, QScrollArea, QStackedWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #0f0f23, stop:0.3 #1a1a3a, stop:0.7 #2d1b69, stop:1 #3b2f87);"
        "border: none;"
        "}"
        "QScrollArea {"
        "background: transparent;"
        "border: none;"
        "}"
        );

    // Enable auto-fill background for smooth rendering
    setAutoFillBackground(true);
    scrollArea->setAutoFillBackground(true);
    stackedWidget->setAutoFillBackground(true);
}
void MainWindow::setupLoginUI()
{
    loginWidget = new QWidget();
    loginWidget->setStyleSheet("QWidget { background: transparent; }");

    // Main layout for centering
    QVBoxLayout *mainLayout = new QVBoxLayout(loginWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(50, 20, 50, 20);

    // Create floating login container with proper sizing
    QWidget *loginContainer = new QWidget();
    loginContainer->setFixedSize(550, 700);
    loginContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(loginContainer);
    containerLayout->setSpacing(20);
    containerLayout->setContentsMargins(40, 25, 40, 25);

    // PROFESSIONAL MAIN WELCOME TITLE
    QLabel *welcomeTitle = new QLabel("⚡ WELCOME TO TIC TAC TOE ⚡");
    welcomeTitle->setAlignment(Qt::AlignCenter);
    welcomeTitle->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #ffffff;"
        "text-shadow: 0 0 15px #ffffff, 0 0 30px #00ffff;"
        "background: transparent; padding: 15px;"
        "margin-bottom: 5px;"
        "}"
        );

    // Professional Ready to Play with modern styling
    QLabel *readyLabel = new QLabel("🎯 Ready to Dominate the Grid? 🎯");
    readyLabel->setAlignment(Qt::AlignCenter);
    readyLabel->setMinimumHeight(60);
    readyLabel->setStyleSheet(
        "QLabel {"
        "font-size: 18px; font-weight: bold; color: #ffffff;"
        "background: rgba(255, 255, 255, 0.1); border-radius: 10px; padding: 12px;"
        "margin-bottom: 10px; min-height: 35px;"
        "}"
        );

    // Professional gaming subtitle
    QLabel *loginTitle = new QLabel("🔐 PLAYER AUTHENTICATION 🔐");
    loginTitle->setAlignment(Qt::AlignCenter);
    loginTitle->setMinimumHeight(65);
    loginTitle->setStyleSheet(
        "QLabel {"
        "font-size: 22px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff, 0 0 40px #0080ff;"
        "background: transparent; padding: 12px;"
        "min-height: 40px;"
        "}"
        );

    // Professional Username field
    QLabel *usernameLabel = new QLabel("🆔 Username:");
    usernameLabel->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; color: #ffffff; margin: 6px; }"
        );

    loginUsernameEdit = new QLineEdit();
    loginUsernameEdit->setPlaceholderText("Enter your username");
    loginUsernameEdit->setMinimumHeight(55);
    loginUsernameEdit->setMaximumHeight(55);
    loginUsernameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 16px; padding: 12px 20px;"
        "background: rgba(255, 255, 255, 0.98);"
        "border: 2px solid #00ffff;"
        "border-radius: 12px; color: #1a1a3a;"
        "min-height: 30px; max-height: 30px;"
        "}"
        "QLineEdit:focus {"
        "border: 2px solid #ff6b6b;"
        "background: rgba(255, 255, 255, 1.0);"
        "}"
        );

    // Professional Password field
    QLabel *passwordLabel = new QLabel("🔑 Password:");
    passwordLabel->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; color: #ffffff; margin: 6px; }"
        );

    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setPlaceholderText("Enter your password");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setMinimumHeight(55);
    loginPasswordEdit->setMaximumHeight(55);
    loginPasswordEdit->setStyleSheet(loginUsernameEdit->styleSheet());

    // Professional gaming-style login button
    loginBtn = new QPushButton("⚡ LAUNCH GAME ⚡");
    loginBtn->setMinimumHeight(60);
    loginBtn->setMaximumHeight(60);
    loginBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff6b6b, stop:0.5 #ff8e53, stop:1 #ff6b6b);"
        "color: white; border: none; border-radius: 12px;"
        "text-shadow: 0 2px 4px rgba(0,0,0,0.3);"
        "min-height: 30px; max-height: 30px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff5252, stop:0.5 #ff7043, stop:1 #ff5252);"
        "}"
        );

    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);

    // Professional register link
    showRegisterBtn = new QPushButton("💎 New Player? Join the Elite!");
    showRegisterBtn->setMinimumHeight(50);
    showRegisterBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; color: #00ffff; background: transparent;"
        "border: none; padding: 10px; text-decoration: underline;"
        "min-height: 25px;"
        "}"
        "QPushButton:hover { color: #ff6b6b; }"
        );

    connect(showRegisterBtn, &QPushButton::clicked, this, &MainWindow::showRegisterScreen);

    // Add to container layout with proper spacing
    containerLayout->addWidget(welcomeTitle);
    containerLayout->addWidget(readyLabel);
    containerLayout->addWidget(loginTitle);
    containerLayout->addWidget(usernameLabel);
    containerLayout->addWidget(loginUsernameEdit);
    containerLayout->addWidget(passwordLabel);
    containerLayout->addWidget(loginPasswordEdit);
    containerLayout->addWidget(loginBtn);
    containerLayout->addWidget(showRegisterBtn);

    mainLayout->addWidget(loginContainer);
    stackedWidget->addWidget(loginWidget);
}

void MainWindow::setupPlayerNamesUI()
{
    playerNamesWidget = new QWidget();
    playerNamesWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(playerNamesWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Professional container with increased height for better spacing
    QWidget *namesContainer = new QWidget();
    namesContainer->setFixedSize(650, 650); // Increased height from 580 to 650
    namesContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(namesContainer);
    containerLayout->setSpacing(12); // Tight professional spacing
    containerLayout->setContentsMargins(40, 30, 40, 30);

    // Professional title - increased height to prevent cropping
    QLabel *title = new QLabel("👥 Player Names 👥");
    title->setAlignment(Qt::AlignCenter);
    title->setFixedHeight(80); // Increased from 65 to 80
    title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    title->setWordWrap(false); // Disable wrap for clean look
    title->setStyleSheet(
        "QLabel {"
        "font-size: 24px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 15px #00ffff;"
        "background: rgba(0, 255, 255, 0.08);"
        "padding: 20px 25px;" // Increased padding from 18px to 20px
        "border: 2px solid rgba(0, 255, 255, 0.4);"
        "border-radius: 12px;"
        "margin: 2px;"
        "}"
        );

    // Player 1 Label - Increased height for better text visibility
    QLabel *player1Label = new QLabel("🎯 Player 1 (X):");
    player1Label->setFixedHeight(45); // Increased from 35 to 45
    player1Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    player1Label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    player1Label->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #ffffff;"
        "padding: 10px 12px;" // Increased padding from 8px to 10px
        "margin: 0px;"
        "background: rgba(255, 255, 255, 0.08);"
        "border: 1px solid rgba(255, 255, 255, 0.2);"
        "border-radius: 6px;"
        "}"
        );

    // Player 1 Input - Increased height for better text display
    player1NameEdit = new QLineEdit();
    player1NameEdit->setPlaceholderText("Enter Player 1 name");
    player1NameEdit->setFixedHeight(58); // Increased from 48 to 58
    player1NameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    player1NameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 15px; padding: 16px 16px;" // Increased padding from 12px to 16px
        "background: rgba(255, 255, 255, 0.98);"
        "border: 2px solid #00ffff;"
        "border-radius: 8px;"
        "color: #1a1a3a;"
        "margin: 0px;"
        "selection-background-color: #4ecdc4;"
        "}"
        "QLineEdit:focus {"
        "border: 2px solid #ff6b6b;"
        "background: rgba(255, 255, 255, 1.0);"
        "box-shadow: 0 0 8px rgba(255, 107, 107, 0.3);"
        "}"
        "QLineEdit:hover {"
        "border: 2px solid rgba(0, 255, 255, 0.8);"
        "}"
        );

    // Player 2 Label - Matching increased style
    QLabel *player2Label = new QLabel("🎯 Player 2 (O):");
    player2Label->setFixedHeight(45); // Increased from 35 to 45
    player2Label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    player2Label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    player2Label->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #ffffff;"
        "padding: 10px 12px;" // Increased padding from 8px to 10px
        "margin: 0px;"
        "background: rgba(255, 255, 255, 0.08);"
        "border: 1px solid rgba(255, 255, 255, 0.2);"
        "border-radius: 6px;"
        "}"
        );

    // Player 2 Input - Matching increased style
    player2NameEdit = new QLineEdit();
    player2NameEdit->setPlaceholderText("Enter Player 2 name");
    player2NameEdit->setFixedHeight(58); // Increased from 48 to 58
    player2NameEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    player2NameEdit->setStyleSheet(player1NameEdit->styleSheet());

    // Professional Start Game Button - Increased height
    QPushButton *confirmBtn = new QPushButton("🎮 Start Game!");
    confirmBtn->setFixedHeight(65); // Increased from 55 to 65
    confirmBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    confirmBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold;"
        "padding: 18px 25px;" // Increased padding from 15px to 18px
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #4ecdc4, stop:1 #44a08d);"
        "color: white; border: none;"
        "border-radius: 12px;"
        "margin: 5px 0px;"
        "text-shadow: 0 1px 3px rgba(0,0,0,0.4);"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #26d0ce, stop:1 #1a2980);"
        "transform: translateY(-1px);"
        "box-shadow: 0 4px 12px rgba(78, 205, 196, 0.4);"
        "}"
        "QPushButton:pressed {"
        "transform: translateY(0px);"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #2c9f96, stop:1 #2d5f5f);"
        "}"
        );
    connect(confirmBtn, &QPushButton::clicked, this, &MainWindow::onPlayerNamesConfirmed);

    // Professional Back Button - Increased height
    QPushButton *backBtn = new QPushButton("⬅️ Back to Setup");
    backBtn->setFixedHeight(55); // Increased from 45 to 55
    backBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    backBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold;"
        "padding: 15px 20px;" // Increased padding from 12px to 15px
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #b2bec3, stop:1 #636e72);"
        "color: #2d3436; border: none;"
        "border-radius: 10px;"
        "margin: 2px 0px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #636e72, stop:1 #b2bec3);"
        "color: #fff;"
        "transform: translateY(-1px);"
        "box-shadow: 0 3px 8px rgba(99, 110, 114, 0.3);"
        "}"
        "QPushButton:pressed {"
        "transform: translateY(0px);"
        "}"
        );
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    // Professional layout with adjusted spacing
    containerLayout->addWidget(title);
    containerLayout->addSpacing(10); // Slightly increased spacing
    containerLayout->addWidget(player1Label);
    containerLayout->addSpacing(5); // Increased gap between label and input
    containerLayout->addWidget(player1NameEdit);
    containerLayout->addSpacing(12); // Increased gap between players
    containerLayout->addWidget(player2Label);
    containerLayout->addSpacing(5); // Increased gap between label and input
    containerLayout->addWidget(player2NameEdit);
    containerLayout->addSpacing(18); // Increased gap before buttons
    containerLayout->addWidget(confirmBtn);
    containerLayout->addSpacing(8); // Increased gap between buttons
    containerLayout->addWidget(backBtn);
    containerLayout->addStretch(); // Push content up

    mainLayout->addWidget(namesContainer);
    stackedWidget->addWidget(playerNamesWidget);
}

void MainWindow::updateGameHistoryDisplay()
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    QList<GameRecord> history = user->getGameHistory();

    // Update each of the 5 game blocks
    for (int i = 0; i < 5; ++i) {
        if (i < history.size()) {
            const GameRecord &record = history[i];

            QString resultEmoji;
            QString resultColor;
            if (record.result == "Won") {
                resultEmoji = "🏆 WON";
                resultColor = "#4ecdc4";
            } else if (record.result == "Lost") {
                resultEmoji = "😞 LOST";
                resultColor = "#ff6b6b";
            } else {
                resultEmoji = "🤝 TIE";
                resultColor = "#f39c12";
            }

            gameResultLabels[i]->setText(resultEmoji);
            gameResultLabels[i]->setStyleSheet(
                QString("QLabel {"
                        "font-size: 14px; font-weight: bold; color: white;"
                        "background: %1; border-radius: 8px; padding: 8px;"
                        "}").arg(resultColor)
                );

            QString details = QString("vs %1\n%2\n%3")
                                  .arg(record.opponent)
                                  .arg(record.gameMode)
                                  .arg(record.timestamp.toString("MMM dd"));

            gameDetailLabels[i]->setText(details);
        } else {
            // Empty game slot
            gameResultLabels[i]->setText("No Game");
            gameResultLabels[i]->setStyleSheet(
                "QLabel {"
                "font-size: 14px; color: #ffffff;"
                "background: rgba(255, 255, 255, 0.1); border-radius: 8px; padding: 8px;"
                "}"
                );
            gameDetailLabels[i]->setText("---");
        }
    }
}


void MainWindow::setupGameUI()
{
    gameWidget = new QWidget();
    gameWidget->setStyleSheet("QWidget { background: transparent; }");

    // Main layout for centering everything
    QVBoxLayout *mainLayout = new QVBoxLayout(gameWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setSpacing(20);

    // Game status with modern glow effect
    statusLabel = new QLabel("🎉 GAME READY! 🎉");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "QLabel {"
        "font-size: 36px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff, 0 0 40px #0080ff;"
        "background: rgba(255, 255, 255, 0.1);"
        "border-radius: 15px; padding: 20px; margin: 20px;"
        "border: 2px solid rgba(0, 255, 255, 0.3);"
        "}"
        );

    statusLabel->setGraphicsEffect(statusOpacityEffect);

    currentPlayerLabel = new QLabel("Current Player: X");
    currentPlayerLabel->setAlignment(Qt::AlignCenter);
    currentPlayerLabel->setStyleSheet(
        "QLabel {"
        "font-size: 24px; color: #ffffff; font-weight: bold;"
        "background: rgba(255, 255, 255, 0.1);"
        "border-radius: 12px; padding: 15px; margin: 10px;"
        "border: 1px solid rgba(255, 255, 255, 0.2);"
        "}"
        );

    // Create centered game board container
    QWidget *boardContainer = new QWidget();
    boardContainer->setFixedSize(450, 450);
    boardContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.05));"
        "border: 3px solid rgba(0, 255, 255, 0.4);"
        "border-radius: 20px;"
        "}"
        );

    // Center the board container
    QHBoxLayout *boardCenterLayout = new QHBoxLayout();
    boardCenterLayout->addStretch();
    boardCenterLayout->addWidget(boardContainer);
    boardCenterLayout->addStretch();

    boardLayout = new QGridLayout(boardContainer);
    boardLayout->setSpacing(10);
    boardLayout->setContentsMargins(25, 25, 25, 25);

    // Create perfectly centered grid cells
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = new QPushButton("");
            cells[i][j]->setFixedSize(120, 120);
            cells[i][j]->setStyleSheet(
                "QPushButton {"
                "font-size: 48px; font-weight: bold;"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 rgba(255, 255, 255, 0.2), stop:1 rgba(255, 255, 255, 0.1));"
                "border: 2px solid rgba(0, 255, 255, 0.3);"
                "border-radius: 15px; color: #ffffff;"
                "text-shadow: 0 0 10px currentColor;"
                "}"
                "QPushButton:hover {"
                "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
                "stop:0 rgba(255, 255, 255, 0.3), stop:1 rgba(255, 255, 255, 0.2));"
                "border: 2px solid rgba(255, 107, 107, 0.6);"
                "transform: scale(1.05);"
                "}"
                "QPushButton:pressed {"
                "background: rgba(255, 107, 107, 0.3);"
                "}"
                );

            cells[i][j]->setProperty("row", i);
            cells[i][j]->setProperty("col", j);
            connect(cells[i][j], &QPushButton::clicked, this, &MainWindow::cellClicked);
            boardLayout->addWidget(cells[i][j], i, j);
        }
    }

    // Modern game controls
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setAlignment(Qt::AlignCenter);

    newGameBtn = new QPushButton("🔄 PLAY AGAIN");
    newGameBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 20px; font-weight: bold; padding: 15px 25px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #4ecdc4, stop:1 #44a08d);"
        "color: white; border: none; border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #26d0ce, stop:1 #1a2980);"
        "transform: translateY(-2px);"
        "}"
        );

    connect(newGameBtn, &QPushButton::clicked, this, &MainWindow::newGame);

    backToSetupBtn = new QPushButton("⚙️ NEW ADVENTURE");
    backToSetupBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 20px; font-weight: bold; padding: 15px 25px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff6b6b, stop:0.5 #ff8e53, stop:1 #ff6b6b);"
        "color: white; border: none; border-radius: 12px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff5252, stop:0.5 #ff7043, stop:1 #ff5252);"
        "transform: translateY(-2px);"
        "}"
        );

    connect(backToSetupBtn, &QPushButton::clicked, this, &MainWindow::startGameSetup);

    controlLayout->addWidget(newGameBtn);
    controlLayout->addWidget(backToSetupBtn);

    // Add all elements to main layout with proper spacing
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(currentPlayerLabel);
    mainLayout->addLayout(boardCenterLayout);
    mainLayout->addLayout(controlLayout);
    mainLayout->addStretch(); // Push everything up

    stackedWidget->addWidget(gameWidget);
}

void MainWindow::setupRegisterUI()
{
    registerWidget = new QWidget();
    registerWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(registerWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Create floating register container
    QWidget *registerContainer = new QWidget();
    registerContainer->setFixedSize(450, 650);
    registerContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(registerContainer);
    containerLayout->setSpacing(18);
    containerLayout->setContentsMargins(35, 25, 35, 25);

    // Register title with proper height
    QLabel *registerTitle = new QLabel("📝 JOIN THE GAME 📝");
    registerTitle->setAlignment(Qt::AlignCenter);
    registerTitle->setMinimumHeight(60);
    registerTitle->setStyleSheet(
        "QLabel {"
        "font-size: 24px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff;"
        "background: transparent; padding: 12px;"
        "min-height: 35px;"
        "}"
        );

    QLabel *registerSubtitle = new QLabel("Create your account and start your journey! 🌟");
    registerSubtitle->setAlignment(Qt::AlignCenter);
    registerSubtitle->setMinimumHeight(55);
    registerSubtitle->setStyleSheet(
        "QLabel {"
        "font-size: 15px; color: #ffffff; margin-bottom: 10px;"
        "background: rgba(255, 255, 255, 0.1); border-radius: 10px; padding: 10px;"
        "min-height: 30px;"
        "}"
        );

    // Username field with proper sizing
    QLabel *regUsernameLabel = new QLabel("👤 Choose Username:");
    regUsernameLabel->setStyleSheet(
        "QLabel { font-size: 15px; font-weight: bold; color: #ffffff; margin: 3px; }"
        );

    registerUsernameEdit = new QLineEdit();
    registerUsernameEdit->setPlaceholderText("Pick a unique username");
    registerUsernameEdit->setMinimumHeight(50);
    registerUsernameEdit->setMaximumHeight(50);
    registerUsernameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 15px; padding: 10px 15px;"
        "background: rgba(255, 255, 255, 0.9);"
        "border: 2px solid #00ffff; border-radius: 10px; color: #1a1a3a;"
        "min-height: 25px; max-height: 25px;"
        "}"
        "QLineEdit:focus { border: 2px solid #ff6b6b; }"
        );

    // Email field with proper sizing
    QLabel *emailLabel = new QLabel("📧 Email (Optional):");
    emailLabel->setStyleSheet(
        "QLabel { font-size: 15px; font-weight: bold; color: #ffffff; margin: 3px; }"
        );

    registerEmailEdit = new QLineEdit();
    registerEmailEdit->setPlaceholderText("your.email@example.com");
    registerEmailEdit->setMinimumHeight(50);
    registerEmailEdit->setMaximumHeight(50);
    registerEmailEdit->setStyleSheet(registerUsernameEdit->styleSheet());

    // Password field with proper sizing
    QLabel *regPasswordLabel = new QLabel("🔒 Create Password:");
    regPasswordLabel->setStyleSheet(
        "QLabel { font-size: 15px; font-weight: bold; color: #ffffff; margin: 3px; }"
        );

    registerPasswordEdit = new QLineEdit();
    registerPasswordEdit->setPlaceholderText("Create a secure password");
    registerPasswordEdit->setEchoMode(QLineEdit::Password);
    registerPasswordEdit->setMinimumHeight(50);
    registerPasswordEdit->setMaximumHeight(50);
    registerPasswordEdit->setStyleSheet(registerUsernameEdit->styleSheet());

    // Register button with proper height
    registerBtn = new QPushButton("🎉 CREATE ACCOUNT");
    registerBtn->setMinimumHeight(55);
    registerBtn->setMaximumHeight(55);
    registerBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 16px; font-weight: bold; padding: 12px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #4ecdc4, stop:1 #44a08d);"
        "color: white; border: none; border-radius: 12px;"
        "min-height: 25px; max-height: 25px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #26d0ce, stop:1 #1a2980);"
        "}"
        );

    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    // Login link with proper height
    showLoginBtn = new QPushButton("🔙 Already have an account? Sign in!");
    showLoginBtn->setMinimumHeight(45);
    showLoginBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 13px; color: #00ffff; background: transparent;"
        "border: none; padding: 8px; text-decoration: underline;"
        "min-height: 20px;"
        "}"
        "QPushButton:hover { color: #ff6b6b; }"
        );

    connect(showLoginBtn, &QPushButton::clicked, this, &MainWindow::showLoginScreen);

    // Add to container layout
    containerLayout->addWidget(registerTitle);
    containerLayout->addWidget(registerSubtitle);
    containerLayout->addWidget(regUsernameLabel);
    containerLayout->addWidget(registerUsernameEdit);
    containerLayout->addWidget(emailLabel);
    containerLayout->addWidget(registerEmailEdit);
    containerLayout->addWidget(regPasswordLabel);
    containerLayout->addWidget(registerPasswordEdit);
    containerLayout->addWidget(registerBtn);
    containerLayout->addWidget(showLoginBtn);

    mainLayout->addWidget(registerContainer);
    stackedWidget->addWidget(registerWidget);
}

void MainWindow::setupUserProfileUI()
{
    userProfileWidget = new QWidget();
    userProfileWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(userProfileWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Create floating profile container - ORIGINAL SIZE
    QWidget *profileContainer = new QWidget();
    profileContainer->setFixedSize(600, 700); // Back to original size
    profileContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(profileContainer);
    containerLayout->setSpacing(3); // Minimal spacing
    containerLayout->setContentsMargins(30, 20, 30, 20);

    // Profile title - Bigger with larger font
    QLabel *profileTitle = new QLabel("👤 PLAYER PROFILE 👤");
    profileTitle->setAlignment(Qt::AlignCenter);
    profileTitle->setFixedHeight(70); // Increased height
    profileTitle->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #00ffff;" // Larger font
        "text-shadow: 0 0 20px #00ffff;"
        "background: transparent; padding: 15px;" // More padding
        "}"
        );

    // Welcome label - Bigger with larger font
    userWelcomeLabel = new QLabel("Welcome back, Champion!");
    userWelcomeLabel->setAlignment(Qt::AlignCenter);
    userWelcomeLabel->setFixedHeight(55); // Increased height
    userWelcomeLabel->setStyleSheet(
        "QLabel {"
        "font-size: 18px; color: #ffffff;" // Larger font
        "background: rgba(255, 255, 255, 0.1); border-radius: 8px; padding: 12px;" // More padding
        "}"
        );

    // Stats label - Bigger with larger font
    userStatsLabel = new QLabel("📊 Your Gaming Statistics");
    userStatsLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    userStatsLabel->setWordWrap(true);
    userStatsLabel->setFixedHeight(220); // Increased height
    userStatsLabel->setStyleSheet(
        "QLabel {"
        "font-size: 16px; color: #ffffff;" // Larger font
        "background: rgba(0, 255, 255, 0.1); border-radius: 10px; padding: 20px;" // More padding
        "border: 1px solid rgba(0, 255, 255, 0.3);"
        "line-height: 1.5;" // Better line spacing
        "}"
        );

    // Modern buttons - Bigger with larger fonts
    QPushButton *historyBtn = new QPushButton("📜 VIEW GAME HISTORY");
    historyBtn->setFixedHeight(55); // Increased height
    historyBtn->setMinimumWidth(520);
    historyBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;" // Larger font and padding
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #9b59b6, stop:1 #8e44ad);"
        "color: white; border: none; border-radius: 10px; margin: 2px 0px;" // Minimal margin
        "text-align: center;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #8e44ad, stop:1 #7d3c98);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #7d3c98, stop:1 #6c2c91);"
        "}"
        );

    connect(historyBtn, &QPushButton::clicked, this, &MainWindow::showGameHistory);

    QPushButton *continueBtn = new QPushButton("🎮 CONTINUE TO GAME");
    continueBtn->setFixedHeight(55);
    continueBtn->setMinimumWidth(520);
    continueBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #4ecdc4, stop:1 #44a08d);"
        "color: white; border: none; border-radius: 10px; margin: 2px 0px;"
        "text-align: center;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #26d0ce, stop:1 #1a2980);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #1a2980, stop:1 #0f1419);"
        "}"
        );

    connect(continueBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    logoutBtn = new QPushButton("🚪 LOGOUT");
    logoutBtn->setFixedHeight(55);
    logoutBtn->setMinimumWidth(520);
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff6b6b, stop:1 #ff5252);"
        "color: white; border: none; border-radius: 10px; margin: 2px 0px;"
        "text-align: center;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff5252, stop:1 #ff3838);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff3838, stop:1 #e53935);"
        "}"
        );

    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    // Remove extra spacers to save space

    // Add to container layout with tight spacing
    containerLayout->addWidget(profileTitle);
    containerLayout->addWidget(userWelcomeLabel);
    containerLayout->addWidget(userStatsLabel);
    containerLayout->addWidget(historyBtn);
    containerLayout->addWidget(continueBtn);
    containerLayout->addWidget(logoutBtn);

    mainLayout->addWidget(profileContainer);
    stackedWidget->addWidget(userProfileWidget);
}

void MainWindow::setupGameHistoryUI()
{
    gameHistoryWidget = new QWidget();
    gameHistoryWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(gameHistoryWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Create floating history container
    QWidget *historyContainer = new QWidget();
    historyContainer->setFixedSize(900, 700);
    historyContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(historyContainer);
    containerLayout->setSpacing(20);
    containerLayout->setContentsMargins(40, 30, 40, 30);

    // History title
    historyTitleLabel = new QLabel("📜 Your Game History 📜");
    historyTitleLabel->setAlignment(Qt::AlignCenter);
    historyTitleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #ffffff;"
        "text-shadow: 0 0 20px #ffffff;"
        "background: transparent; padding: 15px;"
        "}"
        );

    QLabel *historySubtitle = new QLabel("Your last 5 games - Track your progress! 📊");
    historySubtitle->setAlignment(Qt::AlignCenter);
    historySubtitle->setStyleSheet(
        "QLabel {"
        "font-size: 18px; color: #ffffff; margin-bottom: 20px;"
        "background: rgba(255, 255, 255, 0.1); border-radius: 10px; padding: 12px;"
        "}"
        );

    // 5 BLOCKS LAYOUT - HORIZONTAL
    QHBoxLayout *gamesLayout = new QHBoxLayout();
    gamesLayout->setSpacing(15);

    // Initialize the lists
    gameBlocks.clear();
    gameResultLabels.clear();
    gameDetailLabels.clear();

    // Create 5 game blocks
    for (int i = 0; i < 5; ++i) {
        QWidget *gameBlock = new QWidget();
        gameBlock->setFixedSize(150, 200);
        gameBlock->setStyleSheet(
            "QWidget {"
            "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
            "stop:0 rgba(255, 255, 255, 0.2), stop:1 rgba(255, 255, 255, 0.1));"
            "border: 2px solid rgba(0, 255, 255, 0.3);"
            "border-radius: 15px;"
            "}"
            );

        QVBoxLayout *blockLayout = new QVBoxLayout(gameBlock);
        blockLayout->setSpacing(8);
        blockLayout->setContentsMargins(10, 10, 10, 10);

        // Game number
        QLabel *gameNumber = new QLabel(QString("Game %1").arg(i + 1));
        gameNumber->setAlignment(Qt::AlignCenter);
        gameNumber->setStyleSheet(
            "QLabel {"
            "font-size: 16px; font-weight: bold; color: #00ffff;"
            "background: transparent; padding: 5px;"
            "}"
            );

        // Game result placeholder
        QLabel *gameResult = new QLabel("No Game");
        gameResult->setAlignment(Qt::AlignCenter);
        gameResult->setStyleSheet(
            "QLabel {"
            "font-size: 14px; color: #ffffff;"
            "background: rgba(255, 255, 255, 0.1); border-radius: 8px; padding: 8px;"
            "}"
            );

        // Game details placeholder
        QLabel *gameDetails = new QLabel("---");
        gameDetails->setAlignment(Qt::AlignCenter);
        gameDetails->setWordWrap(true);
        gameDetails->setStyleSheet(
            "QLabel {"
            "font-size: 12px; color: #ffffff;"
            "background: transparent; padding: 5px;"
            "}"
            );

        blockLayout->addWidget(gameNumber);
        blockLayout->addWidget(gameResult);
        blockLayout->addWidget(gameDetails);
        blockLayout->addStretch();

        gamesLayout->addWidget(gameBlock);

        // Store references for updating
        gameBlocks.append(gameBlock);
        gameResultLabels.append(gameResult);
        gameDetailLabels.append(gameDetails);
    }

    // Buttons - BIGGER
    replayBtn = new QPushButton("🎬 Watch Game Replays");
    replayBtn->setMinimumHeight(50);
    replayBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e67e22, stop:1 #d35400);"
        "color: white; border: none; border-radius: 12px; margin: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d35400, stop:1 #c0392b);"
        "}"
        );
    connect(replayBtn, &QPushButton::clicked, this, &MainWindow::showGameReplay);

    backFromHistoryBtn = new QPushButton("🔙 Back to Profile");
    backFromHistoryBtn->setMinimumHeight(50);
    backFromHistoryBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 18px; font-weight: bold; padding: 15px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9);"
        "color: white; border: none; border-radius: 12px; margin: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2980b9, stop:1 #1f618d);"
        "}"
        );
    connect(backFromHistoryBtn, &QPushButton::clicked, this, &MainWindow::showUserProfile);

    // Add to container layout
    containerLayout->addWidget(historyTitleLabel);
    containerLayout->addWidget(historySubtitle);
    containerLayout->addLayout(gamesLayout);
    containerLayout->addWidget(replayBtn);
    containerLayout->addWidget(backFromHistoryBtn);

    mainLayout->addWidget(historyContainer);
    stackedWidget->addWidget(gameHistoryWidget);
}

void MainWindow::setupGameReplayUI()
{
    gameReplayWidget = new QWidget();
    gameReplayWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(gameReplayWidget);
    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(30, 30, 30, 30);

    // Create floating replay container
    QWidget *replayContainer = new QWidget();
    replayContainer->setFixedSize(800, 700);
    replayContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(replayContainer);
    containerLayout->setSpacing(15);
    containerLayout->setContentsMargins(30, 25, 30, 25);

    // Replay title
    QLabel *replayTitle = new QLabel("🎬 Game Replay 🎬");
    replayTitle->setAlignment(Qt::AlignCenter);
    replayTitle->setMinimumHeight(55);
    replayTitle->setStyleSheet(
        "QLabel {"
        "font-size: 24px; font-weight: bold; color: #ffffff;"
        "text-shadow: 0 0 20px #ffffff;"
        "background: transparent; padding: 12px;"
        "min-height: 30px;"
        "}"
        );

    // Create horizontal layout for main content (left panel and right grid)
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->setSpacing(20);

    // LEFT PANEL - Controls and Info
    QWidget *leftPanel = new QWidget();
    leftPanel->setFixedWidth(400);
    leftPanel->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *leftPanelLayout = new QVBoxLayout(leftPanel);
    leftPanelLayout->setSpacing(15);
    leftPanelLayout->setContentsMargins(10, 10, 10, 10);

    // Game list
    QLabel *gameListLabel = new QLabel("📋 Select Game:");
    gameListLabel->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; color: #ffffff; margin: 5px; }"
        );

    gameListWidget = new QListWidget();
    gameListWidget->setFixedHeight(150);
    gameListWidget->setStyleSheet(
        "QListWidget {"
        "background: rgba(255, 255, 255, 0.1);"
        "border: 2px solid rgba(0, 255, 255, 0.3);"
        "border-radius: 10px; color: #ffffff;"
        "font-size: 13px; padding: 8px;"
        "}"
        "QListWidget::item {"
        "padding: 6px; border-radius: 5px; margin: 1px;"
        "}"
        "QListWidget::item:selected {"
        "background: rgba(0, 255, 255, 0.3);"
        "}"
        );

    connect(gameListWidget, &QListWidget::itemClicked, this, &MainWindow::onGameSelected);

    // Replay controls
    QLabel *controlsLabel = new QLabel("🎮 Controls:");
    controlsLabel->setStyleSheet(
        "QLabel { font-size: 16px; font-weight: bold; color: #ffffff; margin: 5px; }"
        );

    QHBoxLayout *controlsLayout = new QHBoxLayout();
    controlsLayout->setSpacing(10);

    playBtn = new QPushButton("▶️ Play");
    pauseBtn = new QPushButton("⏸️ Pause");
    resetBtn = new QPushButton("🔄 Reset");

    QString buttonStyle =
        "QPushButton {"
        "font-size: 14px; font-weight: bold; padding: 8px 12px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #4ecdc4, stop:1 #44a08d);"
        "color: white; border: none; border-radius: 8px;"
        "min-height: 20px; max-height: 35px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #26d0ce, stop:1 #1a2980);"
        "}";

    playBtn->setStyleSheet(buttonStyle);
    pauseBtn->setStyleSheet(buttonStyle);
    resetBtn->setStyleSheet(buttonStyle);

    connect(playBtn, &QPushButton::clicked, this, &MainWindow::onReplayPlay);
    connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::onReplayPause);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReplayReset);

    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(pauseBtn);
    controlsLayout->addWidget(resetBtn);

    // Speed control
    QLabel *speedLabel = new QLabel("⚡ Speed:");
    speedLabel->setStyleSheet("color: #ffffff; font-size: 14px; font-weight: bold;");

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 5);
    speedSlider->setValue(3);
    speedSlider->setFixedHeight(30);
    speedSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "border: 1px solid #bdc3c7; height: 8px;"
        "background: rgba(255, 255, 255, 0.3); border-radius: 4px;"
        "}"
        "QSlider::handle:horizontal {"
        "background: #00ffff; border: 1px solid #00ffff;"
        "width: 18px; margin: -5px 0; border-radius: 9px;"
        "}"
        );

    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::onReplaySpeedChanged);

    // Info labels
    replayInfoLabel = new QLabel("Select a game to replay");
    replayInfoLabel->setAlignment(Qt::AlignCenter);
    replayInfoLabel->setMinimumHeight(45);
    replayInfoLabel->setStyleSheet(
        "QLabel {"
        "font-size: 14px; color: #ffffff;"
        "background: rgba(255, 255, 255, 0.1); border-radius: 8px; padding: 8px;"
        "min-height: 25px;"
        "}"
        );

    replayStatusLabel = new QLabel("Ready");
    replayStatusLabel->setAlignment(Qt::AlignCenter);
    replayStatusLabel->setMinimumHeight(40);
    replayStatusLabel->setStyleSheet(
        "QLabel {"
        "font-size: 13px; color: #00ffff;"
        "background: rgba(0, 255, 255, 0.1); border-radius: 8px; padding: 6px;"
        "min-height: 20px;"
        "}"
        );

    // Back button
    QPushButton *backBtn = new QPushButton("🔙 Back to History");
    backBtn->setMinimumHeight(50);
    backBtn->setMaximumHeight(50);
    backBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold; padding: 10px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff6b6b, stop:1 #ff5252);"
        "color: white; border: none; border-radius: 8px;"
        "min-height: 25px; max-height: 25px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff5252, stop:1 #ff3838);"
        "}"
        );
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showGameHistory);

    // Add all left panel components
    leftPanelLayout->addWidget(gameListLabel);
    leftPanelLayout->addWidget(gameListWidget);
    leftPanelLayout->addWidget(controlsLabel);
    leftPanelLayout->addLayout(controlsLayout);
    leftPanelLayout->addWidget(speedLabel);
    leftPanelLayout->addWidget(speedSlider);
    leftPanelLayout->addWidget(replayInfoLabel);
    leftPanelLayout->addWidget(replayStatusLabel);
    leftPanelLayout->addWidget(backBtn);
    leftPanelLayout->addStretch(); // Push everything up

    // RIGHT PANEL - Game Board
    QWidget *rightPanel = new QWidget();
    rightPanel->setFixedWidth(340);
    rightPanel->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *rightPanelLayout = new QVBoxLayout(rightPanel);
    rightPanelLayout->setAlignment(Qt::AlignCenter);
    rightPanelLayout->setContentsMargins(10, 10, 10, 10);

    QLabel *boardLabel = new QLabel("🎯 Game Board");
    boardLabel->setAlignment(Qt::AlignCenter);
    boardLabel->setStyleSheet(
        "QLabel { font-size: 18px; font-weight: bold; color: #ffffff; margin: 10px; }"
        );

    // Replay board
    QWidget *replayBoardContainer = new QWidget();
    replayBoardContainer->setFixedSize(300, 300);
    replayBoardContainer->setStyleSheet(
        "QWidget {"
        "background: rgba(255, 255, 255, 0.1);"
        "border: 2px solid rgba(0, 255, 255, 0.3);"
        "border-radius: 15px;"
        "}"
        );

    QGridLayout *replayBoardLayout = new QGridLayout(replayBoardContainer);
    replayBoardLayout->setSpacing(5);
    replayBoardLayout->setContentsMargins(15, 15, 15, 15);

    // Create replay board cells
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            replayCells[i][j] = new QPushButton("");
            replayCells[i][j]->setFixedSize(80, 80);
            replayCells[i][j]->setEnabled(false);
            replayCells[i][j]->setStyleSheet(
                "QPushButton {"
                "font-size: 24px; font-weight: bold;"
                "background: rgba(255, 255, 255, 0.9);"
                "border: 1px solid #bdc3c7; border-radius: 8px;"
                "color: #2c3e50;"
                "}"
                );
            replayBoardLayout->addWidget(replayCells[i][j], i, j);
        }
    }

    rightPanelLayout->addWidget(boardLabel);
    rightPanelLayout->addWidget(replayBoardContainer);
    rightPanelLayout->addStretch(); // Center the board

    // Add left and right panels to main content layout
    mainContentLayout->addWidget(leftPanel);
    mainContentLayout->addWidget(rightPanel);

    // Initialize replay timer
    replayTimer = new QTimer(this);
    replayTimer->setSingleShot(false);
    connect(replayTimer, &QTimer::timeout, this, &MainWindow::replayNextMove);

    // Add to container layout
    containerLayout->addWidget(replayTitle);
    containerLayout->addLayout(mainContentLayout);

    mainLayout->addWidget(replayContainer);
    stackedWidget->addWidget(gameReplayWidget);
}

void MainWindow::setupGameSetupUI() {
    setupWidget = new QWidget();
    setupWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(setupWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget *setupContainer = new QWidget();
    setupContainer->setFixedSize(500, 450); // Increased height from 400 to 450
    setupContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(setupContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(50, 40, 50, 40);

    // Fixed title label with proper sizing and frame
    QLabel *setupTitle = new QLabel("⚙️ Game Setup ⚙️");
    setupTitle->setAlignment(Qt::AlignCenter);
    setupTitle->setMinimumHeight(70); // Ensure minimum height for emojis
    setupTitle->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setupTitle->setWordWrap(true); // Allow text wrapping if needed
    setupTitle->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff;"
        "background: transparent; padding: 15px;"
        "border: 2px solid rgba(0, 255, 255, 0.5);" // Added frame back
        "border-radius: 12px;" // Rounded corners for the frame
        "}"
        );

    QLabel *modeLabel = new QLabel("🎮 Choose Game Mode:");
    modeLabel->setMinimumHeight(35); // Ensure proper height
    modeLabel->setStyleSheet(
        "QLabel { font-size: 18px; font-weight: bold; color: #ffffff; margin: 8px; }"
        );

    modeButtonGroup = new QButtonGroup(this);

    pvpModeBtn = new QRadioButton("👥 Player vs Player");
    pvaiModeBtn = new QRadioButton("🤖 Player vs AI");

    QString radioStyle =
        "QRadioButton {"
        "font-size: 16px; color: #ffffff; padding: 8px;"
        "min-height: 25px;" // Ensure minimum height for radio buttons
        "}"
        "QRadioButton::indicator {"
        "width: 18px; height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: rgba(255, 255, 255, 0.1);"
        "}"
        "QRadioButton::indicator:checked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: #00ffff;"
        "}";

    pvpModeBtn->setStyleSheet(radioStyle);
    pvpModeBtn->setMinimumHeight(35);
    pvaiModeBtn->setStyleSheet(radioStyle);
    pvaiModeBtn->setMinimumHeight(35);

    modeButtonGroup->addButton(pvpModeBtn);
    modeButtonGroup->addButton(pvaiModeBtn);

    connect(pvpModeBtn, &QRadioButton::clicked, this, &MainWindow::onModeSelected);
    connect(pvaiModeBtn, &QRadioButton::clicked, this, &MainWindow::onModeSelected);

    // Elegant Back Button
    QPushButton *backBtn = new QPushButton("⬅️ Back");
    backBtn->setMinimumHeight(40);
    backBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold; padding: 8px 18px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #b2bec3, stop:1 #636e72);"
        "color: #222; border: none; border-radius: 10px; margin-top: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #636e72, stop:1 #b2bec3);"
        "color: #fff;"
        "}"
        );
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showUserProfile);

    containerLayout->addWidget(setupTitle);
    containerLayout->addWidget(modeLabel);
    containerLayout->addWidget(pvpModeBtn);
    containerLayout->addWidget(pvaiModeBtn);
    containerLayout->addWidget(backBtn);

    mainLayout->addWidget(setupContainer);
    stackedWidget->addWidget(setupWidget);
}
void MainWindow::setupDifficultyWidget()
{
    difficultyWidget = new QWidget();
    difficultyWidget->setStyleSheet("QWidget { background: transparent; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(difficultyWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    QWidget *difficultyContainer = new QWidget();
    difficultyContainer->setFixedSize(500, 450);
    difficultyContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(difficultyContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(50, 40, 50, 40);

    QLabel *difficultyTitle = new QLabel("🎯 Choose Difficulty 🎯");
    difficultyTitle->setAlignment(Qt::AlignCenter);
    difficultyTitle->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff;"
        "background: transparent; padding: 15px;"
        "}"
        );

    difficultyButtonGroup = new QButtonGroup(this);

    easyBtn = new QRadioButton("🟢 Easy - Good for beginners");
    mediumBtn = new QRadioButton("🟡 Medium - Balanced challenge");
    hardBtn = new QRadioButton("🔴 Hard - Expert level");

    QString radioStyle =
        "QRadioButton {"
        "font-size: 16px; color: #ffffff; padding: 10px;"
        "}"
        "QRadioButton::indicator {"
        "width: 18px; height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: rgba(255, 255, 255, 0.1);"
        "}"
        "QRadioButton::indicator:checked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: #00ffff;"
        "}";

    easyBtn->setStyleSheet(radioStyle);
    mediumBtn->setStyleSheet(radioStyle);
    hardBtn->setStyleSheet(radioStyle);

    difficultyButtonGroup->addButton(easyBtn);
    difficultyButtonGroup->addButton(mediumBtn);
    difficultyButtonGroup->addButton(hardBtn);

    connect(easyBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);
    connect(mediumBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);
    connect(hardBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);

    // Elegant Back Button
    QPushButton *backBtn = new QPushButton("⬅️ Back");
    backBtn->setMinimumHeight(40);
    backBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold; padding: 8px 18px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #b2bec3, stop:1 #636e72);"
        "color: #222; border: none; border-radius: 10px; margin-top: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #636e72, stop:1 #b2bec3);"
        "color: #fff;"
        "}"
        );
    connect(backBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    containerLayout->addWidget(difficultyTitle);
    containerLayout->addWidget(easyBtn);
    containerLayout->addWidget(mediumBtn);
    containerLayout->addWidget(hardBtn);
    containerLayout->addWidget(backBtn);

    mainLayout->addWidget(difficultyContainer);
    stackedWidget->addWidget(difficultyWidget);
}

void MainWindow::setupSymbolWidget()
{
    symbolWidget = new QWidget();
    symbolWidget->setStyleSheet("QWidget { background: transparent; }"); // Changed to transparent like other widgets

    QVBoxLayout *mainLayout = new QVBoxLayout(symbolWidget);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Create floating symbol container like difficulty widget
    QWidget *symbolContainer = new QWidget();
    symbolContainer->setFixedSize(500, 450); // Same size as difficulty widget
    symbolContainer->setStyleSheet(
        "QWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1, "
        "stop:0 rgba(255, 255, 255, 0.15), stop:1 rgba(255, 255, 255, 0.08));"
        "border: 2px solid rgba(255, 255, 255, 0.3);"
        "border-radius: 20px;"
        "}"
        );

    QVBoxLayout *containerLayout = new QVBoxLayout(symbolContainer);
    containerLayout->setSpacing(25);
    containerLayout->setContentsMargins(50, 40, 50, 40);

    // Title with consistent styling
    QLabel *symbolTitle = new QLabel("⭐ Pick Your Lucky Symbol! ⭐");
    symbolTitle->setAlignment(Qt::AlignCenter);
    symbolTitle->setStyleSheet(
        "QLabel {"
        "font-size: 28px; font-weight: bold; color: #00ffff;"
        "text-shadow: 0 0 20px #00ffff;"
        "background: transparent; padding: 15px;"
        "}"
        );

    QLabel *symbolLabel = new QLabel("✨ Which symbol represents YOU?");
    symbolLabel->setAlignment(Qt::AlignCenter);
    symbolLabel->setStyleSheet(
        "QLabel {"
        "font-size: 18px; color: #ffffff; margin-bottom: 15px;"
        "background: rgba(255, 255, 255, 0.1); border-radius: 10px; padding: 12px;"
        "}"
        );

    symbolButtonGroup = new QButtonGroup(this);

    xSymbolBtn = new QRadioButton("❌ X - I Love Going First!");
    xSymbolBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 16px; color: #ffffff; padding: 10px;"
        "}"
        "QRadioButton::indicator {"
        "width: 18px; height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: rgba(255, 255, 255, 0.1);"
        "}"
        "QRadioButton::indicator:checked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: #00ffff;"
        "}"
        );

    oSymbolBtn = new QRadioButton("⭕ O - I'm Patient and Strategic!");
    oSymbolBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 16px; color: #ffffff; padding: 10px;"
        "}"
        "QRadioButton::indicator {"
        "width: 18px; height: 18px;"
        "}"
        "QRadioButton::indicator:unchecked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: rgba(255, 255, 255, 0.1);"
        "}"
        "QRadioButton::indicator:checked {"
        "border: 2px solid #00ffff; border-radius: 9px;"
        "background: #00ffff;"
        "}"
        );

    symbolButtonGroup->addButton(xSymbolBtn);
    symbolButtonGroup->addButton(oSymbolBtn);

    connect(xSymbolBtn, &QRadioButton::clicked, this, &MainWindow::onSymbolSelected);
    connect(oSymbolBtn, &QRadioButton::clicked, this, &MainWindow::onSymbolSelected);

    // Elegant Back and Cancel buttons in a horizontal layout
    QHBoxLayout *buttonRow = new QHBoxLayout();
    buttonRow->setSpacing(20);

    QPushButton *backBtn = new QPushButton("⬅️ Back");
    backBtn->setMinimumHeight(40);
    backBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold; padding: 8px 18px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #b2bec3, stop:1 #636e72);"
        "color: #222; border: none; border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #636e72, stop:1 #b2bec3);"
        "color: #fff;"
        "}"
        );
    connect(backBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(difficultyWidget);
    });

    QPushButton *cancelBtn = new QPushButton("❌ Cancel");
    cancelBtn->setMinimumHeight(40);
    cancelBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 15px; font-weight: bold; padding: 8px 18px;"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff6b6b, stop:1 #ff5252);"
        "color: white; border: none; border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
        "stop:0 #ff5252, stop:1 #ff3838);"
        "}"
        );
    connect(cancelBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(setupWidget);
    });

    buttonRow->addWidget(backBtn);
    buttonRow->addWidget(cancelBtn);

    containerLayout->addWidget(symbolTitle);
    containerLayout->addWidget(symbolLabel);
    containerLayout->addWidget(xSymbolBtn);
    containerLayout->addWidget(oSymbolBtn);
    containerLayout->addLayout(buttonRow);

    mainLayout->addWidget(symbolContainer);
    stackedWidget->addWidget(symbolWidget);
}


void MainWindow::showLoginScreen()
{
    stackedWidget->setCurrentWidget(loginWidget);
    setWindowTitle("🎮 Tic Tac Toe - Sign In to Play!");

    // Clear login fields
    loginUsernameEdit->clear();
    loginPasswordEdit->clear();
}

void MainWindow::showRegisterScreen()
{
    stackedWidget->setCurrentWidget(registerWidget);
    setWindowTitle("📝 Tic Tac Toe - Create Account!");

    // Clear register fields
    registerUsernameEdit->clear();
    registerPasswordEdit->clear();
    registerEmailEdit->clear();
}

void MainWindow::showUserProfile()
{
    if (!userManager->isUserLoggedIn()) {
        showLoginScreen();
        return;
    }
    updateUserProfileDisplay();
    stackedWidget->setCurrentWidget(userProfileWidget);
    setWindowTitle("👤 Tic Tac Toe - Your Profile");
}

void MainWindow::updateUserProfileDisplay()
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    userWelcomeLabel->setText(QString("🎉 Welcome back, %1! 🎉").arg(user->getUsername()));

    QString stats = QString(
                        "📊 **Your Gaming Statistics** 📊\n\n"
                        "🎮 Total Games Played: %1\n"
                        "🏆 Victories: %2\n"
                        "😞 Defeats: %3\n"
                        "🤝 Ties: %4\n"
                        "📈 Win Rate: %5%\n\n"
                        "🕐 Last Login: %6"
                        ).arg(user->getTotalGames())
                        .arg(user->getGamesWon())
                        .arg(user->getGamesLost())
                        .arg(user->getGamesTied())
                        .arg(QString::number(user->getWinRate(), 'f', 1))
                        .arg(user->getLastLogin().toString("MMM dd, yyyy hh:mm AP"));

    userStatsLabel->setText(stats);
}

void MainWindow::onLoginClicked()
{
    QString username = loginUsernameEdit->text().trimmed();
    QString password = loginPasswordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        // With this:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Login Failed");
        msgBox.setText("Please enter both username and password! 📝");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        return;
    }

    if (userManager->loginUser(username, password)) {
        // With this:
        QMessageBox msgBox;
        msgBox.setWindowTitle("✅ Welcome Back!");
        msgBox.setText(QString("Hello %1! Ready to play? 🎮").arg(username));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();     showUserProfile();
    } else {
        // With this:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Login Failed");
        msgBox.setText("Invalid username or password. Please try again! 🔐");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        loginPasswordEdit->clear();
        loginPasswordEdit->setFocus();
    }
}

void MainWindow::onRegisterClicked()
{
    QString username = registerUsernameEdit->text().trimmed();
    QString password = registerPasswordEdit->text();
    QString email = registerEmailEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        // Replace username/password empty warning:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Registration Failed");
        msgBox.setText("Please enter both username and password! 📝");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();    return;
    }

    if (username.length() < 3) {
        // Replace username length warning:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Registration Failed");
        msgBox.setText("Username must be at least 3 characters long! 📏");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        return;
    }

    if (password.length() < 4) {
        // Replace password length warning:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Registration Failed");
        msgBox.setText("Password must be at least 4 characters long! 🔒");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        return;
    }

    if (userManager->userExists(username)) {
        // Replace username exists warning:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Registration Failed");
        msgBox.setText("Username already exists. Please choose another! 👤");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        registerUsernameEdit->setFocus();
        return;
    }

    if (userManager->registerUser(username, password, email)) {
        QMessageBox msgBox;
        msgBox.setWindowTitle("🎉 Registration Successful!");
        msgBox.setText(QString("Welcome to Tic Tac Toe, %1! Your account has been created. Please sign in to start playing! 🎮").arg(username));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
        // Pre-fill login form
        loginUsernameEdit->setText(username);
        loginPasswordEdit->clear();
        showLoginScreen();
        loginPasswordEdit->setFocus();
    } else {
        // Replace registration failed message:
        QMessageBox msgBox;
        msgBox.setWindowTitle("❌ Registration Failed");
        msgBox.setText("Failed to create account. Please try again! 🔄");
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
    }
}

void MainWindow::onLogoutClicked()
{
    if (userManager->isUserLoggedIn()) {
        QString username = userManager->getCurrentUser()->getUsername();
        userManager->logoutUser();

        QMessageBox msgBox;
        msgBox.setWindowTitle("👋 Goodbye!");
        msgBox.setText(QString("See you later, %1! Thanks for playing! 🎮").arg(username));
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStyleSheet("background-color: white; color: black;");
        msgBox.exec();
    }
    showLoginScreen();
}


void MainWindow::updateUserStats(bool won, bool lost, bool tied)
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    if (won) user->addWin();
    if (lost) user->addLoss();
    if (tied) user->addTie();

    userManager->saveUserData();
}

void MainWindow::showGameHistory()
{
    if (!userManager->isUserLoggedIn()) {
        showLoginScreen();
        return;
    }
    updateGameHistoryDisplay();
    stackedWidget->setCurrentWidget(gameHistoryWidget);
    setWindowTitle("📜 Tic Tac Toe - Game History");
}

void MainWindow::recordGameResult(const QString &result, const QString &opponent, const QString &gameMode, const QString &playerSymbol)
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    GameRecord record(opponent, result, gameMode, playerSymbol);
    record.moves = QVector<GameMove>::fromList(currentGameMoves);
    user->addGameToHistory(record);
    userManager->saveUserData();

    // Clear moves for next game
    currentGameMoves.clear();
}

void MainWindow::recordGameMove(int row, int col, int player)
{
    GameMove move(row, col, player);
    currentGameMoves.append(move);
}

void MainWindow::showGameReplay()
{
    if (!userManager->isUserLoggedIn()) {
        showLoginScreen();
        return;
    }

    // Clear the current game list
    gameListWidget->clear();

    // Load ALL game history from database (not just last 5)
    User* currentUser = userManager->getCurrentUser();
    if (currentUser) {
        QString username = currentUser->getUsername();
        QList<GameRecord> allHistory = userManager->loadAllGameHistoryFromDatabase(username);

        // Populate the game list with all historical games
        for (int i = 0; i < allHistory.size(); ++i) {
            const GameRecord &record = allHistory[i];
            QString resultEmoji = (record.result == "Won") ? "🏆" :
                                      (record.result == "Lost") ? "😞" : "🤝";
            QString itemText = QString("%1 Game %2 - vs %3 (%4)\n📅 %5")
                                   .arg(resultEmoji)
                                   .arg(i + 1)
                                   .arg(record.opponent)
                                   .arg(record.result)
                                   .arg(record.timestamp.toString("MMM dd, hh:mm"));

            QListWidgetItem *item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, QVariant::fromValue(record));
            gameListWidget->addItem(item);
        }
    }

    stackedWidget->setCurrentWidget(gameReplayWidget);
    setWindowTitle("🎬 Tic Tac Toe - Game Replay");
}

void MainWindow::onGameSelected()
{
    QListWidgetItem *item = gameListWidget->currentItem();
    if (!item) return;

    int gameIndex = item->data(Qt::UserRole).toInt();
    User* user = userManager->getCurrentUser();
    if (!user) return;

    QList<GameRecord> history = user->getGameHistory();
    if (gameIndex >= 0 && gameIndex < history.size()) {
        initializeReplay(history[gameIndex]);
    }
}


void MainWindow::initializeReplay(const GameRecord &record)
{
    currentReplayGame = record;
    currentReplayMoveIndex = 0;
    isReplaying = false;

    resetReplayBoard();
    updateReplayBoard();

    QString info = QString("🎮 %1 vs %2 | %3 | %4")
                       .arg(record.playerSymbol == "X" ? "You (X)" : record.opponent + " (X)")
                       .arg(record.playerSymbol == "O" ? "You (O)" : record.opponent + " (O)")
                       .arg(record.gameMode)
                       .arg(record.result);

    replayInfoLabel->setText(info);
    replayStatusLabel->setText("🎬 Ready to replay - Press Play!");

    // Enable controls
    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);
    resetBtn->setEnabled(true);
}

void MainWindow::onReplayPlay()
{
    if (currentReplayGame.moves.isEmpty()) return;

    isReplaying = true;
    playBtn->setEnabled(false);
    pauseBtn->setEnabled(true);

    int speed = speedSlider->value();
    int interval = 2000 - (speed - 1) * 300; // 1700ms to 500ms
    replayTimer->start(interval);

    replayStatusLabel->setText("▶️ Playing replay...");
}

void MainWindow::onReplayPause()
{
    isReplaying = false;
    replayTimer->stop();

    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);

    replayStatusLabel->setText("⏸️ Replay paused");
}

void MainWindow::onReplayReset()
{
    isReplaying = false;
    replayTimer->stop();
    currentReplayMoveIndex = 0;

    resetReplayBoard();
    updateReplayBoard();

    playBtn->setEnabled(true);
    pauseBtn->setEnabled(false);

    replayStatusLabel->setText("🔄 Replay reset - Ready to play!");
}

void MainWindow::onReplaySpeedChanged(int speed)
{
    if (isReplaying) {
        int interval = 2000 - (speed - 1) * 300;
        replayTimer->setInterval(interval);
    }
}

void MainWindow::replayNextMove()
{
    if (currentReplayMoveIndex >= currentReplayGame.moves.size()) {
        // Replay finished
        onReplayPause();
        replayStatusLabel->setText("🎉 Replay completed!");
        return;
    }

    const GameMove &move = currentReplayGame.moves[currentReplayMoveIndex];
    replayBoard[move.row][move.col] = move.player;

    updateReplayBoard();

    QString playerName = (move.player == 1) ? "X" : "O";
    replayStatusLabel->setText(QString("▶️ Move %1: %2 played at (%3, %4)")
                                   .arg(currentReplayMoveIndex + 1)
                                   .arg(playerName)
                                   .arg(move.row + 1)
                                   .arg(move.col + 1));

    currentReplayMoveIndex++;
}

void MainWindow::updateReplayBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            QString text = "";
            QString color = "rgba(255, 255, 255, 0.9)";
            QString textColor = "#2c3e50";

            if (replayBoard[i][j] == 1) {
                text = "X";
                color = "#e74c3c";
                textColor = "white";
            } else if (replayBoard[i][j] == 2) {
                text = "O";
                color = "#3498db";
                textColor = "white";
            }

            replayCells[i][j]->setText(text);
            replayCells[i][j]->setStyleSheet(
                QString("QPushButton {"
                        "font-size: 16px; font-weight: bold; background: %1;"
                        "border: 1px solid #bdc3c7; border-radius: 8px; color: %2;"
                        "}")
                    .arg(color, textColor)
                );
        }
    }
}

void MainWindow::resetReplayBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            replayBoard[i][j] = 0;
        }
    }
}

void MainWindow::showPlayerNamesInput()
{
    stackedWidget->setCurrentWidget(playerNamesWidget);
    setWindowTitle("👥 Tic Tac Toe - Player Names");

    // Pre-fill player 1 with logged-in user's name
    if (userManager->isUserLoggedIn()) {
        player1NameEdit->setText(userManager->getCurrentUser()->getUsername());
    } else {
        player1NameEdit->setText("Player 1");
    }

    player2NameEdit->setText("Player 2");
    player2NameEdit->setFocus();
    player2NameEdit->selectAll();
}

void MainWindow::onPlayerNamesConfirmed()
{
    QString p1Name = player1NameEdit->text().trimmed();
    QString p2Name = player2NameEdit->text().trimmed();

    if (p1Name.isEmpty()) p1Name = "Player 1";
    if (p2Name.isEmpty()) p2Name = "Player 2";

    player1Name = p1Name;
    player2Name = p2Name;

    // Show game and start playing
    showGame();
    newGame();
}

void MainWindow::createFloatingDecorations()
{
    // Clear previous decorations
    for (QLabel* label : decorationLabels) {
        label->deleteLater();
    }
    decorationLabels.clear();

    // Create floating celebration decorations
    QStringList decorations = {"🎉", "🎊", "✨", "🌟", "🎈", "🏆", "🥳", "💫", "🎁", "🎀"};

    for (int i = 0; i < 8; ++i) {  // REDUCED number of decorations
        QLabel* decoration = new QLabel(gameWidget);
        decoration->setText(decorations[i % decorations.size()]);
        decoration->setStyleSheet("font-size: 20px; background: transparent;");  // REDUCED size
        decoration->setAttribute(Qt::WA_TransparentForMouseEvents);

        // Random position
        int x = rand() % (gameWidget->width() - 30);
        int y = rand() % (gameWidget->height() - 30);
        decoration->move(x, y);
        decoration->show();

        decorationLabels.append(decoration);

        // Animate the decoration
        QPropertyAnimation* animation = new QPropertyAnimation(decoration, "pos");
        animation->setDuration(2000 + rand() % 1000);  // REDUCED duration
        animation->setStartValue(QPoint(x, y));
        animation->setEndValue(QPoint(x + (rand() % 100 - 50), y - 50 - rand() % 50));
        animation->setEasingCurve(QEasingCurve::OutQuad);

        // Fade out animation
        QPropertyAnimation* fadeAnimation = new QPropertyAnimation(decoration, "windowOpacity");
        fadeAnimation->setDuration(2000);  // REDUCED duration
        fadeAnimation->setStartValue(1.0);
        fadeAnimation->setEndValue(0.0);

        animation->start(QAbstractAnimation::DeleteWhenStopped);
        fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::createCryingEmojis()
{
    // Clear previous crying emojis
    for (QLabel* label : cryingEmojiLabels) {
        label->deleteLater();
    }
    cryingEmojiLabels.clear();

    // Create crying emojis
    QStringList cryingEmojis = {"😭", "😢", "😿", "💧", "😞", "😔"};

    for (int i = 0; i < 6; ++i) {  // REDUCED number
        QLabel* cryingEmoji = new QLabel(gameWidget);
        cryingEmoji->setText(cryingEmojis[i % cryingEmojis.size()]);
        cryingEmoji->setStyleSheet("font-size: 24px; background: transparent;");  // REDUCED size
        cryingEmoji->setAttribute(Qt::WA_TransparentForMouseEvents);

        // Random position
        int x = rand() % (gameWidget->width() - 30);
        int y = rand() % (gameWidget->height() - 30);
        cryingEmoji->move(x, y);
        cryingEmoji->show();

        cryingEmojiLabels.append(cryingEmoji);

        // Animate the crying emoji falling down like tears
        QPropertyAnimation* animation = new QPropertyAnimation(cryingEmoji, "pos");
        animation->setDuration(1500 + rand() % 500);  // REDUCED duration
        animation->setStartValue(QPoint(x, y));
        animation->setEndValue(QPoint(x + (rand() % 40 - 20), y + 80 + rand() % 50));
        animation->setEasingCurve(QEasingCurve::InQuad);

        // Fade out animation
        QPropertyAnimation* fadeAnimation = new QPropertyAnimation(cryingEmoji, "windowOpacity");
        fadeAnimation->setDuration(1800);  // REDUCED duration
        fadeAnimation->setStartValue(1.0);
        fadeAnimation->setEndValue(0.0);

        animation->start(QAbstractAnimation::DeleteWhenStopped);
        fadeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void MainWindow::playWinAnimationWithDecorations(Player winner)
{
    // Create floating decorations
    createFloatingDecorations();

    // Create celebration animation sequence
    winAnimationGroup->clear();

    // Scale animation for status label
    QPropertyAnimation *scaleUp = new QPropertyAnimation(statusLabel, "geometry");
    QRect originalGeometry = statusLabel->geometry();
    QRect scaledGeometry = originalGeometry;
    scaledGeometry.setWidth(originalGeometry.width() * 1.2);  // REDUCED scale
    scaledGeometry.setHeight(originalGeometry.height() * 1.2);
    scaledGeometry.moveCenter(originalGeometry.center());

    scaleUp->setDuration(300);  // REDUCED duration
    scaleUp->setStartValue(originalGeometry);
    scaleUp->setEndValue(scaledGeometry);
    scaleUp->setEasingCurve(QEasingCurve::OutBounce);

    // Scale back down
    QPropertyAnimation *scaleDown = new QPropertyAnimation(statusLabel, "geometry");
    scaleDown->setDuration(300);  // REDUCED duration
    scaleDown->setStartValue(scaledGeometry);
    scaleDown->setEndValue(originalGeometry);
    scaleDown->setEasingCurve(QEasingCurve::InBounce);

    // Flash effects for celebration
    QPropertyAnimation *flash1 = new QPropertyAnimation(statusOpacityEffect, "opacity");
    flash1->setDuration(100);  // REDUCED duration
    flash1->setStartValue(1.0);
    flash1->setEndValue(0.3);

    QPropertyAnimation *flash2 = new QPropertyAnimation(statusOpacityEffect, "opacity");
    flash2->setDuration(100);  // REDUCED duration
    flash2->setStartValue(0.3);
    flash2->setEndValue(1.0);

    // Use winner's cell color as background
    QString winnerColor;
    if (winner == HUMAN) {
        winnerColor = "#e74c3c"; // X color (red)
    } else {
        winnerColor = "#3498db"; // O color (blue)
    }

    // Update status label style with winner's color background
    statusLabel->setStyleSheet(
        QString("font-size: 14px; font-weight: bold; color: white; "  // REDUCED font size
                "background: %1; "
                "border-radius: 8px; padding: 6px; margin: 3px;"  // REDUCED
                "border: 2px solid #ffffff;").arg(winnerColor)
        );

    // Add animations to sequence
    winAnimationGroup->addAnimation(scaleUp);
    winAnimationGroup->addAnimation(flash1);
    winAnimationGroup->addAnimation(flash2);
    winAnimationGroup->addAnimation(flash1);
    winAnimationGroup->addAnimation(flash2);
    winAnimationGroup->addAnimation(scaleDown);

    winAnimationGroup->start();
}

void MainWindow::playLossAnimationWithCrying()
{
    // Create crying emojis
    createCryingEmojis();

    // Create sad animation sequence
    winAnimationGroup->clear();

    // Enhanced shake animation for loss
    QPropertyAnimation *shake1 = new QPropertyAnimation(statusLabel, "geometry");
    QRect originalGeometry = statusLabel->geometry();
    QRect shakeLeft = originalGeometry;
    shakeLeft.translate(-8, 0);  // REDUCED shake distance

    shake1->setDuration(60);  // REDUCED duration
    shake1->setStartValue(originalGeometry);
    shake1->setEndValue(shakeLeft);

    QPropertyAnimation *shake2 = new QPropertyAnimation(statusLabel, "geometry");
    QRect shakeRight = originalGeometry;
    shakeRight.translate(8, 0);  // REDUCED shake distance

    shake2->setDuration(60);  // REDUCED duration
    shake2->setStartValue(shakeLeft);
    shake2->setEndValue(shakeRight);

    QPropertyAnimation *shake3 = new QPropertyAnimation(statusLabel, "geometry");
    shake3->setDuration(60);  // REDUCED duration
    shake3->setStartValue(shakeRight);
    shake3->setEndValue(shakeLeft);

    QPropertyAnimation *shake4 = new QPropertyAnimation(statusLabel, "geometry");
    shake4->setDuration(60);  // REDUCED duration
    shake4->setStartValue(shakeLeft);
    shake4->setEndValue(originalGeometry);

    // Sad fade animation
    QPropertyAnimation *fadeOut = new QPropertyAnimation(statusOpacityEffect, "opacity");
    fadeOut->setDuration(400);  // REDUCED duration
    fadeOut->setStartValue(1.0);
    fadeOut->setEndValue(0.5);

    QPropertyAnimation *fadeIn = new QPropertyAnimation(statusOpacityEffect, "opacity");
    fadeIn->setDuration(400);  // REDUCED duration
    fadeIn->setStartValue(0.5);
    fadeIn->setEndValue(1.0);

    // Update status label style for sadness with crying theme
    statusLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: white; "  // REDUCED font size
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #74b9ff, stop:0.5 #0984e3, stop:1 #6c5ce7); "
        "border-radius: 8px; padding: 6px; margin: 3px;"  // REDUCED
        "border: 2px solid #a29bfe;"
        );

    winAnimationGroup->addAnimation(shake1);
    winAnimationGroup->addAnimation(shake2);
    winAnimationGroup->addAnimation(shake3);
    winAnimationGroup->addAnimation(shake4);
    winAnimationGroup->addAnimation(fadeOut);
    winAnimationGroup->addAnimation(fadeIn);

    winAnimationGroup->start();
}

void MainWindow::playTieAnimation()
{
    // Create tie animation sequence
    winAnimationGroup->clear();

    // Gentle bounce animation for tie
    QPropertyAnimation *bounceUp = new QPropertyAnimation(statusLabel, "geometry");
    QRect originalGeometry = statusLabel->geometry();
    QRect bounceGeometry = originalGeometry;
    bounceGeometry.translate(0, -10);  // REDUCED bounce distance

    bounceUp->setDuration(300);  // REDUCED duration
    bounceUp->setStartValue(originalGeometry);
    bounceUp->setEndValue(bounceGeometry);
    bounceUp->setEasingCurve(QEasingCurve::OutQuad);

    QPropertyAnimation *bounceDown = new QPropertyAnimation(statusLabel, "geometry");
    bounceDown->setDuration(300);  // REDUCED duration
    bounceDown->setStartValue(bounceGeometry);
    bounceDown->setEndValue(originalGeometry);
    bounceDown->setEasingCurve(QEasingCurve::OutBounce);

    // Update status label style for tie
    statusLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: white; "  // REDUCED font size
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #fdcb6e, stop:1 #e17055); "
        "border-radius: 8px; padding: 6px; margin: 3px;"  // REDUCED
        "border: 2px solid #fd79a8;"
        );

    winAnimationGroup->addAnimation(bounceUp);
    winAnimationGroup->addAnimation(bounceDown);

    winAnimationGroup->start();
}

void MainWindow::animateButton(QPushButton *button)
{
    // Animate the clicked button
    QPropertyAnimation *buttonAnimation = new QPropertyAnimation(button, "geometry");
    QRect originalGeometry = button->geometry();
    QRect scaledGeometry = originalGeometry;
    scaledGeometry.setWidth(originalGeometry.width() * 1.05);  // REDUCED scale
    scaledGeometry.setHeight(originalGeometry.height() * 1.05);
    scaledGeometry.moveCenter(originalGeometry.center());

    buttonAnimation->setDuration(100);  // REDUCED duration
    buttonAnimation->setStartValue(originalGeometry);
    buttonAnimation->setEndValue(scaledGeometry);
    buttonAnimation->setEasingCurve(QEasingCurve::OutBack);

    // Scale back
    connect(buttonAnimation, &QPropertyAnimation::finished, [=]() {
        QPropertyAnimation *scaleBack = new QPropertyAnimation(button, "geometry");
        scaleBack->setDuration(100);  // REDUCED duration
        scaleBack->setStartValue(scaledGeometry);
        scaleBack->setEndValue(originalGeometry);
        scaleBack->setEasingCurve(QEasingCurve::InBack);
        scaleBack->start(QAbstractAnimation::DeleteWhenStopped);
    });

    buttonAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::showGameSetup()
{
    if (!userManager->isUserLoggedIn()) {
        showLoginScreen();
        return;
    }

    stackedWidget->setCurrentWidget(setupWidget);
    setWindowTitle("🎮 Tic Tac Toe - Game Setup");

    // Reset setup to initial state - NO DEFAULT SELECTIONS
    modeButtonGroup->setExclusive(false);
    pvpModeBtn->setChecked(false);
    pvaiModeBtn->setChecked(false);
    modeButtonGroup->setExclusive(true);

    difficultyButtonGroup->setExclusive(false);
    easyBtn->setChecked(false);
    mediumBtn->setChecked(false);
    hardBtn->setChecked(false);
    difficultyButtonGroup->setExclusive(true);

    symbolButtonGroup->setExclusive(false);
    xSymbolBtn->setChecked(false);
    oSymbolBtn->setChecked(false);
    symbolButtonGroup->setExclusive(true);
}

void MainWindow::showGame()
{
    stackedWidget->setCurrentWidget(gameWidget);
    setWindowTitle("🎮 Tic Tac Toe - Playing!");
}

void MainWindow::startGameSetup()
{
    showGameSetup();
}

void MainWindow::onModeSelected()
{
    QRadioButton *senderBtn = qobject_cast<QRadioButton*>(sender());
    if (!senderBtn) return;

    isPvP = pvpModeBtn->isChecked();

    if (isPvP) {
        game->setGameMode(true);
        QTimer::singleShot(300, this, [this]() {  // REDUCED delay
            showPlayerNamesInput();
        });
    } else {
        QTimer::singleShot(300, this, [this]() {  // REDUCED delay
            stackedWidget->setCurrentWidget(difficultyWidget);
        });
    }
}

void MainWindow::onDifficultySelected()
{
    QRadioButton *senderBtn = qobject_cast<QRadioButton*>(sender());
    if (!senderBtn) return;

    if (easyBtn->isChecked()) selectedDifficulty = 1;
    else if (mediumBtn->isChecked()) selectedDifficulty = 2;
    else selectedDifficulty = 3;

    game->setDifficulty(selectedDifficulty);

    QTimer::singleShot(300, this, [this]() {  // REDUCED delay
        stackedWidget->setCurrentWidget(symbolWidget);
    });
}

void MainWindow::onSymbolSelected()
{
    QRadioButton *senderBtn = qobject_cast<QRadioButton*>(sender());
    if (!senderBtn) return;

    isPlayerX = xSymbolBtn->isChecked();

    if (isPlayerX) {
        humanPlayer = HUMAN;
        aiPlayer = AI;
    } else {
        humanPlayer = AI;
        aiPlayer = HUMAN;
    }

    game->setGameMode(false);

    QTimer::singleShot(300, this, [this]() {  // REDUCED delay
        showGame();
        newGame();
    });
}

void MainWindow::cellClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    // Animate the clicked button
    animateButton(button);

    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    Player movePlayer;
    if (isPvP) {
        movePlayer = game->getCurrentPlayer();
    } else {
        movePlayer = humanPlayer;

        if (game->getCurrentPlayer() != humanPlayer) {
            return;
        }
    }

    if (game->makeMove(row, col, movePlayer)) {
        // Record the move for replay
        recordGameMove(row, col, static_cast<int>(movePlayer));

        updateBoard();
        checkGameEnd();

        if (!game->isBoardFull() && !game->checkWin(HUMAN) && !game->checkWin(AI)) {
            game->switchPlayer();

            if (!isPvP && game->getCurrentPlayer() == aiPlayer) {
                statusLabel->setText("AI is thinking...");
                QString aiSymbol = (aiPlayer == HUMAN) ? "X" : "O";
                currentPlayerLabel->setText("Current Player: " + aiSymbol + " (AI)");

                for (int i = 0; i < 3; ++i) {
                    for (int j = 0; j < 3; ++j) {
                        cells[i][j]->setEnabled(false);
                    }
                }
                aiTimer->start(1000);  // REDUCED AI thinking time
            } else {
                if (isPvP) {
                    // Show actual player names
                    if (game->getCurrentPlayer() == HUMAN) {
                        currentPlayerLabel->setText(QString("Current Player: %1 (X)").arg(player1Name));
                    } else {
                        currentPlayerLabel->setText(QString("Current Player: %1 (O)").arg(player2Name));
                    }
                } else {
                    QString humanSymbol = (humanPlayer == HUMAN) ? "X" : "O";
                    currentPlayerLabel->setText("Current Player: " + humanSymbol + " (You)");
                    statusLabel->setText("Your turn!");
                }
            }
        }
    }
}

void MainWindow::makeAIMove()
{
    auto move = game->getAIMove();
    if (move.first != -1 && move.second != -1) {
        // Record AI move for replay
        recordGameMove(move.first, move.second, static_cast<int>(aiPlayer));

        game->makeMove(move.first, move.second, aiPlayer);
        updateBoard();
        checkGameEnd();

        if (!game->isBoardFull() && !game->checkWin(HUMAN) && !game->checkWin(AI)) {
            game->switchPlayer();
            QString humanSymbol = (humanPlayer == HUMAN) ? "X" : "O";
            currentPlayerLabel->setText("Current Player: " + humanSymbol + " (You)");
            statusLabel->setText("Your turn!");
        }
    }
}

void MainWindow::updateBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            Player cell = game->getCell(i, j);
            QString text = "";
            QString color = "rgba(255, 255, 255, 0.9)";
            QString textColor = "#2c3e50";

            if (cell == HUMAN) {
                text = "X";
                color = "#e74c3c";
                textColor = "white";
            } else if (cell == AI) {
                text = "O";
                color = "#3498db";
                textColor = "white";
            }

            cells[i][j]->setText(text);
            cells[i][j]->setStyleSheet(
                QString("QPushButton {"
                        "font-size: %1px; font-weight: bold; background: %2;"  // Dynamic font size
                        "border: 1px solid #bdc3c7; border-radius: 8px; color: %3;"
                        "}"
                        "QPushButton:hover { background: %2; border: 2px solid #3498db; }"
                        "QPushButton:disabled { background: %2; }")
                    .arg(cells[i][j]->width() * 0.25)  // Font size based on cell size
                    .arg(color, textColor)
                );
            cells[i][j]->setEnabled(cell == NONE);
        }
    }
}

void MainWindow::checkGameEnd()
{
    if (game->checkWin(HUMAN)) {
        QString winner;
        bool playerWon = false;
        QString gameMode, opponent, result, playerSymbol;

        if (isPvP) {
            // Use actual player names
            winner = QString("🎉🎊 %1 Won! Congratulations! 🎊🎉").arg(player1Name);
            playerWon = true;

            // Record for logged-in user if they are player 1
            if (userManager->isUserLoggedIn() &&
                userManager->getCurrentUser()->getUsername() == player1Name) {
                gameMode = "PvP";
                opponent = player2Name;
                result = "Won";
                playerSymbol = "X";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            }
        } else {
            if (humanPlayer == HUMAN) {
                winner = "🎉🎊 YOU WON! Amazing! 🎊🎉";
                playerWon = true;
                updateUserStats(true, false, false);

                // Record game history
                QString difficulty = (selectedDifficulty == 1) ? "Easy" :
                                         (selectedDifficulty == 2) ? "Medium" : "Hard";
                gameMode = "PvAI " + difficulty;
                opponent = "AI";
                result = "Won";
                playerSymbol = (humanPlayer == HUMAN) ? "X" : "O";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            } else {
                winner = "😭💔 You Lost... 💔😭";
                playerWon = false;
                updateUserStats(false, true, false);

                // Record game history
                QString difficulty = (selectedDifficulty == 1) ? "Easy" :
                                         (selectedDifficulty == 2) ? "Medium" : "Hard";
                gameMode = "PvAI " + difficulty;
                opponent = "AI";
                result = "Lost";
                playerSymbol = (humanPlayer == HUMAN) ? "X" : "O";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            }
        }
        statusLabel->setText(winner);

        // Play appropriate animation with decorations/crying
        if (playerWon) {
            playWinAnimationWithDecorations(HUMAN);
        } else {
            playLossAnimationWithCrying();
        }

        resetBoard();

    } else if (game->checkWin(AI)) {
        QString winner;
        bool playerWon = false;
        QString gameMode, opponent, result, playerSymbol;

        if (isPvP) {
            // Use actual player names
            winner = QString("🎉🎊 %1 Won! Congratulations! 🎊🎉").arg(player2Name);
            playerWon = true;

            // Record for logged-in user if they are player 2
            if (userManager->isUserLoggedIn() &&
                userManager->getCurrentUser()->getUsername() == player2Name) {
                gameMode = "PvP";
                opponent = player1Name;
                result = "Won";
                playerSymbol = "O";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            }
        } else {
            if (humanPlayer == AI) {
                winner = "🎉🎊 YOU WON! Incredible! 🎊🎉";
                playerWon = true;
                updateUserStats(true, false, false);

                // Record game history
                QString difficulty = (selectedDifficulty == 1) ? "Easy" :
                                         (selectedDifficulty == 2) ? "Medium" : "Hard";
                gameMode = "PvAI " + difficulty;
                opponent = "AI";
                result = "Won";
                playerSymbol = (humanPlayer == HUMAN) ? "X" : "O";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            } else {
                winner = "😭💔 You Lost... 💔😭";
                playerWon = false;
                updateUserStats(false, true, false);

                // Record game history
                QString difficulty = (selectedDifficulty == 1) ? "Easy" :
                                         (selectedDifficulty == 2) ? "Medium" : "Hard";
                gameMode = "PvAI " + difficulty;
                opponent = "AI";
                result = "Lost";
                playerSymbol = (humanPlayer == HUMAN) ? "X" : "O";
                recordGameResult(result, opponent, gameMode, playerSymbol);
            }
        }
        statusLabel->setText(winner);

        // Play appropriate animation with decorations/crying
        if (playerWon) {
            playWinAnimationWithDecorations(AI);
        } else {
            playLossAnimationWithCrying();
        }

        resetBoard();

    } else if (game->isBoardFull()) {
        statusLabel->setText("🤝✨ It's a Tie! Great Game! ✨🤝");

        if (!isPvP) {
            updateUserStats(false, false, true);

            // Record tie game history
            QString difficulty = (selectedDifficulty == 1) ? "Easy" :
                                     (selectedDifficulty == 2) ? "Medium" : "Hard";
            QString gameMode = "PvAI " + difficulty;
            QString opponent = "AI";
            QString result = "Tie";
            QString playerSymbol = (humanPlayer == HUMAN) ? "X" : "O";
            recordGameResult(result, opponent, gameMode, playerSymbol);
        } else {
            // Record tie for PvP if logged-in user is playing
            if (userManager->isUserLoggedIn()) {
                QString username = userManager->getCurrentUser()->getUsername();
                if (username == player1Name) {
                    recordGameResult("Tie", player2Name, "PvP", "X");
                } else if (username == player2Name) {
                    recordGameResult("Tie", player1Name, "PvP", "O");
                }
            }
        }

        playTieAnimation();
        resetBoard();
    }
}

void MainWindow::resetBoard()
{
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j]->setEnabled(false);
        }
    }
}

void MainWindow::newGame()
{
    game->resetGame();
    updateBoard();
    statusLabel->setText("🎉 Let's Play and Have Fun! 🎉");

    // Clear moves for new game
    currentGameMoves.clear();

    // Reset status label style
    statusLabel->setStyleSheet(
        "font-size: 14px; font-weight: bold; color: #2c3e50; "  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 8px; padding: 6px; margin: 3px;"  // REDUCED
        "border: 2px solid #b8c6db;"
        );

    // Clear any remaining decorations or crying emojis
    for (QLabel* label : decorationLabels) {
        label->deleteLater();
    }
    decorationLabels.clear();

    for (QLabel* label : cryingEmojiLabels) {
        label->deleteLater();
    }
    cryingEmojiLabels.clear();

    if (isPvP) {
        // Show actual player names
        currentPlayerLabel->setText(QString("Current Player: %1 (X)").arg(player1Name));
        // Enable all cells for PvP
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                cells[i][j]->setEnabled(true);
            }
        }
    } else {
        // PvAI mode - check who goes first
        if (game->getCurrentPlayer() == humanPlayer) {
            QString humanSymbol = (humanPlayer == HUMAN) ? "X" : "O";
            currentPlayerLabel->setText("Current Player: " + humanSymbol + " (You)");
            statusLabel->setText("You go first! Good luck! 🍀");

            // Enable all cells for human
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    cells[i][j]->setEnabled(true);
                }
            }
        } else {
            QString aiSymbol = (aiPlayer == HUMAN) ? "X" : "O";
            currentPlayerLabel->setText("Current Player: " + aiSymbol + " (AI)");
            statusLabel->setText("AI goes first! Watch and learn! 🤖");

            // Disable all cells and let AI make first move
            for (int i = 0; i < 3; ++i) {
                for (int j = 0; j < 3; ++j) {
                    cells[i][j]->setEnabled(false);
                }
            }
            aiTimer->start(800);  // REDUCED AI delay
        }
    }
}

