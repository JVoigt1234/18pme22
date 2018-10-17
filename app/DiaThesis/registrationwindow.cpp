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

void RegistrationWindow::on_pushButton_clicked()
{}

void RegistrationWindow::on_back_btn_1_clicked()
{}

void RegistrationWindow::on_next_btn_1_clicked()
{
   ui->stackedWidget->setCurrentIndex(1);
}
