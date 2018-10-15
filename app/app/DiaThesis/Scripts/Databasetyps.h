///-----------------------------------------------------------------
/// Namespace:
/// Class:              UserType, Gender, BloodSugar, BloodPressure,
///                     UserData (abstract), Patient, Doctor, Member
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              failed = -2  : No request could be sent to the database
/// 					error = -1   : Some data have been skipped
/// 					run = 1      : Connection to database running
/// 					completed = 0: All data have been downloaded from the database and are available as List<DICOMFile>
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef JSONTYPS_H
#define JSONTYPS_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QGeoAddress>

enum class UserType
{
    admin,
    patient,
    doctor,
    member
};

enum class Gender
{
    man,
    woman,
    other
};

class BloodSugar
{
public:
    int m_userID;
    QDateTime m_timeStemp;
    double BloodSugarValue;
};

class BloodPressure
{
private:
    int m_userID;
    QDateTime m_timeStemp;
    double m_value;
public:
    BloodPressure(int userID, QDateTime timeStemp, double value)
    {
        m_userID = userID;
        m_timeStemp = timeStemp;
        m_value = value;
    }

};

class User
{    
protected:
    QString m_userID;
    QString m_name;
    UserType m_userType;    
    QString m_eMail;
    QString m_phone;

public:
    User(QString userID, QString name, UserType type, QString eMail, QString phone)
    {
        m_userID = userID;
        m_name = name;
        m_userType = type;
        m_eMail = eMail;
        m_phone = phone;
    }
    virtual ~User() {}
    virtual QString getUserID(void) const = 0;  //it is a pure virtual function and must be redefined in the derived class!
    virtual QString getName(void) const = 0;
    virtual UserType getUserType(void) const = 0;
    virtual QString getE_Mail(void) const = 0;
    virtual QString getPhone(void) const = 0;

    virtual void setUserID(const QString id) = 0;
    virtual void setName(const QString name) = 0;
    virtual void setUserType(const UserType type) = 0;
    virtual void setE_Mail(const QString eMail) = 0;
    virtual void setPhone(const QString phone) = 0;
};

class Patient : virtual public User
{
private:
    int m_age;
    double m_weight;
    double m_bodysize;
    Gender m_gender;
    double m_targetBloodSugar;
    double m_minBloodSugar;
    double m_maxBloodSugar;
    bool m_alcohol;
    bool m_cigaret;
    QGeoAddress m_address;
    QList<BloodSugar> m_bloodSugar;
    QList<BloodPressure> m_bloodPressure;

public:
    Patient(QString userID, QString name, UserType type, QString eMail, QString phone) : User(userID, name, type, eMail, phone)
    {
        m_age = 0;
        m_weight = 0;
        m_bodysize = 0;
        m_gender = Gender::other;
        m_targetBloodSugar = 0;
        m_minBloodSugar = 0;
        m_maxBloodSugar = 0;
        m_alcohol = false;
        m_cigaret = false;
    }
    Patient(QString userID, QString name, UserType type, QString eMail, QString phone, int age, double weight, double bodysize, Gender gender, double targetBS,
             double minBS, double maxBS, bool alc, bool cig, QList<BloodSugar> bloodSugar, QList<BloodPressure> bloodPressure, QGeoAddress address) : User(userID, name, type, eMail, phone)
    {
        m_age = age;
        m_weight = weight;
        m_bodysize = bodysize;
        m_gender = gender;
        m_targetBloodSugar = targetBS;
        m_minBloodSugar = minBS;
        m_maxBloodSugar = maxBS;
        m_alcohol = alc;
        m_cigaret = cig;
        m_bloodSugar = bloodSugar;
        m_bloodPressure = bloodPressure;
        m_address = address;
    }

    QString getUserID(void) const override  {return m_userID;}
    QString getName(void) const override {return m_name;}
    UserType getUserType(void) const override {return m_userType;}
    QString getE_Mail(void) const override {return m_eMail;}
    QString getPhone(void) const override {return m_phone;}

    void setUserID(const QString id) override { m_userID = id; }
    void setName(const QString name) override { m_name = name; }
    void setUserType(const UserType type) override {m_userType = type; }
    void setE_Mail(const QString eMail) override { m_eMail = eMail; }
    void setPhone(const QString phone) override { m_phone = phone; }

    int getAge(void) const {return m_age;}
    double getWeight(void) const {return m_weight;}
    double getBodysize(void) const {return m_bodysize;}
    Gender getGender(void) const {return m_gender;}
    double getTargetBloodSugar(void) const {return m_targetBloodSugar;}
    double getMinBloodSugar(void) const {return m_minBloodSugar;}
    double getMaxBloodSugar(void) const {return m_maxBloodSugar;}
    bool isAlcohol(void) const {return m_alcohol;}
    bool isCigaret(void) const {return m_cigaret;}
    void getBloodSugar(QList<BloodSugar> bloodSugar) const {bloodSugar = m_bloodSugar;}
    void getBloodPressure(QList<BloodPressure> bloodPressure) const {bloodPressure = m_bloodPressure;}
    QGeoAddress getAddress(void) const {return m_address;}

    void setAge(const int age) {m_age = age;}
    void setWeight(const double weight) { m_weight = weight;}
    void setBodysize(const double size) { m_bodysize = size;}
    void setGender(const Gender type) { m_gender = type;}
    void setTargetBloodSugar(const double value) { m_targetBloodSugar = value;}
    void setMinBloodSugar(const double value) { m_minBloodSugar = value;}
    void setMaxBloodSugar(const double value) { m_maxBloodSugar = value;}
    void setAlcohol(const bool value) { m_alcohol = value;}
    void setCigaret(const bool value) { m_cigaret = value;}
    void setBloodSugar(const QList<BloodSugar>* bloodSugar) { m_bloodSugar = *bloodSugar;}
    void setBloodPressure(const QList<BloodPressure>* bloodPressure) { m_bloodPressure = *bloodPressure;}
    void setAddress(const QGeoAddress address) {m_address = address;}
};

class Doctor : virtual public User
{
private:
    //all doctor properties
public:
    Doctor(QString userID, QString name, UserType type, QString eMail, QString phone) : User(userID, name, type, eMail, phone) {}

    QString getUserID(void) const override {return m_userID;}
    QString getName(void) const override {return m_name;}
    UserType getUserType(void) const override {return m_userType;}
    QString getE_Mail(void) const override {return m_eMail;}
    QString getPhone(void) const override {return m_phone;}

    void setUserID(const QString id) override { m_userID = id; }
    void setName(const QString name) override { m_name = name; }
    void setUserType(const UserType type) override {m_userType = type; }
    void setE_Mail(const QString eMail) override { m_eMail = eMail; }
    void setPhone(const QString phone) override { m_phone = phone; }
};

class Member : virtual public User
{
private:
    //all member properties
public:
    Member(QString userID, QString name, UserType type, QString eMail, QString phone) : User(userID, name, type, eMail, phone) {}

    QString getUserID(void) const override {return m_userID;}
    QString getName(void) const override  {return m_name;}
    UserType getUserType(void) const override {return m_userType;}
    QString getE_Mail(void) const override {return m_eMail;}
    QString getPhone(void) const override {return m_phone;}

    void setUserID(const QString id) override { m_userID = id; }
    void setName(const QString name) override { m_name = name; }
    void setUserType(const UserType type) override {m_userType = type; }
    void setE_Mail(const QString eMail) override { m_eMail = eMail; }
    void setPhone(const QString phone) override { m_phone = phone; }
};

#endif // JSONTYPS_H
