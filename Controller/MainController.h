#pragma once
#include <QObject>
#include <QString>
#include "Model/ItemRepository.h"
#include "Model/BorrowRepository.h"
#include "Model/User.h"

class MainController : public QObject {
    Q_OBJECT

public:
    explicit MainController(QObject *parent = nullptr);
    ~MainController();

    void setCurrentUser(User* user);

    // Item operations
    std::vector<Item> getAllItems() const;
    bool borrowItem(int itemId, int quantity);
    bool returnItem(int itemId, int quantity);

    // Borrow operations
    std::vector<Borrow> getUserBorrows() const;
    std::vector<Borrow> getAllBorrows() const;

    // Admin operations
    bool addItem(const QString& name, int quantity);
    bool updateItem(int itemId, const QString& name, int quantity);
    bool deleteItem(int itemId);

    // Validation
    bool canUserBorrow(int itemId, int quantity) const;

private:
    User* currentUser;
    ItemRepository* itemRepo;
    BorrowRepository* borrowRepo;
};
