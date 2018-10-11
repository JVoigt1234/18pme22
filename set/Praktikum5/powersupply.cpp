#include "powersupply.h"

PowerSupply::PowerSupply(QObject *parent) : QObject(parent)
{
    m_volt = 0;
    m_watt = 0;
    m_power = 0;

}

int PowerSupply::getVolt()
{
    return m_volt;
}

int PowerSupply::getPower()
{
    return m_power;
}

int PowerSupply::getWatt()
{

    //return m_watt = m_volt * m_power;
    return m_watt;
}

void PowerSupply::setVolt(int m_U)
{
    if(m_volt != m_U){

        m_volt = m_U;

        if(m_volt > m_watt)
            m_volt = m_watt;

        if(m_volt != 0)
            m_power = m_watt/m_volt;

        qDebug() << "Volt has been changed";
        emit powerChanged();
        emit voltChanged();
    }
}

void PowerSupply::setPower(int m_I)
{
    if(m_power != m_I){

        m_power = m_I;

        if(m_power > m_watt)
            m_power = m_watt;

        if(m_power != 0)
            m_volt = m_watt/m_power;

        qDebug() << "Power has been changed";
        emit voltChanged();
        emit powerChanged();
    }
}

void PowerSupply::setWatt(int m_P)
{
    if(m_watt != m_P){
        m_watt = m_P;

        if(m_power == 0)
            m_power = 1;

        m_volt = m_watt/m_power;

        qDebug() << "Watt has been changed";
        emit wattChanged();
        emit powerChanged();
        emit voltChanged();
    }
}

