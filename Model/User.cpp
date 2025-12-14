#include "User.h"
#include <QString>

// Constructor - URUTAN HARUS SAMA dengan userList.txt
User::User(int id, const std::string& username,
           const std::string& password, int role)
    : id(id), username(username), password(password), role(role) {
    // Constructor sesuai format: id,username,password,role
}

// Default constructor
User::User() : id(0), username(""), password(""), role(0) {}

// Getters
int User::getId() const {
    return id;
}

std::string User::getUsername() const {
    return username;
}

std::string User::getPassword() const {
    return password;
}

int User::getRole() const {
    return role;
}

// Qt compatible getters
QString User::getUsernameQString() const {
    return QString::fromStdString(username);
}

QString User::getRoleString() const {
    return (role == 1) ? "Admin" : "User";
}

bool User::isAdmin() const {
    return (role == 1);
}
