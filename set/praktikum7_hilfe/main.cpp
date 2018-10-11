#include <QGuiApplication>
#include <QtQuick>
#include "eulerthread.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QScopedPointer<QQmlApplicationEngine> engine(new QQmlApplicationEngine);
    engine->addImportPath(":/");

    //qmlRegisterType<EulerThread>("eulerthread", 1,0, "Eulerthread"); //url,version,qml Name

    EulerThread eulerThread;

    engine->rootContext()->setContextProperty("eulerThread",&eulerThread);
    QObject::connect(&eulerThread,&EulerThread::newEuler,[&](QString result) {qDebug()<<result;});
    //eulerThread.start();

    engine->load(QUrl("qrc:/main.qml"));

    return app.exec();
}
