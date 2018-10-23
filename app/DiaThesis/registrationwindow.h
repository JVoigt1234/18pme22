#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QMainWindow>

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistrationWindow(QWidget *parent = nullptr);
    ~RegistrationWindow();

private slots:

    void on_back_btn_2_clicked();

    void on_next_btn_1_clicked();


    void on_datenschutz_cb_stateChanged(int arg1);

    void on_agb_cb_stateChanged(int arg1);

    void on_back_btn_1_clicked();

    void on_next_btn_2_clicked();

private:
    Ui::RegistrationWindow *ui;
};

#endif // REGISTRATIONWINDOW_H
