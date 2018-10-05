#ifndef EULERTHREAD_H
#define EULERTHREAD_H

#include <QThread>
#include <QDebug>
#include <QObject>
#include "worker.h"
#include "filewriter.h"

class EulerThread : public QObject
{
    Q_OBJECT

   QThread m_eulerthread;
   QThread m_thread;
   Worker m_worker;
   fileWriter m_filewriter;

public:
   explicit EulerThread()
   {
       m_worker.moveToThread(&m_eulerthread);
       connect(&m_worker, &Worker::result, this, &EulerThread::result);
       connect(this, &EulerThread::work, &m_worker, &Worker::work);
       m_eulerthread.start(QThread::HighestPriority);

       m_filewriter.moveToThread((&m_thread));
       connect(this, &EulerThread::writef, &m_filewriter, &fileWriter::writef);
       m_thread.start(QThread::HighestPriority);
   }

private:
   void result(double result)
   {
       QString s=QString::number (result, 'g',20);
       qDebug() << "Eulerzahl:" << s;
       emit newEuler(s);
   }

signals:
    void work();
    void writef();
    void newEuler(QString result);

public slots:
    void calcEuler()
    {
        emit work();        // ruft Berechnung auf
    }
};

#endif // EULERTHREAD_H
