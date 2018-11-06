#ifndef REGISTRATIONWINDOW_H
#define REGISTRATIONWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <Scripts/Database/databasecontroller.h>
#include <Scripts/Database/Databasetyps.h>

namespace Ui {
class RegistrationWindow;
}

class RegistrationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegistrationWindow(DatabaseController* database, QWidget *parent = nullptr);
    ~RegistrationWindow();
    //Variable zur Prüfung der eingegebenen Daten zur Registrierung
    bool alleAngabenRichtig = true;
    //Messagebox zur Warnung bei falsch oder nicht ausgefüllten Feldern
    QMessageBox msgBox;
    //Nachricht über falsch oder nicht ausgefüllte Felder
    QString falscheFelder;

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

    DatabaseController* m_database;

    bool pwHasCapitalLetter, pwHasSmallLetter, pwHasNumber, pwHasSixChars;
};

#endif // REGISTRATIONWINDOW_H
