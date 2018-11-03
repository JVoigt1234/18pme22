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
    QList<Measurement> pressureList;
    data.loadDataset(pressureList, MeasurementType::bloodPressure);
    data.uploadData(pressureList);

    QDateTime from, to;
    QList<Measurement> testList;
    from = QDateTime::fromString("2018-10-10 15:00:40", TimeStampFormate);
    to = QDateTime::fromString("2018-10-10 15:20:10", TimeStampFormate);
    data.getBloodPressure(from, to, testList);

    QList<Measurement> sugarList;
    data.loadDataset(sugarList, MeasurementType::bloodSugar);
    data.uploadData(sugarList);

    //data.deleteBloodPressureData(from, to);

    //data.deleteDatabase();

    return a.exec();
}
