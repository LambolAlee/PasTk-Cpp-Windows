cmake_minimum_required(VERSION 3.5)

project(PasTk-Cpp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Gui Core REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        home.cpp
        home.h
        home.ui
        data.h data.cpp
        detailwindow.cpp detailwindow.h detailwindow.ui
        datamodel.h datamodel.cpp
        rc.qrc
        item.h item.cpp
        animatedstackedwidget.h animatedstackedwidget.cpp
        singleton.hpp
        mergedialog.h mergedialog.cpp mergedialog.ui
        selectiondialog.h selectiondialog.cpp selectiondialog.ui
        continuousdialog.h continuousdialog.cpp continuousdialog.ui
        pasteutil.h pasteutil.cpp
        attentiondialog.h attentiondialog.cpp attentiondialog.ui
        itemdelegate.h itemdelegate.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PasTk-Cpp WIN32
        "app_win32.rc"
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PasTk-Cpp APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PasTk-Cpp SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PasTk-Cpp
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(PasTk-Cpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(PasTk-Cpp PRIVATE Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Core)

set_target_properties(PasTk-Cpp PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PasTk-Cpp)
endif()
