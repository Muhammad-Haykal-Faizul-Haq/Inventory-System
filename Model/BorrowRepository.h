#pragma once
#include <vector>
#include <string>
#include <QString>
#include "Borrow.h"

class BorrowRepository {
private:
    std::vector<Borrow> borrowList;
    std::string filename;

public:
    BorrowRepository(const std::string& filename = "Resources/borrowList.txt");

    void loadFromFile();
    void saveToFile();

    void addBorrow(const std::string& username, int itemId, int qty);
    const std::vector<Borrow>& getBorrowList() const;

    std::vector<Borrow> getBorrowsByUsername(const std::string& username) const;
    std::vector<Borrow> getAllBorrows() const;

    void removeBorrowAt(size_t index);
    void reduceBorrowQuantity(size_t index, int qty);
    bool removeBorrow(const std::string& username, int itemId);

    int getTotalBorrows() const;
    int getActiveBorrowCount() const;
};
