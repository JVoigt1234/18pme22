#include "filewriter.h"

/*FileWriter::FileWriter(QObject *parent) : QObject(parent)
{
}*/

void FileWriter::outList()
{


    /*if(summanden.empty() == true){
        qDebug() << "Liste ist leer.";
        return;
    }

    qDebug() << "Anz Inhalt:" << summanden.size() << "Aufruf:" << j;*/

    QString filename = "C:/Users/Kevin_Costner1427728/Documents/Studium/3. Semster/Softwareentwicklungsmethoden und Tools/Praktikum/SetPraktikum/trunk/apps/Praktikum7/qml/EulerSummanden.txt";       // oder c:/euler.txt, c:\\euler.txt ???
    QFile file( filename );
    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        QTextStream out( &file );

         m_mute.lock();

             for(int i = 0; i < EulerThread::summanden.size(); i++)
             out << EulerThread::summanden.at(i) << endl;

         m_mute.unlock();
    }


}

