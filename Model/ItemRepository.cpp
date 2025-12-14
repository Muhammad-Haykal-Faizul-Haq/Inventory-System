#include "ItemRepository.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QStringList>

ItemRepository::ItemRepository(const std::string &filename)
    : filename(filename) {
    loadItems();
}

void ItemRepository::loadItems() {
    items.clear();
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string idStr, name, qtyStr;

        if (!std::getline(ss, idStr, ',') ||
            !std::getline(ss, name, ',') ||
            !std::getline(ss, qtyStr, ',')) {
            continue;
        }

        if (idStr.empty() || name.empty() || qtyStr.empty())
            continue;

        try {
            int id = std::stoi(idStr);
            int qty = std::stoi(qtyStr);
            items.emplace_back(id, name, qty);
        } catch (...) {
            continue;
        }
    }
    file.close();
}

void ItemRepository::saveItems() {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot save to file " << filename << std::endl;
        return;
    }

    for (const auto &item : items) {
        file << item.getItemId() << ","
             << item.getItemName() << ","
             << item.getQuantity() << "\n";
    }
    file.close();
}

std::vector<Item> ItemRepository::getAllItems() const {
    return items;
}

std::vector<QString> ItemRepository::getAllItemNames() const {
    std::vector<QString> names;
    for (const auto& item : items) {
        names.push_back(QString::fromStdString(item.getItemName()));
    }
    return names;
}

Item* ItemRepository::getItemById(int id) {
    for (auto &item : items) {
        if (item.getItemId() == id)
            return &item;
    }
    return nullptr;
}

Item* ItemRepository::getItemByName(const std::string& name) {
    for (auto &item : items) {
        if (item.getItemName() == name)
            return &item;
    }
    return nullptr;
}

void ItemRepository::addItem(const Item &item) {
    items.push_back(item);
    saveItems();
}

bool ItemRepository::removeItem(int id) {
    for (size_t i = 0; i < items.size(); i++) {
        if (items[i].getItemId() == id) {
            items.erase(items.begin() + i);
            saveItems();
            return true;
        }
    }
    return false;
}

bool ItemRepository::updateItem(const Item &updatedItem) {
    for (auto &item : items) {
        if (item.getItemId() == updatedItem.getItemId()) {
            item = updatedItem;
            saveItems();
            return true;
        }
    }
    return false;
}

void ItemRepository::saveToFile() {
    saveItems();
}

Item* ItemRepository::findById(int id) {
    return getItemById(id);
}

bool ItemRepository::borrowItem(int id, int quantity) {
    Item* item = getItemById(id);
    if (!item || item->getQuantity() < quantity) {
        return false;
    }

    item->setQuantity(item->getQuantity() - quantity);
    saveItems();
    return true;
}

bool ItemRepository::returnItem(int id, int quantity) {
    Item* item = getItemById(id);
    if (!item) {
        return false;
    }

    item->setQuantity(item->getQuantity() + quantity);
    saveItems();
    return true;
}

int ItemRepository::getTotalItems() const {
    return items.size();
}

int ItemRepository::getTotalQuantity() const {
    int total = 0;
    for (const auto& item : items) {
        total += item.getQuantity();
    }
    return total;
}
