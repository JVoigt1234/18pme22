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

    m_hostname = hostname;     //"db.inftech.hs-mannheim.de"
    m_databasename = "1814116_DiaThesis";
    m_username = "1814116";
    m_password = "#DiaThesis";

    m_database = QSqlDatabase::addDatabase("QPSQL"); //QSqlDatabase !warning! software need to be installed
    m_database.setConnectOptions();

    m_database.setDatabaseName(m_databasename);
    m_database.setHostName(m_hostname);
    m_database.setUserName(m_username);

    if(m_database.open(m_username,m_password))
    {
        qDebug() << "connected to" << m_hostname;
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
       user->geteMail().isNull() || user->geteMail().trimmed().isEmpty() )
    {
        throw InvalidUser("Some data are missing.");
    }

    return true;
}

bool DatabaseController::isUserAvailable(const User* user)
{
    QByteArray id = QCryptographicHash::hash( QString(user->getUserID() + QString(user->getUserID().length()) ).toUtf8(), QCryptographicHash::Md5).toHex();

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
    userObject.insert("email", QJsonValue::fromVariant(patient->geteMail() ));
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

    QByteArray id = QCryptographicHash::hash( QString(userID + QString(userID.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray pw = QCryptographicHash::hash( QString(password + QString(id)).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT userid,password,usertyp"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND password = '" + QString(pw) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return UserType::inValidUser;
    }

    query.first();      //i need this to select the first one

    if(QByteArray::fromHex(query.value(0).toByteArray()) == id && QByteArray::fromHex(query.value(1).toByteArray()) == pw )
    {
        return UserType::inValidUser;
    }

    QByteArray usertyp = QByteArray::fromHex(query.value(2).toByteArray());

    if(usertyp.isEmpty())
    {
        return UserType::inValidUser;
    }

    usertyp = crypt(usertyp, pw);
    return static_cast<UserType>(usertyp.toInt());
}

bool DatabaseController::isUserCreated(User* user, QString password)
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
    QByteArray userByteArray = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();

    //password
    QByteArray passwordByteArray = QCryptographicHash::hash( QString(password + QString(userByteArray)).toUtf8(), QCryptographicHash::Md5).toHex();

    //data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();
    jsonByteArray = crypt(jsonByteArray, passwordByteArray);

    //userTyp
    QByteArray usertyp;
    usertyp.setNum(int(user->getUserType()));
    usertyp = crypt(usertyp, passwordByteArray ).toHex();

    creating.bindValue(0, QString(userByteArray) );
    creating.bindValue(1, jsonByteArray.toHex());
    creating.bindValue(2, QString(passwordByteArray) );
    creating.bindValue(3, QString(usertyp) );

    creating.exec();

    if(creating.lastError().type() == QSqlError::NoError)
    {
        return true;
    }
    else
    {
        return false;
    }

}

bool DatabaseController::isUserDeleted(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    try {
        if(isUserOK(user) == false || isUserAvailable(user) == false || password.isNull() || password.trimmed().isEmpty())
        {
            return false;
        }
    } catch (InvalidUser e) {
        qDebug() << e.getMessage();
        return false;
    }

    QByteArray id = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();
    QByteArray pw = QCryptographicHash::hash( QString(password + QString(id)).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery creating;
    creating.exec("DELETE FROM userdata"
                " WHERE userid = '" + QString(id) + "'"
                " AND password = '" + QString(pw) + "'" );


    if(creating.lastError().type() == QSqlError::NoError)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int DatabaseController::getBloodPressure(const QString userID_patient, const QDateTime from, const QDateTime to, QList<BloodPressure> listBloodPressure) const
{
//    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
//    {
//        return int(DatabaseController::error);
//    }

//Datenbankabfrage from BETWEEN to
    return DatabaseController::sucessfull;
}

int DatabaseController::getBloodSugar(const QString userID_patient, const QDateTime from, const QDateTime to, QList<BloodSugar> listBloodSugar) const
{
//    if(user->getUserType() == UserType::doctor || user->getUserType() == UserType::member)
//    {
//        return int(DatabaseController::error);
//    }
//Datenbankabfrage from BETWEEN to
    return int(DatabaseController::sucessfull);
}

int DatabaseController::getListPatient(QList<Patient> listPatient) const
{
//    if(user->getUserType() != UserType::admin || user->getUserType() != UserType::doctor)
//    {
//        m_currentState = DatabaseController::error;
//        throw InvalidUser("Invalid User.");
//    }

    //m_currentState = DatabaseController::sucessfull;
    return int(m_currentState);
}

Doctor DatabaseController::getDoctorData(const QString userID)
{
}

///throws InvalidUser, UserNotFound Exception
Patient DatabaseController::getPatientData(const QString userID)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(userID.isNull() == true || userID.trimmed().isEmpty() == true)
    {
        throw InvalidUser("Incorrect userID");
    }

    QByteArray id = QCryptographicHash::hash(QString(userID + QString(userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT password,data,usertyp"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + userID + " not found.").toLocal8Bit().data());
    }
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());

    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QJsonArray array = jsonObject["address"].toArray();
    QGeoAddress address = convertJSONArray2Address(array);

    QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);
    QStringList bs = jsonObject["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);

    QByteArray usertyp = QByteArray::fromHex(query.value(2).toByteArray());

    if(usertyp.isEmpty())
    {
        throw InvalidUser("No UserTyp.");
    }

    usertyp = crypt(usertyp, query.value(0).toByteArray());
    if(name.length() != 2)
    {
        throw InvalidUser("Not enough parameters for name");
    }

    UserType typ = static_cast<UserType>(usertyp.toInt());

    if(bs.isEmpty()){
        return Patient(name[0], name[1], typ, jsonObject["email"].toString());
    }

    if(bs.length() == 2)
    {

        return Patient(name[0], name[1], typ, jsonObject["email"].toString(), jsonObject["age"].toInt(), jsonObject["weight"].toDouble(),
                  jsonObject["bodysize"].toDouble(), static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(),
                   bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address);
    }

}

Member DatabaseController::getMemberData(const QString userID)
{

}

bool DatabaseController::updateUser(const Doctor* user)
{

}

///throws InvaildUser exception
bool DatabaseController::updateUser(const Patient* user)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user) == false || user->getUserType() != UserType::patient)
    {
        throw InvalidUser("Invaild User.");
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
    QByteArray id = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();

    //Data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();

    QSqlQuery query("SELECT password"
                    " FROM userdata"
                    " WHERE userid = '" + QString(id) + "'" );

    if(creating.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.first();

    jsonByteArray = crypt(jsonByteArray, query.value(0).toByteArray());

    creating.exec("UPDATE userdata"
                  " SET data = '" + jsonByteArray.toHex() + "'" +
                  " WHERE userid = '" + QString(id) + "'" );

    if(creating.lastError().type() == QSqlError::NoError)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool DatabaseController::updateUser(const Member* user)
{

}

bool DatabaseController::uploadData(const BloodPressure* bloodPressure)
{

}

bool DatabaseController::uploadData(const QList<BloodPressure>* listBloodPressure)
{

}

bool DatabaseController::uploadData(const BloodSugar* bloodSugar)
{

}

bool DatabaseController::uploadData(const QList<BloodSugar>* listBloodSugar)
{

}

bool DatabaseController::deleteBloodPressureData(const User* user, const QDateTime timeStemp)
{

}

bool DatabaseController::deleteBloodPressureData(const User* user, const QDateTime from, const QDateTime to)
{
    //delete from your_table
    //where id between bottom_value and top_value;
}

bool DatabaseController::deleteBloodSugarData(const User* user, const QDateTime timeStemp)
{

}

bool DatabaseController::deleteBloodSugarData(const User* user, const QDateTime from, const QDateTime to)
{
    //delete from your_table
    //where id between bottom_value and top_value;
}

void DatabaseController::loadDataset(QList<Patient>& list, QString path)
{
    QString val;
    QFile file;
    QJsonDocument jsonDoc;
    QJsonObject jsonObject;

    QDir directory(path);
    QStringList data = directory.entryList(QStringList() << "*.json",QDir::Files);

    foreach(QString filename, data) {
        file.setFileName(path + filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        jsonObject = jsonDoc.object();
        //jsonVal = jsonObj["patientID"];
        QStringList name = jsonDoc["patientName"].toString().split('/', QString::SkipEmptyParts);
        QStringList bs = jsonDoc["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);

        QGeoAddress address;
        address.setStreet("Mainstreet");
        address.setPostalCode("68219");
        address.setCity("Mannheim");
        address.setCountry("Germany");

         patient.append( Patient(name[0], name[1], UserType::patient, jsonObject["email"].toString(), jsonObject["age"].toInt(), jsonObject["weight"].toDouble(), jsonObject["bodysize"].toDouble(),
                Gender::other, jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(), bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address) );
    }

    list = patient;
}
