#include "BorrowRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

BorrowRepository::BorrowRepository(const std::string& filename)
    : filename(filename) {
    loadFromFile();
}

void BorrowRepository::loadFromFile() {
    borrowList.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string username, itemIdStr, quantityStr;

        if (!std::getline(ss, username, ',') ||
            !std::getline(ss, itemIdStr, ',') ||
            !std::getline(ss, quantityStr, ',')) {
            continue;
        }

        try {
            int itemId = std::stoi(itemIdStr);
            int qty = std::stoi(quantityStr);
            borrowList.emplace_back(username, itemId, qty);
        } catch (...) {
            continue;
        }
    }
    file.close();
}

void BorrowRepository::saveToFile() {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    for (auto& b : borrowList) {
        file << b.getUsername() << ","
             << b.getItemId() << ","
             << b.getQuantity() << "\n";
    }
    file.close();
}

void BorrowRepository::addBorrow(const std::string& username, int itemId, int qty) {
    borrowList.emplace_back(username, itemId, qty);
    saveToFile();
}

const std::vector<Borrow>& BorrowRepository::getBorrowList() const {
    return borrowList;
}

std::vector<Borrow> BorrowRepository::getBorrowsByUsername(const std::string& username) const {
    std::vector<Borrow> result;
    for (const auto& borrow : borrowList) {
        if (borrow.getUsername() == username) {
            result.push_back(borrow);
        }
    }
    return result;
}

std::vector<Borrow> BorrowRepository::getAllBorrows() const {
    return borrowList;
}

void BorrowRepository::removeBorrowAt(size_t index) {
    if (index < borrowList.size()) {
        borrowList.erase(borrowList.begin() + index);
        saveToFile();
    }
}

void BorrowRepository::reduceBorrowQuantity(size_t index, int qty) {
    if (index < borrowList.size()) {
        borrowList[index].setQuantity(borrowList[index].getQuantity() - qty);
        if (borrowList[index].getQuantity() <= 0) {
            borrowList.erase(borrowList.begin() + index);
        }
        saveToFile();
    }
}

bool BorrowRepository::removeBorrow(const std::string& username, int itemId) {
    for (size_t i = 0; i < borrowList.size(); i++) {
        if (borrowList[i].getUsername() == username &&
            borrowList[i].getItemId() == itemId) {
            borrowList.erase(borrowList.begin() + i);
            saveToFile();
            return true;
        }
    }
    return false;
}

int BorrowRepository::getTotalBorrows() const {
    return borrowList.size();
}

int BorrowRepository::getActiveBorrowCount() const {
    return borrowList.size();
}
