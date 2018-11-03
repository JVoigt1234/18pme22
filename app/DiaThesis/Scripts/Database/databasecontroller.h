///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:        Sends queries to an active PostgreSQL database, it is possible
///                     to create a new user or check if it is valid or available.
///                     You can also edit and/or delete users or measurements from the database.
/// Author:             Kevin Kastner & Martin Bechberger
/// Date:               Oct 2018
/// Notes:              throws Exception: InvalidDateTimeFormate, InvalidUser,
///                     UserNotFound and SqlError
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QThread>
#include <QString>
#include <QList>

#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QtSql>
#include <QCryptographicHash>

//only once
#include <QDirIterator>
#include <QDebug>

#include "Scripts/Database/Databasetyps.h"

class DatabaseController : public QThread
{
private:
    //attribute
    QSqlDatabase m_database;
    QString m_hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;

    QString m_userID;
    UserType m_userType;

    QByteArray crypt (const QByteArray text, const QByteArray key);
    bool isUserOK(const User* user);
    Patient patientData(QString patientID);

    //conversion features
    QGeoAddress convertJSON2Address(const QJsonObject jsonArray);
    QJsonObject convertUser2JSON(const User* patient);
    QJsonObject convertAddress2JSON(const QGeoAddress address);

public:
    DatabaseController(QString hostname);
    ~DatabaseController();

    //Database
    bool isConnected() const;
    bool isUserAvailable(const QString eMail);
    bool isUserCreated(User* user, QString password);
    bool isUserDeleted(User* user, QString password);
    bool isIDAuthorized(const QString patientID, const QString foreignID);
    UserType isValidUser(QString eMail, QString password);

    //for other user types
    bool getBloodPressure(const QDateTime timestamp, Measurement& measurement);
    bool getBloodPressure(const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements);
    bool getBloodPressure(const QString patientID, const QDateTime timestamp, Measurement& measurement);
    bool getBloodPressure(const QString patientID, const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements);

    bool getBloodSugar(const QDateTime timestamp, Measurement& measurement);
    bool getBloodSugar(const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements);
    bool getBloodSugar(const QString patientID, const QDateTime timestamp, Measurement& measurement);
    bool getBloodSugar(const QString patientID, const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements);

    //only for doctor
    bool getListPatient(QList<Patient>& listPatient);

    //only for patient
    bool allowAccess(const QString foreignID);
    bool denyAccess(const QString foreignID);
    QString getFact(void);

    //getter functions for users
    Doctor getDoctorData(void);
    Doctor getDoctorData(const QString doctorID);
    Patient getPatientData(void);
    //Patient getPatientData(QString foreignID);
    Member getMemberData(void);
    Member getMemberData(const QString memberID);

    //upload functions
    bool updateUser(const Doctor* user);
    bool updateUser(const Patient* user);
    bool updateUser(const Member* user);

    //upload functions (write new data to the database)
    bool uploadData(const Measurement& measurement);
    bool uploadData(const QList<Measurement>& listOfMeasurements);

    //delete measurements
    bool deleteBloodPressureData(const QDateTime timeStamp);
    bool deleteBloodPressureData(const QDateTime from, const QDateTime to);

    bool deleteBloodSugarData(const QDateTime timeStamp);
    bool deleteBloodSugarData(const QDateTime from, const QDateTime to);

    void loadDataset(QList<Patient>& list);
    void loadDataset(QList<Doctor>& list);
    void loadDataset(QList<Member>& list);
    void loadDataset(QList<Measurement>& list, MeasurementType type);
    void loadDateset(void);

    bool creatDatabase();
    bool deleteDatabase();
};

#endif // DATABASECONTROLLER_H
