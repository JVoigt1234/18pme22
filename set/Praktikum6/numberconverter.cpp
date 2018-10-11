#include "numberconverter.h"

NumberConverter::NumberConverter(QObject *parent) : QObject(parent)
{
    //bin = dec = oct = hex = "0000";
}

//get-Funktionen
QString NumberConverter::getbin()
{
    return bin;
}

QString NumberConverter::getdec()
{
    return dec;
}

QString NumberConverter::getoct()
{
    return oct;
}

QString NumberConverter::gethex()
{
    return hex;
}

//set-Funktionen
void NumberConverter::setBin(QString m_b)
{
    m_b.remove('.');
    m_b.remove(' ');
    bool ok = false;

    int checkbin = m_b.toInt(&ok,2);

    if(!ok){
        qWarning() << "Wrong input by Binary";
        return;
    }

    update(checkbin);
    qDebug() << "Successful convert from Binary";

}

void NumberConverter::setDec(QString m_d)
{
    m_d.remove(' ');

    bool ok = false;

    int checkdec = m_d.toInt(&ok,10);

    if(!ok){
        qWarning() << "Wrong input by Decimal";
        return;
    }

    update(checkdec);
    qDebug() << "Successful convert from Decimal";

}

void NumberConverter::setOct(QString m_o)
{
    m_o.remove(' ');

    bool ok = false;

    int checkoct = m_o.toInt(&ok,8);

    if(!ok){
        qWarning() << "Wrong input by Octal";
        return;
    }

    update(checkoct);
    qDebug() << "Successful convert from Octal";

}

void NumberConverter::setHex(QString m_h)
{
    m_h.remove(' ');

    bool ok = false;

    int checkhex = m_h.toInt(&ok,16);

    if(!ok){
        qWarning() << "Wrong input by Hexadecimal";
        return;
    }

    update(checkhex);
    qDebug() << "Successful convert from Hexadecimal";

}

//Funktion die sobald sich eine Eingabe ändert alle anderen Zahlensystem aktualisiert
void NumberConverter::update(int numberConversion)
{
    QString help = QString::number(numberConversion,2);
    if(help != bin){
        formatBin(help);
        bin = help;
        emit binChanged();
    }

    help = QString::number(numberConversion,10);
    if(help != dec){
        dec = help;
        emit decChanged();
    }

    help = QString::number(numberConversion, 8);
    if(help != oct){
        formatOct(help);
        oct = help;
        emit octChanged();
    }

    help = QString::number(numberConversion,16).toUpper();
    if(help != hex){
        hex = help;
        emit hexChanged();
    }
    
}


//Funktion zur gewünschten Darstellung von Binärmodus
void NumberConverter::formatBin(QString &bin)
{
    bool point = true;

    //wieviele Nullen fehlen
    while (bin.size()%4 ) {
        bin.prepend("0");
    }

    for(int len = bin.size()-4; len >= 4; len -=4){ //-4 deswegen weil sonst am anfang mit len = 20

        if(!point){
            bin.insert(len, " ");
            point = true;
            continue;
        }
        bin.insert(len, ".");
        point = false;

    }
}

void NumberConverter::formatOct(QString &oct)
{

    //wieviele Nullen fehlen
    while ( oct.size()%3 ) {
        oct.prepend("0");
    }

    for(int len = oct.size()-3; len >= 3; len -= 3){
        oct.insert(len, " ");
    }
}

