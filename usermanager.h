#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "user.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QCryptographicHash>
#include <QString>

class UserManager {
public:
    UserManager();
    ~UserManager();

    bool registerUser(const QString &username, const QString &password, const QString &email = "");
    bool loginUser(const QString &username, const QString &password);
    void logoutUser();
    bool isUserLoggedIn() const { return currentUser != nullptr; }
    User* getCurrentUser() const { return currentUser; }

    void saveUserData();
    void loadUserData();
    bool userExists(const QString &username) const;
    QStringList getAllUsernames() const;

    // Made public for replay functionality
    QList<GameRecord> loadGameHistoryFromDatabase(const QString &username);
    QList<GameRecord> loadAllGameHistoryFromDatabase(const QString &username);

private:
    QSqlDatabase db;
    User* currentUser;
    QString dbPath;

    bool initializeDatabase();
    QString hashPassword(const QString &password) const;
    bool createTables();
    bool saveUserToDatabase(const User &user);
    User loadUserFromDatabase(const QString &username);
    bool saveGameHistoryToDatabase(const QString &username, const QList<GameRecord> &history);

    // Helper methods for move serialization
    QString movesToString(const QVector<GameMove> &moves) const;
    QVector<GameMove> movesFromString(const QString &movesStr) const;
};

#endif // USERMANAGER_H
