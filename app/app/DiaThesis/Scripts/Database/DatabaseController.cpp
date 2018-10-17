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

#include "DatabaseController.h"

DatabaseController::DatabaseController(QString hostname)
{
     //   m_user = User("hans", "otto", "1234", UserType::member, "hallo@ge.de", "123499");
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

QByteArray DatabaseController::crypt (const QByteArray text, const QByteArray key)
{
  QByteArray crypted(text);

  for(uint i = 0; i < uint(crypted.length()); i++)
  {
    crypted[i] = text[i] ^ key[i % uint(key.length() )];
  }

  return crypted;
}

///throw Exception InvalidUser()
bool DatabaseController::isUserOK(const User* user)
{
    if(user == nullptr)
    {
        return false;
    }

    if(user->getUserID().isNull() || user->getUserID().trimmed().isEmpty() ||
       user->getForename().isNull() || user->getForename().trimmed().isEmpty()||
       user->getSurname().isNull() || user->getSurname().trimmed().isEmpty()||
       user->getE_Mail().isNull() || user->getE_Mail().trimmed().isEmpty() )
    {
        throw InvalidUser("Some data are missing.");
    }

    return true;
}

bool DatabaseController::isUserAvailable(const User* user)
{
    QByteArray id = QCryptographicHash::hash(user->getUserID().toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT userid"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() == QSqlError::NoError && query.size() == 1)
    {
        return true;
    }

    return false;
}

QGeoAddress DatabaseController::convertJSONArray2Address(const QJsonArray jsonArray)
{
    QGeoAddress address;
    foreach (const QJsonValue &value, jsonArray)
    {
        QJsonObject obj = value.toObject();
        address.setStreet(obj["street"].toString());
        address.setCity(obj["city"].toString());
        address.setCountry(obj["country"].toString());
        address.setPostalCode(obj["postalCode"].toString());
    }

    return address;
}

QJsonObject DatabaseController::convertUser2JSON(const User* patient)
{
    QJsonObject userObject;

    userObject.insert("patientName", QJsonValue::fromVariant(patient->getForename() + "/" + patient->getSurname()) );
    userObject.insert("email", QJsonValue::fromVariant(patient->getE_Mail() ));
    userObject.insert("phone", QJsonValue::fromVariant(patient->getPhone() ));

    return userObject;
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

UserType DatabaseController::isValidUser(QString userID, QString password)
{
    if(userID.isNull() || userID.trimmed().isEmpty() || password.isNull() || password.trimmed().isEmpty() )
    {
        return UserType::inValidUser;
    }

    QByteArray id = QCryptographicHash::hash(QString(userID).toUtf8(),QCryptographicHash::Md5).toHex();
    QByteArray pw = QCryptographicHash::hash(QString(password + userID).toUtf8(),QCryptographicHash::Md5).toHex();
    QByteArray usertyp;

    QSqlQuery query("SELECT userid,password,usertyp"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND password = '" + QString(pw) + "'" );

    if(query.lastError().type() != QSqlError::NoError && query.size() != 1)
    {
        return UserType::inValidUser;
    }

    query.first();      //i need this to select the first one

    if(QByteArray::fromHex(query.value(0).toByteArray()) == id && QByteArray::fromHex(query.value(1).toByteArray()) == pw )
    {
        return UserType::inValidUser;
    }

    usertyp = QByteArray::fromHex(query.value(2).toByteArray());

    if(usertyp.isEmpty())
    {
        return UserType::inValidUser;
    }

    usertyp = crypt(usertyp, pw);
    return static_cast<UserType>(usertyp.toInt());
}

bool DatabaseController::creatUser(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    try {
        if(isUserAvailable(user) == true || isUserOK(user) == false || password.isNull() || password.trimmed().isEmpty())
        {
            return false;
        }
    } catch (InvalidUser e) {
        qDebug() << e.getMessage();
        return false;
    }

    QSqlQuery creating;

    //creating a formular
    creating.prepare("INSERT INTO userdata(userid, data, password, usertyp) "
                     "VALUES (:userid, :data, :password, :usertyp)" );

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    //user
    QByteArray userByteArray = QCryptographicHash::hash(QString(user->getUserID() ).toUtf8(),QCryptographicHash::Md5).toHex();

    //data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData().toHex();

    //password
    QByteArray passwordByteArray = QCryptographicHash::hash(QString(password + user->getUserID()).toUtf8(),QCryptographicHash::Md5).toHex();

    //userTyp
    QByteArray usertyp;
    usertyp.setNum(int(user->getUserType()));
    usertyp = crypt(usertyp, passwordByteArray).toHex();

    creating.bindValue(0, QString(userByteArray) );
    creating.bindValue(1, jsonByteArray);
    creating.bindValue(2, QString(passwordByteArray) );
    creating.bindValue(3, QString(usertyp) );

    qDebug() << creating.size();
    creating.exec();

    qDebug() << m_database;
    qDebug() << m_database.record(m_database.tables().at(0));
    qDebug() << creating.lastError();
}

int DatabaseController::getBooldPressure(const QString userID_patient, const QDateTime From, const QDateTime To, QList<BloodPressure> listBloodPressure) const
{
//    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
//    {
//        return int(DatabaseController::error);
//    }

//Datenbankabfrage from BETWEEN to
    listBloodPressure = m_listBloodPressure;
    return DatabaseController::sucessfull;
}

int DatabaseController::getBooldSugar(const QString userID_patient, const QDateTime From, const QDateTime To, QList<BloodSugar> listBloodSugar) const
{
//    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
//    {
//        return int(DatabaseController::error);
//    }
//Datenbankabfrage from BETWEEN to
    listBloodSugar = m_listBloodSugar;
    return int(DatabaseController::sucessfull);
}

int DatabaseController::getListPatient(QList<Patient> listPatient) const
{
//    if(user->getUserType() != UserType::admin || user->getUserType() != UserType::doctor)
//    {
//        m_currentState = DatabaseController::error;
//        throw InvalidUser("Invalid User.");
//    }

    listPatient = m_listPatient;
    //m_currentState = DatabaseController::sucessfull;
    return int(m_currentState);
}

Doctor DatabaseController::getDoctorData(const QString userID)
{
}

///throws InvalidUser, UserNotFound Exception
Patient DatabaseController::getPatientData(const QString userID)
{
    if(!m_database.isOpen() || userID.isNull() == true || userID.trimmed().isEmpty() == true)
    {
        throw InvalidUser("Incorrect userID");
    }

    QByteArray id = QCryptographicHash::hash(userID.toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT data,password,usertyp"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() != QSqlError::NoError && query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + userID + " not found.").toLocal8Bit().data());
    }
    query.first();

    QJsonObject jsonObject = QJsonDocument::fromBinaryData(QByteArray::fromHex( query.value(0).toByteArray()) ).object();
    QJsonArray array = jsonObject["address"].toArray();
    QGeoAddress address = convertJSONArray2Address(array);

    QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);
    QStringList bs = jsonObject["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);

    QByteArray usertyp = QByteArray::fromHex(query.value(1).toByteArray());

    if(usertyp.isEmpty())
    {
        throw InvalidUser("No UserTyp.");
    }

    usertyp = crypt(usertyp, QByteArray::fromHex(query.value(2).toByteArray()));
    if(name.length() != 2 || bs.length() != 2)
    {
        throw InvalidUser("Not enough parameters for name");
    }

    return Patient(name[0], name[1], static_cast<UserType>(usertyp.toInt()), jsonObject["email"].toString(), jsonObject["age"].toInt(), jsonObject["weight"].toDouble(),
                  jsonObject["bodysize"].toDouble(), static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(),
                   bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address);

}

Member DatabaseController::getMemberData(const QString userID)
{

}

bool DatabaseController::uploadData(const Doctor* user)
{

}

///throws InvaildUser exception
bool DatabaseController::uploadData(const Patient* user)
{
    if(!m_database.isOpen() || isUserOK(user) == false || isUserAvailable(user) == false)
    {
        return int(DatabaseController::failed);
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    if(userObject.isEmpty())
    {
        return  DatabaseController::error;
    }

    userObject.insert("age", QJsonValue::fromVariant(user->getAge() ));
    userObject.insert("weight", QJsonValue::fromVariant(user->getWeight() ));
    userObject.insert("bodysize", QJsonValue::fromVariant(user->getBodysize()));
    userObject.insert("gender", QJsonValue::fromVariant(int(user->getGender() )));
    userObject.insert("targetBloodSugar", QJsonValue::fromVariant(user->getTargetBloodSugar() ));
    userObject.insert("bloodSugarRange", QJsonValue::fromVariant(QString::number(user->getMinBloodSugar() ) + "/" + QString::number(user->getMaxBloodSugar() ) ));
    userObject.insert("alcohol", QJsonValue::fromVariant(user->isAlcohol() ));
    userObject.insert("cigaret", QJsonValue::fromVariant(user->isCigaret() ));

    QJsonObject addressObject;
    addressObject = convertAddress2JSON(user->getAddress());
    userObject.insert("adress", addressObject);

    QSqlQuery creating;
    QByteArray id = QCryptographicHash::hash(user->getUserID().toUtf8(),QCryptographicHash::Md5).toHex();

//    QJsonObject temp;
//    temp.insert("age", QJsonValue::fromVariant(user->getAge() ));
    //Data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();

    creating.exec("UPDATE userdata"
                  " SET data =" + jsonByteArray +
                  "WHERE userID = " + QString(id));

    if(creating.lastError().type() != QSqlError::NoError)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DatabaseController::uploadData(const Member* user)
{

}

Patient DatabaseController::loadDataset()
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
        QStringList name = jsonDoc["patientName"].toString().split('/', QString::SkipEmptyParts);
        Patient patient(name[0], name[1], static_cast<UserType>(jsonDoc["user"].toInt()),
                        jsonDoc["email"].toString());
        QGeoAddress g;
        g.setCity("Mannheim");
        g.setCountry("Germany");
        patient.setAddress(g);

        return patient;

    }while (it.hasNext());


}
