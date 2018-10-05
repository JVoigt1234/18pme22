#include <QGuiApplication>
#include <QtQuick>

#include "patient.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    //Verbindung zwischen c++ und qml

    qmlRegisterType<Patient>("PatientenData", 1,0 , "Patient");

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");
    
    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
