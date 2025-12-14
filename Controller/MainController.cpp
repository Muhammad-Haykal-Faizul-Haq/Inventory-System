#include "MainController.h"
#include <algorithm>

MainController::MainController(QObject *parent)
    : QObject(parent), currentUser(nullptr),
    itemRepo(new ItemRepository()), borrowRepo(new BorrowRepository()) {}

MainController::~MainController() {
    delete itemRepo;
    delete borrowRepo;
}

void MainController::setCurrentUser(User* user) {
    currentUser = user;
}

std::vector<Item> MainController::getAllItems() const {
    return itemRepo->getAllItems();
}

bool MainController::borrowItem(int itemId, int quantity) {
    if (!currentUser || !canUserBorrow(itemId, quantity)) {
        return false;
    }

    bool success = itemRepo->borrowItem(itemId, quantity);
    if (success) {
        borrowRepo->addBorrow(currentUser->getUsername(), itemId, quantity);
    }
    return success;
}

bool MainController::returnItem(int itemId, int quantity) {
    if (!currentUser) return false;

    bool success = itemRepo->returnItem(itemId, quantity);
    if (success) {
        // Update borrow record
        borrowRepo->removeBorrow(currentUser->getUsername(), itemId);
    }
    return success;
}

std::vector<Borrow> MainController::getUserBorrows() const {
    if (!currentUser) return {};
    return borrowRepo->getBorrowsByUsername(currentUser->getUsername());
}

std::vector<Borrow> MainController::getAllBorrows() const {
    return borrowRepo->getAllBorrows();
}

bool MainController::addItem(const QString& name, int quantity) {
    // Find next available ID
    auto items = itemRepo->getAllItems();
    int maxId = 0;
    for (const auto& item : items) {
        if (item.getItemId() > maxId) {
            maxId = item.getItemId();
        }
    }

    Item newItem(maxId + 1, name.toStdString(), quantity);
    itemRepo->addItem(newItem);
    return true;
}

bool MainController::updateItem(int itemId, const QString& name, int quantity) {
    Item* item = itemRepo->findById(itemId);
    if (!item) return false;

    item->setName(name.toStdString());
    item->setQuantity(quantity);
    itemRepo->saveToFile();
    return true;
}

bool MainController::deleteItem(int itemId) {
    return itemRepo->removeItem(itemId);
}

bool MainController::canUserBorrow(int itemId, int quantity) const {
    Item* item = itemRepo->findById(itemId);
    return item && item->getQuantity() >= quantity;
}
