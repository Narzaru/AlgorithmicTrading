cmake_minimum_required(VERSION 3.22.1)

project(A8_Algorithmic_trading VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-O2 -Wall -Wextra -Wshadow)

find_package(QT NAMES Qt5 Qt6 COMPONENTS Widgets PrintSupport Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets PrintSupport Charts)

set(PROJECT_SOURCES
        main.cc
        mainwindow.cc
        mainwindow.h
        mainwindow.ui
        menus.cc
        menus.h
        menus.ui
        charts.h
        charts.cc)

if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(A8_Algorithmic_trading
            MANUAL_FINALIZATION
            ${PROJECT_SOURCES}
            )
    # Define target properties for Android with Qt 6 as:
    #    set_property(TARGET A8_Algorithmic_trading APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else ()
    if (ANDROID)
        add_library(A8_Algorithmic_trading SHARED
                ${PROJECT_SOURCES}
                )
        # Define properties for Android with Qt 5 after find_package() calls as:
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else ()
        add_executable(A8_Algorithmic_trading
                ${PROJECT_SOURCES}
                )
    endif ()
endif ()

add_subdirectory(models)

target_link_libraries(A8_Algorithmic_trading PRIVATE
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::PrintSupport
        Qt${QT_VERSION_MAJOR}::Charts
        qt_models)

target_include_directories(A8_Algorithmic_trading PUBLIC .)

set_target_properties(A8_Algorithmic_trading PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )

install(TARGETS A8_Algorithmic_trading
        DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/install)

if (QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(A8_Algorithmic_trading)
endif ()
