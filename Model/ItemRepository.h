#pragma once
#include <vector>
#include <string>
#include <QString>
#include "Item.h"

class ItemRepository {
private:
    std::vector<Item> items;
    std::string filename;

    void loadItems();
    void saveItems();

public:
    ItemRepository(const std::string &filename = "Resources/itemList.txt");

    std::vector<Item> getAllItems() const;
    std::vector<QString> getAllItemNames() const;
    Item* getItemById(int id);
    Item* getItemByName(const std::string& name);

    void addItem(const Item &item);
    bool removeItem(int id);
    bool updateItem(const Item &updatedItem);
    void saveToFile();
    Item* findById(int id);

    bool borrowItem(int id, int quantity);
    bool returnItem(int id, int quantity);

    int getTotalItems() const;
    int getTotalQuantity() const;
};
