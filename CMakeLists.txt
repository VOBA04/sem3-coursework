cmake_minimum_required(VERSION 3.16)

project(photored LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


find_package(Qt6 6.2 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(OpenCV REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(TR_DIR  "${CMAKE_CURRENT_SOURCE_DIR}/translations")

set(PROJECT_SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/mainwindow.cpp
    ${SRC_DIR}/image_processing.cpp
    ${SRC_DIR}/utils.cpp
    ${SRC_DIR}/workerthread.cpp
    ${SRC_DIR}/cvmatandqimage.cpp
    ${SRC_DIR}/image_filters.cpp
    ${SRC_DIR}/set_FilterName_window.cpp
    ${INC_DIR}/mainwindow.h
    ${INC_DIR}/image_processing.h
    ${INC_DIR}/iconautosizepushbutton.h
    ${INC_DIR}/workerthread.h
    ${INC_DIR}/cvmatandqimage.h
    ${INC_DIR}/viewwithoutwheel.h
    ${INC_DIR}/image_filters.h
    ${INC_DIR}/MyRing.h
    ${INC_DIR}/set_FilterName_window.h
    ${RES_DIR}/mainwindow.ui
    ${RES_DIR}/set_FilterName_window.ui
    ${RES_DIR}/icons.qrc
)

qt_add_executable(photored
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

# Let AUTOUIC search for .ui files in resources/
set(CMAKE_AUTOUIC_SEARCH_PATHS "${RES_DIR}")
set_property(TARGET photored PROPERTY AUTOUIC_SEARCH_PATHS "${RES_DIR}")

qt_add_translations(photored TS_FILES
    ${TR_DIR}/photored_ru.ts
    ${TR_DIR}/photored_be.ts
)

target_link_libraries(photored PRIVATE Qt6::Widgets ${OpenCV_LIBS})

target_include_directories(photored PRIVATE
    ${INC_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

set_target_properties(photored PROPERTIES
    WIN32_EXECUTABLE $<$<BOOL:${WIN32}>:TRUE>
)

include(GNUInstallDirs)

install(TARGETS photored
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(photored)
