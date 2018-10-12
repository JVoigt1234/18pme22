#include <QGuiApplication>
#include <QtQuick>

#include "imagescaling.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    //Verbindung zwischen c++ und qml
     qmlRegisterType<ImageScaling>("ImageScaling", 1,0, "ImageScaling"); //sollte hier am besten stehen nicht unter engine->load(QUrl.....)

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");
    
    engine->load(QUrl("qrc:/main.qml"));



    return app.exec();
}
