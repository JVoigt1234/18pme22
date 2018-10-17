#include "mainwindow.h"
#include "registrationwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    RegistrationWindow rw;
    w.show();

    DatabaseController test("http://http://localhost:4181");
    test.start();


    return a.exec();
}
