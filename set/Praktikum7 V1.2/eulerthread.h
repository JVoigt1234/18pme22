#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <qdebug.h>

#include <QMutex>
#include <QList>

#include "filewriter.h"
#include "worker.h"



class EulerThread : public QObject
{
    Q_OBJECT

    QThread m_euler;
    QThread m_write;

    Worker m_work;
    FileWriter write;

public:

    static QList<double> summanden;
    static QMutex m_mute;

    explicit EulerThread(/*QObject *parent = 0*/);

    void euler(double resulat);
signals:

    void eulerFinish(QString result);
    void writef();
    void startwrite();

public slots:
    void calcEuler()
    {
        emit startwrite();
    }
};

#endif // MYTHREAD_H
