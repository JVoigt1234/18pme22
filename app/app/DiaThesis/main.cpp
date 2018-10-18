///-----------------------------------------------------------------
/// Namespace:
/// Class:              main
/// Description:        Example how to use the DatabaseController
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              -
/// Revision History:   First release
///-----------------------------------------------------------------

#include "mainwindow.h"
#include <QApplication>

#include "Scripts/Database/DatabaseController.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController data("db.inftech.hs-mannheim.de");

    Patient p = data.loadDataset();     //only for example

    if(data.isUserDeleted(&p, "hallo"))
    {
        data.isUserCreated(&p, "hallo");
    }


    data.updateUser(&p);
    p = data.getPatientData(p.getUserID());
    UserType g = data.isValidUser(p.getUserID(), "hallo");

    if(g > UserType::inValidUser )
    {
        qDebug() << "UserType: " << int(g);
        p = data.getPatientData(p.getUserID());
        qDebug() << "UserID: " << p.getUserID();
        qDebug() << "Bodaysize: " << p.getBodysize();
        qDebug() << "TargetBloodSugar: " << p.getTargetBloodSugar();
    }

    return a.exec();
}
