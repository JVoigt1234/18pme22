#ifndef WORKER
#define WORKER

#include <QObject>
#include <QMutex>
#include <qlisting.h>

class Worker : public QObject
{
    Q_OBJECT

    QMutex m_mute;

public:
    //explicit FileWriter(/*QObject *parent = 0*/);
    void writeSummand(){
            double euler = 0;

            for(int i = 170; i >= 0; i--){
                euler += (1/fakultaet(i));
                m_mute.lock();
                    qlisting::summanden << euler;
                m_mute.unlock();
            }

            emit writeFinish(euler);
    }

    double fakultaet(unsigned a)
    {
        if (a <= 1){
            return 1;
        }
        return a * fakultaet(a - 1);
    }

signals:
    void writeFinish(double result);

public slots:

};
#endif // WORKER

