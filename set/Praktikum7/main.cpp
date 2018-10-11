#include <QGuiApplication>
#include <QtQuick>
#include "eulerthread.h"

#include "filewriter.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");

    //für die Verbindung zwischen c++-Klasse und main.qml
    qmlRegisterType<EulerThread>("EulerThread", 1,0, "EulerThread");      //in < > muss der Klassenname stehen

    //um Thread zu starten
    EulerThread eulerThread;

    engine->rootContext()->setContextProperty( "eulerThread", &eulerThread);
    //QObject::connect( &eulerThread, EulerThread::eulerFinish, [&](QString erg){qDebug() << erg; eulerThread.exit(); } /*, &MyThread::euler()*/  );

    //myThread.start();


    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
