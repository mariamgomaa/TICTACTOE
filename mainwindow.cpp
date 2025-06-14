#include "mainwindow.h"
//the first version ------->fatma version<--------------
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
    resetReplayBoard();

    setupUI();

    // Show login screen first
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

    setWindowTitle("🎮 Tic Tac Toe - Sign In to Play!");

    // UPDATED: Use full screen size with small margins
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Use 95% of screen size for near full-screen experience
    int windowWidth = static_cast<int>(screenGeometry.width() * 0.95);
    int windowHeight = static_cast<int>(screenGeometry.height() * 0.95);

    // Ensure minimum size for very small screens
    int minWidth = qMin(800, screenGeometry.width() - 50);
    int minHeight = qMin(600, screenGeometry.height() - 50);

    // Use the larger of calculated or minimum
    windowWidth = qMax(windowWidth, minWidth);
    windowHeight = qMax(windowHeight, minHeight);

    // Set window size
    setMinimumSize(minWidth, minHeight);
    resize(windowWidth, windowHeight);

    // Center on screen
    int x = (screenGeometry.width() - windowWidth) / 2;
    int y = (screenGeometry.height() - windowHeight) / 2;
    move(qMax(0, x), qMax(0, y));

    // Allow window to be resizable
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint |
                   Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    // Eye-comfortable soft gradient background
    setStyleSheet(
        "QMainWindow, QWidget, QScrollArea, QStackedWidget {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "border: none;"
        "}"
        "QScrollArea {"
        "background: transparent;"
        "border: none;"
        "}"
        );

    setAutoFillBackground(true);
    scrollArea->setAutoFillBackground(true);
    stackedWidget->setAutoFillBackground(true);
}


void MainWindow::setupLoginUI()
{
    loginWidget = new QWidget();
    loginWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    loginWidget->setAutoFillBackground(true);

    QVBoxLayout *loginLayout = new QVBoxLayout(loginWidget);
    loginLayout->setSpacing(15);  // REDUCED spacing
    loginLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact login title
    QLabel *loginTitle = new QLabel("🎮 Welcome to Tic Tac Toe! 🎮");
    loginTitle->setAlignment(Qt::AlignCenter);
    loginTitle->setStyleSheet(
        "QLabel {"
        "font-size: 20px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 15px; padding: 12px;"  // REDUCED padding
        "margin-bottom: 8px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *loginSubtitle = new QLabel("Sign in to track your progress and compete! 🏆");
    loginSubtitle->setAlignment(Qt::AlignCenter);
    loginSubtitle->setStyleSheet(
        "font-size: 12px; color: #34495e; margin-bottom: 12px; font-style: italic;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 10px; padding: 6px;"  // REDUCED padding
        );

    // Username field
    QLabel *usernameLabel = new QLabel("👤 Username:");
    usernameLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #2c3e50; margin: 3px;");  // REDUCED

    loginUsernameEdit = new QLineEdit();
    loginUsernameEdit->setPlaceholderText("Enter your username");
    loginUsernameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 11px; padding: 8px; border: 2px solid #bdc3c7; border-radius: 8px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #3498db; }"
        );

    // Password field
    QLabel *passwordLabel = new QLabel("🔒 Password:");
    passwordLabel->setStyleSheet("font-size: 12px; font-weight: bold; color: #2c3e50; margin: 3px;");  // REDUCED

    loginPasswordEdit = new QLineEdit();
    loginPasswordEdit->setPlaceholderText("Enter your password");
    loginPasswordEdit->setEchoMode(QLineEdit::Password);
    loginPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 11px; padding: 8px; border: 2px solid #bdc3c7; border-radius: 8px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #3498db; }"
        );

    // Login button
    loginBtn = new QPushButton("🚀 Sign In");
    loginBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 12px; font-weight: bold; padding: 10px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9);"
        "color: white; border: none; border-radius: 8px; margin: 6px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2980b9, stop:1 #1f618d);"
        "}"
        );
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);

    // Register link
    showRegisterBtn = new QPushButton("📝 Don't have an account? Register here!");
    showRegisterBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; color: #3498db; background: transparent; border: none; padding: 6px;"  // REDUCED
        "text-decoration: underline;"
        "}"
        "QPushButton:hover { color: #2980b9; }"
        );
    connect(showRegisterBtn, &QPushButton::clicked, this, &MainWindow::showRegisterScreen);

    // Add to layout
    loginLayout->addWidget(loginTitle);
    loginLayout->addWidget(loginSubtitle);
    loginLayout->addWidget(usernameLabel);
    loginLayout->addWidget(loginUsernameEdit);
    loginLayout->addWidget(passwordLabel);
    loginLayout->addWidget(loginPasswordEdit);
    loginLayout->addWidget(loginBtn);
    loginLayout->addWidget(showRegisterBtn);
    loginLayout->addStretch();

    stackedWidget->addWidget(loginWidget);
}

