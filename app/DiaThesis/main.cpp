#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"
#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SqlDatabase dataBase;
    UserData firstperson;
    dataBase.startDatabase();
    dataBase.setPerson(firstperson);

    //DatabaseController test("http://http://localhost:4181");
    //test.start();


    return a.exec();
}
