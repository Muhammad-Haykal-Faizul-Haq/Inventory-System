QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = InventorySystem
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

# Sources files - GUNAKAN BACKSLASH untuk Windows
SOURCES += main.cpp \
    Model/Item.cpp \
    Model/User.cpp \
    Model/Borrow.cpp \
    Model/ItemRepository.cpp \
    Model/UserRepository.cpp \
    Model/BorrowRepository.cpp \
    View/LoginWindow.cpp \
    View/MainWindow.cpp

# Header files
HEADERS += Model/Item.h \
    Model/User.h \
    Model/Borrow.h \
    Model/ItemRepository.h \
    Model/UserRepository.h \
    Model/BorrowRepository.h \
    View/LoginWindow.h \
    View/MainWindow.h

# UI forms
FORMS += View/LoginWindow.ui \
    View/MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Build directory
DESTDIR = $$PWD/build
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

# Include path
INCLUDEPATH += $$PWD



RESOURCES +=
    appresources.qrc
