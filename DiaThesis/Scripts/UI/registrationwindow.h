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

    void on_datenschutz_cb_stateChanged(int arg1);

    void on_agb_cb_stateChanged(int arg1);

    void on_zurSeite1Reg_btn_clicked();

    void on_accountErstellen_btn_clicked();

    void on_zurAnmeldung_btn_clicked();

    void on_seiteZweiReg_btn_clicked();

    void on_password_le_textChanged(const QString &arg1);

private:
    Ui::RegistrationWindow *ui;

    bool pwHasCapitalLetter, pwHasSmallLetter, pwHasNumber, pwHasSixChars;
};

#endif // REGISTRATIONWINDOW_H
