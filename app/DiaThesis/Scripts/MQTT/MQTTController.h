///-----------------------------------------------------------------
/// Namespace:
/// Class:              MQTTController
/// Description:
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:
/// Revision History:   First release
///-----------------------------------------------------------------

#ifndef MQTTCONTROLLER_H
#define MQTTCONTROLLER_H

#include <QObject>
#include <QtCore/QMap>
#include

class MQTTController : public QObject
{
    Q_OBJECT
public:
    explicit MQTTController(QObject *parent = nullptr);

signals:

public slots:
};

#endif // MQTTCONTROLLER_H
