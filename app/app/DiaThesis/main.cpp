#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

//    SqlDatabase dataBase;
    //UserData firstperson;
//    dataBase.startDatabase();
//    dataBase.setPerson(firstperson);
    //Patient p("1", "hallo", UserType::admin, "admin@gmail.com", "123456789");

    DatabaseController data("db.inftech.hs-mannheim.de");
    //data.uploadUserData2Database(&firstperson);
    data.loadDataset();
    //DatabaseController test("http://http://localhost:4181");
    //test.start();


    return a.exec();
}
