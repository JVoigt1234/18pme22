///-----------------------------------------------------------------
/// Namespace:
/// Class:              DatabaseController
/// Description:
/// Author:             Kevin Kastner & Martin Bechberger
/// Date:               Oct 2018
/// Notes:              failed = -2  : No request could be sent to the database
/// 					error = -1   : Some data have been skipped
/// 					run = 1      : Connection to database running
/// 					completed = 0: All data have been downloaded from the database and are available as List<DICOMFile>
/// Revision History:   First release
///-----------------------------------------------------------------

#include "databasecontroller.h"

DatabaseController::DatabaseController(QString hostname)
{
    m_serverUrl = hostname;     //"db.inftech.hs-mannheim.de"
    m_databasename = "1814116_DiaThesis";
    m_username = "1814116";
    m_password = "#DiaThesis";

    m_database = QSqlDatabase::addDatabase("QPSQL"); //QSqlDatabase !warning! software need to be installed
    m_database.setConnectOptions();

    m_database.setDatabaseName(m_databasename);
    m_database.setHostName(m_serverUrl);
    m_database.setUserName(m_username);

    if(m_database.open(m_username,m_password))
    {
        qDebug() << "connected to" << m_serverUrl;
    }
    else
    {
        qDebug() << "Error =" << m_database.lastError().text();
    }
}

DatabaseController::~DatabaseController()
{
    if(m_database.isOpen())
        m_database.close();
}

Patient DatabaseController::convertJSON2Patient(const QJsonObject* jsonObject)
{

}

QJsonObject DatabaseController::convertUser2JSON(const User* patient)
{
    QJsonObject userObject;
    userObject.insert("patientID", QJsonValue::fromVariant(patient->getUserID()) );
    userObject.insert("patientName", QJsonValue::fromVariant(patient->getName()) );
    userObject.insert("user", QJsonValue::fromVariant(int(patient->getUserType() )));
    userObject.insert("email", QJsonValue::fromVariant(patient->getE_Mail() ));
    userObject.insert("phone", QJsonValue::fromVariant(patient->getPhone() ));

    return userObject;
}

QGeoAddress DatabaseController::convertJSONArray2Address(const QJsonArray* array)
{

}

QJsonObject DatabaseController::convertAddress2JSON(const QGeoAddress address)
{
    QJsonObject addressObject;
    addressObject.insert("street", address.street() );
    addressObject.insert("city", address.city() );
    addressObject.insert("country", address.country() );
    addressObject.insert("postalCode", address.postalCode() );

    return addressObject;
}

bool DatabaseController::isConnected() const
{
    m_database.isOpen();
}

int DatabaseController::getBooldPressure(const User* user, const QDateTime From, const QDateTime To, QList<BloodPressure> listBloodPressure) const
{
    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
    {
        return int(DatabaseController::error);
    }

//Datenbankabfrage from BETWEEN to
    listBloodPressure = m_listBloodPressure;
    return DatabaseController::sucessfull;
}

int DatabaseController::getBooldSugar(const User* user, const QDateTime From, const QDateTime To, QList<BloodSugar> listBloodSugar) const
{
    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
    {
        return int(DatabaseController::error);
    }
//Datenbankabfrage from BETWEEN to
    listBloodSugar = m_listBloodSugar;
    return int(DatabaseController::sucessfull);
}

int DatabaseController::getListPatient(const User* user, QList<Patient> listPatient) const
{
    if(user->getUserType() != UserType::admin || user->getUserType() != UserType::doctor)
    {
        return int(DatabaseController::error);
    }
    listPatient = m_listPatient;
    return int(DatabaseController::sucessfull);
}

bool DatabaseController::uploadUserData2Database(const Patient* patient)
{
    if(!m_database.isOpen())
    {
        return int(DatabaseController::failed);
    }
    if(patient == nullptr)
    {
        return int(DatabaseController::error);
    }

    QSqlQuery query("SELECT * FROM UserData");
    while (query.next()) {
        qDebug() << query.value(0).toString();
    }

    //make .json Object
    if(patient->getUserID().isNull() || patient->getUserID().trimmed().isEmpty() ||
       patient->getName().isNull() || patient->getName().trimmed().isEmpty()||
       patient->getE_Mail().isNull() || patient->getE_Mail().trimmed().isEmpty() ||
       patient->getPhone().isNull() || patient->getPhone().trimmed().isEmpty() )
    {
        return DatabaseController::error;
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(patient);
    userObject.insert("age", QJsonValue::fromVariant(patient->getAge() ));
    userObject.insert("weight", QJsonValue::fromVariant(patient->getWeight() ));
    userObject.insert("gender", QJsonValue::fromVariant(int(patient->getGender() )));
    userObject.insert("targetBloodSugar", QJsonValue::fromVariant(patient->getTargetBloodSugar() ));
    userObject.insert("bloodSugarRange", QJsonValue::fromVariant(QString::number(patient->getMinBloodSugar() ) + "/" + QString::number(patient->getMaxBloodSugar() ) ));
    userObject.insert("alcohol", QJsonValue::fromVariant(patient->isAlcohol() ));
    userObject.insert("cigaret", QJsonValue::fromVariant(patient->isCigaret() ));

    QJsonObject addressObject;
    addressObject = convertAddress2JSON(patient->getAddress());
    userObject.insert("adress", addressObject);

    QSqlQuery creating;

    //only make one table
    if(m_database.tables().size() < 1)
    {
        creating.exec("create table person (id int primary key, firstname varchar(20), lastname varchar(20))");         //insert all properties here
    }

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

    return true;

}

void DatabaseController::loadDataset()
{
    QString val, filename, path;
    QFile file;
    QJsonDocument jsonDoc;
    QJsonObject jsonObj;
    //QJsonValue jsonVal;
    //QDir dir("C:/Users/Kevin_Costner1814116/Documents/Studium/Master/2. Semester/Projekt Labor Medizin/18pme22/app/app/DiaThesis/TestDaten");
    path = "C:/Users/Kevin_Costner1814116/Documents/Studium/Master/2. Semester/Projekt Labor Medizin/18pme22/app/app/DiaThesis/TestDaten";
    filename = "C:/Users/Kevin_Costner1814116/Documents/Studium/Master/2. Semester/Projekt Labor Medizin/18pme22/app/app/DiaThesis/TestDaten/Patient.json";
    QDirIterator it(path, QStringList() << ".json", QDir::Files, QDirIterator::Subdirectories);
    do
    {
        //filename = it.next();
        file.setFileName(filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();
        jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        jsonObj = jsonDoc.object();
        //jsonVal = jsonObj["patientID"];
        Patient patient(jsonDoc["patientID"].toString(), jsonDoc["patientName"].toString(), static_cast<UserType>(jsonDoc["user"].toInt()),
                        jsonDoc["email"].toString(), jsonDoc["phone"].toString());
        QGeoAddress g;
        g.setCity("Mannheim");
        g.setCountry("Germany");
        patient.setAddress(g);

        this->uploadUserData2Database(&patient);

    }while (it.hasNext());


}
