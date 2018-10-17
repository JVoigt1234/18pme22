///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              failed = -3       :  No request could be sent to the database
///                     disconnected = -2 :  No active connection to the database
/// 					error = -1        :  Something went wrong with the SQL query.
///                     connected = 0     :  Active connection to the database
/// 					sucessfull = 1    :  Successful database query
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QThread>
#include <QString>
#include <QList>

#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtSql>
#include <QCryptographicHash>

//only once
#include <QDirIterator>

#include "Scripts/Databasetyps.h"

///failed: -3, disconnected: -2, error: -1, connected: 0, sucessfull: 1
class DatabaseController : public QThread
{
private:
    enum { failed = -3, disconnected, error, connected, sucessfull} m_currentState;
    //attribute
    QSqlDatabase m_database;
    QString m_serverUrl;    //Hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;
    QSqlQuery m_query;

    QList<Patient> m_listPatient;
    QList<BloodSugar> m_listBloodSugar;
    QList<BloodPressure> m_listBloodPressure;

    QByteArray crypt (const QByteArray text, const QByteArray key);
    bool isUserOK(const User* user);
    bool isUserAvailable(const User* user);

    //conversion features
    QGeoAddress convertJSONArray2Address(const QJsonArray jsonArray);

    QJsonObject convertUser2JSON(const User* patient);
    QJsonObject convertAddress2JSON(const QGeoAddress address);

public:
    DatabaseController(QString serverURL);
    ~DatabaseController();

    //Database
    bool isConnected() const;
    UserType isValidUser(QString userID, QString password);
    bool creatUser(User* user, QString password);

    //for every user
    int getBooldPressure(const QString userID, const QDateTime From, const QDateTime To, QList<BloodPressure> listBloodPressure) const;
    int getBooldSugar(const QString userID, const QDateTime From, const QDateTime To, QList<BloodSugar> listBloodSugar) const;

    //only for doctor
    int getListPatient(QList<Patient> listPatient) const;

    //getter functions for users
    Doctor getDoctorData(const QString userID);
    Patient getPatientData(const QString userID);
    Member getMemberData(const QString userID);

    //upload functions
    bool uploadData(const Doctor* user);
    bool uploadData(const Patient* user);
    bool uploadData(const Member* user);


    Patient loadDataset();
};

#endif // DATABASECONTROLLER_H
