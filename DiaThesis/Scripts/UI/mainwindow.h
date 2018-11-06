#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "registrationwindow.h"
#include <QMainWindow>
#include "Scripts/UI/userwindow.h"
#include <QTimer>
#include "Scripts/UI/qcustomplot.h"

#include <Scripts/Database/databasecontroller.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DatabaseController *database,QWidget *parent = nullptr);
    ~MainWindow();

public slots:

    void on_timeout();

private slots:
    void on_anmelden_btn_clicked();

    void on_registrieren_btn_clicked();

private:
    Ui::MainWindow *ui;

    DatabaseController* m_database;

    RegistrationWindow* m_registrationwindow;
    UserWindow* m_userwindow;
    QTimer* m_timer;
};

#endif // MAINWINDOW_H
