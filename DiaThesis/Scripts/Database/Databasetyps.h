///-----------------------------------------------------------------
/// Namespace:
/// Class:              UserType, Gender, BloodSugar, BloodPressure,
///                     UserData (abstract), Patient, Doctor, Member
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              -
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef JSONTYPS_H
#define JSONTYPS_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QGeoAddress>
#include <QCryptographicHash>

#include "Scripts/Exceptions/InvalidExceptions.h"

enum class UserType
{
    inValidUser = -1,
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

///Time stamp conforming to ISO 8601
///throws InvalidDateTimeFormate Execption
class BloodSugar
{
private:
    QDateTime m_timeStemp;
    double m_value;
public:
    ///Time stamp conforming to ISO 8601
    ///yyyy-MM-dd hh:mm:ss
    BloodSugar(QString &timeStemp, double value)
    {
        m_timeStemp = QDateTime::fromString(timeStemp,"yyyy.MM.dd hh:mm:ss");
        if(m_timeStemp.isNull())
        {
            throw InvalidDateTimeFormate("Invalid Date- or Timeformat.");
        }
        m_value = value;
    }

    ///Time stamp conforming to ISO 8601
    ///yyyy-MM-dd hh:mm:ss
    QString getTimeStemp(void) const {return (m_timeStemp.date().toString("yyyy-MM-dd") + " " + m_timeStemp.time().toString("hh:mm:ss")); }
    double getValue(void) const {return m_value;}
};

///Time stamp conforming to ISO 8601
///throws InvalidDateTimeFormate Execption
class BloodPressure
{
private:
    QDateTime m_timeStemp;
    double m_value;
public:
    BloodPressure(QString &timeStemp, double value)
    {
        m_timeStemp = QDateTime::fromString(timeStemp,"yyyy.MM.dd hh:mm:ss");
        if(m_timeStemp.isNull())
        {
            throw InvalidDateTimeFormate("Invalid Date- or Timeformat. It musst be yyyy.MM.dd hh:mm:ss");
        }
        m_value = value;
    }

    ///Time stamp conforming to ISO 8601 (yyyy-MM-dd hh:mm:ss)
    QString getTimeStemp(void) const {return (m_timeStemp.date().toString("yyyy-MM-dd") + " " + m_timeStemp.time().toString("hh:mm:ss")); }
    double getValue(void) const { return m_value;}
};

class User
{
protected:
    QString m_userID;
    QString m_forename;
    QString m_surname;
    QString m_eMail;
    QString m_phone;
    UserType m_userType;
    QGeoAddress m_address;

public:
    User(QString forename, QString surname, UserType type, QString eMail)
    {
        m_userID = eMail;
        m_forename = forename;
        m_surname = surname;
        m_userType = type;
        m_eMail = eMail;
    }
    User(QString forename, QString surname, UserType type, QString eMail,  QGeoAddress address, QString phone = nullptr) : User(forename, surname, type, eMail)
    {
        m_phone = phone;
        m_address = address;
    }
    virtual ~User() {}
    virtual QString getUserID(void) const {return m_userID;}
    virtual QString getForename(void) const {return m_forename;}
    virtual QString getSurname(void) const {return m_surname;}
    virtual UserType getUserType(void) const {return m_userType;}
    virtual QString geteMail(void) const {return m_eMail;}
    virtual QString getPhone(void) const {return m_phone;}
    virtual QGeoAddress getAddress(void) const {return m_address;}

