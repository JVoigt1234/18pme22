#include "sqldatabase.h"

#include <QtSql>
#include <QtDebug>

SqlDatabase::SqlDatabase()
{

}

void SqlDatabase::run()
{
    QString hostname = "db.inftech.hs-mannheim.de";
    QString dbname = "1813680_DiaThesis";
    QString user = "1813680";
    QString passwd = "DiaThesis2018";

    QSqlDatabase database = QSqlDatabase::addDatabase("QPSQL"); //QSqlDatabase !warning! software need to be installed

    database.setConnectOptions();

    database.setDatabaseName(dbname);
    database.setHostName(hostname);

    if(database.open(user, passwd))
    {
        qDebug() << "Opend!";

        QSqlQuery creating("CREATE TABLE tasks (taskId INTEGER, PRIMARY KEY(taskId))");
        creating.exec();

        //writing
        creating.prepare("INSERT INTO tasks (taskId) VALUES (:id)");
        int value = 3;
        creating.bindValue(":id", value);
        creating.exec();

        qDebug() << database;
        qDebug() << database.record(database.tables().at(0));

        //reading
        QSqlQuery reading;
        if(reading.exec("SELECT * FROM tasks"))
        {
            while(reading.next())
            {
               //qDebug() << database.record(database.tables().at(0));
            }
        }
        else
            qDebug() << "Error =" << database.lastError().text();

        qDebug() << "Closed!";
        if(database.isOpen())
            database.close();
    }
    else
    {
        qDebug() << "Error =" << database.lastError().text();
    }

}
