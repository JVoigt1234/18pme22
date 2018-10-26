#include "userwindow.h"
#include "ui_userwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "qcustomplot.h"

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

void UserWindow::setupTagesansicht(QCustomPlot *Plot)
{
    // generate daily data:
    QVector<double> x(96), y(96); // initialize with entries 0..100
    for (int i=0; i<96; ++i)
    {
        x[i] = i/4; // x goes from -1 to 1
        y[i] = x[i]*x[i];  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    Plot->addGraph();
    Plot->graph(0)->setData(x, y);
    // give the axes some labels:
    Plot->xAxis->setLabel("Uhrzeit");
    Plot->yAxis->setLabel("Blutzucker mg/dl");
    // set axes ranges, so we see all data:
    Plot->xAxis->setRange(0, 24);
    Plot->yAxis->setRange(0, 300);
}

void UserWindow::setupMonatsansicht(QCustomPlot *Plot)
{
    // generate month data:
    QVector<double> x(30), ymax(30), ymin(30); // initialize with entries 0..100
    for (int i=0; i<30; ++i)
    {
      x[i] = i; // x goes from -1 to 1
      ymax[i] = x[i]*x[i];  // let's plot a quadratic function
      ymin[i] = 1/2*(x[i]*x[i]);  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    Plot->addGraph();
    Plot->graph(0)->setData(x, ymax);
    Plot->addGraph();
    Plot->graph(1)->setData(x, ymin);
    // give the axes some labels:
    Plot->xAxis->setLabel("Tage");
    Plot->yAxis->setLabel("Blutzucker mg/dl");
    // set axes ranges, so we see all data:
    Plot->xAxis->setRange(0, 30);
    Plot->yAxis->setRange(0, 300);
}

void UserWindow::setupQuartalsansicht(QCustomPlot *Plot)
{
    // generate quartals data:
    QVector<double> x(90), y(90); // initialize with entries 0..100
    for (int i=0; i<90; ++i)
    {
      x[i] = i/30; // x goes from -1 to 1
      y[i] = x[i]*0.3;  // let's plot a quadratic function
    }
    // create graph and assign data to it:
    Plot->addGraph();
    Plot->graph(0)->setData(x, y);
    // give the axes some labels:
    Plot->xAxis->setLabel("Monate");
    Plot->yAxis->setLabel("Blutzucker mg/dl");
    // set axes ranges, so we see all data:
    Plot->xAxis->setRange(0, 3);
    Plot->yAxis->setRange(0, 300);
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
    setupTagesansicht(ui->Tagesplot);
    ui->mainStackWidget->setCurrentIndex(2);
}

void UserWindow::on_actionMonatsansicht_triggered()
{
    setupMonatsansicht(ui->Monatplot);
    ui->mainStackWidget->setCurrentIndex(3);
}

void UserWindow::on_actionQuartalsansicht_triggered()
{
    setupQuartalsansicht(ui->Quartalsplot);
    ui->mainStackWidget->setCurrentIndex(4);
}

void UserWindow::on_actionLogout_triggered()
{
    this->close();
}
