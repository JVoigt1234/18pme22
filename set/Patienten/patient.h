#ifndef PATIENT_H
#define PATIENT_H

#include <QObject>

#include <QFile>
#include <QTextStream>
#include <QList>

class Patient : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ getName WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString vorname READ getVorname WRITE setVorname NOTIFY vornameChanged)
    Q_PROPERTY(QString sex READ getSex WRITE setSex NOTIFY sexChanged)
    Q_PROPERTY(QString birth READ getBirth WRITE setBirth NOTIFY birthChanged)

    QString name, vorname, sex, birth;
    //QList<QString> patientendata;

public:
    explicit Patient(QObject *parent = 0);

    void setName(QString n);
    void setVorname(QString v);
    void setSex(QString s);
    void setBirth(QString b);

    QString getName() {return name; }
    QString getVorname() {return vorname; }
    QString getSex() {return sex; }
    QString getBirth() {return birth; }

signals:

    void nameChanged();
    void vornameChanged();
    void sexChanged();
    void birthChanged();

public slots:
    void saveData();
};

#endif // PATIENT_H
