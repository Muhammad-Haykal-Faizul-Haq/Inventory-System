#include "LoginWindow.h"
#include "./ui_LoginWindow.h"
#include "../View/MainWindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    , userRepo(new UserRepository())
    , currentUser(nullptr) {

    setupUI();
    connectSignals();
}

LoginWindow::~LoginWindow() {
    delete userRepo;
    delete ui;
}

void LoginWindow::setupUI() {
    ui->setupUi(this);

    // Set window properties
    setWindowTitle("Inventory System - Login");
    setFixedSize(400, 300);
}

void LoginWindow::connectSignals() {
    // Connect button signal to slot
    connect(ui->loginButton, &QPushButton::clicked,
            this, &LoginWindow::onLoginButtonClicked);
}

void LoginWindow::onLoginButtonClicked() {
    QString username = ui->usernameEdit->text().trimmed();
    QString password = ui->passwordEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Input Error",
                             "Please enter both username and password");
        return;
    }

    currentUser = userRepo->validateLogin(username.toStdString(),
                                          password.toStdString());

    if (currentUser) {
        // Hide login window
        hide();

        // Create and show main window
        MainWindow* mainWindow = new MainWindow(currentUser);
        mainWindow->show();
    } else {
        QMessageBox::critical(this, "Login Failed",
                              "Invalid username or password");
        ui->passwordEdit->clear();
        ui->usernameEdit->setFocus();
    }
}
