#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <qdebug.h>
#include "filewriter.h"
#include "worker.h"

class EulerThread : public QObject
{
    Q_OBJECT

    //Funktionen
    QThread m_euler;
    QThread m_write;

    Worker m_work;
    FileWriter write;

public:
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
