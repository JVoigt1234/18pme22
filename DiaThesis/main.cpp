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

    DatabaseController test("db.inftech.hs-mannheim.de");
    test.start();


    return a.exec();
}
