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

#include "Scripts/Database/databasecontroller.h"

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
    {
        m_database.close();
    }
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

///checks whether the user has empty or null values
///returns true when all is well, otherwise false
///     Exception: InvalidUser()
bool DatabaseController::isUserOK(const User* user)
{
    if(user == nullptr)
    {
        return false;
    }

    if(user->getUserID().isNull() || user->getUserID().trimmed().isEmpty() ||
       user->getForename().isNull() || user->getForename().trimmed().isEmpty()||
       user->getSurname().isNull() || user->getSurname().trimmed().isEmpty()||
       user->geteMail().isNull() || user->geteMail().trimmed().isEmpty() || !user->geteMail().contains('@'))
    {
        throw InvalidUser("Some data are wrong.");
    }

    return true;
}

///Converts a given jsonArray to a QGeoAddress object
///The values used are street, city, country, and postal code
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

///Converts a user into a QJsonObject with the values patientName (Forename/Surename), e-mail and phone
QJsonObject DatabaseController::convertUser2JSON(const User* patient)
{
    QJsonObject userObject;

    userObject.insert("patientName", QJsonValue::fromVariant(patient->getForename() + "/" + patient->getSurname()) );
    userObject.insert("email", QJsonValue::fromVariant(patient->geteMail() ));
    userObject.insert("phone", QJsonValue::fromVariant(patient->getPhone() ));

    return userObject;
}

///Converts a QGeoAddress into a QJsonObject with the values street, city, country and postalCode
QJsonObject DatabaseController::convertAddress2JSON(const QGeoAddress address)
{
    QJsonObject addressObject;
    addressObject.insert("street", address.street() );
    addressObject.insert("city", address.city() );
    addressObject.insert("country", address.country() );
    addressObject.insert("postalCode", address.postalCode() );

    return addressObject;
}

///checks if the database is open and returns the result
bool DatabaseController::isConnected() const
{
    m_database.isOpen();
}

///Checks whether the user with the specified ID is stored in the database
///returns true if yes otherwise false
///     Exception: SqlError
bool DatabaseController::isUserAvailable(const QString userID)
{
    QByteArray id = QCryptographicHash::hash( QString(userID + QString(userID.length()) ).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT userid"
                    " FROM registration"
                    " WHERE userid = '" + QString(id) + "'"
                    );

    if(query.lastError().type() == QSqlError::NoError && query.size() == 1)
    {
        return true;
    }

    if(query.lastError().type() != QSqlError::NoError)
    {
        throw SqlError(query.lastError().text());
    }

    return false;
}

