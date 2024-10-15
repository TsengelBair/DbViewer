#include <QApplication>

#include "dbwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbWidget db("postgres", "localhost", "25944");
    db.show();
    return a.exec();
}
