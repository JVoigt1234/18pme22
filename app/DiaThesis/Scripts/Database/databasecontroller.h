///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              failed = -2       :  No request could be sent to the database
/// 					error = -1        :  Something went wrong with the SQL query.
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

///failed: 0, sucessfull: 1
class DatabaseController : public QThread
{
private:
    //attribute
    QSqlDatabase m_database;
    QString m_hostname;    //Hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;

    QList<Patient> patient;

    QByteArray crypt (const QByteArray text, const QByteArray key);
    bool isUserOK(const User* user);
    bool isUserAvailable(const User* user);

    //conversion features
    QGeoAddress convertJSONArray2Address(const QJsonArray jsonArray);

    QJsonObject convertUser2JSON(const User* patient);
    QJsonObject convertAddress2JSON(const QGeoAddress address);

public:
    DatabaseController(QString hostname);
    ~DatabaseController();

    enum { failed = 0, sucessfull,  error};
    //Database
    bool isConnected() const;
    UserType isValidUser(QString userID, QString password);
    bool isUserCreated(User* user, QString password);
    bool isUserDeleted(User* user, QString password);

    //for every user
    int getBloodPressure(const QString userID, const QDateTime From, const QDateTime To, QList<BloodPressure> listBloodPressure) const;
    int getBloodSugar(const QString userID, const QDateTime From, const QDateTime To, QList<BloodSugar> listBloodSugar) const;

    //only for doctor
    int getListPatient(QList<Patient>& listPatient) const;

    //getter functions for users
    Doctor getDoctorData(const QString userID);
    Patient getPatientData(const QString userID);
    Member getMemberData(const QString userID);

    //upload functions
    bool updateUser(const Doctor* user);
    bool updateUser(const Patient* user);
    bool updateUser(const Member* user);

    bool uploadData(const BloodPressure* bloodPressure);
    bool uploadData(const QList<BloodPressure>& listBloodPressure);
    bool uploadData(const BloodSugar* bloodSugar);
    bool uploadData(const QList<BloodSugar>& listBloodSugar);

    bool deleteBloodPressureData(const User* user, const QDateTime timeStemp);
    bool deleteBloodPressureData(const User* user, const QDateTime from, const QDateTime to);
    bool deleteBloodSugarData(const User* user, const QDateTime timeStemp);
    bool deleteBloodSugarData(const User* user, const QDateTime from, const QDateTime to);

    void loadDataset(QList<Patient>& list, QString path);
    bool creatDatabase();
    bool deleteDatabase();
};

#endif // DATABASECONTROLLER_H
