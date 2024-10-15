cmake_minimum_required(VERSION 3.5)

project(test VERSION 0.1 LANGUAGES CXX)

# Для использования Qt
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь до библиотеки lib1
set(DB_VIEWER_PATH "../db_viewer/build")

# Указываем директорию, где находится библиотека
link_directories(${DB_VIEWER_PATH})

set(PROJECT_SOURCES
    main.cpp
)

add_executable(test ${PROJECT_SOURCES})

target_link_libraries(test PRIVATE db_viewer Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Указываем директорию с заголовочными файлами
target_include_directories(test PRIVATE ../db_viewer)
