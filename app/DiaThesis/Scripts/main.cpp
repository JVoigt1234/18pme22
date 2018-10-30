///-----------------------------------------------------------------
/// Namespace:
/// Class:              main
/// Description:        Example how to use the DatabaseController
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              -
/// Revision History:   First release
///-----------------------------------------------------------------

#include "Scripts/UI/mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController data("db.inftech.hs-mannheim.de");
    QList<Patient> pList;
    QList<Doctor> dList;
    QList<Member> mList;
    data.loadDataset(pList);
    data.loadDataset(dList);
    data.loadDataset(mList);

    //data.creatDatabase();
    try {
       if(data.isUserCreated(&dList[0], "hallo") == true) qDebug() << "Doktor angelegt.";
       if(data.isUserCreated(&pList[0], "hallo") == true) qDebug() << "Patient angelegt.";
       if(data.isUserCreated(&mList[0], "hallo") == true) qDebug() << "Member angelegt.";
       if(data.isValidUser(dList[0].getUserID(), "hallo") == UserType::doctor)
       {
           qDebug() << "Doktor vorhanden.";
           data.updateUser(&dList[0]);
       }
       if(data.isValidUser(mList[0].getUserID(), "hallo") == UserType::member)
       {
           qDebug() << "Member vorhanden.";
           data.updateUser(&mList[0]);
       }
       if(data.isValidUser(pList[0].getUserID(), "hallo") == UserType::patient)
       {
           qDebug() << "Patient vorhanden.";
           data.updateUser(&pList[0]);
       }

       data.allowAccess(dList[0].getUserID());
       Doctor d = data.getDoctorData(dList[0].getUserID());
       if(d == dList[0])
       {qDebug() << "same";}
       data.denyAccess(dList[0].getUserID());

    } catch (InvalidUser e) {
        qDebug() << e.getMessage();
    }
    catch (UserNotFound e) {
            qDebug() << e.getMessage();
    }
    catch (InvalidDateTimeFormate e) {
        qDebug() << e.getMessage();
    }

    //data.deleteDatabase();

    return a.exec();
}
