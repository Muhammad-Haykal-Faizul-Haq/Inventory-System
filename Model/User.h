#pragma once
#include <string>
#include <QString>

class User {
private:
    int id;
    std::string username;
    std::string password;
    int role;  // 1 = Admin, 0 = User

public:
    // Constructor - SESUAI URUTAN di userList.txt: id,username,password,role
    User(int id, const std::string& username,
         const std::string& password, int role);

    // Default constructor
    User();

    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPassword() const;
    int getRole() const;

    // Qt compatible getters
    QString getUsernameQString() const;
    QString getRoleString() const;
    bool isAdmin() const;
};
