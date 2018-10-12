///-----------------------------------------------------------------
/// Namespace:
/// Class:              SqlDatabase
/// Description:
/// Author:             Martin Bechberger
/// Date:               Oct 2018
/// Notes:  			error = -1   : error occours
/// 					completed = 0: Function was sucessfull
/// Revision History:   First release
///-----------------------------------------------------------------

#include "sqldatabase.h"

SqlDatabase::SqlDatabase()
{
    m_hostname = "db.inftech.hs-mannheim.de";
    m_databasename = "1813680_DiaThesis";

    m_username = "1813680";
    m_password = "DiaThesis2018";

    m_database = QSqlDatabase::addDatabase("QPSQL"); //QSqlDatabase !warning! software need to be installed
    m_database.setConnectOptions();

    m_database.setDatabaseName(m_databasename);
    m_database.setHostName(m_hostname);

}

SqlDatabase::~SqlDatabase()
{
    if(m_database.isOpen())
        m_database.close();
}

int SqlDatabase::startDatabase()
{
    if(m_database.open(m_username,m_password))
    {
        qDebug() << "connected to" << m_hostname;
        return 0;
    }
    else
    {
        qDebug() << "Error =" << m_database.lastError().text();
        return -1;
    }

}

int SqlDatabase::setPerson(UserData person)
{
    if(m_database.isOpen())
    {
        QSqlQuery creating;

        //only make one table
        if(m_database.tables().size() < 1)
            creating.exec("create table person (id int primary key, firstname varchar(20), lastname varchar(20))");         //insert all properties here

        //creating a formular
        creating.prepare("INSERT INTO person (id,firstname,lastname)"
                         "VALUES (:id, :firstname, :lastname)");

        //insert person data here!
        creating.bindValue(0, 1);
        creating.bindValue(1, "Martin");
        creating.bindValue(2, "Bechberger");

        creating.exec();

        creating.prepare("INSERT INTO person (id,firstname,lastname)"
                         "VALUES (:id, :firstname, :lastname)");

        //insert person data here!
        creating.bindValue(0, 2);
        creating.bindValue(1, "Martin");
        creating.bindValue(2, "Bechberger");

        creating.exec();

        qDebug() << m_database;
        qDebug() << m_database.record(m_database.tables().at(0));

        return 0;
    }

    return -1;
}

//idea: calling after login -> searching person in table and returning all UserData
int SqlDatabase::getPerson(UserData* person)
{
    if(m_database.isOpen())
    {
        QSqlQuery reading;
        if(reading.exec("SELECT * FROM tasks"))
        {
            while(reading.next())
            {
               //search here :)
               //(maybe other Query exec!)
            }
            return 0;
        }
        else
            qDebug() << "Error =" << m_database.lastError().text();
    }

    return -1;
}

int SqlDatabase::getPersonCount()
{
    if(m_database.isOpen())
    {
        m_personCount = 0;
        QSqlQuery reading;
        if(reading.exec("SELECT * FROM person"))
        {
            while(reading.next())
            {
               m_personCount++;
            }
            return m_personCount;
        }
        else
            qDebug() << "Error =" << m_database.lastError().text();
    }

    return -1;
}

int SqlDatabase::getData(int personId, BloodSugar* values, QDate startTime, QDate endTime)
{
    if(m_database.isOpen())
    {
        QSqlQuery reading;
        if(reading.exec("SELECT * FROM tasks"))
        {
            while(reading.next())
            {
                //search with personId (maybe other Query exec!)
            }
            return 0;
        }
        else
            qDebug() << "Error =" << m_database.lastError().text();
    }

    return -1;
}

int SqlDatabase::getPersonList(UserData* person)
{
    if(m_database.isOpen())
    {
        QSqlQuery reading;
        if(reading.exec("SELECT * FROM tasks"))
        {
            while(reading.next())
            {

            }
            return 0;
        }
        else
            qDebug() << "Error =" << m_database.lastError().text();
    }

    return -1;
}
