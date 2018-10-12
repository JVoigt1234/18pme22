#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"
#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SqlDatabase test2;
    test2.start();

    //DatabaseController test("http://http://localhost:4181");
    //test.start();


    return a.exec();
}
