#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "workwindow.h"
#include "registrationwindow.h"
#include <QPixmap>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    QPixmap logo("Z:/UI_branch/18pme22/app/DiaThesis/Pictures/Logo_DiaThesis.png");
    ui->picture_lb->setPixmap(logo);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_anmelden_btn_clicked()
{
    workwindow workwindow;
    workwindow.setModal(true);
    workwindow.exec();
}

void MainWindow::on_registrieren_btn_clicked()
{
    registrationwindow.show();
}