    virtual void setForename(const QString name) { m_forename = name; }
    virtual void setSurname(const QString name) { m_surname = name; }
    virtual void setPhone(const QString phone) { m_phone = phone; }
    virtual void setAddress(const QGeoAddress address) { m_address = address;}
    //virtual void seteMail(const QString eMail) {m_eMail = eMail; m_userID = eMail; }
    bool operator==(User const& otherUser) { return (this->m_userID == otherUser.m_userID) && (this->m_userType == otherUser.getUserType());}

};

///Time stamp conforming to ISO 8601
///throws InvalidDateTimeFormate Execption
class Patient : virtual public User
{
private:
    double m_weight;
    double m_bodysize;
    Gender m_gender;
    double m_targetBloodSugar;
    double m_minBloodSugar;
    double m_maxBloodSugar;
    bool m_alcohol;
    bool m_cigaret;
    QDateTime m_birthDay;

public:
    Patient(QString forename, QString surname, UserType type, QString eMail, QString birthDay) : User(forename, surname, type, eMail)
    {
        m_birthDay = QDateTime::fromString(birthDay,"yyyy.MM.dd");
        if(m_birthDay.isNull())
        {
            throw InvalidDateTimeFormate("Invalid Date- or Timeformat. It musst be yyyy.MM.dd");
        }
        m_weight = 0;
        m_bodysize = 0;
        m_gender = Gender::other;
        m_targetBloodSugar = 0;
        m_minBloodSugar = 0;
        m_maxBloodSugar = 0;
        m_alcohol = false;
        m_cigaret = false;
    }
    Patient(QString forename, QString surname, UserType type, QString eMail, QString phone, QString birthDay, double weight, double bodysize, Gender gender, double targetBS,
             double minBS, double maxBS, bool alc, bool cig, QGeoAddress address) : User(forename, surname, type, eMail, address, phone)
    {
        m_birthDay = QDateTime::fromString(birthDay,"yyyy.MM.dd");
        if(m_birthDay.isNull())
        {
            throw InvalidDateTimeFormate("Invalid Date- or Timeformat. It musst be yyyy.MM.dd");
        }
        m_weight = weight;
        m_bodysize = bodysize;
        m_gender = gender;
        m_targetBloodSugar = targetBS;
        m_minBloodSugar = minBS;
        m_maxBloodSugar = maxBS;
        m_alcohol = alc;
        m_cigaret = cig;
        m_address = address;
    }

    int getAge(void) const {return 0 /*m_age*/;}		//rechnen
    double getWeight(void) const {return m_weight;}
    double getBodysize(void) const {return m_bodysize;}
    Gender getGender(void) const {return m_gender;}
    double getTargetBloodSugar(void) const {return m_targetBloodSugar;}
    double getMinBloodSugar(void) const {return m_minBloodSugar;}
    double getMaxBloodSugar(void) const {return m_maxBloodSugar;}
    QString getBirthDay(void) const {return m_birthDay.toString("yyyy.MM.dd");}
    bool isAlcohol(void) const {return m_alcohol;}
    bool isCigaret(void) const {return m_cigaret;}

    void setWeight(const double weight) { m_weight = weight;}
    void setBodysize(const double size) { m_bodysize = size;}
    void setGender(const Gender type) { m_gender = type;}
    void setTargetBloodSugar(const double value) { m_targetBloodSugar = value;}
    void setMinBloodSugar(const double value) { m_minBloodSugar = value;}
    void setMaxBloodSugar(const double value) { m_maxBloodSugar = value;}
    void setAlcohol(const bool value) { m_alcohol = value;}
    void setCigaret(const bool value) { m_cigaret = value;}
    void setBirthDay(const QString date) {        m_birthDay = QDateTime::fromString(date,"yyyy.MM.dd");
                                                  if(m_birthDay.isNull()) throw InvalidDateTimeFormate("Invalid Date- or Timeformat. It musst be yyyy.MM.dd"); }
};

class Doctor : virtual public User
{
private:
    QString m_institutionName;
    //all doctor properties
public:
    Doctor(QString forename, QString surname, UserType type, QString eMail, QString institutionName, QGeoAddress address, QString phone) : User(forename, surname, type, eMail, address, phone)
    {
        m_institutionName = institutionName;
    }

    QString getInstitutionName(void) const {return m_institutionName;}

    void setInstitutionName(const QString name) {m_institutionName = name;}

};

class Member : virtual public User
{
private:
    //all member properties
    QList<QString> m_patientRelease;
public:
    Member(QString forename, QString surname, UserType type, QString eMail, QList<QString> patientRelease, QGeoAddress address, QString phone = nullptr) : User(forename, surname, type, eMail, address, phone)
    {
        m_patientRelease.append(patientRelease);
    }

    ///add a new access id
    bool addPatientRealease(const QString patientID)
    {
        if(m_patientRelease.contains(patientID))
            return false;
        m_patientRelease.append(patientID);
        return true;
    }
    ///retrun all access id
    void getPatientRealease(const QList<QString>* list) const {list = &m_patientRelease;}
    ///delete all ids
    bool deletePatientRealease(void) { m_patientRelease.clear(); return true;}
    ///delete the given id
    bool deletePatientRealease(const QString patientID) {return m_patientRelease.removeOne(patientID); }
};

#endif // JSONTYPS_H
