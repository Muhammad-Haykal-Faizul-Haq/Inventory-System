#pragma once

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QTableWidget>
#include "../Model/User.h"
#include "../Model/ItemRepository.h"
#include "../Model/BorrowRepository.h"
#include "../Model/UserRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(User* currentUser, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_logoutButton_clicked();
    void on_refreshItemButton_clicked();
    void on_borrowButton_clicked();
    void on_refreshBorrowButton_clicked();
    void on_returnButton_clicked();

    // Admin buttons
    void on_addItemButton_clicked();
    void on_editItemButton_clicked();
    void on_deleteItemButton_clicked();
    void on_viewUsersButton_clicked();
    void on_viewBorrowsButton_clicked();
    void on_validateReturnButton_clicked();

private:
    Ui::MainWindow *ui;
    User* currentUser;
    ItemRepository* itemRepo;
    BorrowRepository* borrowRepo;
    UserRepository* userRepo;

    void loadItemsToTable();
    void loadUserBorrowsToTable();
    void showAdminMessage(const QString& message);

    int getSelectedItemId() const;
    int getSelectedBorrowIndex() const;
    void updateWelcomeLabel();
};
