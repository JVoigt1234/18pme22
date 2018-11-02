#include "Scripts/UI/registrationwindow.h"
#include "ui_registrationwindow.h"
#include <QChar>
#include <QMessageBox>
#include <QDate>


RegistrationWindow::RegistrationWindow(DatabaseController* database, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationWindow)
{
    m_database = database;

    setWindowIcon(QIcon("Pictures/Logo_DiaThesis.png"));
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    //Initialisiere die Prüfvariablen für die Passwortprüfung
    pwHasCapitalLetter=false;
    pwHasSmallLetter=false;
    pwHasNumber=false;
    pwHasSixChars=false;

    //Set the RegularExpression for the Mail-window
    QRegularExpression rx("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b",
                          QRegularExpression::CaseInsensitiveOption);
    ui->user_le->setValidator(new QRegularExpressionValidator(rx, this));
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
    //Hier als allererstes alle Angaben auf Richtigkeit prüfen:
    //--------------------------------------------------------------
    alleAngabenRichtig = true;
    falscheFelder = "";

    //vorname
    if (ui->vorname_le->text().isEmpty())
    {
        alleAngabenRichtig=false;
        ui->vorname_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\tVorname\n";
    }
    else{ui->vorname_le->setStyleSheet("border: 1px solid grey");}

    //nachname
    if (ui->nachname_le->text().isEmpty())
    {
        alleAngabenRichtig=false;
        ui->nachname_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\tNachname\n";
    }
    else{ui->nachname_le->setStyleSheet("border: 1px solid grey");}

    //usertyp
    if (!(ui->Arzt_rb->isChecked()||ui->Patient_rb->isChecked()||ui->Angehoeriger_rb->isChecked()))
    {
        alleAngabenRichtig = false;
        ui->nutzertyp_gb->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\tBenutzertyp (Sind Sie Arzt, Patient oder Angehöriger?)\n";
    }
    else{ui->nutzertyp_gb->setStyleSheet("border: 1px solid grey");}

    //geburtstag
    QDate date;
    date = QDate::fromString(ui->geburtstag_le->text(),"yyyy.MM.dd");
    if (!date.isValid())
    {
        alleAngabenRichtig = false;
        ui->geburtstag_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\tGeburtstag\n";
    }
    else{ ui->geburtstag_le->setStyleSheet("border: 1px solid grey");}

    //email
    if(!ui->user_le->hasAcceptableInput())
    {
        alleAngabenRichtig = false;
        ui->user_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\tEmail\n";
    }
    else{ ui->user_le->setStyleSheet("border: 1px solid grey");}

    //Passwort identisch? Oder einer der beiden Eingaben leer??
    if (!(ui->password_le->text() == ui->password2_le->text()))
    {
        alleAngabenRichtig = false;
        ui->password_le->setStyleSheet("border: 1px solid red");
        ui->password2_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\nIhre Passwörter stimmen nicht überein!";
    }
    else if (ui->password_le->text().length() == 0 || ui->password2_le->text().length() == 0)
    {
        alleAngabenRichtig = false;
        ui->password_le->setStyleSheet("border: 1px solid red");
        ui->password2_le->setStyleSheet("border: 1px solid red");
        falscheFelder = falscheFelder + "\nBitte geben Sie Ihr Passwort zwei mal ein!";
    }
    else{ ui->password_le->setStyleSheet("border: 1px solid grey"); ui->password2_le->setStyleSheet("border: 1px solid grey");}

    //Erst hier wenn alle Angaben richtig waren,
    //______________________________________________________________________________________________
    if (alleAngabenRichtig)
    {
        //Erst dann neuen Benutzer anlegen und in die Datenbank schreiben
        User* newUser;
        //Benutzerprofil anlegen (User)
        if (ui->Arzt_rb->isChecked())
        {
            //newUser = new Doctor(ui->vorname_le, ui->nachname_le, UserType::doctor, ui->user_le, );
        }
        else if (ui->Patient_rb->isChecked())
        {
            newUser = new Patient(ui->vorname_le->text(), ui->nachname_le->text(), UserType::patient, ui->user_le->text(), ui->geburtstag_le->text());

            if(m_database->isUserAvailable(ui->user_le->text()) == false)
            {
                if(m_database->isUserCreated(newUser,ui->password_le->text()))
                {
                    qDebug() << "successful created";
                }
                else
                {
                    msgBox.setWindowTitle("Fehler beim Erstellen des Accounts");
                    msgBox.setText("Die E-Mail Adresse ist leider schon vergeben!");
                    msgBox.setStandardButtons(QMessageBox::Ok);
                    msgBox.show();
                }
            }
        }
        else if (ui->Angehoeriger_rb->isChecked())
        {
            //newUser = new Member(ui->vorname_le->text(), ui->nachname_le->text(), UserType::member,...... PATIENT RELEASE?!?! );
        }

        //Dieses Fenster schließen
        this->close(); //zuvor muss das neue Profil in der Datenbank gespeichert werden
    }
    else
    {
        //Warnung anzeigen, welche Felder nicht korrekt ausgefüllt wurden.

        msgBox.setWindowTitle("Fehler beim Erstellen des Accounts");
        msgBox.setText("Bitte die Angaben in den folgenden Feldern prüfen: \n"+falscheFelder);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.show();
    }


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

