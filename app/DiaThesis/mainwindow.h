#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "registrationwindow.h"
#include <QMainWindow>

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

private slots:
    void on_anmelden_btn_clicked();

    void on_registrieren_btn_clicked();

    void on_login_btn_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
