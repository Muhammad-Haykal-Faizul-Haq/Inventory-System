#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include "../Model/UserRepository.h"

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow {
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginButtonClicked();

private:
    Ui::LoginWindow *ui;
    UserRepository* userRepo;
    User* currentUser;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    QLabel* titleLabel;
    QLabel* usernameLabel;
    QLineEdit* usernameEdit;
    QLabel* passwordLabel;
    QLineEdit* passwordEdit;
    QPushButton* loginButton;

    void setupUI();
    void connectSignals();
};
