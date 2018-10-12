///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              failed = -2  : No request could be sent to the database
/// 					error = -1   : Some data have been skipped
/// 					run = 1      : Connection to database running
/// 					completed = 0: All data have been downloaded from the database and are available as List<DICOMFile>
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

#include "Scripts/jsontyps.h"

class DatabaseController : public QThread
{
private:
    enum { failed = -2, error, completed, running} m_status;
    //attribute
    QString m_serverUrl;
    QNetworkAccessManager networkManager;
    QList<UserData> m_listPatient;
    QList<BloodSugar> m_listBloodSugar;

    void parseJSONListBloodSugar(QNetworkReply* reply);
    void replyError(QNetworkReply::NetworkError error);
    void sendHTTPRequest(QString subUrl, QNetworkAccessManager* networkAccessManager);

    //funtcion
public slots:
    void parseJSONListPatient(QNetworkReply* reply);



protected:
    void run() override;

public:
    //DatabaseController();
    DatabaseController(QString serverURL);
    ~DatabaseController();
    int getBooldSugar(QList<BloodSugar> listBloodSugar) const;
    int getListPatient(QList<UserData> listPatient) const;
    void request4ListPatient();
    void request4ListBloodSugar(QDate startDate, QDate endDate);
};

#endif // DATABASECONTROLLER_H
