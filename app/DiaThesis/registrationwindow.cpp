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


void RegistrationWindow::on_back_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void RegistrationWindow::on_next_btn_1_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}


void RegistrationWindow::on_datenschutz_cb_stateChanged(int arg1)
{
    if ((arg1 == 2 )&& (ui->agb_cb->isChecked() == true))
    {
        ui->next_btn_2->setEnabled(true);
    }
    else
    {
        ui->next_btn_2->setEnabled(false);
    }
}

void RegistrationWindow::on_agb_cb_stateChanged(int arg1)
{
    if ((arg1 == 2 )&& (ui->datenschutz_cb->isChecked() == true))
    {
        ui->next_btn_2->setEnabled(true);
    }
    else
    {
        ui->next_btn_2->setEnabled(false);
    }
}