void MainWindow::setupGameUI()
{
    gameWidget = new QWidget();
    gameWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    gameWidget->setAutoFillBackground(true);
    mainLayout = new QVBoxLayout(gameWidget);

    // Larger status labels for full screen
    statusLabel = new QLabel("🎉 Let's Play and Have Fun! 🎉", this);
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet(
        "font-size: 24px; font-weight: bold; color: #2c3e50; "  // INCREASED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 15px; padding: 20px; margin: 15px;"  // INCREASED
        "border: 2px solid #b8c6db;"
        );

    statusLabel->setGraphicsEffect(statusOpacityEffect);

    currentPlayerLabel = new QLabel("Current Player: X", this);
    currentPlayerLabel->setAlignment(Qt::AlignCenter);
    currentPlayerLabel->setStyleSheet(
        "font-size: 18px; color: #2c3e50; background: rgba(255, 255, 255, 0.7); "  // INCREASED font size
        "border-radius: 12px; padding: 15px; margin: 10px; font-weight: bold;"  // INCREASED
        );

    // Larger game controls
    controlLayout = new QHBoxLayout();

    newGameBtn = new QPushButton("🔄 Play Again!", this);
    newGameBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 16px; font-weight: bold; "  // INCREASED font size
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d4edda, stop:1 #c3e6cb);"
        "color: #2c3e50; border: 1px solid #28a745; border-radius: 12px; padding: 15px 25px;"  // INCREASED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c8e6c9, stop:1 #a5d6a7);"
        "}"
        );
    connect(newGameBtn, &QPushButton::clicked, this, &MainWindow::newGame);

    backToSetupBtn = new QPushButton("⚙️ New Adventure", this);
    backToSetupBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 16px; font-weight: bold; "  // INCREASED font size
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffecd2, stop:1 #fcb69f);"
        "color: #2c3e50; border: 1px solid #f39c12; border-radius: 12px; padding: 15px 25px;"  // INCREASED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffe0b2, stop:1 #ffab91);"
        "}"
        );
    connect(backToSetupBtn, &QPushButton::clicked, this, &MainWindow::startGameSetup);

    controlLayout->addWidget(newGameBtn);
    controlLayout->addStretch();
    controlLayout->addWidget(backToSetupBtn);

    // Much larger game board for full screen
    boardLayout = new QGridLayout();
    boardLayout->setSpacing(15);  // INCREASED spacing

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    // Much larger cell sizes for full screen experience
    int cellSize;
    if (screenGeometry.width() < 1024) {
        cellSize = 100;  // Small screens
    } else if (screenGeometry.width() < 1440) {
        cellSize = 120;  // Medium screens
    } else if (screenGeometry.width() < 1920) {
        cellSize = 140;  // Large screens
    } else {
        cellSize = 160;  // Very large screens
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j] = new QPushButton("", this);
            cells[i][j]->setFixedSize(cellSize, cellSize);
            cells[i][j]->setStyleSheet(
                QString("QPushButton {"
                        "font-size: %1px; font-weight: bold; "
                        "background: rgba(255, 255, 255, 0.9); "
                        "border: 3px solid #bdc3c7; "  // INCREASED border
                        "border-radius: 20px; color: #2c3e50;"  // INCREASED radius
                        "box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"  // INCREASED shadow
                        "}"
                        "QPushButton:hover {"
                        "background: rgba(255, 255, 255, 1.0); "
                        "border: 3px solid #3498db;"
                        "box-shadow: 0 6px 12px rgba(0, 0, 0, 0.15);"
                        "}"
                        "QPushButton:pressed { "
                        "background: rgba(240, 248, 255, 1.0); "
                        "}"
                        "QPushButton:disabled { "
                        "background: rgba(248, 249, 250, 0.8); "
                        "}").arg(cellSize * 0.4)  // Larger font proportion
                );
            cells[i][j]->setProperty("row", i);
            cells[i][j]->setProperty("col", j);
            connect(cells[i][j], &QPushButton::clicked, this, &MainWindow::cellClicked);
            boardLayout->addWidget(cells[i][j], i, j);
        }
    }

    // Larger layout margins
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(currentPlayerLabel);
    mainLayout->addLayout(controlLayout);
    mainLayout->addLayout(boardLayout);
    mainLayout->setContentsMargins(30, 30, 30, 30);  // INCREASED margins

    stackedWidget->addWidget(gameWidget);
}


void MainWindow::setupRegisterUI()
{
    registerWidget = new QWidget();
    registerWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    registerWidget->setAutoFillBackground(true);

    QVBoxLayout *registerLayout = new QVBoxLayout(registerWidget);
    registerLayout->setSpacing(12);  // REDUCED spacing
    registerLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact register title
    QLabel *registerTitle = new QLabel("📝 Create Your Account! 📝");
    registerTitle->setAlignment(Qt::AlignCenter);
    registerTitle->setStyleSheet(
        "QLabel {"
        "font-size: 18px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 12px; padding: 10px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *registerSubtitle = new QLabel("Join the fun and start your gaming journey! 🌟");
    registerSubtitle->setAlignment(Qt::AlignCenter);
    registerSubtitle->setStyleSheet(
        "font-size: 11px; color: #34495e; margin-bottom: 10px; font-style: italic;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 8px; padding: 5px;"  // REDUCED padding
        );

    // Username field
    QLabel *regUsernameLabel = new QLabel("👤 Choose Username:");
    regUsernameLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #2c3e50; margin: 2px;");  // REDUCED

    registerUsernameEdit = new QLineEdit();
    registerUsernameEdit->setPlaceholderText("Pick a unique username");
    registerUsernameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 10px; padding: 6px; border: 2px solid #bdc3c7; border-radius: 6px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #27ae60; }"
        );

    // Email field
    QLabel *emailLabel = new QLabel("📧 Email (Optional):");
    emailLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #2c3e50; margin: 2px;");  // REDUCED

    registerEmailEdit = new QLineEdit();
    registerEmailEdit->setPlaceholderText("your.email@example.com");
    registerEmailEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 10px; padding: 6px; border: 2px solid #bdc3c7; border-radius: 6px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #27ae60; }"
        );

    // Password field
    QLabel *regPasswordLabel = new QLabel("🔒 Create Password:");
    regPasswordLabel->setStyleSheet("font-size: 11px; font-weight: bold; color: #2c3e50; margin: 2px;");  // REDUCED

    registerPasswordEdit = new QLineEdit();
    registerPasswordEdit->setPlaceholderText("Create a secure password");
    registerPasswordEdit->setEchoMode(QLineEdit::Password);
    registerPasswordEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 10px; padding: 6px; border: 2px solid #bdc3c7; border-radius: 6px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #27ae60; }"
        );

    // Register button
    registerBtn = new QPushButton("🎉 Create Account");
    registerBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 11px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #27ae60, stop:1 #229954);"
        "color: white; border: none; border-radius: 8px; margin: 5px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #229954, stop:1 #1e8449);"
        "}"
        );
    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);

    // Login link
    showLoginBtn = new QPushButton("🔙 Already have an account? Sign in here!");
    showLoginBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; color: #27ae60; background: transparent; border: none; padding: 4px;"  // REDUCED
        "text-decoration: underline;"
        "}"
        "QPushButton:hover { color: #229954; }"
        );
    connect(showLoginBtn, &QPushButton::clicked, this, &MainWindow::showLoginScreen);

    // Add to layout
    registerLayout->addWidget(registerTitle);
    registerLayout->addWidget(registerSubtitle);
    registerLayout->addWidget(regUsernameLabel);
    registerLayout->addWidget(registerUsernameEdit);
    registerLayout->addWidget(emailLabel);
    registerLayout->addWidget(registerEmailEdit);
    registerLayout->addWidget(regPasswordLabel);
    registerLayout->addWidget(registerPasswordEdit);
    registerLayout->addWidget(registerBtn);
    registerLayout->addWidget(showLoginBtn);
    registerLayout->addStretch();

    stackedWidget->addWidget(registerWidget);
}

