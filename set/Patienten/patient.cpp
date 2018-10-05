#include "patient.h"

Patient::Patient(QObject *parent) : QObject(parent)
{

}



void Patient::setName(QString n)
{
    if(n == name)
        return;

    name = n;
    //emit nameChanged();
}

void Patient::setVorname(QString v)
{
    if(v == vorname)
        return;

    vorname = v;
    //emit vornameChanged();
}

void Patient::setSex(QString s)
{
    if(s == sex)
        return;

    sex = s;
    //emit sexChanged();
}

void Patient::setBirth(QString b)
{
    if(b == birth)
        return;

    birth = b;
    //emit birthChanged();
}

void Patient::saveData()
{
    QString filename = "C:/Users/Kevin_Costner1427728/Documents/Studium/3. Semster/Softwareentwicklungsmethoden und Tools/Praktikum/SetPraktikum/trunk/apps/Patienten/qml/PatientenDaten.txt";       // oder c:/euler.txt, c:\\euler.txt ???
    QFile file( filename );

    if ( file.open(QIODevice::ReadWrite | QIODevice::Text) )
    {
        QTextStream data( &file );
        /*for(int i = 0; i < patientendata.size(); i++)
            data << patientendata.at(i) << endl;*/
        data << "[" << endl << "{" << endl;

        data << "\"Name\": " << "\"" <<  name << "\"," << endl;
        data << "\"Vorname\": " << "\""  << vorname << "\","  << endl;
        data << "\"Geschlecht\": " << "\""  << sex << "\","  << endl;
        data << "\"Geburtstag\": " << "\""  << birth << "\""  << endl;

        data << "}" << endl << "]" << endl;
    }

}
