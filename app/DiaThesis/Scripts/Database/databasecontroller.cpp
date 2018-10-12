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

#include "databasecontroller.h"

//private part
void DatabaseController::parseJSONListBloodSugar(QNetworkReply* reply)
{

    QStringList propertyNames;
    QStringList propertyKeys;

    QString strReply = reply->readAll();
    qDebug() << strReply;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
    qDebug() << jsonDoc;

    delete reply;
}

void DatabaseController::parseJSONListPatient(QNetworkReply* reply)
{
    QStringList propertyNames;
    QStringList propertyKeys;

    QString strReply = reply->readAll();
    qDebug() << strReply;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(strReply.toUtf8());
    qDebug() << jsonDoc;

    delete reply;
}

void DatabaseController::replyError(QNetworkReply::NetworkError error)
{
    qDebug() << error;
}

void DatabaseController::sendHTTPRequest(QString subUrl, QNetworkAccessManager* networkAccessManager)
{
    m_status = DatabaseController::running;
    QNetworkRequest request;
    request.setUrl(QUrl(m_serverUrl + subUrl));
    //request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    networkAccessManager->get(request);
    //connect(networkAccessManager, SIGNAL(sslErrors()), this, SLOT(replyError(QNetworkReply::NetworkError)));  falsches Signal
}

//protected part

void DatabaseController::run()
{
    forever{
        request4ListPatient();
        exec();
    }

}

//public part
DatabaseController::DatabaseController(QString serverURL)
{
    m_serverUrl = serverURL;
}

DatabaseController::~DatabaseController()
{

}

int DatabaseController::getBooldSugar(QList<BloodSugar> listBloodSugar) const
{
    listBloodSugar = m_listBloodSugar;
    return  int(m_status);
}

int DatabaseController::getListPatient(QList<UserData> listPatient) const
{
    listPatient = m_listPatient;
    return int(m_status);
}

void DatabaseController::request4ListPatient()
{
    QString listPatient = "/diathesis/_design/patient/_view/listPatient";
    //disconnect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseJSONListBloodSugar(QNetworkReply*)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseJSONListPatient(QNetworkReply*)));
    sendHTTPRequest(listPatient, &networkManager);
}
void DatabaseController::request4ListBloodSugar(QDate startDate, QDate endDate)
{
    QString listPatient = "/diathesis/_design/patient/_view/listPatient";
    disconnect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseJSONListPatient(QNetworkReply*)));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseJSONListBloodSugar(QNetworkReply*)));
    sendHTTPRequest(listPatient, &networkManager);
}