void MainWindow::setupUserProfileUI()
{
    userProfileWidget = new QWidget();
    userProfileWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    userProfileWidget->setAutoFillBackground(true);

    QVBoxLayout *profileLayout = new QVBoxLayout(userProfileWidget);
    profileLayout->setSpacing(15);  // REDUCED spacing
    profileLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact profile title
    QLabel *profileTitle = new QLabel("👤 Your Gaming Profile 👤");
    profileTitle->setAlignment(Qt::AlignCenter);
    profileTitle->setStyleSheet(
        "QLabel {"
        "font-size: 18px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 12px; padding: 10px;"  // REDUCED padding
        "margin-bottom: 8px; border: 2px solid #b8c6db;"
        "}"
        );

    // Welcome label
    userWelcomeLabel = new QLabel("Welcome back, Player!");
    userWelcomeLabel->setAlignment(Qt::AlignCenter);
    userWelcomeLabel->setStyleSheet(
        "font-size: 12px; color: #2c3e50; margin-bottom: 10px; font-weight: bold;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 8px; padding: 8px;"  // REDUCED padding
        );

    // Stats label
    userStatsLabel = new QLabel("📊 Your Game Statistics");
    userStatsLabel->setAlignment(Qt::AlignCenter);
    userStatsLabel->setStyleSheet(
        "font-size: 10px; color: #34495e; margin-bottom: 10px;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.7); border-radius: 8px; padding: 8px;"  // REDUCED padding
        "border: 1px solid #bdc3c7;"
        );

    // Game History button
    QPushButton *historyBtn = new QPushButton("📜 View Game History");
    historyBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #9b59b6, stop:1 #8e44ad);"
        "color: white; border: none; border-radius: 8px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #8e44ad, stop:1 #7d3c98);"
        "}"
        );
    connect(historyBtn, &QPushButton::clicked, this, &MainWindow::showGameHistory);

    // Continue to game button
    QPushButton *continueBtn = new QPushButton("🎮 Continue to Game");
    continueBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9);"
        "color: white; border: none; border-radius: 8px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2980b9, stop:1 #1f618d);"
        "}"
        );
    connect(continueBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    // Logout button
    logoutBtn = new QPushButton("🚪 Logout");
    logoutBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; font-weight: bold; padding: 6px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e74c3c, stop:1 #c0392b);"
        "color: white; border: none; border-radius: 6px; margin: 3px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c0392b, stop:1 #a93226);"
        "}"
        );
    connect(logoutBtn, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    // Add to layout
    profileLayout->addWidget(profileTitle);
    profileLayout->addWidget(userWelcomeLabel);
    profileLayout->addWidget(userStatsLabel);
    profileLayout->addWidget(historyBtn);
    profileLayout->addWidget(continueBtn);
    profileLayout->addWidget(logoutBtn);
    profileLayout->addStretch();

    stackedWidget->addWidget(userProfileWidget);
}

void MainWindow::setupGameHistoryUI()
{
    gameHistoryWidget = new QWidget();
    gameHistoryWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    gameHistoryWidget->setAutoFillBackground(true);

    QVBoxLayout *historyLayout = new QVBoxLayout(gameHistoryWidget);
    historyLayout->setSpacing(12);  // REDUCED spacing
    historyLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact history title
    historyTitleLabel = new QLabel("📜 Your Game History 📜");
    historyTitleLabel->setAlignment(Qt::AlignCenter);
    historyTitleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 8px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *historySubtitle = new QLabel("Your last 5 games - Track your progress! 📊");
    historySubtitle->setAlignment(Qt::AlignCenter);
    historySubtitle->setStyleSheet(
        "font-size: 10px; color: #34495e; margin-bottom: 8px; font-style: italic;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 6px; padding: 4px;"  // REDUCED padding
        );

    // History content
    historyContentLabel = new QLabel("No games played yet. Start playing to see your history!");
    historyContentLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    historyContentLabel->setWordWrap(true);
    historyContentLabel->setStyleSheet(
        "QLabel {"
        "font-size: 9px; color: #2c3e50; line-height: 1.4;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.9); border-radius: 8px; padding: 8px;"  // REDUCED padding
        "border: 1px solid #bdc3c7; margin: 4px;"
        "}"
        );

    // Replay button
    replayBtn = new QPushButton("🎬 Watch Game Replays");
    replayBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #e67e22, stop:1 #d35400);"
        "color: white; border: none; border-radius: 8px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d35400, stop:1 #c0392b);"
        "}"
        );
    connect(replayBtn, &QPushButton::clicked, this, &MainWindow::showGameReplay);

    // Back button
    backFromHistoryBtn = new QPushButton("🔙 Back to Profile");
    backFromHistoryBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #3498db, stop:1 #2980b9);"
        "color: white; border: none; border-radius: 8px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #2980b9, stop:1 #1f618d);"
        "}"
        );
    connect(backFromHistoryBtn, &QPushButton::clicked, this, &MainWindow::showUserProfile);

    // Add to layout
    historyLayout->addWidget(historyTitleLabel);
    historyLayout->addWidget(historySubtitle);
    historyLayout->addWidget(historyContentLabel);
    historyLayout->addWidget(replayBtn);
    historyLayout->addWidget(backFromHistoryBtn);
    historyLayout->addStretch();

    stackedWidget->addWidget(gameHistoryWidget);
}