///checks if the databse has the given user and password
UserType DatabaseController::isValidUser(QString userID, QString password)
{
    if(userID.isNull() || userID.trimmed().isEmpty() || password.isNull() || password.trimmed().isEmpty() )
    {
        return UserType::inValidUser;
    }

    QByteArray id = QCryptographicHash::hash( QString(userID + QString(userID.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray pw = QCryptographicHash::hash( QString(password + QString(id)).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT userid,password,usertyp"
                    " FROM registration"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND password = '" + QString(pw) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return UserType::inValidUser;
    }

    query.first();      //i need this to select the first one

    QByteArray usertyp = QByteArray::fromHex(query.value(2).toByteArray());

    if(usertyp.isEmpty())
    {
        return UserType::inValidUser;
    }

    usertyp = crypt(usertyp, pw);
    return static_cast<UserType>(usertyp.toInt());

}

///create a new user on the database
bool DatabaseController::isUserCreated(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    try {
        if(isUserAvailable(user->getUserID()) == true || isUserOK(user) == false || password.isNull() || password.trimmed().isEmpty())
        {
            return false;
        }
    } catch (InvalidUser e) {
        qDebug() << e.getMessage();
        return false;
    }

    QSqlQuery creating;

    //user
    QByteArray id = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();
    //password
    QByteArray pw = QCryptographicHash::hash( QString(password + QString(id)).toUtf8(), QCryptographicHash::Md5).toHex();


    creating.prepare("INSERT INTO registration(userid,password,usertyp) "
                     "VALUES (:userid, :password, :usertyp)" );
    //userTyp
    QByteArray usertyp;
    usertyp.setNum(int(user->getUserType()));
    usertyp = crypt(usertyp, pw ).toHex();

    creating.bindValue(0, QString(id) );
    creating.bindValue(1, QString(pw) );
    creating.bindValue(2, usertyp);
    creating.exec();

    qDebug() << creating.lastError();
    if(creating.lastError().type() != QSqlError::NoError)
    {
        return false;
    }

    QString tablename;

    switch (user->getUserType()) {
        case UserType::admin:
        case UserType::doctor: tablename = "doctor";
            break;
        case UserType::patient: tablename = "patient";
            break;
        case UserType::member: tablename = "member";
            break;
        default:
            return false;
    }

    //creating a formular
    creating.prepare("INSERT INTO " + tablename + "(userid,password,data) "
                     "VALUES ("
                     "(SELECT userid from registration WHERE userid = '" + QString(id) + "' AND password = '" + QString(pw) + "'),"
                     "(SELECT password from registration WHERE userid = '" + QString(id) + "' AND password = '" + QString(pw) + "'),"
                     ":data)" );

    //data
    QJsonObject userObject;
    userObject = convertUser2JSON(user);
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();
    jsonByteArray = crypt(jsonByteArray, pw);

    creating.bindValue(0, jsonByteArray.toHex());

    creating.exec();

    if(creating.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    //if it was not possible to write user to "user"table
    creating.exec("DELETE FROM registration"
                " WHERE userid = '" + QString(id) + "'"
                " AND password = '" + QString(pw) + "'" );

    return false;
}

///delete a user from the database
bool DatabaseController::isUserDeleted(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    try {
        if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || password.isNull() || password.trimmed().isEmpty())
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
    creating.exec("DELETE FROM registration"
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

bool DatabaseController::getBloodPressure(const Patient* patientID, const QDateTime From, const QDateTime To, QList<BloodPressure>& listBloodPressure) const
{
    //Datenbankabfrage from BETWEEN to
}

bool DatabaseController::getBloodPressure(const User* userID, const QString patientID, const QDateTime From, const QDateTime To, QList<BloodPressure>& listBloodPressure) const
{

}

bool DatabaseController::getBloodSugar(const Patient* patientID, const QDateTime From, const QDateTime To, QList<BloodSugar>& listBloodSugar) const
{

}

bool DatabaseController::getBloodSugar(const User* userID, const QString patientID, const QDateTime From, const QDateTime To, QList<BloodSugar>& listBloodSugar) const
{

}

bool DatabaseController::getListPatient(QList<Patient>& listPatient) const
{
//    if(user->getUserType() != UserType::admin || user->getUserType() != UserType::doctor)
//    {
//        m_currentState = DatabaseController::error;
//        throw InvalidUser("Invalid User.");
//    }

    //m_currentState = DatabaseController::sucessfull;
    listPatient.append(Patient("jladf","kaf",UserType::admin,"jalkdöf","akjldf"));
}

Doctor DatabaseController::getDoctorData(const Doctor* doctor)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( isUserOK(doctor) == false || isUserAvailable(doctor->getUserID() ) == false || doctor->getUserType() != UserType::doctor )
    {
        throw InvalidUser("Incorrect user");
    }

    QString userID = doctor->getUserID();

    QByteArray id = QCryptographicHash::hash(QString(userID + QString(userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();


    QSqlQuery query("SELECT password,data"
                    " FROM doctor"
                    " WHERE userid = '" + QString(id) + "'" );


    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + userID + " not found.").toLocal8Bit().data());
    }
    qDebug() << query.size();
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;
    QStringList bs;

    if(jsonObject.contains("address") == true)
    {
        QJsonArray array = jsonObject["address"].toArray();
        address = convertJSONArray2Address(array);
    }
    else
    {
        throw InvalidUser("No address.");
    }

    QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);

    if(name.isEmpty() || name.size() != 2)
    {
        throw InvalidUser("patient name is wrong.");
    }

    if(jsonObject.contains("phone") )
    {
        return Doctor(name[0], name[1], UserType::doctor, jsonObject["email"].toString(), address, jsonObject["phone"].toString());
    }
    else
    {
        throw InvalidUser("No phone.");
    }

}

///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Patient DatabaseController::getPatientData(const Patient* patient)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( isUserOK(patient) == false || isUserAvailable(patient->getUserID() ) == false ||patient->getUserType() != UserType::patient )
    {
        throw InvalidUser("Incorrect user");
    }

    QString userID = patient->getUserID();

    QByteArray id = QCryptographicHash::hash(QString(userID + QString(userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();


    QSqlQuery query("SELECT password,data"
                    " FROM patient"
                    " WHERE userid = '" + QString(id) + "'" );


    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + userID + " not found.").toLocal8Bit().data());
    }
    qDebug() << query.size();
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;
    QStringList bs;

    if(jsonObject.contains("address") == true)
    {
        QJsonArray array = jsonObject["address"].toArray();
        address = convertJSONArray2Address(array);
    }

    if(jsonObject.contains("bloodSugarRange") == true)
    {
        bs = jsonObject["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);
    }

    QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);

    if(name.isEmpty() || name.size() != 2)
    {
        throw InvalidUser("patient name is wrong.");
    }

    if(jsonObject.contains("birthDate") == false)
    {
        throw InvalidUser("No birthdate.");
    }

    if(bs.isEmpty()){
        return Patient(name[0], name[1], UserType::patient, jsonObject["email"].toString(), jsonObject["birthDate"].toString() );
    }

    if(bs.length() == 2)
    {

        return Patient(name[0], name[1], UserType::patient, jsonObject["email"].toString(), jsonObject["phone"].toString() , jsonObject["birthDate"].toString(), jsonObject["age"].toInt(), jsonObject["weight"].toDouble(),
                  jsonObject["bodysize"].toDouble(), static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(),
                   bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address);
    }

}

Member DatabaseController::getMemberData(const Member* member)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( isUserOK(member) == false || isUserAvailable(member->getUserID() ) == false || member->getUserType() != UserType::member )
    {
        throw InvalidUser("Incorrect user");
    }

    QString userID = member->getUserID();

    QByteArray id = QCryptographicHash::hash(QString(userID + QString(userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();


    QSqlQuery query("SELECT password,data"
                    " FROM member"
                    " WHERE userid = '" + QString(id) + "'" );


    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + userID + " not found.").toLocal8Bit().data());
    }
    qDebug() << query.size();
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;

    if(jsonObject.contains("address") == true)
    {
        QJsonArray array = jsonObject["address"].toArray();
        address = convertJSONArray2Address(array);
    }
    else
    {
        throw InvalidUser("No address.");
    }

    QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);

    if(name.isEmpty() || name.size() != 2)
    {
        throw InvalidUser("patient name is wrong.");
    }

    QList<QString> list;
    if(jsonObject.contains("patientRelease") == true)
    {
        QJsonArray array = jsonObject["patientRelease"].toArray();
        foreach (const QJsonValue & value, array) {
            list.append(value.toString());
        }

    }

    return Member(name[0], name[1], UserType::member, jsonObject["eMail"].toString(), list, address, jsonObject["phone"].toString());
}

bool DatabaseController::updateUser(const Doctor* user)
{

}

///     Exception: InvaildUser
bool DatabaseController::updateUser(const Patient* user)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || user->getUserType() != UserType::patient ||
            user->getBirthDay().isNull() || user->getBirthDay().trimmed().isEmpty() )
    {
        throw InvalidUser("Invaild User.");
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    if(userObject.isEmpty())
    {
        return  false;
    }

    userObject.insert("age", QJsonValue::fromVariant(user->getAge() ));
    userObject.insert("weight", QJsonValue::fromVariant(user->getWeight() ));
    userObject.insert("bodysize", QJsonValue::fromVariant(user->getBodysize()));
    userObject.insert("gender", QJsonValue::fromVariant(int(user->getGender() )));
    userObject.insert("targetBloodSugar", QJsonValue::fromVariant(user->getTargetBloodSugar() ));
    userObject.insert("bloodSugarRange", QJsonValue::fromVariant(QString::number(user->getMinBloodSugar() ) + "/" + QString::number(user->getMaxBloodSugar() ) ));
    userObject.insert("alcohol", QJsonValue::fromVariant(user->isAlcohol() ));
    userObject.insert("cigaret", QJsonValue::fromVariant(user->isCigaret() ));
    userObject.insert("birthDate", QJsonValue::fromVariant(user->getBirthDay() ));

    QJsonObject addressObject;
    addressObject = convertAddress2JSON(user->getAddress());
    userObject.insert("address", addressObject);

    QSqlQuery creating;
    QByteArray id = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();

    //Data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();

    QSqlQuery query("SELECT password,data"
                    " FROM patient"
                    " WHERE userid = '" + QString(id) + "'" );

    if(creating.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.first();

    jsonByteArray = crypt(jsonByteArray, query.value(0).toByteArray());

    creating.exec("UPDATE patient"
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
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || user->getUserType() != UserType::member )
    {
        throw InvalidUser("Invaild User.");
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    if(userObject.isEmpty())
    {
        return  false;
    }

    QJsonObject addressObject;
    addressObject = convertAddress2JSON(user->getAddress());
    userObject.insert("address", addressObject);

    QJsonArray patientRelease;
    QList<QString>* p;
    user->getPatientRealease(p);
    patientRelease = QJsonArray::fromStringList(*p);
    userObject.insert("address", patientRelease);

    QSqlQuery creating;
    QByteArray id = QCryptographicHash::hash(QString(user->getUserID() + QString(user->getUserID().length())).toUtf8(),QCryptographicHash::Md5).toHex();

    //Data
    QJsonDocument doc(userObject);
    QByteArray jsonByteArray = doc.toBinaryData();

    QSqlQuery query("SELECT password,data"
                    " FROM member"
                    " WHERE userid = '" + QString(id) + "'" );

    if(creating.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.first();

    jsonByteArray = crypt(jsonByteArray, query.value(0).toByteArray());

    creating.exec("UPDATE member"
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

bool DatabaseController::uploadData(const BloodPressure& bloodPressure)
{

}

bool DatabaseController::uploadData(const QList<BloodPressure>& listBloodPressure)
{

}

bool DatabaseController::uploadData(const BloodSugar& bloodSugar)
{

}

bool DatabaseController::uploadData(const QList<BloodSugar>& listBloodSugar)
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

        filename.remove(".json");
        jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        jsonObject = jsonDoc.object();
        //jsonVal = jsonObj["patientID"];
        //QStringList name = jsonDoc["patientName"].toString().split('/', QString::SkipEmptyParts);
        //QStringList bs = jsonDoc["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);

        QGeoAddress address;
        address.setStreet("Mainstreet");
        address.setPostalCode("68219");
        address.setCity("Mannheim");
        address.setCountry("Germany");

         //list.append( Patient(name[0], name[1], static_cast<UserType>(filename.toInt()), jsonObject["email"].toString(), jsonObject["phone"].toString() , jsonObject["birthDate"].toString(), jsonObject["age"].toInt(), jsonObject["weight"].toDouble(), jsonObject["bodysize"].toDouble(),
           //     static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(), bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address) );
    }
}

bool DatabaseController::creatDatabase()
{
    QSqlQuery query;
    query.exec(
                "CREATE TABLE registration("
                "userid text NOT NULL PRIMARY KEY,"
                "password text NOT NULL,"
                "usertyp bytea NOT NULL,"
                "UNIQUE(userid, password)"
                ");"

                "CREATE TABLE patient("
                "userid text  NOT NULL,"
                "password text  NOT NULL,"
                "data bytea,"
                "UNIQUE(userid, password),"
                "PRIMARY KEY (userid),"
                "FOREIGN KEY (userid, password) REFERENCES registration (userid, password) ON DELETE CASCADE ON UPDATE CASCADE"
                ");"

                "CREATE TABLE doctor("
                "userid text  NOT NULL,"
                "password text  NOT NULL,"
                "data bytea,"
                "UNIQUE(userid, password),"
                "PRIMARY KEY (userid),"
                "FOREIGN KEY (userid, password) REFERENCES registration (userid, password) ON DELETE CASCADE ON UPDATE CASCADE"
                ");"

                "CREATE TABLE member("
                "userid text  NOT NULL,"
                "password text  NOT NULL,"
                "data bytea,"
                "UNIQUE(userid, password),"
                "PRIMARY KEY (userid),"
                "FOREIGN KEY (userid, password) REFERENCES registration (userid, password) ON DELETE CASCADE ON UPDATE CASCADE"
                ");"

                "CREATE TABLE sugarmeasurment("
                "userid text NOT NULL REFERENCES patient(userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                "value double precision,"
                "timestemp text NOT NULL,"
                "UNIQUE(userid, timestemp),"
                "PRIMARY KEY (userid, timestemp)"
                ");"

                "CREATE TABLE bloodmeasurment("
                "userid text NOT NULL REFERENCES patient(userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                "value double precision,"
                "timestemp text NOT NULL,"
                "UNIQUE(userid, timestemp),"
                "PRIMARY KEY (userid, timestemp)"
                ");"
               );
    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}

bool DatabaseController::deleteDatabase()
{
    QSqlQuery query;
    query.exec( "DROP TABLE patient, member, doctor, access, registration, sugarmeasurment, bloodmeasurment ");

    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}

