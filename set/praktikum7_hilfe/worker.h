#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QMutex>
#include <QTextStream>
#include <qlisting.h>


class Worker : public QObject
{
    Q_OBJECT

    QMutex m_mutex;

public:
    void work()
    {
        double e=0.0;
        unsigned int listPosition =0;
        QTextStream debugOut(stdout);
        for(int i=170; i>=0; i--)
        {
           e=e+(1/fakultaet(i));    //berechnet Eulerzahl
           m_mutex.lock();      //sperrt Zugriff das nebenläufige Prozesse nicht gleichzeitig auf Daten zugreifen
           qlisting::sumList.append(e);     //speichert Summanden in einer Liste
           debugOut << qlisting::sumList.at(listPosition++) << '\n'; // Debug Ausgabe, zählt durch die Liste
           //und gibt einzelne Summanden aus
           m_mutex.unlock();    // Zugriffssperre wird aufgehoben
        }

        emit result(e);
    }

    double fakultaet(unsigned int zahl)     //Berechnung der Fakultät
    {
        if (zahl <= 1)
        {
            return 1; // Die Fakultät von 0 und 1 ist als 1 definiert.
        }
        return fakultaet(zahl - 1) * zahl;
    }



signals:
     void result(double result);    //QString


public slots:
};

#endif // WORKER_H