void MainWindow::setupGameReplayUI()
{
    gameReplayWidget = new QWidget();
    gameReplayWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    gameReplayWidget->setAutoFillBackground(true);

    QHBoxLayout *replayMainLayout = new QHBoxLayout(gameReplayWidget);
    replayMainLayout->setSpacing(10);  // REDUCED spacing
    replayMainLayout->setContentsMargins(10, 10, 10, 10);  // REDUCED margins

    // Left side - Game list
    QVBoxLayout *leftLayout = new QVBoxLayout();

    QLabel *selectGameLabel = new QLabel("🎬 Select Game to Replay");
    selectGameLabel->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #2c3e50; margin-bottom: 5px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.8); border-radius: 6px; padding: 5px;"  // REDUCED
        );

    gameListWidget = new QListWidget();
    gameListWidget->setStyleSheet(
        "QListWidget {"
        "background: rgba(255, 255, 255, 0.9); border: 1px solid #bdc3c7; border-radius: 6px;"
        "font-size: 9px; padding: 3px;"  // REDUCED
        "}"
        "QListWidget::item {"
        "padding: 4px; margin: 1px; border-radius: 3px;"  // REDUCED
        "}"
        "QListWidget::item:selected {"
        "background: #3498db; color: white;"
        "}"
        "QListWidget::item:hover {"
        "background: #ecf0f1;"
        "}"
        );
    connect(gameListWidget, &QListWidget::itemClicked, this, &MainWindow::onGameSelected);

    leftLayout->addWidget(selectGameLabel);
    leftLayout->addWidget(gameListWidget);

    // Right side - Replay board and controls
    QVBoxLayout *rightLayout = new QVBoxLayout();

    replayInfoLabel = new QLabel("Select a game to start replay");
    replayInfoLabel->setAlignment(Qt::AlignCenter);
    replayInfoLabel->setStyleSheet(
        "font-size: 10px; font-weight: bold; color: #2c3e50; margin-bottom: 5px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.8); border-radius: 6px; padding: 5px;"  // REDUCED
        );

    replayStatusLabel = new QLabel("🎬 Ready to Replay");
    replayStatusLabel->setAlignment(Qt::AlignCenter);
    replayStatusLabel->setStyleSheet(
        "font-size: 9px; color: #34495e; margin-bottom: 8px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.7); border-radius: 4px; padding: 4px;"  // REDUCED
        );

    // Compact replay board
    QGridLayout *replayBoardLayout = new QGridLayout();
    replayBoardLayout->setSpacing(3);  // REDUCED spacing

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            replayCells[i][j] = new QPushButton("", this);
            replayCells[i][j]->setFixedSize(40, 40);  // REDUCED size
            replayCells[i][j]->setStyleSheet(
                "QPushButton {"
                "font-size: 16px; font-weight: bold; "  // REDUCED font size
                "background: rgba(255, 255, 255, 0.9); "
                "border: 1px solid #bdc3c7; "
                "border-radius: 8px; color: #2c3e50;"
                "}"
                );
            replayCells[i][j]->setEnabled(false);
            replayBoardLayout->addWidget(replayCells[i][j], i, j);
        }
    }

    // Compact replay controls
    QHBoxLayout *controlsLayout = new QHBoxLayout();

    playBtn = new QPushButton("▶️ Play");
    playBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; font-weight: bold; padding: 6px 8px;"  // REDUCED
        "background: #27ae60; color: white; border: none; border-radius: 4px; margin: 2px;"  // REDUCED
        "}"
        "QPushButton:hover { background: #229954; }"
        "QPushButton:disabled { background: #95a5a6; }"
        );
    connect(playBtn, &QPushButton::clicked, this, &MainWindow::onReplayPlay);

    pauseBtn = new QPushButton("⏸️ Pause");
    pauseBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; font-weight: bold; padding: 6px 8px;"  // REDUCED
        "background: #f39c12; color: white; border: none; border-radius: 4px; margin: 2px;"  // REDUCED
        "}"
        "QPushButton:hover { background: #e67e22; }"
        "QPushButton:disabled { background: #95a5a6; }"
        );
    connect(pauseBtn, &QPushButton::clicked, this, &MainWindow::onReplayPause);

    resetBtn = new QPushButton("🔄 Reset");
    resetBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; font-weight: bold; padding: 6px 8px;"  // REDUCED
        "background: #e74c3c; color: white; border: none; border-radius: 4px; margin: 2px;"  // REDUCED
        "}"
        "QPushButton:hover { background: #c0392b; }"
        "QPushButton:disabled { background: #95a5a6; }"
        );
    connect(resetBtn, &QPushButton::clicked, this, &MainWindow::onReplayReset);

    controlsLayout->addWidget(playBtn);
    controlsLayout->addWidget(pauseBtn);
    controlsLayout->addWidget(resetBtn);

    // Speed control
    QLabel *speedLabel = new QLabel("⚡ Replay Speed:");
    speedLabel->setStyleSheet("font-size: 8px; font-weight: bold; color: #2c3e50;");  // REDUCED

    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 5);
    speedSlider->setValue(3);
    speedSlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "border: 1px solid #bdc3c7; height: 4px; background: #ecf0f1; border-radius: 2px;"  // REDUCED
        "}"
        "QSlider::handle:horizontal {"
        "background: #3498db; border: 1px solid #2980b9; width: 12px; margin: -3px 0; border-radius: 6px;"  // REDUCED
        "}"
        );
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::onReplaySpeedChanged);

    // Back button
    backFromReplayBtn = new QPushButton("🔙 Back to History");
    backFromReplayBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; font-weight: bold; padding: 6px 10px;"  // REDUCED
        "background: #95a5a6; color: white; border: none; border-radius: 6px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover { background: #7f8c8d; }"
        );
    connect(backFromReplayBtn, &QPushButton::clicked, this, &MainWindow::showGameHistory);

    // Setup replay timer
    replayTimer = new QTimer(this);
    connect(replayTimer, &QTimer::timeout, this, &MainWindow::replayNextMove);

    // Add to right layout
    rightLayout->addWidget(replayInfoLabel);
    rightLayout->addWidget(replayStatusLabel);
    rightLayout->addLayout(replayBoardLayout);
    rightLayout->addLayout(controlsLayout);
    rightLayout->addWidget(speedLabel);
    rightLayout->addWidget(speedSlider);
    rightLayout->addWidget(backFromReplayBtn);
    rightLayout->addStretch();

    // Add to main layout
    replayMainLayout->addLayout(leftLayout, 1);
    replayMainLayout->addLayout(rightLayout, 2);

    stackedWidget->addWidget(gameReplayWidget);

    // Initialize replay state
    currentReplayMoveIndex = 0;
    isReplaying = false;
    resetReplayBoard();
}

