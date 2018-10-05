#ifndef FILEWRITER_H
#define FILEWRITER_H
#include <QFile>
#include <QTextStream>
#include <iostream>
#include <QMutex>
#include <qlisting.h>


class fileWriter : public QObject
{

    Q_OBJECT

    QMutex m_mutex;

public:
    void writef()
    {
        QString filename = "C:/Users/Marius/Documents/Marius HS Mannheim/MTB 3. Semester/Softwareentwicklugsmethoden und Tools/SET Praktikum/trunk/apps/praktikum7/Ergebnis.txt";
            QFile file(filename);
            if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {      // QIODevice::Text allows line feed by using "endl"
                QTextStream stream(&file);

                m_mutex.lock();
                for(int i=0; i<qlisting::sumList.length(); i++)
                    stream << qlisting::sumList.at(i) << endl;
                m_mutex.unlock();
            }


    }

signals:
};

#endif // FILEWRITER_H
