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
#include <QList>

#include "Scripts/Database/databasecontroller.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController data("db.inftech.hs-mannheim.de");    
    //data.creatDatabase();

    QList<Patient> pList;
    QList<Doctor> dList;
    QList<Member> mList;
    data.loadDataset(pList);
    data.loadDataset(dList);
    data.loadDataset(mList);

    for(int i=0;i<pList.length();i++)
    {
        if(data.isUserCreated(&pList[i], "Passwort") == true)
        {
            data.updateUser(&pList[i]);
            qDebug() << pList[i].geteMail() << "created.";
        }
    }

    for(int i=0;i<dList.length();i++)
    {
        if(data.isUserCreated(&dList[i], "Passwort") == true)
        {
            data.updateUser(&dList[i]);
            qDebug() << dList[i].geteMail() << "created.";
        }
    }

    for(int i=0;i<mList.length();i++)
    {
        if(data.isUserCreated(&mList[i], "Passwort") == true)
        {
            data.updateUser(&mList[i]);
            qDebug() << mList[i].geteMail() << "created.";
        }
    }

    data.isValidUser(pList[7].geteMail(),"Passwort");

    qDebug() << pList[7].geteMail();
    QList<BloodPressure> pressureList;
    data.loadDataset(pressureList);

    QList<BloodSugar> sugarList;
    data.loadDataset(sugarList);

//    try {
//       if(data.isUserCreated(&dList[0], "hallo") == true) qDebug() << "Doktor angelegt.";
//       if(data.isUserCreated(&pList[0], "hallo") == true) qDebug() << "Patient angelegt.";
//       if(data.isUserCreated(&mList[0], "hallo") == true) qDebug() << "Member angelegt.";
//       if(data.isValidUser(dList[0].getUserID(), "hallo") == UserType::doctor)
//       {
//           qDebug() << "Doktor vorhanden.";
//           data.updateUser(&dList[0]);
//       }
//       if(data.isValidUser(mList[0].getUserID(), "hallo") == UserType::member)
//       {
//           qDebug() << "Member vorhanden.";
//           data.updateUser(&mList[0]);
//       }
//       if(data.isValidUser(pList[0].getUserID(), "hallo") == UserType::patient)
//       {
//           qDebug() << "Patient vorhanden.";
//           data.updateUser(&pList[0]);
//       }

//       data.allowAccess(dList[0].getUserID());
//       Doctor d = data.getDoctorData(dList[0].getUserID());
//       if(d == dList[0])
//       {qDebug() << "same";}
//       data.denyAccess(dList[0].getUserID());

//    } catch (InvalidUser e) {
//        qDebug() << e.getMessage();
//    }
//    catch (UserNotFound e) {
//            qDebug() << e.getMessage();
//    }
//    catch (InvalidDateTimeFormate e) {
//        qDebug() << e.getMessage();
//    }

    //data.deleteDatabase();

    return a.exec();
}
