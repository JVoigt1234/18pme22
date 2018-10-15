///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              failed = -2  : No request could be sent to the database
/// 					error = -1   : Some data have been skipped
/// 					sucessfull = 0: All data have been downloaded from the database and are available as List<DICOMFile>
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include <QThread>
#include <QString>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonDocument>
#include <QtSql>

//only once
#include <QDirIterator>

#include "Scripts/Databasetyps.h"

class DatabaseController : public QThread
{
private:
    enum { failed = -2, error, sucessfull};
    //attribute
    QSqlDatabase m_database;
    QString m_serverUrl;    //Hostname;
    QString m_databasename;
    QString m_username;
    QString m_password;

    QNetworkAccessManager networkManager;
    QList<Patient> m_listPatient;
    QList<BloodSugar> m_listBloodSugar;
    QList<BloodPressure> m_listBloodPressure;

    Patient convertJSON2Patient(const QJsonObject* jsonObject);
    Doctor convertJSON2Doctor(const QJsonObject* jsonObject);
    Member convertJSON2Member(const QJsonObject* jsconObject);

    QJsonObject convertUser2JSON(const User* patient);
    QJsonObject convertAddress2JSON(const QGeoAddress address);
    QGeoAddress convertJSONArray2Address(const QJsonArray* array);

public:
    //DatabaseController();
    DatabaseController(QString serverURL);
    ~DatabaseController();
    bool isConnected() const;
    int getBooldPressure(const User* user, const QDateTime From, const QDateTime To, QList<BloodPressure> listBloodPressure) const;
    int getBooldSugar(const User* user, const QDateTime From, const QDateTime To, QList<BloodSugar> listBloodSugar) const;
    int getListPatient(const User* user, QList<Patient> listPatient) const;
    bool uploadUserData2Database(const Patient* user);

    void loadDataset();
};



#endif // DATABASECONTROLLER_H
