#pragma once
#include <QObject>
#include <QString>
#include "../Model/UserRepository.h"

class LoginController : public QObject {
    Q_OBJECT

public:
    explicit LoginController(QObject *parent = nullptr);
    ~LoginController();

    bool authenticate(const QString& username, const QString& password);
    User* getCurrentUser() const;
    QString getUserRoleString() const;

signals:
    void loginSuccess(const QString& username, const QString& role);
    void loginFailed(const QString& errorMessage);

private:
    UserRepository* userRepo;
    User* currentUser;
};
