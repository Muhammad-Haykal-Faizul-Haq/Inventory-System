#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QHeaderView>

MainWindow::MainWindow(User* currentUser, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentUser(currentUser)
    , itemRepo(new ItemRepository())
    , borrowRepo(new BorrowRepository())
    , userRepo(new UserRepository()) {

    ui->setupUi(this);

    updateWelcomeLabel();

    // Hide admin tab if not admin
    if (currentUser->getRole() != 1) {
        ui->tabWidget->removeTab(2); // Remove admin tab
    }

    // Setup table properties
    ui->itemTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->itemTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->itemTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->itemTable->horizontalHeader()->setStretchLastSection(true);

    ui->borrowTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->borrowTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->borrowTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->borrowTable->horizontalHeader()->setStretchLastSection(true);

    // Load initial data
    loadItemsToTable();
    loadUserBorrowsToTable();
}

MainWindow::~MainWindow() {
    delete itemRepo;
    delete borrowRepo;
    delete userRepo;
    delete ui;
}

void MainWindow::updateWelcomeLabel() {
    ui->welcomeLabel->setText(
        QString("Welcome, %1 (%2)")
            .arg(QString::fromStdString(currentUser->getUsername()))
            .arg(currentUser->getRoleString())
        );
}

void MainWindow::loadItemsToTable() {
    ui->itemTable->setRowCount(0);
    std::vector<Item> items = itemRepo->getAllItems();

    for (const auto& item : items) {
        int row = ui->itemTable->rowCount();
        ui->itemTable->insertRow(row);

        ui->itemTable->setItem(row, 0,
                               new QTableWidgetItem(QString::number(item.getItemId())));
        ui->itemTable->setItem(row, 1,
                               new QTableWidgetItem(QString::fromStdString(item.getItemName())));
        ui->itemTable->setItem(row, 2,
                               new QTableWidgetItem(QString::number(item.getQuantity())));
    }

    ui->itemTable->resizeColumnsToContents();
}

void MainWindow::loadUserBorrowsToTable() {
    ui->borrowTable->setRowCount(0);
    std::vector<Borrow> borrows = borrowRepo->getBorrowsByUsername(
        currentUser->getUsername());

    for (const auto& borrow : borrows) {
        int row = ui->borrowTable->rowCount();
        ui->borrowTable->insertRow(row);

        Item* item = itemRepo->findById(borrow.getItemId());
        QString itemName = item ? QString::fromStdString(item->getItemName()) : "Unknown";

        ui->borrowTable->setItem(row, 0,
                                 new QTableWidgetItem(QString::number(borrow.getItemId())));
        ui->borrowTable->setItem(row, 1,
                                 new QTableWidgetItem(itemName));
        ui->borrowTable->setItem(row, 2,
                                 new QTableWidgetItem(QString::number(borrow.getQuantity())));
        ui->borrowTable->setItem(row, 3,
                                 new QTableWidgetItem(borrow.getBorrowDateString()));
    }

    ui->borrowTable->resizeColumnsToContents();
}

void MainWindow::on_logoutButton_clicked() {
    close();

    // Show login window again
    QWidget* parent = parentWidget();
    if (parent) {
        parent->show();
    }
}

void MainWindow::on_refreshItemButton_clicked() {
    loadItemsToTable();
}

void MainWindow::on_borrowButton_clicked() {
    int itemId = getSelectedItemId();
    if (itemId == -1) {
        QMessageBox::warning(this, "Selection Error",
                             "Please select an item to borrow");
        return;
    }

    Item* item = itemRepo->findById(itemId);
    if (!item) {
        QMessageBox::critical(this, "Error", "Item not found!");
        return;
    }

    bool ok;
    int quantity = QInputDialog::getInt(this, "Borrow Item",
                                        QString("Quantity to borrow (Available: %1):")
                                            .arg(item->getQuantity()),
                                        1, 1, item->getQuantity(), 1, &ok);

    if (ok && quantity > 0) {
        if (itemRepo->borrowItem(itemId, quantity)) {
            borrowRepo->addBorrow(currentUser->getUsername(), itemId, quantity);
            loadItemsToTable();
            loadUserBorrowsToTable();

            QMessageBox::information(this, "Success",
                                     QString("Successfully borrowed %1 of %2")
                                         .arg(quantity)
                                         .arg(QString::fromStdString(item->getItemName())));
        } else {
            QMessageBox::critical(this, "Error",
                                  "Failed to borrow item. Insufficient stock.");
        }
    }
}

void MainWindow::on_refreshBorrowButton_clicked() {
    loadUserBorrowsToTable();
}

void MainWindow::on_returnButton_clicked() {
    int index = getSelectedBorrowIndex();
    if (index == -1) {
        QMessageBox::warning(this, "Selection Error",
                             "Please select a borrow record to return");
        return;
    }

    std::vector<Borrow> borrows = borrowRepo->getBorrowsByUsername(
        currentUser->getUsername());

    if (index >= static_cast<int>(borrows.size())) {
        QMessageBox::critical(this, "Error", "Invalid selection");
        return;
    }

    Borrow borrow = borrows[index];
    Item* item = itemRepo->findById(borrow.getItemId());

    if (!item) {
        QMessageBox::critical(this, "Error", "Associated item not found!");
        return;
    }

    bool ok;
    int quantity = QInputDialog::getInt(this, "Return Item",
                                        QString("Quantity to return (Borrowed: %1):")
                                            .arg(borrow.getQuantity()),
                                        1, 1, borrow.getQuantity(), 1, &ok);

    if (ok && quantity > 0) {
        if (currentUser->getRole() == 1) {
            // Admin can return directly
            itemRepo->returnItem(borrow.getItemId(), quantity);

            // Update borrow record
            if (quantity == borrow.getQuantity()) {
                // Full return, remove record
                borrowRepo->removeBorrow(currentUser->getUsername(), borrow.getItemId());
            } else {
                // Partial return - but BorrowRepository tidak punya fungsi reduce quantity by username
                // Untuk sementara, hapus dan buat baru
                borrowRepo->removeBorrow(currentUser->getUsername(), borrow.getItemId());
                if (quantity < borrow.getQuantity()) {
                    borrowRepo->addBorrow(currentUser->getUsername(),
                                          borrow.getItemId(),
                                          borrow.getQuantity() - quantity);
                }
            }

            loadItemsToTable();
            loadUserBorrowsToTable();

            QMessageBox::information(this, "Success",
                                     QString("Returned %1 of %2")
                                         .arg(quantity)
                                         .arg(QString::fromStdString(item->getItemName())));
        } else {
            // Regular users need admin validation
            QMessageBox::information(this, "Return Request",
                                     "Please contact an admin to validate your return.\n"
                                     "Bring the items to the admin station.");
        }
    }
}

// Admin functions
void MainWindow::on_addItemButton_clicked() {
    bool ok;
    QString name = QInputDialog::getText(this, "Add New Item",
                                         "Item Name:", QLineEdit::Normal,
                                         "", &ok);

    if (ok && !name.isEmpty()) {
        int quantity = QInputDialog::getInt(this, "Add New Item",
                                            "Initial Quantity:",
                                            1, 0, 10000, 1, &ok);
        if (ok) {
            // Find next available ID
            std::vector<Item> items = itemRepo->getAllItems();
            int maxId = 0;
            for (const auto& item : items) {
                if (item.getItemId() > maxId) {
                    maxId = item.getItemId();
                }
            }

            Item newItem(maxId + 1, name.toStdString(), quantity);
            itemRepo->addItem(newItem);
            loadItemsToTable();

            showAdminMessage(QString("Added new item: %1 (ID: %2, Qty: %3)")
                                 .arg(name).arg(maxId + 1).arg(quantity));
        }
    }
}

void MainWindow::on_editItemButton_clicked() {
    int itemId = getSelectedItemId();
    if (itemId == -1) {
        QMessageBox::warning(this, "Selection Error",
                             "Please select an item to edit");
        return;
    }

    Item* item = itemRepo->findById(itemId);
    if (!item) {
        QMessageBox::critical(this, "Error", "Item not found!");
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(this, "Edit Item",
                                            "New Item Name:",
                                            QLineEdit::Normal,
                                            QString::fromStdString(item->getItemName()),
                                            &ok);

    if (ok && !newName.isEmpty()) {
        int newQuantity = QInputDialog::getInt(this, "Edit Item",
                                               "New Quantity:",
                                               item->getQuantity(),
                                               0, 10000, 1, &ok);
        if (ok) {
            item->setName(newName.toStdString());
            item->setQuantity(newQuantity);
            itemRepo->saveToFile();
            loadItemsToTable();

            showAdminMessage(QString("Updated item ID %1: %2 (Qty: %3)")
                                 .arg(itemId).arg(newName).arg(newQuantity));
        }
    }
}

void MainWindow::on_deleteItemButton_clicked() {
    int itemId = getSelectedItemId();
    if (itemId == -1) {
        QMessageBox::warning(this, "Selection Error",
                             "Please select an item to delete");
        return;
    }

    Item* item = itemRepo->findById(itemId);
    if (!item) {
        QMessageBox::critical(this, "Error", "Item not found!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Delete",
                                  QString("Are you sure you want to delete:\n"
                                          "%1 (ID: %2)?\n\n"
                                          "This action cannot be undone.")
                                      .arg(QString::fromStdString(item->getItemName()))
                                      .arg(itemId),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (itemRepo->removeItem(itemId)) {
            loadItemsToTable();
            showAdminMessage(QString("Deleted item ID %1: %2")
                                 .arg(itemId)
                                 .arg(QString::fromStdString(item->getItemName())));
        } else {
            QMessageBox::critical(this, "Error", "Failed to delete item");
        }
    }
}

void MainWindow::on_viewUsersButton_clicked() {
    std::vector<User> users = userRepo->getAllUsers();

    QString userList = "Registered Users:\n\n";
    for (const auto& user : users) {
        userList += QString("ID: %1 | Username: %2 | Role: %3\n")
        .arg(user.getId())
            .arg(QString::fromStdString(user.getUsername()))
            .arg(user.getRoleString());
    }

    QMessageBox::information(this, "All Users", userList);
}

void MainWindow::on_viewBorrowsButton_clicked() {
    std::vector<Borrow> allBorrows = borrowRepo->getAllBorrows();

    if (allBorrows.empty()) {
        QMessageBox::information(this, "All Borrows", "No active borrows found.");
        return;
    }

    QString borrowList = "All Active Borrows:\n\n";
    for (const auto& borrow : allBorrows) {
        Item* item = itemRepo->findById(borrow.getItemId());
        QString itemName = item ? QString::fromStdString(item->getItemName()) : "Unknown";

        borrowList += QString("User: %1 | Item: %2 (ID: %3) | Qty: %4 | Date: %5\n")
                          .arg(QString::fromStdString(borrow.getUsername()))
                          .arg(itemName)
                          .arg(borrow.getItemId())
                          .arg(borrow.getQuantity())
                          .arg(borrow.getBorrowDateString());
    }

    QMessageBox::information(this, "All Borrows", borrowList);
}

void MainWindow::on_validateReturnButton_clicked() {
    std::vector<Borrow> allBorrows = borrowRepo->getAllBorrows();

    if (allBorrows.empty()) {
        QMessageBox::information(this, "Validate Returns",
                                 "No active borrows to validate.");
        return;
    }

    QStringList borrowItems;
    for (size_t i = 0; i < allBorrows.size(); i++) {
        const auto& borrow = allBorrows[i];
        Item* item = itemRepo->findById(borrow.getItemId());
        QString itemName = item ? QString::fromStdString(item->getItemName()) : "Unknown";

        borrowItems << QString("%1. %2 - %3 (Qty: %4)")
                           .arg(i + 1)
                           .arg(QString::fromStdString(borrow.getUsername()))
                           .arg(itemName)
                           .arg(borrow.getQuantity());
    }

    bool ok;
    QString selected = QInputDialog::getItem(this, "Validate Return",
                                             "Select a borrow to validate:",
                                             borrowItems, 0, false, &ok);

    if (ok && !selected.isEmpty()) {
        int index = borrowItems.indexOf(selected);
        if (index >= 0 && index < static_cast<int>(allBorrows.size())) {
            Borrow borrow = allBorrows[index];
            Item* item = itemRepo->findById(borrow.getItemId());

            if (!item) {
                QMessageBox::critical(this, "Error", "Item not found!");
                return;
            }

            int returnQty = QInputDialog::getInt(this, "Validate Return",
                                                 QString("Quantity returned by %1\n"
                                                         "Item: %2\n"
                                                         "Borrowed: %3")
                                                     .arg(QString::fromStdString(borrow.getUsername()))
                                                     .arg(QString::fromStdString(item->getItemName()))
                                                     .arg(borrow.getQuantity()),
                                                 borrow.getQuantity(),
                                                 1, borrow.getQuantity(), 1, &ok);

            if (ok) {
                // Update item stock
                itemRepo->returnItem(borrow.getItemId(), returnQty);

                // Update borrow record
                if (returnQty == borrow.getQuantity()) {
                    // Full return, remove record
                    borrowRepo->removeBorrow(borrow.getUsername(), borrow.getItemId());
                } else {
                    // Partial return - remove and create new
                    borrowRepo->removeBorrow(borrow.getUsername(), borrow.getItemId());
                    borrowRepo->addBorrow(borrow.getUsername(),
                                          borrow.getItemId(),
                                          borrow.getQuantity() - returnQty);
                }

                loadItemsToTable();
                loadUserBorrowsToTable();

                QMessageBox::information(this, "Success",
                                         QString("Validated return of %1 %2 by %3")
                                             .arg(returnQty)
                                             .arg(QString::fromStdString(item->getItemName()))
                                             .arg(QString::fromStdString(borrow.getUsername())));
            }
        }
    }
}

int MainWindow::getSelectedItemId() const {
    QList<QTableWidgetItem*> selected = ui->itemTable->selectedItems();
    if (selected.isEmpty()) return -1;

    int row = selected.first()->row();
    QTableWidgetItem* idItem = ui->itemTable->item(row, 0);
    return idItem ? idItem->text().toInt() : -1;
}

int MainWindow::getSelectedBorrowIndex() const {
    QList<QTableWidgetItem*> selected = ui->borrowTable->selectedItems();
    if (selected.isEmpty()) return -1;
    return selected.first()->row();
}

void MainWindow::showAdminMessage(const QString& message) {
    QMessageBox::information(this, "Admin Action", message);
}
