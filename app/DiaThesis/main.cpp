#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController test("http://localhost:4181");
    test.start();


    return a.exec();
}