void MainWindow::setupPlayerNamesUI()
{
    playerNamesWidget = new QWidget();
    playerNamesWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    playerNamesWidget->setAutoFillBackground(true);

    QVBoxLayout *playerNamesLayout = new QVBoxLayout(playerNamesWidget);
    playerNamesLayout->setSpacing(12);  // REDUCED spacing
    playerNamesLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact player names title
    QLabel *playerNamesTitle = new QLabel("👥 Enter Player Names! 👥");
    playerNamesTitle->setAlignment(Qt::AlignCenter);
    playerNamesTitle->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 8px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *playerNamesSubtitle = new QLabel("Let's personalize your gaming experience! 🎮");
    playerNamesSubtitle->setAlignment(Qt::AlignCenter);
    playerNamesSubtitle->setStyleSheet(
        "font-size: 10px; color: #34495e; margin-bottom: 8px; font-style: italic;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 6px; padding: 4px;"  // REDUCED padding
        );

    // Player 1 name field
    QLabel *player1Label = new QLabel("🎯 Player 1 Name (X):");
    player1Label->setStyleSheet("font-size: 11px; font-weight: bold; color: #2c3e50; margin: 2px;");  // REDUCED

    player1NameEdit = new QLineEdit();
    player1NameEdit->setPlaceholderText("Enter Player 1's name");
    player1NameEdit->setText("Player 1");
    player1NameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 10px; padding: 6px; border: 2px solid #e74c3c; border-radius: 6px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #c0392b; }"
        );

    // Player 2 name field
    QLabel *player2Label = new QLabel("⭕ Player 2 Name (O):");
    player2Label->setStyleSheet("font-size: 11px; font-weight: bold; color: #2c3e50; margin: 2px;");  // REDUCED

    player2NameEdit = new QLineEdit();
    player2NameEdit->setPlaceholderText("Enter Player 2's name");
    player2NameEdit->setText("Player 2");
    player2NameEdit->setStyleSheet(
        "QLineEdit {"
        "font-size: 10px; padding: 6px; border: 2px solid #3498db; border-radius: 6px;"  // REDUCED
        "background: rgba(255, 255, 255, 0.9); color: #2c3e50;"
        "}"
        "QLineEdit:focus { border: 2px solid #2980b9; }"
        );

    // Confirm button
    confirmNamesBtn = new QPushButton("✅ Let's Play!");
    confirmNamesBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 11px; font-weight: bold; padding: 8px;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #27ae60, stop:1 #229954);"
        "color: white; border: none; border-radius: 8px; margin: 5px;"  // REDUCED
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #229954, stop:1 #1e8449);"
        "}"
        );
    connect(confirmNamesBtn, &QPushButton::clicked, this, &MainWindow::onPlayerNamesConfirmed);

    // Back button
    QPushButton *backToModeBtn = new QPushButton("⬅️ Back to Mode Selection");
    backToModeBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 9px; background: rgba(149, 165, 166, 0.8); color: white; font-weight: bold;"  // REDUCED
        "border: none; border-radius: 6px; padding: 6px 10px; margin: 4px;"  // REDUCED
        "}"
        "QPushButton:hover { background: rgba(127, 140, 141, 0.9); }"
        );
    connect(backToModeBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    // Add to layout
    playerNamesLayout->addWidget(playerNamesTitle);
    playerNamesLayout->addWidget(playerNamesSubtitle);
    playerNamesLayout->addWidget(player1Label);
    playerNamesLayout->addWidget(player1NameEdit);
    playerNamesLayout->addWidget(player2Label);
    playerNamesLayout->addWidget(player2NameEdit);
    playerNamesLayout->addWidget(confirmNamesBtn);
    playerNamesLayout->addWidget(backToModeBtn);
    playerNamesLayout->addStretch();

    stackedWidget->addWidget(playerNamesWidget);
}

