#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workwindow.h"
#include "registrationwindow.h"
#include <QPixmap>
#include "userwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    QPixmap logo("Pictures/Logo_DiaThesis.png");
    ui->picture_lb->setPixmap(logo);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_anmelden_btn_clicked()
{
    userwindow.show();
}

void MainWindow::on_registrieren_btn_clicked()
{
    registrationwindow.show();
}

void MainWindow::on_login_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
