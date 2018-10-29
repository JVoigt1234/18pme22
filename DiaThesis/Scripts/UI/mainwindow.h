#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "registrationwindow.h"
#include <QMainWindow>
#include "userwindow.h"
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
     RegistrationWindow registrationwindow;
     UserWindow userwindow;
     QTimer m_timer;

private slots:
    void on_anmelden_btn_clicked();

    void on_registrieren_btn_clicked();

    void on_login_btn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