void MainWindow::setupGameSetupUI()
{
    setupWidget = new QWidget();
    setupWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    setupWidget->setAutoFillBackground(true);

    setupLayout = new QVBoxLayout(setupWidget);
    setupLayout->setSpacing(12);  // REDUCED spacing
    setupLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact user info header
    QLabel *userInfoLabel = new QLabel();
    userInfoLabel->setAlignment(Qt::AlignCenter);
    userInfoLabel->setStyleSheet(
        "QLabel {"
        "font-size: 10px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.7); border-radius: 6px; padding: 5px;"  // REDUCED padding
        "margin-bottom: 5px; border: 1px solid #bdc3c7;"
        "}"
        );

    // Profile and logout buttons
    QHBoxLayout *userControlLayout = new QHBoxLayout();

    profileBtn = new QPushButton("👤 Profile");
    profileBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 8px; background: rgba(52, 152, 219, 0.8); color: white; font-weight: bold;"  // REDUCED
        "border: none; border-radius: 4px; padding: 4px 6px; margin: 1px;"  // REDUCED
        "}"
        "QPushButton:hover { background: rgba(41, 128, 185, 0.9); }"
        );
    connect(profileBtn, &QPushButton::clicked, this, &MainWindow::showUserProfile);

    QPushButton *logoutBtnSmall = new QPushButton("🚪 Logout");
    logoutBtnSmall->setStyleSheet(
        "QPushButton {"
        "font-size: 8px; background: rgba(231, 76, 60, 0.8); color: white; font-weight: bold;"  // REDUCED
        "border: none; border-radius: 4px; padding: 4px 6px; margin: 1px;"  // REDUCED
        "}"
        "QPushButton:hover { background: rgba(192, 57, 43, 0.9); }"
        );
    connect(logoutBtnSmall, &QPushButton::clicked, this, &MainWindow::onLogoutClicked);

    userControlLayout->addWidget(profileBtn);
    userControlLayout->addStretch();
    userControlLayout->addWidget(logoutBtnSmall);

    // Update user info display
    if (userManager->isUserLoggedIn()) {
        User* user = userManager->getCurrentUser();
        userInfoLabel->setText(QString("🎮 Player: %1 | 🏆 Wins: %2 | 📊 Win Rate: %3%")
                                   .arg(user->getUsername())
                                   .arg(user->getGamesWon())
                                   .arg(QString::number(user->getWinRate(), 'f', 1)));
    }

    // Compact title
    setupTitleLabel = new QLabel("🌟 Welcome to Tic Tac Toe! 🌟");
    setupTitleLabel->setAlignment(Qt::AlignCenter);
    setupTitleLabel->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 8px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    // Gentle instruction
    QLabel *instructionLabel = new QLabel("Ready for some awesome fun? Let's get started! 🚀");
    instructionLabel->setAlignment(Qt::AlignCenter);
    instructionLabel->setStyleSheet(
        "font-size: 11px; color: #34495e; margin-bottom: 8px; font-style: italic;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 6px; padding: 4px;"  // REDUCED padding
        );

    // Compact mode selection
    QLabel *modeLabel = new QLabel("🎯 Choose Your Fun Adventure!");
    modeLabel->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #2c3e50; "  // REDUCED font size
        "background: rgba(255, 255, 255, 0.7); border-radius: 8px; padding: 6px; margin: 4px;"  // REDUCED
        "border: 1px solid #d5dbdb;"
        );

    modeButtonGroup = new QButtonGroup(this);

    pvpModeBtn = new QRadioButton("👫 Play with a Friend - Double the Fun!");
    pvpModeBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffecd2, stop:1 #fcb69f);"
        "border-radius: 8px; margin: 4px; border: 1px solid #f39c12;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #27ae60; border-radius: 8px; background: #27ae60;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffe0b2, stop:1 #ffab91);"
        "}"
        );

    pvaiModeBtn = new QRadioButton("🤖 Challenge the Smart AI - Test Your Skills!");
    pvaiModeBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d4edda, stop:1 #c3e6cb);"
        "border-radius: 8px; margin: 4px; border: 1px solid #28a745;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #27ae60; border-radius: 8px; background: #27ae60;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c8e6c9, stop:1 #a5d6a7);"
        "}"
        );

    modeButtonGroup->addButton(pvpModeBtn);
    modeButtonGroup->addButton(pvaiModeBtn);

    connect(pvpModeBtn, &QRadioButton::clicked, this, &MainWindow::onModeSelected);
    connect(pvaiModeBtn, &QRadioButton::clicked, this, &MainWindow::onModeSelected);

    // Add to layout
    setupLayout->addWidget(userInfoLabel);
    setupLayout->addLayout(userControlLayout);
    setupLayout->addWidget(setupTitleLabel);
    setupLayout->addWidget(instructionLabel);
    setupLayout->addWidget(modeLabel);
    setupLayout->addWidget(pvpModeBtn);
    setupLayout->addWidget(pvaiModeBtn);
    setupLayout->addStretch();

    stackedWidget->addWidget(setupWidget);
}

