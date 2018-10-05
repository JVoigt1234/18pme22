#ifndef NUMBERCONVERTER_H
#define NUMBERCONVERTER_H

#include <QObject>
#include <qstring.h>
#include <qdebug.h>     //zur Fehlerausgabe

class NumberConverter : public QObject
{
    Q_OBJECT



    Q_PROPERTY(QString bin READ getbin WRITE setBin NOTIFY binChanged)
    Q_PROPERTY(QString dec READ getdec WRITE setDec NOTIFY decChanged)
    Q_PROPERTY(QString oct READ getoct WRITE setOct NOTIFY octChanged)
    Q_PROPERTY(QString hex READ gethex WRITE setHex NOTIFY hexChanged)

    QString bin;
    QString dec;
    QString oct;
    QString hex;

protected:
    void formatBin(QString &bin);
    void formatOct(QString &oct);
    void update(int numberConversion);

public:
    explicit NumberConverter(QObject *parent = 0);



    QString getbin();
    QString getdec();
    QString getoct();
    QString gethex();

signals:

    void binChanged();
    void decChanged();
    void octChanged();
    void hexChanged();

public slots:       //bewirkt den zugriff auch von der main.qml aus!!!!!!

    void setBin(QString m_b);
    void setDec(QString m_d);
    void setOct(QString m_o);
    void setHex(QString m_h);
};

#endif // NUMBERCONVERTER_H
