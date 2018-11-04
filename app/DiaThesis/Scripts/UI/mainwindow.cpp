#include "Scripts/UI/mainwindow.h"
#include "ui_mainwindow.h"
#include "Scripts/UI/workwindow.h"
#include "Scripts/UI/registrationwindow.h"
#include <QPixmap>
#include "Scripts/UI/userwindow.h"

#include <QWinTaskbarButton>
#include <QIcon>

MainWindow::MainWindow(DatabaseController* database,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    m_database = database;

    m_userwindow = new UserWindow(m_database);
    m_registrationwindow = new RegistrationWindow(m_database);

    QWinTaskbarButton *button = new QWinTaskbarButton(this);
    button->setWindow(this->windowHandle());
    button->setOverlayIcon(QIcon("Pictures/Logo_DiaThesis.png"));

    setWindowIcon(QIcon("Pictures/Logo_DiaThesis.png"));
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
    if(m_database->isValidUser(ui->mail_le->text(),ui->passwort_le->text()) == UserType::patient)
    {
        m_userwindow->show();
        this->close();
    }
}

void MainWindow::on_registrieren_btn_clicked()
{
    m_registrationwindow->show();
}

void MainWindow::on_login_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}
