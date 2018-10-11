#include "mythread.h"

MyThread::MyThread(/*QObject *parent*/) /*: QObject(parent)*/
{
    write.moveToThread(&m_thread);
    connect(&write, &FileWriter::writeFinish , this, &MyThread::run );
    connect(this, &MyThread::startwrite , &write, &FileWriter::work );
    m_thread.start(QThread::HighestPriority);
}

void MyThread::run(){

    double euler = 0;
    QString res = "0";

    for(int i = 170; i >= 0; i--){
        euler += (1/fakultaet(i));
        emit startwrite(euler);
        //qDebug() << "noch" << i;
        m_thread.exit();
    }

    res = QString::number(euler,'f',50);

    emit eulerFinish(res);
}

double MyThread::fakultaet(unsigned a)
{
    if (a <= 1){
        return 1;
    }
    return a * fakultaet(a - 1);
    /*double wert = 1;

    for (unsigned i = 2; i <= a; ++i) {
          wert *= i;
        }
    qDebug() << wert;
    return wert;*/

}
