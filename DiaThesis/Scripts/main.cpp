    ///-----------------------------------------------------------------
/// Namespace:
/// Class:              main
/// Description:        Example how to use the DatabaseController
/// Author:             Kevin Kastner
/// Date:               Oct 2018
/// Notes:              -
/// Revision History:   First release
///-----------------------------------------------------------------

#include "Scripts/UI/mainwindow.h"
#include <QApplication>

#include "Scripts/Database/databasecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseController *data;
    data = new DatabaseController("db.inftech.hs-mannheim.de");

    MainWindow w(data);

    w.show();
    return a.exec();
}
