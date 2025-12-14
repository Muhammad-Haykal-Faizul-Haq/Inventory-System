#include "LoginController.h"

LoginController::LoginController(QObject *parent)
    : QObject(parent), userRepo(new UserRepository()), currentUser(nullptr) {}

LoginController::~LoginController() {
    delete userRepo;
}

bool LoginController::authenticate(const QString& username, const QString& password) {
    currentUser = userRepo->validateLogin(username.toStdString(),
                                          password.toStdString());
    return currentUser != nullptr;
}

User* LoginController::getCurrentUser() const {
    return currentUser;
}

QString LoginController::getUserRoleString() const {
    if (currentUser) {
        return currentUser->getRoleString();
    }
    return "Unknown";
}
