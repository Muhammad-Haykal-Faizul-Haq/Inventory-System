#include <QApplication>
#include <QStyleFactory>
#include "View/LoginWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Set application style
    app.setStyle(QStyleFactory::create("Fusion"));

    // Set application info
    app.setApplicationName("Inventory Management System");
    app.setOrganizationName("InventoryApp");

    // Create and show login window
    LoginWindow loginWindow;
    loginWindow.show();

    QApplication::setWindowIcon(QIcon(":/Resources/LogoIP.ico"));
    app.setWindowIcon(QIcon(":/Resources/LogoIP.ico"));

    return app.exec();
}
