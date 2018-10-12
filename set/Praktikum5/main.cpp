#include <QGuiApplication>
#include <QtQuick>
#include <QtCore>
#include "powersupply.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<PowerSupply>("Power",1,0, "PowerSupply");
    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");
    
    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
