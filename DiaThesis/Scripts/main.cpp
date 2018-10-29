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
//#include <QRandomGenerator>

#include "Scripts/Database/databasecontroller.h"
//#include "Scripts/Database/sqldatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    DatabaseController data("db.inftech.hs-mannheim.de");
    Patient p("hans", "otto", UserType::patient, "hans@gmail.com", "1992.03.23");
    UserType g;
    QList<Patient> pList;
    //data.loadDataset(pList,"TestDaten/user/" );

    //data.creatDatabase();
//    try {
//        if(data.isUserAvailable(pList[1].getUserID()) == false)
//        {
//            data.isUserCreated(&pList[1], "hallo");
//            if(data.isValidUser(pList[1].getUserID(), "hallo") == UserType::patient )
//            {
//                qDebug() << "User Patient angelegt";
//            }
//        }
//        else{
//            qDebug() << "User existiert bereits.";
//        }

//        p = data.getPatientData(&pList[1]);

//        if(data.isValidUser(pList[1].getUserID(), "hallo") == UserType::patient )
//        {
//            qDebug() << "User Patient vorhanden";
//            data.isUserDeleted(&pList[1], "hallo");
//        }


//    } catch (InvalidUser e) {
//        qDebug() << e.getMessage();
//    }
//    catch (UserNotFound e) {
//            qDebug() << e.getMessage();
//    }
//    catch (InvalidDateTimeFormate e) {
//        qDebug() << e.getMessage();
//    }

    return a.exec();
}
