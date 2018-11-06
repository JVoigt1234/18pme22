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

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    QWinTaskbarButton *button = new QWinTaskbarButton(this);
    button->setWindow(this->windowHandle());
    button->setOverlayIcon(QIcon("Pictures/Logo_DiaThesis.png"));

    setWindowIcon(QIcon("Pictures/Logo_DiaThesis.png"));
    ui->setupUi(this);

    QPixmap logo("Pictures/Logo_DiaThesis.png");
    ui->picture_lb->setPixmap(logo);

    ui->random_facts->setText(m_database->getFact());

    ui->stackedWidget->setCurrentIndex(0);

    m_timer->start(6000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_timeout()
{
   ui->stackedWidget->setCurrentIndex(1);
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
