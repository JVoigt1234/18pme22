#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <qdebug.h>
#include "filewriter.h"

class MyThread : public QThread
{
    Q_OBJECT

    //Funktionen

    double fakultaet(unsigned a);
    void run();         //muss run heißen da eine Reimplemintierung ansonsten einen Slot in main.cpp
    //Aufgabenteil Erweiterung
    FileWriter write;
    QThread m_thread;

public:
    explicit MyThread(/*QObject *parent = 0*/);

    //void euler();
signals:

    void eulerFinish(QString result = 0);
    void startwrite(double summand);
public slots:
};

#endif // MYTHREAD_H
