cmake_minimum_required(VERSION 3.16)
project(InventorySystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Set sumber files
set(SOURCES
    main.cpp
    Model/Item.cpp
    Model/User.cpp
    Model/Borrow.cpp
    Model/ItemRepository.cpp
    Model/UserRepository.cpp
    Model/BorrowRepository.cpp
    Controller/LoginController.cpp
    Controller/MainController.cpp
    Controller/AdminController.cpp
)

set(HEADERS
    Model/Item.h
    Model/User.h
    Model/Borrow.h
    Model/ItemRepository.h
    Model/UserRepository.h
    Model/BorrowRepository.h
    Controller/LoginController.h
    Controller/MainController.h
    Controller/AdminController.h
)

set(UI_FILES
    View/LoginWindow.ui
    View/MainWindow.ui
    View/AdminWindow.ui
    View/UserWindow.ui
)

# Buat executable
qt_add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

# Copy data files ke build directory
file(COPY Resources/ DESTINATION ${CMAKE_BINARY_DIR}/Resources)