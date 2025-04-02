cmake_minimum_required(VERSION 3.10)
project(QSimpleUpdater
    LANGUAGES CXX
)

option(QSIMPLE_UPDATER_BUILD_TESTS "Build the unit tests" ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Network)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Widgets Network)

if(QSIMPLE_UPDATER_BUILD_TESTS)
    find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Test)
endif()

add_library(QSimpleUpdater STATIC
    etc/resources/qsimpleupdater.qrc
    include/QSimpleUpdater.h
    src/AuthenticateDialog.cpp
    src/AuthenticateDialog.h
    src/AuthenticateDialog.ui
    src/Downloader.cpp
    src/Downloader.h
    src/Downloader.ui
    src/QSimpleUpdater.cpp
    src/Updater.cpp
    src/Updater.h
)
target_include_directories(QSimpleUpdater PUBLIC include)
target_link_libraries(QSimpleUpdater PUBLIC Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Widgets PRIVATE Qt${QT_VERSION_MAJOR}::Network)

add_subdirectory(tutorial)

if(QSIMPLE_UPDATER_BUILD_TESTS)
    enable_testing()
    add_executable(UnitTests
        tests/main.cpp
        tests/Test_Versioning.h
        tests/Test_Updater.h
        tests/Test_QSimpleUpdater.h
        tests/Test_Downloader.h
    )
    add_test(NAME ApiTest COMMAND ApiTest)
    target_link_libraries(UnitTests PRIVATE Qt${QT_VERSION_MAJOR}::Test QSimpleUpdater)
endif()
