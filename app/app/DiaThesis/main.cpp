#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/DatabaseController.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    //Patient p("1", "hallo", UserType::admin, "admin@gmail.com", "123456789");

    DatabaseController data("db.inftech.hs-mannheim.de");
    Patient p = data.loadDataset();
    //data.creatUser(&p, "hallo");
    p = data.getPatientData(p.getUserID());

    data.uploadData(&p);
    UserType g = data.isValidUser(p.getUserID(), "hallo");
    qDebug() << int(g);

//    data.getPatientData(p.getUserID());
    //Patient pü = data.getUser();

    //DatabaseController test("http://http://localhost:4181");
    //test.start();


    return a.exec();
}
