#include "eulerthread.h"

EulerThread::EulerThread(/*QObject *parent*/) /*: QObject(parent)*/
{
    m_work.moveToThread(&m_euler);
    connect(&m_work, &Worker::writeFinish , this, &EulerThread::euler );
    connect(this, &EulerThread::startwrite , &m_work, &Worker::writeSummand );
    m_euler.start(QThread::HighestPriority);

    write.moveToThread(&m_write);
    connect(this, &EulerThread::writef, &write, &FileWriter::outList);
    m_write.start(QThread::HighestPriority);
}

void EulerThread::euler(double result){

    QString res = QString::number(result,'f',50);
    qDebug() << "Eulerzahl:\n" << res;
    emit eulerFinish(res);
}

//Liste und Mutex

QList<double> EulerThread::summanden;
QMutex EulerThread::m_mute;