void MainWindow::setupDifficultyWidget()
{
    difficultyWidget = new QWidget();
    difficultyWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    difficultyWidget->setAutoFillBackground(true);

    QVBoxLayout *difficultyLayout = new QVBoxLayout(difficultyWidget);
    difficultyLayout->setSpacing(12);  // REDUCED spacing
    difficultyLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact difficulty title
    QLabel *difficultyTitle = new QLabel("🎯 Choose Your Challenge Level! 🎯");
    difficultyTitle->setAlignment(Qt::AlignCenter);
    difficultyTitle->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 8px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *difficultyLabel = new QLabel("🌟 How challenging do you want it to be?");
    difficultyLabel->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #2c3e50; "  // REDUCED font size
        "background: rgba(255, 255, 255, 0.7); border-radius: 8px; padding: 6px; margin: 4px;"  // REDUCED
        );

    QLabel *difficultyHint = new QLabel("Don't worry - you can always try a different level later! 😊");
    difficultyHint->setAlignment(Qt::AlignCenter);
    difficultyHint->setStyleSheet(
        "font-size: 10px; color: #34495e; font-style: italic; margin: 4px;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 6px; padding: 4px;"  // REDUCED padding
        );

    difficultyButtonGroup = new QButtonGroup(this);

    easyBtn = new QRadioButton("😊 Easy Peasy - Perfect for Learning!");
    easyBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #d4edda, stop:1 #c3e6cb);"
        "border-radius: 8px; margin: 4px; border: 1px solid #28a745;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #27ae60; border-radius: 8px; background: #27ae60;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #c8e6c9, stop:1 #a5d6a7);"
        "}"
        );

    mediumBtn = new QRadioButton("😎 Medium Challenge - Bring It On!");
    mediumBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #fff3cd, stop:1 #ffeaa7);"
        "border-radius: 8px; margin: 4px; border: 1px solid #ffc107;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #f39c12; border-radius: 8px; background: #f39c12;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #ffe8a1, stop:1 #fdd835);"
        "}"
        );

    hardBtn = new QRadioButton("🔥 Expert Mode - Ultimate Challenge!");
    hardBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f8d7da, stop:1 #f5c6cb);"
        "border-radius: 8px; margin: 4px; border: 1px solid #dc3545;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #e74c3c; border-radius: 8px; background: #e74c3c;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f1b0b7, stop:1 #f8cecc);"
        "}"
        );

    difficultyButtonGroup->addButton(easyBtn);
    difficultyButtonGroup->addButton(mediumBtn);
    difficultyButtonGroup->addButton(hardBtn);

    connect(easyBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);
    connect(mediumBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);
    connect(hardBtn, &QRadioButton::clicked, this, &MainWindow::onDifficultySelected);

    // Compact back button
    QPushButton *difficultyBackBtn = new QPushButton("⬅️ Oops, Go Back");
    difficultyBackBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; background: rgba(255, 255, 255, 0.8); color: #2c3e50; font-weight: bold;"  // REDUCED
        "border: 1px solid #bdc3c7; border-radius: 8px; padding: 6px 10px; margin: 6px;"  // REDUCED
        "}"
        "QPushButton:hover { "
        "background: rgba(255, 255, 255, 0.9); "
        "border: 1px solid #95a5a6;"
        "}"
        );
    connect(difficultyBackBtn, &QPushButton::clicked, this, &MainWindow::showGameSetup);

    difficultyLayout->addWidget(difficultyTitle);
    difficultyLayout->addWidget(difficultyLabel);
    difficultyLayout->addWidget(difficultyHint);
    difficultyLayout->addWidget(easyBtn);
    difficultyLayout->addWidget(mediumBtn);
    difficultyLayout->addWidget(hardBtn);
    difficultyLayout->addWidget(difficultyBackBtn);
    difficultyLayout->addStretch();

    stackedWidget->addWidget(difficultyWidget);
}

