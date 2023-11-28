cmake_minimum_required(VERSION 3.5)

project(photored)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        image_processing.h
        image_processing.cpp
        utils.cpp
        iconautosizepushbutton.h
        workerthread.h
        workerthread.cpp
        cvmatandqimage.h
        cvmatandqimage.cpp
)

qt_add_executable(photored
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_link_libraries(photored PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(photored PROPERTIES
    WIN32_EXECUTABLE TRUE
)

install(TARGETS photored
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
find_package(OpenCV REQUIRED)
target_link_libraries(photored PRIVATE ${OpenCV_LIBS})
qt_finalize_executable(photored)


include_directories( ${OpenCV_INCLUDE_DIRS})
