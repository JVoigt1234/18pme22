#include "Scripts/UI/registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QChar>

RegistrationWindow::RegistrationWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    pwHasCapitalLetter=false;
    pwHasSmallLetter=false;
    pwHasNumber=false;
    pwHasSixChars=false;
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

void RegistrationWindow::on_password_le_textChanged(const QString &arg1)
{
    pwHasCapitalLetter=false;
    pwHasNumber=false;
    pwHasSixChars=false;
    pwHasSmallLetter=false;
    ui->PwHint_lb->text()="";
  for (int i = 0; i<arg1.length(); i++)
  {
      if (arg1.at(i).isLower())
      {
          pwHasSmallLetter=true;
      }
      if(arg1.at(i).isUpper())
      {
          pwHasCapitalLetter=true;
      }
      if(arg1.at(i).isNumber())
      {
          pwHasNumber=true;
      }
      if (arg1.length()>=6)
      {
          pwHasSixChars=true;
      }
  } //hier koplettes einegegbenes Passwort geprüft

  if (pwHasCapitalLetter&&pwHasNumber&&pwHasSixChars&&pwHasSmallLetter)
  {
      ui->PwHint_lb->setStyleSheet("QLabel { background-color : green;}");
  }
  else
  {
      ui->PwHint_lb->setStyleSheet("QLabel { background-color : red;}");
  }

}

