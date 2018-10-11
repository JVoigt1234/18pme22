#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H

#include <QObject>
#include <qdebug.h>

class PowerSupply : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int volt READ getVolt WRITE setVolt NOTIFY voltChanged)
    Q_PROPERTY(int power READ getPower WRITE setPower NOTIFY powerChanged)
    Q_PROPERTY(int watt READ getWatt WRITE setWatt NOTIFY wattChanged)

    int m_volt;
    int m_power;
    int m_watt;

public:
    explicit PowerSupply(QObject *parent = 0);

    int getVolt();
    int getPower();
    int getWatt();

    void setVolt(int m_U);
    void setPower(int m_I);
    void setWatt(int m_P);

signals:
    void voltChanged();
    void powerChanged();
    void wattChanged();

public slots:
};

#endif // POWERSUPPLY_H
