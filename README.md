### CMake файл проекта, в котором хотим использовать библиотеку 

```cmake
cmake_minimum_required(VERSION 3.5)

project(test VERSION 0.1 LANGUAGES CXX)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Sql REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем путь до библиотеки
set(DB_VIEWER_PATH "../db_viewer/build")

# Указываем директорию, где находится библиотека
link_directories(${DB_VIEWER_PATH})

set(PROJECT_SOURCES
    main.cpp
)

add_executable(test ${PROJECT_SOURCES})

target_link_libraries(test PRIVATE db_viewer Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Sql)

# Указываем директорию с заголовочным файлом
target_include_directories(test PRIVATE ../db_viewer)
```

Подключаем хэдер из библиотеки для доступа к виджету DbWidget.

Сам виджет ожидает три параметра:

1) имя пользователя;
2) хост;
3) пароль;

```c++
#include "dbwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbWidget db("postgres", "localhost", "25944");
    db.show();
    return a.exec();
}
```

![1](/img/1.jpg)

![2](/img/2.jpg)

![3](/img/3.jpg)

![4](/img/4.jpg)