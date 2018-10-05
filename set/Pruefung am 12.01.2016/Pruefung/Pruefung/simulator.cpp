#include "simulator.h"

Simulator::Simulator(QObject *parent) : QObject(parent)
{
    //connect(&m_timer, &QTimer::timeout, this, &Simulator::update );

    connect(&m_timer,SIGNAL( timeout() ), this, SLOT( update() ) );
    m_rotX = 0;
    m_rotY = 0;
    m_rotZ = 0;

    xValue = 0;
    yValue = 0;
    zValue = 0;
    m_timer.start(50);
}

void Simulator::stepSizeX(double value)
{
    xValue = value;
}

void Simulator::stepSizeY(double value)
{
    yValue = value;
}

void Simulator::stepSizeZ(double value)
{
    zValue = value;
}

void Simulator::update()
{
    m_rotX += (xValue/20);
    m_rotY += (yValue/20);
    m_rotZ += (zValue/20);

    emit rotXChanged();
    emit rotYChanged();
    emit rotZChanged();
}