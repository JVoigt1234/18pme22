#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "eulerthread.h"
//#include <qlisting.h>

#include <QFile>
#include <QTextStream>

class FileWriter : public QObject
{
    Q_OBJECT

public:
    //explicit FileWriter(/*QObject *parent = 0*/);
    void writeSummand();        //muss die so heißen oder kann es auch eine andere sein

signals:
    void writeFinish(double result);

public slots:    
    void outList();

};

#endif // FILEWRITER_H
