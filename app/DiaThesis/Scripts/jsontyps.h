#ifndef JSONTYPS_H
#define JSONTYPS_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Patient
{

};

class BloodSugar
{
public:
    QString PatientID;
    QDateTime TimeStemp;
    double BloodSugarValue;
};

//template<class T>
//class JSONRow
//{
//public:
//    QString id;
//    QString key;
//    T value;
//};

//template<class T>
//class JSONRootObject
//{
//public:
//    int total_rows;
//    int offset;
//    QList<JSONRow<T>> JSON_Row;
//};



#endif // JSONTYPS_H
