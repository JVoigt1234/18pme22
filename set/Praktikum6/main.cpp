#include <QGuiApplication>
#include <QtQuick>
#include "numberconverter.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<NumberConverter>("NumberConvert",1,0, "NumberConverter");

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");
    
    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
