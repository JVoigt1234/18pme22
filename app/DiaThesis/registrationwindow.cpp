#include "registrationwindow.h"
#include "ui_registrationwindow.h"

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
}

RegistrationWindow::~RegistrationWindow()
{
    delete ui;
}

void RegistrationWindow::on_datenschutz_cb_stateChanged(int arg1)
{
    if ((arg1 == 2 )&& (ui->agb_cb->isChecked() == true))
    {
        ui->accountErstellen_btn->setEnabled(true);
    }
    else
    {
        ui->accountErstellen_btn->setEnabled(false);
    }
}

void RegistrationWindow::on_agb_cb_stateChanged(int arg1)
{
    if ((arg1 == 2 )&& (ui->datenschutz_cb->isChecked() == true))
    {
        ui->accountErstellen_btn->setEnabled(true);
    }
    else
    {
        ui->accountErstellen_btn->setEnabled(false);
    }
}

void RegistrationWindow::on_zurSeite1Reg_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void RegistrationWindow::on_accountErstellen_btn_clicked()
{
    this->close(); //zuvor muss das neue Profil in der Datenbank gespeichert werden
}



void RegistrationWindow::on_zurAnmeldung_btn_clicked()
{
    this->close();
}

void RegistrationWindow::on_seiteZweiReg_btn_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}
