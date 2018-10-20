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

#include "Scripts/Database/databasecontroller.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController data("db.inftech.hs-mannheim.de");
    Patient p("hans", "otto", UserType::patient, "hans@gmail.com");
    UserType g;
    QList<Patient> pList;

    try {
        g = data.isValidUser("hans@gmail.com", "hallo");
        if( g == UserType::inValidUser)
        {
            if(data.isUserCreated(&p, "hallo") == true)
            {
               qDebug() << "Patient erzeugt.";
               qDebug() << "UserType: " << int(p.getUserType());
            }
        }
        else
        {
           qDebug() << "Benutzer vorhanden.";
           qDebug() << "UserType: " << int(g);
        }

        p = data.getPatientData(p.getUserID());
        qDebug() << p.getForename();
        qDebug() << p.getUserID();
        qDebug() << "Bodaysize: " << p.getBodysize();

        data.loadDataset(pList, "TestDaten/user/");
        //data.loadDataset(pList, "TestDaten/measurement");

        data.updateUser(&pList[0]);
        qDebug() << "User update. New Data";

        g = data.isValidUser(p.getUserID(), "hallo");

        switch (g) {

        case UserType::patient:
            p = data.getPatientData(p.getUserID());
            qDebug() << "UserType: " << int(g);;
            qDebug() << "UserID: " << p.getUserID();
            qDebug() << "Bodaysize: " << p.getBodysize();
            qDebug() << "TargetBloodSugar: " << p.getTargetBloodSugar();

            p.setTargetBloodSugar(150);
            data.updateUser(&p);
            p = data.getPatientData(p.getUserID());
            qDebug() << "New TargetBloodSugar: " << p.getTargetBloodSugar();
            break;

        case UserType::inValidUser:
            qDebug() << "Invaild User.";
            break;
        }

        if(data.isUserDeleted(&p, "hallo") == true )
        {
            qDebug() << "Patient gelöscht.";
        }

    } catch (InvalidUser e) {
        qDebug() << e.getMessage();
    }
    catch (UserNotFound e) {
            qDebug() << e.getMessage();
    }

    return a.exec();
}
