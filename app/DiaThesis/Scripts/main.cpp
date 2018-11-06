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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseController *data;
    data = new DatabaseController("db.inftech.hs-mannheim.de");
//***************************************************************************************
//    DatabaseController data("db.inftech.hs-mannheim.de", "1814116_PME2Development");
//    data.creatDatabase();
//    data.loadDateset();

    QList<Patient> pList;
//    QList<Doctor> dList;
//    QList<Member> mList;
//    QList<User> allList;
    data->loadDataset(pList);
//    data.loadDataset(dList);
//    data.loadDataset(mList);

    qDebug() << data->getFact();
//    int deleted = 0;

////    for(int i=0;i<pList.length();i++)
////    {
////        if(data.isValidUser(pList[i].geteMail(), "Passwort") == UserType::patient)
////        {
////            if(data.isUserDeleted(&pList[i], "Passwort") == true)
////                deleted++;
////        }
////    }

////    for(int i=0;i<dList.length();i++)
////    {
////        if(data.isValidUser(dList[i].geteMail(), "Passwort") == UserType::doctor)
////        {
////            if(data.isUserDeleted(&dList[i], "Passwort") == true)
////                deleted++;
////        }
////    }

////    for(int i=0;i<mList.length();i++)
////    {
////        if(data.isValidUser(mList[i].geteMail(), "Passwort") == UserType::member)
////        {
////            if(data.isUserDeleted(&mList[i], "Passwort") == true)
////                deleted++;
////        }
////    }

////    qDebug() << QString::number(deleted) + " von " + QString::number( (pList.length()+dList.length()+mList.length()) ) + " gelöscht";

//    for(int i=0;i<pList.length();i++)
//    {
//        if(data.isUserCreated(&pList[i], "Passwort") == true)
//        {
//            data.updateUser(&pList[i]);
//            qDebug() << pList[i].geteMail() << "created.";
//        }
//    }

//    for(int i=0;i<dList.length();i++)
//    {
//        if(data.isUserCreated(&dList[i], "Passwort") == true)
//        {
//            data.updateUser(&dList[i]);
//            qDebug() << dList[i].geteMail() << "created.";
//        }
//    }

//    for(int i=0;i<mList.length();i++)
//    {
//        if(data.isUserCreated(&mList[i], "Passwort") == true)
//        {
//            data.updateUser(&mList[i]);
//            qDebug() << mList[i].geteMail() << "created.";
//        }
//    }

    data->isValidUser(pList[7].geteMail(),"Passwort");


//    QList<Measurement> pressureList;
//    data.loadDataset(pressureList, MeasurementType::bloodPressure);
//    data.uploadData(pressureList);

    QDateTime from, to;
    QList<Measurement> testList;
    from = QDateTime::fromString("2018-10-10 15:00:40", TimeStampFormate);
    to = QDateTime::fromString("2018-10-10 15:20:10", TimeStampFormate);
    data->getBloodPressure(from, to, testList);

    qDebug() << "pressure:";
    for(int i = 0; i < testList.length(); i++)
    {
        qDebug() << testList[i].getValue();
    }
    testList.clear();

//    QList<Measurement> sugarList;
//    data.loadDataset(sugarList, MeasurementType::bloodSugar);
//    data.uploadData(sugarList);

    data->getBloodSugar(from, to, testList);

    qDebug() << "sugar:";
    for(int i = 0; i < testList.length(); i++)
    {
        qDebug() << testList[i].getValue();
    }

    qDebug() << data->getFact();

//    //data.deleteBloodPressureData(from, to);

//    //data.deleteDatabase();
//    MainWindow w(&data);

//***************************************************************************************

    MainWindow w(data);

    w.show();
    return a.exec();
}
