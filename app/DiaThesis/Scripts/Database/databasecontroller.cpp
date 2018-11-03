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

    m_userID = "";
    m_userType = UserType::inValidUser;

    if(m_database.open(m_username,m_password))
    {
        qDebug() << "connected to" << m_hostname;
    }
    else
    {
        qDebug() << "Error =" << m_database.lastError().text();
    }

    this->start();
}

DatabaseController::~DatabaseController()
{
    if(m_database.isOpen())
    {
        m_database.close();
    }

    if(this->isRunning())
    {
        this->terminate();
    }

}

/// encrypts the data with the given key
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

///Converts a given jsonObject to a QGeoAddress object
///The values used are street, city, country, and postal code
QGeoAddress DatabaseController::convertJSON2Address(const QJsonObject jsonArray)
{
    QGeoAddress address;
    address.setStreet(jsonArray["street"].toString());
    address.setCity(jsonArray["city"].toString());
    address.setCountry(jsonArray["country"].toString());
    address.setPostalCode(jsonArray["postalCode"].toString());

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
    return m_database.isOpen();
}

///Checks whether the user with the specified ID is stored in the database
///returns true if yes otherwise false
///     Exception: SqlError
bool DatabaseController::isUserAvailable(const QString eMail)
{
    QByteArray id = QCryptographicHash::hash( QString(eMail + QString(eMail.length()) ).toUtf8(), QCryptographicHash::Md5).toHex();

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
///    Exception: InvalidUser
UserType DatabaseController::isValidUser(QString eMail, QString password)
{
    if(eMail.isNull() || eMail.trimmed().isEmpty() || password.isNull() || password.trimmed().isEmpty() )
    {
        m_userID = "";
        m_userType = UserType::inValidUser;
        return UserType::inValidUser;
    }

    QByteArray id = QCryptographicHash::hash( QString(eMail + QString(eMail.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray pw = QCryptographicHash::hash( QString(password + QString(id)).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT userid,password,usertyp"
                    " FROM registration"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND password = '" + QString(pw) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        m_userID = "";
        m_userType = UserType::inValidUser;
        return UserType::inValidUser;
    }

    query.first();      //i need this to select the first one

    QByteArray usertyp = QByteArray::fromHex(query.value(2).toByteArray());

    if(usertyp.isEmpty())
    {
        m_userID = "";
        m_userType = UserType::inValidUser;
        return UserType::inValidUser;
    }

    usertyp = crypt(usertyp, pw);
    m_userID = eMail;
    m_userType = static_cast<UserType>(usertyp.toInt());
    return m_userType;

}

bool DatabaseController::isIDAuthorized(const QString patientID, const QString foreignID)
{
    if(isUserAvailable(patientID) == false || isUserAvailable(foreignID) == false)
    {
        return true;
    }

    QByteArray patientid = QCryptographicHash::hash( QString(patientID + QString(patientID.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray foreignid = QCryptographicHash::hash( QString(foreignID + QString(foreignID.length())).toUtf8(), QCryptographicHash::Md5).toHex();

//    QSqlQuery query("SELECT *"
//                    " FROM accessmember as m, accessdoctor as d"
//                    " WHERE (m.patientid = '" + QString(patientid) + "' AND m.memberid = '" + QString(foreignid) + "')"
//                    " OR"
//                    " (d.patientid = '" + QString(patientid) + "' AND d.doctorid = '" + QString(foreignid) + "')"
//                    );
//    if(query.lastError().type() == QSqlError::NoError && query.size() > 0)
//    {
//        return true;
//    }

    QSqlQuery query("SELECT *"
                    " FROM accessdoctor"
                    " WHERE patientid = '" + QString(patientid) + "' AND "
                    " doctorid = '" + QString(foreignid) + "'");

    if(query.lastError().type() == QSqlError::NoError && query.size() == 1)
    {
        return true;
    }

    query.prepare("SELECT *"
                  " FROM accessmember"
                  " WHERE patientid = '" + QString(patientid) + "' AND "
                  " meberid = '" + QString(foreignid) + "'");

    query.exec();
    if(query.lastError().type() == QSqlError::NoError && query.size() == 1)
    {
        return true;
    }

    return false;
}

///create a new user on the database
///     Exception: InvalidUser
bool DatabaseController::isUserCreated(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    if(isUserAvailable(user->getUserID()) == true || isUserOK(user) == false || password.isNull() || password.trimmed().isEmpty())
    {
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
                               m_userID = "";
                               m_userType = UserType::inValidUser;
            return false;
    }

    //creating a formular
    creating.prepare("INSERT INTO " + tablename + "(userid,password,data) "
                     "VALUES ("
                     "(SELECT userid FROM registration WHERE userid = '" + QString(id) + "' AND password = '" + QString(pw) + "'),"
                     "(SELECT password FROM registration WHERE userid = '" + QString(id) + "' AND password = '" + QString(pw) + "'),"
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
        m_userID = user->getUserID();
        m_userType = user->getUserType();
        return true;
    }

    //if it was not possible to write user to "user"table
    creating.exec("DELETE FROM registration"
                " WHERE userid = '" + QString(id) + "'"
                " AND password = '" + QString(pw) + "'" );

    m_userID = "";
    m_userType = UserType::inValidUser;
    return false;
}

///delete a user from the database
///     Exception: InvalidUser
bool DatabaseController::isUserDeleted(User* user, QString password)
{
    if(!isConnected() )
    {
        return false;
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || password.isNull() || password.trimmed().isEmpty())
    {
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
        m_userID = "";
        m_userType = UserType::inValidUser;
        return true;
    }
    else
    {
        return false;
    }
}

/// returns a object from typeof Patient if the user exists
///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Patient DatabaseController::patientData(QString patientID)
{
    QByteArray id = QCryptographicHash::hash(QString(patientID + QString(patientID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT password,data"
                    " FROM patient"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + patientID + " not found.").toLocal8Bit().data());
    }
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;
    QStringList bs;

    if(jsonObject.contains("address") == true)
    {
        address = convertJSON2Address(jsonObject["address"].toObject());
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

        return Patient(name[0], name[1], UserType::patient, jsonObject["email"].toString(), jsonObject["phone"].toString() , jsonObject["birthDate"].toString(), jsonObject["weight"].toDouble(),
                  jsonObject["bodysize"].toDouble(), static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(),
                   bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address);
    }
}

///  a patient can give a stranger access to his own data
/// this function is only the patients
///     Exception: InvalidUser, SqlError
bool DatabaseController::allowAccess(const QString foreignID)
{
    if(m_userType != UserType::patient || m_userID.trimmed().isEmpty())
    {
         throw InvalidUser("Only for patient.");
    }

    QByteArray patientid = QCryptographicHash::hash( QString(m_userID + QString(m_userID.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray foreignid = QCryptographicHash::hash( QString(foreignID + QString(foreignID.length())).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT password,usertyp"
                    " FROM registration"
                    " WHERE userid = '" + QString(foreignid) + "'");

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.first();      //i need this to select the first one

    QByteArray usertyp = QByteArray::fromHex(query.value(1).toByteArray());

    if(usertyp.isEmpty())
    {
        return false;
    }

    usertyp = crypt(usertyp, query.value(0).toByteArray());
    QString tablename;

    switch (static_cast<UserType>(usertyp.toInt())) {
        case UserType::admin:
        case UserType::doctor: tablename = "doctor";
            break;
        case UserType::patient:
        case UserType::member: tablename = "member";
            break;
        default:
            return false;
    }

    query.prepare("SELECT patientid," + tablename + "id"    //alternative *
                  " FROM access" + tablename +
                  " WHERE patientid = '" + QString(patientid) + "'"
                  " AND " + tablename + "id = '" + QString(foreignid) + "'" );
    query.exec();

    if(query.lastError().type() == QSqlError::NoError && query.size() == 1)
    {
        return true;
    }

    if(query.size() > 1)
    {
        return false;
    }

    query.prepare("INSERT INTO access" + tablename + "(patientid," + tablename + "id) "
                  "VALUES ("
                  "(SELECT userid FROM patient WHERE userid = '" + QString(patientid) + "'), "
                  "(SELECT userid FROM " + tablename + " WHERE userid = '" + QString(foreignid) + "') )");

    query.exec();
    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;

}

///  a patient can deny a stranger access to his own data.
/// this function is only the patients
///     Exception: InvalidUser, SqlError
bool DatabaseController::denyAccess(const QString foreignID)
{
    if(m_userType != UserType::patient || m_userID.trimmed().isEmpty())
    {
         throw InvalidUser("Only for patient.");
    }

    QByteArray patientid = QCryptographicHash::hash( QString(m_userID + QString(m_userID.length())).toUtf8(), QCryptographicHash::Md5).toHex();
    QByteArray foreignid = QCryptographicHash::hash( QString(foreignID + QString(foreignID.length())).toUtf8(), QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT password,usertyp"
                    " FROM registration"
                    " WHERE userid = '" + QString(foreignid) + "'");

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.first();      //i need this to select the first one

    QByteArray usertyp = QByteArray::fromHex(query.value(1).toByteArray());

    if(usertyp.isEmpty())
    {
        return false;
    }

    usertyp = crypt(usertyp, query.value(0).toByteArray());
    QString tablename;

    switch (static_cast<UserType>(usertyp.toInt())) {
        case UserType::admin:
        case UserType::doctor: tablename = "doctor";
            break;
        case UserType::patient:
        case UserType::member: tablename = "member";
            break;
        default:
            return false;
    }

    query.prepare("SELECT patientid," + tablename + "id"    //alternative *
                  " FROM access" + tablename +
                  " WHERE patientid = '" + QString(patientid) + "'"
                  " AND " + tablename + "id = '" + QString(foreignid) + "'" );
    query.exec();

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        return false;
    }

    query.prepare("DELETE FROM access" + tablename +
                  " WHERE patientid = '" + QString(patientid) + "'"
                  " AND " + tablename + "id = '" + QString(foreignid) + "'" );

    query.exec();
    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}

/// returns a list of all patients the doctor can access. Only for the user type doctor
///     Exception: InvalidUser, SqlError
bool DatabaseController::getListPatient(QList<Patient>& listPatient)
{
    if(m_userType != UserType::doctor)
    {
        throw InvalidUser("Only for doctor.");
    }

    QByteArray id = QCryptographicHash::hash(QString(m_userID + QString(m_userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT patientid"
                    " FROM accessdoctor"
                    " WHERE doctorid = '" + QString(id) + "'");

    if(query.lastError().type() != QSqlError::NoError)
    {
        throw SqlError(query.lastError().text());
    }

    while (query.next())
    {
        listPatient.append(patientData(query.value(0).toString()));
    }

    return true;
}

/// returns a object from typeof Doctor if the user exists and is a doctor
///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Doctor DatabaseController::getDoctorData(void)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::doctor || isUserAvailable(m_userID) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    return getDoctorData(m_userID);
}

/// returns a object from typeof Doctor if the user exists and is a doctor
///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Doctor DatabaseController::getDoctorData(const QString doctorID)
{
    if(m_userID != doctorID)
    {
        if(isIDAuthorized(m_userID, doctorID) == false)
        {
            throw InvalidUser("ID not authorized to get doctor information.");
        }
    }

    QByteArray id = QCryptographicHash::hash(QString(doctorID + QString(doctorID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT password,data"
                    " FROM doctor"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + doctorID + " not found.").toLocal8Bit().data());
    }

    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;
    QStringList bs;

    if(jsonObject.contains("address") == true)
    {
        address = convertJSON2Address(jsonObject["address"].toObject());
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

    if(jsonObject.contains("phone") && jsonObject.contains("institutionName") )
    {
        return Doctor(name[0], name[1], UserType::doctor, jsonObject["email"].toString(), jsonObject["institutionName"].toString(), address, jsonObject["phone"].toString());
    }
    else
    {
        throw InvalidUser("No phone or institutionName.");
    }

}

/// returns a object from typeof Patient if the user exists and is a patient
///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Patient DatabaseController::getPatientData(void)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::patient || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    return patientData(m_userID);
}

/// returns a object from typeof Member if the user exists and is a member
///     Exception: InvalidUser, UserNotFound, InvalidDateTimeFormate and SqlError Exception
Member DatabaseController::getMemberData(void)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::member  || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    QByteArray id = QCryptographicHash::hash(QString(m_userID + QString(m_userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();


    QSqlQuery query("SELECT password,data"
                    " FROM member"
                    " WHERE userid = '" + QString(id) + "'" );

    if(query.lastError().type() != QSqlError::NoError || query.size() != 1)
    {
        throw UserNotFound(QString("User with ID: " + m_userID + " not found.").toLocal8Bit().data());
    }
    qDebug() << query.size();
    query.first();

    QByteArray data = crypt(QByteArray::fromHex(query.value(1).toByteArray()), query.value(0).toByteArray());
    QJsonObject jsonObject = QJsonDocument::fromBinaryData(data ).object();
    QGeoAddress address;

    if(jsonObject.contains("address") == true)
    {
        address = convertJSON2Address(jsonObject["address"].toObject());
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

///     Exception: InvaildUser
bool DatabaseController::updateUser(const Doctor* user)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || user->getUserType() != UserType::doctor || m_userID != user->getUserID() )
    {
        throw InvalidUser("Invaild User.");
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    if(userObject.isEmpty() || user->getInstitutionName().trimmed().isEmpty())
    {
        return  false;
    }

    userObject.insert("address", convertAddress2JSON(user->getAddress()));
    userObject.insert("institutionName", user->getInstitutionName());

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

    creating.exec("UPDATE doctor"
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

///     Exception: InvaildUser
bool DatabaseController::updateUser(const Patient* user)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || user->getUserType() != UserType::patient ||
            user->getBirthDay().isNull() || user->getBirthDay().trimmed().isEmpty() || m_userID != user->getUserID()  )
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

    userObject.insert("address", convertAddress2JSON(user->getAddress()));

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

///     Exception: InvaildUser
bool DatabaseController::updateUser(const Member* user)
{
    if(!m_database.isOpen())
    {
        throw InvalidUser("Database closed.");
    }

    if(isUserOK(user) == false || isUserAvailable(user->getUserID()) == false || user->getUserType() != UserType::member || m_userID != user->getUserID() )
    {
        throw InvalidUser("Invaild User.");
    }

    QJsonObject userObject;
    userObject = convertUser2JSON(user);

    if(userObject.isEmpty())
    {
        return  false;
    }

    userObject.insert("address", convertAddress2JSON(user->getAddress()));

    QJsonArray patientRelease;
    QList<QString> p;
    user->getPatientRealease(p);
    if(p.length() > 0)
    {
        patientRelease = QJsonArray::fromStringList(p);
        userObject.insert("patientRelease", patientRelease);
    }

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

///     Exception: InvaildUser
bool DatabaseController::uploadData(const Measurement& measurement)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    QByteArray id = QCryptographicHash::hash(QString(m_userID + QString(m_userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();
    QString tablename;

    switch (measurement.getMeasurementType()) {
        case MeasurementType::bloodPressure: tablename = "pressuremeasurements";
            break;
        case MeasurementType::bloodSugar: tablename = "sugarmeasurements";
            break;
        default: return false;      //for undefined value (trust me i got it)
    }

    QSqlQuery creating;

    creating.prepare("INSERT INTO " + tablename + " (userid,value,timestamp)"
                     "VALUES ("
                     "(SELECT userid FROM patient WHERE userid = '" + QString(id) + "'),"
                     ":value,"
                     ":timestamp)");

    creating.bindValue(0, measurement.getValue());
    creating.bindValue(1, measurement.getTimeStamp());

    creating.exec();

    if(creating.lastError().type() != QSqlError::NoError)
    {
        creating.exec("UPDATE " + tablename +
                      " SET value = " + QString::number(measurement.getValue()) +
                      " WHERE userid = '" + QString(id) + "'"
                      " AND timestamp = '" + measurement.getTimeStamp() + "'");

        if(creating.lastError().type() != QSqlError::NoError)
        {
            return false;
        }
        return true;
    }

    return true;
}

///     Exception: InvaildUser
bool DatabaseController::uploadData(const QList<Measurement>& listOfMeasurements)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    for(int i = 0; i < listOfMeasurements.count(); i++)
    {
        if(uploadData(listOfMeasurements[i]) == false)
        {
            for(int j = 0; j < i; j++)
            {
                switch(listOfMeasurements[i].getMeasurementType())
                {
                    case MeasurementType::bloodPressure: deleteBloodPressureData( QDateTime::fromString(listOfMeasurements[i].getTimeStamp(), TimeStampFormate));
                        break;
                    case MeasurementType::bloodSugar: deleteBloodSugarData(QDateTime::fromString(listOfMeasurements[i].getTimeStamp(), TimeStampFormate));
                        break;
                }
            }
            return false;
        }
    }
    return true;
}

/// for patient typs
///     Exception: InvalidUser, InvalidDateTimeRange and UserNotFound
bool DatabaseController::getBloodPressure(const QDateTime timestamp, Measurement& measurement)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    QList<Measurement> list;
    if(getBloodPressure(m_userID, timestamp, timestamp, list) == false)
    {
        return false;
    }
    measurement = list[0];
    return true;

}

/// for patient typs
///     Exception: InvalidUser, InvalidDateTimeRange and UserNotFound
bool DatabaseController::getBloodPressure(const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    return getBloodPressure(m_userID, from, to, listOfMeasurements);
}

/// for patient typs
///     Exception: InvalidUser, InvalidDateTimeRange and UserNotFound
bool DatabaseController::getBloodPressure(const QString patientID, const QDateTime timestamp, Measurement& measurement)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    QList<Measurement> list;
    if(getBloodPressure(patientID, timestamp, timestamp, list) == false)
    {
        return false;
    }
    measurement = list[0];
    return true;
}

/// for other typs
///     Exception: InvalidUser and UserNotFound
bool DatabaseController::getBloodPressure(const QString patientID, const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements)
{
    if(patientID != m_userID)
    {
        if(isIDAuthorized(patientID, m_userID) == false)
        {
            throw InvalidUser("User not authorized.");
        }
    }
    else
    {
        if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
        {
            throw InvalidUser("Incorrect user");
        }
    }

    if(from > to)
    {
        throw InvalidDateTimeRange("The query must begin with the past.");
    }

    QByteArray id = QCryptographicHash::hash(QString(patientID + QString(patientID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT value, timestamp"
                    " FROM pressuremeasurements"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND (timestamp BETWEEN '" + from.toString(TimeStampFormate) + "' AND '" +
                        to.toString(TimeStampFormate) + "')" );

    id.clear();
    if(query.lastError().type() != QSqlError::NoError)
    {
        throw UserNotFound(QString("User with ID: " + patientID + " not found.").toLocal8Bit().data());
    }

    while (query.next())
    {
        QString timestamp = query.value(1).toString();
        timestamp.replace('T', " ");
        timestamp.remove(".000");
        listOfMeasurements.append(Measurement(timestamp, query.value(0).toDouble(), MeasurementType::bloodPressure ) );
    }
    return true;
}

/// for patient typs
///     Exception: InvalidUser, InvalidDateTimeRange and UserNotFound
bool DatabaseController::getBloodSugar(const QDateTime timestamp, Measurement& measurement)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    QList<Measurement> list;
    if(getBloodPressure(m_userID, timestamp, timestamp, list) == false)
    {
        return false;
    }
    measurement = list[0];
    return true;
}

/// for patient typs
///     Exception: InvalidUser, InvalidDateTimeRange and UserNotFound
bool DatabaseController::getBloodSugar(const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
    {
        throw InvalidUser("Incorrect user");
    }

    return getBloodPressure(m_userID, from, to, listOfMeasurements);
}

/// for other typs
///     Exception: InvalidUser and UserNotFound
bool DatabaseController::getBloodSugar(const QString patientID, const QDateTime timestamp, Measurement& measurement)
{
    if(!m_database.isOpen())
    {
        throw SqlError("Database closed.");
    }

    QList<Measurement> list;
    if(getBloodPressure(patientID, timestamp, timestamp, list) == false)
    {
        return false;
    }
    measurement = list[0];
    return true;

}

/// for other typs
///     Exception: InvalidUser and UserNotFound
bool DatabaseController::getBloodSugar(const QString patientID, const QDateTime from, const QDateTime to, QList<Measurement>& listOfMeasurements)
{
    if(patientID != m_userID)
    {
        if(isIDAuthorized(patientID, m_userID) == false)
        {
            throw InvalidUser("User not authorized.");
        }
    }
    else
    {
        if( m_userType != UserType::patient  || isUserAvailable(m_userID ) == false )
        {
            throw InvalidUser("Incorrect user");
        }
    }

    if(from > to)
    {
        throw InvalidDateTimeRange("The query must begin with the past.");
    }

    QByteArray id = QCryptographicHash::hash(QString(patientID + QString(patientID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query("SELECT value, timestamp"
                    " FROM sugarmeasurements"
                    " WHERE userid = '" + QString(id) + "'"
                    " AND (timestamp BETWEEN '" + from.toString(TimeStampFormate) + "' AND '" +
                        to.toString(TimeStampFormate) + "')" );

    id.clear();
    if(query.lastError().type() != QSqlError::NoError)
    {
        throw UserNotFound(QString("User with ID: " + patientID + " not found.").toLocal8Bit().data());
    }

    while (query.next())
    {
        QString timestamp = query.value(1).toString();
        timestamp.replace('T', " ");
        timestamp.remove(".000");
        listOfMeasurements.append(Measurement(timestamp, query.value(0).toDouble(), MeasurementType::bloodPressure ) );
    }
    return true;
}

///     Exception: InvaildUser
bool DatabaseController::deleteBloodPressureData(const QDateTime timeStamp)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    return deleteBloodPressureData(timeStamp, timeStamp);
}

///     Exception: InvaildUser and InvalidDateTimeRange
bool DatabaseController::deleteBloodPressureData(const QDateTime from, const QDateTime to)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    if(from > to)
    {
        throw InvalidDateTimeRange("The query must begin with the past.");
    }

    QByteArray id = QCryptographicHash::hash(QString(m_userID + QString(m_userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query;
    query.exec("DELETE FROM pressuremeasurements"
                " WHERE userid = '" + QString(id) + "'"
                " AND (timestamp BETWEEN '" + from.toString(TimeStampFormate) + "' AND '" +
                to.toString(TimeStampFormate) + "')" );

    id.clear();

    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}

///     Exception: InvaildUser
bool DatabaseController::deleteBloodSugarData(const QDateTime timeStamp)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    return deleteBloodSugarData(timeStamp, timeStamp);

}

///     Exception: InvaildUser
bool DatabaseController::deleteBloodSugarData(const QDateTime from, const QDateTime to)
{
    if(m_userType != UserType::patient || isUserAvailable(m_userID) == false)
    {
        throw InvalidUser("Invaild User.");
    }

    if(from > to)
    {
        throw InvalidDateTimeRange("The query must begin with the past.");
    }

    QByteArray id = QCryptographicHash::hash(QString(m_userID + QString(m_userID.length())).toUtf8(),QCryptographicHash::Md5).toHex();

    QSqlQuery query;
    query.exec("DELETE FROM sugarmeasurements"
                " WHERE userid = '" + QString(id) + "'"
                " AND (timestamp BETWEEN '" + from.toString(TimeStampFormate) + "' AND '" +
                to.toString(TimeStampFormate) + "')" );

    id.clear();

    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}


//only for development

void DatabaseController::loadDataset(QList<Patient>& list)
{
    QString val, path = "TestDaten/patient/";
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
        QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);
        QStringList bs = jsonDoc["bloodSugarRange"].toString().split('/', QString::SkipEmptyParts);

        QGeoAddress address = convertJSON2Address(jsonObject["address"].toObject());
         list.append( Patient(name[0], name[1], UserType::patient, jsonObject["email"].toString(), jsonObject["phone"].toString() , jsonObject["birthDate"].toString(), jsonObject["weight"].toDouble(), jsonObject["bodysize"].toDouble(),
                static_cast<Gender>(jsonObject["gender"].toInt()), jsonObject["targetBloodSugar"].toDouble(), bs[0].toDouble(), bs[1].toDouble(), jsonObject["alcohol"].toBool(), jsonObject["cigaret"].toBool(), address) );
    }
}

void DatabaseController::loadDataset(QList<Doctor>& list)
{
    QString val, path = "TestDaten/doctor/";
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
        QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);

        QGeoAddress address = convertJSON2Address(jsonObject["address"].toObject());
         list.append( Doctor(name[0], name[1], UserType::doctor, jsonObject["email"].toString(), jsonObject["institutionName"].toString(), address, jsonObject["phone"].toString() )  );
    }
}

void DatabaseController::loadDataset(QList<Member>& list)
{
    QString val, path = "TestDaten/member/";
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
        QStringList name = jsonObject["patientName"].toString().split('/', QString::SkipEmptyParts);
        QGeoAddress address = convertJSON2Address(jsonObject["address"].toObject());
        QList<QString> plist;
        if(jsonObject.contains("patientRelease") == true)
        {
            QJsonArray array = jsonObject["patientRelease"].toArray();
            foreach (const QJsonValue & value, array) {
                plist.append(value.toString());
            }
        }
        list.append(Member(name[0], name[1], UserType::member, jsonObject["email"].toString(), plist, address, jsonObject["phone"].toString() ) );
    }
}

void DatabaseController::loadDataset(QList<Measurement>& list, MeasurementType type)
{
    QString val, path = "TestDaten/bloodpressure/";
    QFile file;
    QJsonDocument jsonDoc;
    QJsonArray jsonObject;

    QDir directory(path);
    QStringList data = directory.entryList(QStringList() << "*.json",QDir::Files);

    if(type == MeasurementType::bloodSugar)
        path = "TestDaten/bloodsugar/";


    foreach(QString filename, data) {
        file.setFileName(path + filename);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        val = file.readAll();
        file.close();

        jsonDoc = QJsonDocument::fromJson(val.toUtf8());
        jsonObject = jsonDoc.array();
        QString id = jsonObject.at(0)["userid"].toString();

        if(m_userID == id)
        {
            QString timestamp;
            double pressureValue;

            for(int i=0; i < jsonObject.count(); i++)
            {
                timestamp = jsonObject.at(i)["timestamp"].toString();  //QDateTime timestemp = jsonObject.at(i)["timestemp"].toVariant().toDateTime();
                pressureValue = jsonObject.at(i)["pressureValue"].toDouble();
                if(type == MeasurementType::bloodSugar)
                    pressureValue = jsonObject.at(i)["sugarValue"].toDouble();

                Measurement data(timestamp,pressureValue, type);
                list.append(data);
            }
        }
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

                "CREATE TABLE accessmember("
                "patientid text  NOT NULL,"
                "memberid text  NOT NULL,"
                "UNIQUE(patientid, memberid),"
                "PRIMARY KEY (patientid, memberid),"
                " FOREIGN KEY (patientid) REFERENCES patient (userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                " FOREIGN KEY (memberid) REFERENCES member (userid) ON DELETE CASCADE ON UPDATE CASCADE"
                ");"

                "CREATE TABLE accessdoctor("
                "patientid text  NOT NULL,"
                "doctorid text  NOT NULL,"
                "UNIQUE(patientid, doctorid),"
                "PRIMARY KEY (patientid, doctorid),"
                "FOREIGN KEY (patientid) REFERENCES patient (userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                " FOREIGN KEY (doctorid) REFERENCES doctor (userid) ON DELETE CASCADE ON UPDATE CASCADE"
                ");"

                "CREATE TABLE sugarmeasurements("
                "userid text NOT NULL REFERENCES patient(userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                "value double precision,"
                "timestamp timestamp NOT NULL,"
                "UNIQUE(userid, timestamp),"
                "PRIMARY KEY (userid, timestamp)"
                ");"

                "CREATE TABLE pressuremeasurements("
                "userid text NOT NULL REFERENCES patient(userid) ON DELETE CASCADE ON UPDATE CASCADE,"
                "value double precision,"
                "timestamp timestamp NOT NULL,"
                "UNIQUE(userid, timestamp),"
                "PRIMARY KEY (userid, timestamp)"
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
    query.exec( "DROP TABLE bloodmeasurements, sugarmeasurements, accessdoctor, accessmember, member, doctor, patient, registration");

    if(query.lastError().type() == QSqlError::NoError)
    {
        return true;
    }

    return false;
}

