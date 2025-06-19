#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QTimer>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QRadioButton>
#include <QScrollArea>
#include <QGuiApplication>
#include <QScreen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QEasingCurve>
#include <QLineEdit>
#include <QMessageBox>
#include <QListWidget>
#include <QSlider>
#include "tictactoegame.h"
#include "usermanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void cellClicked();
    void newGame();
    void startGameSetup();
    void onModeSelected();
    void onDifficultySelected();
    void onSymbolSelected();
    void makeAIMove();

    // Login/Register slots
    void showLoginScreen();
    void showRegisterScreen();
    void onLoginClicked();
    void onRegisterClicked();
    void onLogoutClicked();
    void showUserProfile();

    // Player name slots
    void showPlayerNamesInput();
    void onPlayerNamesConfirmed();

    // Game history slots
    void showGameHistory();

    // Replay slots
    void showGameReplay();
    void onGameSelected();
    void onReplayPlay();
    void onReplayPause();
    void onReplayReset();
    void onReplaySpeedChanged(int speed);
    void replayNextMove();

private:
    void setupUI();
    void setupLoginUI();
    void setupRegisterUI();
    void setupUserProfileUI();
    void setupGameHistoryUI();
    void setupGameReplayUI();
    void setupPlayerNamesUI();
    void setupGameSetupUI();
    void setupGameUI();
    void setupDifficultyWidget();
    void setupSymbolWidget();
    void updateBoard();
    void checkGameEnd();
    void resetBoard();
    void showGameSetup();
    void showGame();
    void updateUserStats(bool won, bool lost, bool tied);
    void updateUserProfileDisplay();
    void updateGameHistoryDisplay();
    void recordGameResult(const QString &result, const QString &opponent, const QString &gameMode, const QString &playerSymbol);
    void recordGameMove(int row, int col, int player);

    // Replay methods
    void initializeReplay(const GameRecord &record);
    void updateReplayBoard();
    void resetReplayBoard();

    // Enhanced animation methods with decorations and crying emojis
    void playWinAnimationWithDecorations(Player winner);
    void playLossAnimationWithCrying();
    void playTieAnimation();
    void animateButton(QPushButton *button);
    void createFloatingDecorations();
    void createCryingEmojis();

    TicTacToeGame *game;
    UserManager *userManager;

    QPushButton *cells[3][3];
    QPushButton *newGameBtn;
    QPushButton *backToSetupBtn;
    QLabel *statusLabel;
    QLabel *currentPlayerLabel;
    QTimer *aiTimer;

    // Animation members
    QPropertyAnimation *celebrationAnimation;
    QSequentialAnimationGroup *winAnimationGroup;
    QGraphicsOpacityEffect *statusOpacityEffect;

    // Decoration and emoji labels
    QList<QLabel*> decorationLabels;
    QList<QLabel*> cryingEmojiLabels;

    // Login/Register widgets
    QWidget *loginWidget;
    QWidget *registerWidget;
    QWidget *userProfileWidget;
    QLineEdit *loginUsernameEdit;
    QLineEdit *loginPasswordEdit;
    QLineEdit *registerUsernameEdit;
    QLineEdit *registerPasswordEdit;
    QLineEdit *registerEmailEdit;
    QPushButton *loginBtn;
    QPushButton *registerBtn;
    QPushButton *showRegisterBtn;
    QPushButton *showLoginBtn;
    QPushButton *logoutBtn;
    QPushButton *profileBtn;
    QLabel *userWelcomeLabel;
    QLabel *userStatsLabel;


    //

    // Game history UI components - FIXED
    QList<QWidget*> gameBlocks;
    QList<QLabel*> gameResultLabels;
    QList<QLabel*> gameDetailLabels;

    // Add this line to your member variables:
    QTimer *replayTimer;

    // Current game moves for replay - ADD THIS
    QList<GameMove> currentGameMoves;  // You have QVector, change to QList

    //


    // Game history widgets
    QWidget *gameHistoryWidget;
    QLabel *historyTitleLabel;
    QLabel *historyContentLabel;
    QPushButton *backFromHistoryBtn;
    QPushButton *replayBtn;

    // Game replay widgets
    QWidget *gameReplayWidget;
    QListWidget *gameListWidget;
    QPushButton *replayCells[3][3];
    QPushButton *playBtn;
    QPushButton *pauseBtn;
    QPushButton *resetBtn;
    QSlider *speedSlider;
    QLabel *replayStatusLabel;
    QLabel *replayInfoLabel;
    QPushButton *backFromReplayBtn;
    // QTimer *replayTimer;

    // Player names widgets
    QWidget *playerNamesWidget;
    QLineEdit *player1NameEdit;
    QLineEdit *player2NameEdit;
    QPushButton *confirmNamesBtn;

    // Game setup widgets
    QStackedWidget *stackedWidget;
    QWidget *setupWidget;
    QWidget *gameWidget;
    QScrollArea *scrollArea;

    // Setup screen widgets
    QVBoxLayout *setupLayout;
    QLabel *setupTitleLabel;
    QButtonGroup *modeButtonGroup;
    QRadioButton *pvpModeBtn;
    QRadioButton *pvaiModeBtn;

    QButtonGroup *difficultyButtonGroup;
    QRadioButton *easyBtn;
    QRadioButton *mediumBtn;
    QRadioButton *hardBtn;
    QWidget *difficultyWidget;

    QButtonGroup *symbolButtonGroup;
    QRadioButton *xSymbolBtn;
    QRadioButton *oSymbolBtn;
    QWidget *symbolWidget;

    // Game widgets
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    QGridLayout *boardLayout;
    QHBoxLayout *controlLayout;

    // Game state
    bool isPlayerX;
    bool isPvP;
    int selectedDifficulty;
    Player humanPlayer;
    Player aiPlayer;

    // Player names
    QString player1Name;
    QString player2Name;

    // Current game recording
    //QVector<GameMove> currentGameMoves;

    // Replay state
    GameRecord currentReplayGame;
    int currentReplayMoveIndex;
    bool isReplaying;
    int replayBoard[3][3];
};

#endif // MAINWINDOW_H
