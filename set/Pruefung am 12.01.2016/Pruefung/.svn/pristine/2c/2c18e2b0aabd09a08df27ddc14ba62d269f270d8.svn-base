#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <QObject>

#include <QTimer>

class Simulator : public QObject
{
    Q_OBJECT

    Q_PROPERTY(double rotX READ getRotX NOTIFY rotXChanged)
    Q_PROPERTY(double rotY READ getRotY NOTIFY rotYChanged)
    Q_PROPERTY(double rotZ READ getRotZ NOTIFY rotZChanged)

    double m_rotX, xValue;
    double m_rotY, yValue;
    double m_rotZ, zValue;

    QTimer m_timer;

public:
    explicit Simulator(QObject *parent = 0);

    double getRotX() {return m_rotX;}
    double getRotY() {return m_rotY;}
    double getRotZ() {return m_rotZ;}\

signals:
    void rotXChanged();
    void rotYChanged();
    void rotZChanged();

public slots:
    void update();

    void stepSizeX(double value);
    void stepSizeY(double value);
    void stepSizeZ(double value);
};

#endif // SIMULATOR_H
