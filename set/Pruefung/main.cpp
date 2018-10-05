#include <QGuiApplication>
#include <QtQuick>

#include "fisibubele.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    //Verbindung zwischen C++ und qml

    qmlRegisterType<Fisibubele>("Fisibubele",1,0,"Pruefung");

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");
    
    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
