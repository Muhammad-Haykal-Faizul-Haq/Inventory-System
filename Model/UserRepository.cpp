#include "UserRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QStringList>

UserRepository::UserRepository(const std::string& filename)
    : filename(filename) {
    loadFromFile();
}

void UserRepository::loadFromFile() {
    users.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, username, password, roleStr;

        if (!std::getline(ss, idStr, ',') ||
            !std::getline(ss, username, ',') ||
            !std::getline(ss, password, ',') ||
            !std::getline(ss, roleStr, ',')) {
            continue;
        }

        try {
            int id = std::stoi(idStr);
            int role = std::stoi(roleStr);
            users.emplace_back(id, username, password, role);
        } catch (...) {
            continue;
        }
    }
    file.close();
}

void UserRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to save file: " << filename << std::endl;
        return;
    }

    for (const auto& user : users) {
        file << user.getId() << ","
             << user.getUsername() << ","
             << user.getPassword() << ","
             << user.getRole() << "\n";
    }
    file.close();
}

User* UserRepository::validateLogin(const std::string& username, const std::string& password) {
    for (auto& u : users) {
        if (u.getUsername() == username && u.getPassword() == password) {
            return &u;
        }
    }
    return nullptr;
}

User* UserRepository::findByUsername(const std::string& username) {
    for (auto& u : users) {
        if (u.getUsername() == username) {
            return &u;
        }
    }
    return nullptr;
}

std::vector<QString> UserRepository::getAllUsernames() const {
    std::vector<QString> usernames;
    for (const auto& user : users) {
        usernames.push_back(user.getUsernameQString());
    }
    return usernames;
}

bool UserRepository::addUser(const User& user) {
    if (findByUsername(user.getUsername())) {
        return false;  // Username sudah ada
    }
    users.push_back(user);
    saveToFile();
    return true;
}

bool UserRepository::removeUser(int id) {
    for (size_t i = 0; i < users.size(); i++) {
        if (users[i].getId() == id) {
            users.erase(users.begin() + i);
            saveToFile();
            return true;
        }
    }
    return false;
}

int UserRepository::getTotalUsers() const {
    return users.size();
}

int UserRepository::getAdminCount() const {
    int count = 0;
    for (const auto& user : users) {
        if (user.getRole() == 1) count++;
    }
    return count;
}

int UserRepository::getUserCount() const {
    return getTotalUsers() - getAdminCount();
}
