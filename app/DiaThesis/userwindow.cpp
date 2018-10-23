#include "userwindow.h"
#include "ui_userwindow.h"

UserWindow::UserWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow)
{
    ui->setupUi(this);
    ui->mainStackWidget->setCurrentIndex(0);

}

UserWindow::~UserWindow()
{
    delete ui;
}


void UserWindow::on_actionProfileinstellungen_triggered()
{
    ui->mainStackWidget->setCurrentIndex(0);
}

void UserWindow::on_actionDaten_aktualisieren_triggered()
{
    ui->mainStackWidget->setCurrentIndex(1);
}

void UserWindow::on_actionTagesansicht_triggered()
{
    ui->mainStackWidget->setCurrentIndex(2);
}

void UserWindow::on_actionMonatsansicht_triggered()
{
    ui->mainStackWidget->setCurrentIndex(3);
}

void UserWindow::on_actionQuartalsansicht_triggered()
{
    ui->mainStackWidget->setCurrentIndex(4);
}

void UserWindow::on_actionLogout_triggered()
{
    this->close();
}
