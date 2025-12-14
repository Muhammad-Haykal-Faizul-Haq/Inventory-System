#pragma once
#include <vector>
#include <string>
#include <QString>
#include "User.h"

class UserRepository {
private:
    std::vector<User> users;
    std::string filename;

public:
    UserRepository(const std::string& filename = "Resources/userList.txt");

    void loadFromFile();
    void saveToFile();

    User* validateLogin(const std::string& username, const std::string& password);
    User* findByUsername(const std::string& username);

    const std::vector<User>& getAllUsers() const { return users; }
    std::vector<QString> getAllUsernames() const;

    bool addUser(const User& user);
    bool removeUser(int id);

    int getTotalUsers() const;
    int getAdminCount() const;
    int getUserCount() const;
};
