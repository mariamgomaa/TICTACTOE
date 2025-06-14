#ifndef USER_H
#define USER_H

#include <QMetaType>
#include <QString>
#include <QDateTime>
#include <QList>
#include <QVector>

struct GameMove {
    int row;
    int col;
    int player; // 1 for X, 2 for O

    GameMove() = default;
    GameMove(int r, int c, int p) : row(r), col(c), player(p) {}
};

struct GameRecord {
    QDateTime timestamp;
    QString opponent;
    QString result;
    QString gameMode;
    QString playerSymbol;
    QVector<GameMove> moves; // Store all moves for replay

    GameRecord() = default;
    GameRecord(const QString &opp, const QString &res, const QString &mode, const QString &symbol)
        : timestamp(QDateTime::currentDateTime()), opponent(opp), result(res), gameMode(mode), playerSymbol(symbol) {}
};

class User {
public:
    User();
    User(const QString &username, const QString &password, const QString &email = "");

    QString getUsername() const { return username; }
    QString getPassword() const { return password; }
    QString getEmail() const { return email; }
    QDateTime getLastLogin() const { return lastLogin; }
    int getGamesWon() const { return gamesWon; }
    int getGamesLost() const { return gamesLost; }
    int getGamesTied() const { return gamesTied; }
    QList<GameRecord> getGameHistory() const { return gameHistory; }

    void setUsername(const QString &username) { this->username = username; }
    void setPassword(const QString &password) { this->password = password; }
    void setEmail(const QString &email) { this->email = email; }
    void setLastLogin(const QDateTime &lastLogin) { this->lastLogin = lastLogin; }
    void setGameHistory(const QList<GameRecord> &history) { this->gameHistory = history; }

    void addWin() { gamesWon++; }
    void addLoss() { gamesLost++; }
    void addTie() { gamesTied++; }

    // Modified to store all games, not just last 5
    void addGameToHistory(const GameRecord &record);

    int getTotalGames() const { return gamesWon + gamesLost + gamesTied; }
    double getWinRate() const;

private:
    QString username;
    QString password;
    QString email;
    QDateTime lastLogin;
    int gamesWon;
    int gamesLost;
    int gamesTied;
    QList<GameRecord> gameHistory; // Now stores all games for database sync
};
Q_DECLARE_METATYPE(GameMove)
Q_DECLARE_METATYPE(GameRecord)
#endif // USER_H
