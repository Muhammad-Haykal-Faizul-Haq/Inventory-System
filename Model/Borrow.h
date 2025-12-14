#pragma once
#include <string>
#include <QString>
#include <QDateTime>

class Borrow {
private:
    std::string username;
    int itemId;
    int quantity;
    QDateTime borrowDate;

public:
    Borrow(const std::string& username, int itemId, int quantity);
    Borrow();

    std::string getUsername() const;
    QString getUsernameQString() const;
    int getItemId() const;
    int getQuantity() const;
    QDateTime getBorrowDate() const;
    QString getBorrowDateString() const;

    void setQuantity(int quantity);
};