void MainWindow::setupSymbolWidget()
{
    symbolWidget = new QWidget();
    symbolWidget->setStyleSheet(
        "QWidget { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:1, "
        "stop:0 #e8f5e8, stop:0.5 #f0f8ff, stop:1 #fff0f5);"
        "}"
        );
    symbolWidget->setAutoFillBackground(true);

    QVBoxLayout *symbolLayout = new QVBoxLayout(symbolWidget);
    symbolLayout->setSpacing(12);  // REDUCED spacing
    symbolLayout->setContentsMargins(20, 15, 20, 15);  // REDUCED margins

    // Compact symbol title
    QLabel *symbolTitle = new QLabel("⭐ Pick Your Lucky Symbol! ⭐");
    symbolTitle->setAlignment(Qt::AlignCenter);
    symbolTitle->setStyleSheet(
        "QLabel {"
        "font-size: 16px; font-weight: bold; color: #2c3e50;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.8); border-radius: 10px; padding: 8px;"  // REDUCED padding
        "margin-bottom: 6px; border: 2px solid #b8c6db;"
        "}"
        );

    QLabel *symbolLabel = new QLabel("✨ Which symbol represents YOU?");
    symbolLabel->setStyleSheet(
        "font-size: 12px; font-weight: bold; color: #2c3e50; "  // REDUCED font size
        "background: rgba(255, 255, 255, 0.7); border-radius: 8px; padding: 6px; margin: 4px;"  // REDUCED
        );

    QLabel *symbolHint = new QLabel("Remember: X always gets the first move! 🎯");
    symbolHint->setAlignment(Qt::AlignCenter);
    symbolHint->setStyleSheet(
        "font-size: 10px; color: #34495e; font-style: italic; margin: 4px;"  // REDUCED font size
        "background: rgba(255, 255, 255, 0.6); border-radius: 6px; padding: 4px;"  // REDUCED padding
        );

    symbolButtonGroup = new QButtonGroup(this);

    xSymbolBtn = new QRadioButton("❌ X - I Love Going First!");
    xSymbolBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f8d7da, stop:1 #f5c6cb);"
        "border-radius: 8px; margin: 4px; border: 1px solid #dc3545;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #e74c3c; border-radius: 8px; background: #e74c3c;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #f1b0b7, stop:1 #f8cecc);"
        "}"
        );

    oSymbolBtn = new QRadioButton("⭕ O - I'm Patient and Strategic!");
    oSymbolBtn->setStyleSheet(
        "QRadioButton {"
        "font-size: 10px; padding: 8px; color: #2c3e50; font-weight: bold;"  // REDUCED
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #cce5ff, stop:1 #b3d9ff);"
        "border-radius: 8px; margin: 4px; border: 1px solid #007bff;"  // REDUCED
        "}"
        "QRadioButton::indicator { width: 16px; height: 16px; }"  // REDUCED
        "QRadioButton::indicator::unchecked {"
        "border: 2px solid #95a5a6; border-radius: 8px; background: white;"
        "}"
        "QRadioButton::indicator::checked {"
        "border: 2px solid #3498db; border-radius: 8px; background: #3498db;"
        "}"
        "QRadioButton:hover { "
        "background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #99ccff, stop:1 #80bfff);"
        "}"
        );

    symbolButtonGroup->addButton(xSymbolBtn);
    symbolButtonGroup->addButton(oSymbolBtn);

    connect(xSymbolBtn, &QRadioButton::clicked, this, &MainWindow::onSymbolSelected);
    connect(oSymbolBtn, &QRadioButton::clicked, this, &MainWindow::onSymbolSelected);

    // Compact back button
    QPushButton *symbolBackBtn = new QPushButton("⬅️ Let Me Reconsider");
    symbolBackBtn->setStyleSheet(
        "QPushButton {"
        "font-size: 10px; background: rgba(255, 255, 255, 0.8); color: #2c3e50; font-weight: bold;"  // REDUCED
        "border: 1px solid #bdc3c7; border-radius: 8px; padding: 6px 10px; margin: 6px;"  // REDUCED
        "}"
        "QPushButton:hover { "
        "background: rgba(255, 255, 255, 0.9); "
        "border: 1px solid #95a5a6;"
        "}"
        );
    connect(symbolBackBtn, &QPushButton::clicked, [this]() {
        stackedWidget->setCurrentWidget(difficultyWidget);
    });

    symbolLayout->addWidget(symbolTitle);
    symbolLayout->addWidget(symbolLabel);
    symbolLayout->addWidget(symbolHint);
    symbolLayout->addWidget(xSymbolBtn);
    symbolLayout->addWidget(oSymbolBtn);
    symbolLayout->addWidget(symbolBackBtn);
    symbolLayout->addStretch();

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
        QMessageBox::warning(this, "❌ Login Failed",
                             "Please enter both username and password! 📝");
        return;
    }

    if (userManager->loginUser(username, password)) {
        QMessageBox::information(this, "✅ Welcome Back!",
                                 QString("Hello %1! Ready to play? 🎮").arg(username));
        showUserProfile();
    } else {
        QMessageBox::warning(this, "❌ Login Failed",
                             "Invalid username or password. Please try again! 🔐");
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
        QMessageBox::warning(this, "❌ Registration Failed",
                             "Please enter both username and password! 📝");
        return;
    }

    if (username.length() < 3) {
        QMessageBox::warning(this, "❌ Registration Failed",
                             "Username must be at least 3 characters long! 📏");
        return;
    }

    if (password.length() < 4) {
        QMessageBox::warning(this, "❌ Registration Failed",
                             "Password must be at least 4 characters long! 🔒");
        return;
    }

    if (userManager->userExists(username)) {
        QMessageBox::warning(this, "❌ Registration Failed",
                             "Username already exists. Please choose another! 👤");
        registerUsernameEdit->setFocus();
        return;
    }

    if (userManager->registerUser(username, password, email)) {
        QMessageBox::information(this, "🎉 Registration Successful!",
                                 QString("Welcome to Tic Tac Toe, %1! Your account has been created. Please sign in to start playing! 🎮").arg(username));

        // Pre-fill login form
        loginUsernameEdit->setText(username);
        loginPasswordEdit->clear();
        showLoginScreen();
        loginPasswordEdit->setFocus();
    } else {
        QMessageBox::warning(this, "❌ Registration Failed",
                             "Failed to create account. Please try again! 🔄");
    }
}

void MainWindow::onLogoutClicked()
{
    if (userManager->isUserLoggedIn()) {
        QString username = userManager->getCurrentUser()->getUsername();
        userManager->logoutUser();
        QMessageBox::information(this, "👋 Goodbye!",
                                 QString("See you later, %1! Thanks for playing! 🎮").arg(username));
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

void MainWindow::updateGameHistoryDisplay()
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    QList<GameRecord> history = user->getGameHistory();

    if (history.isEmpty()) {
        historyContentLabel->setText(
            "🎮 No games played yet!\n\n"
            "Start playing to see your game history here.\n"
            "Your last 5 games will be displayed with details about:\n"
            "• When you played\n"
            "• Who you played against\n"
            "• What the result was\n"
            "• Which symbol you used\n"
            "• What game mode you played"
            );
        return;
    }

    QString historyText = "🎮 **Your Recent Games** 🎮\n\n";

    for (int i = 0; i < history.size(); ++i) {
        const GameRecord &record = history[i];

        QString resultEmoji;
        if (record.result == "Won") {
            resultEmoji = "🏆";
        } else if (record.result == "Lost") {
            resultEmoji = "😞";
        } else {
            resultEmoji = "🤝";
        }

        historyText += QString(
                           "**Game %1** %2\n"
                           "📅 **Date:** %3\n"
                           "👤 **Opponent:** %4\n"
                           "🎯 **Result:** %5 %6\n"
                           "🎮 **Mode:** %7\n"
                           "🎲 **Your Symbol:** %8\n\n"
                           ).arg(i + 1)
                           .arg(resultEmoji)
                           .arg(record.timestamp.toString("MMM dd, yyyy hh:mm AP"))
                           .arg(record.opponent)
                           .arg(resultEmoji)
                           .arg(record.result)
                           .arg(record.gameMode)
                           .arg(record.playerSymbol);
    }

    if (history.size() < 5) {
        historyText += QString("\n💡 **Tip:** Play %1 more game(s) to fill your complete history!").arg(5 - history.size());
    }

    historyContentLabel->setText(historyText);
}

void MainWindow::recordGameResult(const QString &result, const QString &opponent, const QString &gameMode, const QString &playerSymbol)
{
    User* user = userManager->getCurrentUser();
    if (!user) return;

    GameRecord record(opponent, result, gameMode, playerSymbol);
    record.moves = currentGameMoves;
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

