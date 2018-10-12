///-----------------------------------------------------------------
/// Namespace:
/// Class:              SqlDatabase
/// Description:
/// Author:             Martin Bechberger
/// Date:               Oct 2018
/// Notes:      		error = -1   : error occours
/// 					completed = 0: function was sucessfull
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef SQLDATABASE_H
#define SQLDATABASE_H

#include <QThread>
#include <QtSql>
#include <QtDebug>
#include <QDate>

#include "Scripts/jsontyps.h"

class SqlDatabase
{
public:

    SqlDatabase();
    ~SqlDatabase();

    int startDatabase();

    //only after setup
    int setPerson(UserData person);
    //while login
    int getPerson(UserData* person);

    //for the doctor list
    int getPersonList(UserData* person);
    int getPersonCount();

    //for graph
    int getData(int personId, BloodSugar* values, QDate startTime, QDate endTime);

private:

    QSqlDatabase m_database;

    QString m_hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;

    int m_personCount;

};

#endif // SQLDATABASE_H
