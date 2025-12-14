#include "Borrow.h"
#include <QDateTime>

Borrow::Borrow(const std::string& username, int itemId, int quantity)
    : username(username), itemId(itemId), quantity(quantity),
    borrowDate(QDateTime::currentDateTime()) {}

Borrow::Borrow() : itemId(0), quantity(0), borrowDate(QDateTime::currentDateTime()) {}

std::string Borrow::getUsername() const { return username; }
QString Borrow::getUsernameQString() const { return QString::fromStdString(username); }
int Borrow::getItemId() const { return itemId; }
int Borrow::getQuantity() const { return quantity; }
QDateTime Borrow::getBorrowDate() const { return borrowDate; }

QString Borrow::getBorrowDateString() const {
    return borrowDate.toString("dd/MM/yyyy HH:mm");
}

void Borrow::setQuantity(int quantity) { this->quantity = quantity; }
