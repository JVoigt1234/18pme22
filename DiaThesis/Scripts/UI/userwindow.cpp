#include "userwindow.h"
#include "ui_userwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include "qcustomplot.h"
#include <QtMath>

UserWindow::UserWindow(DatabaseController* database, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UserWindow)
{
    m_database = database;

    setWindowIcon(QIcon("Pictures/Logo_DiaThesis.png"));
    ui->setupUi(this);
    ui->mainStackWidget->setCurrentIndex(0);
    ui->actionNutzername->setText(m_database->getPatientData().getForename() + " " + m_database->getPatientData().getSurname());
}

UserWindow::~UserWindow()
{
    delete ui;
}

void UserWindow::setupTagesansicht(QCustomPlot *Plot)
{
    // generate daily data:
    QVector<double> x(97), z(97), p(97), y(97);
    QList<Measurement> listMeasurement;

    //Minimaler und Maximaler Blutzuckerwert
    double max= m_database->getPatientData().getMaxBloodSugar();
    double min= m_database->getPatientData().getMinBloodSugar();


//    QDateTime to = QDateTime::currentDateTime();
//    QDateTime from = to.addDays(-1);
    QDateTime from = QDateTime::fromString("2018-10-10 15:00:10", TimeStampFormate);
    QDateTime to = QDateTime::fromString("2018-10-10 15:29:55", TimeStampFormate);

    m_database->getBloodSugar(from,to,listMeasurement);

    for (int i=0; i<97; i++)
    {
        z[i]= max;
        p[i]= min;
        x[i]= i/4;
        y[i]= listMeasurement[i].getValue().toDouble();
    }



    //Legende
    /*
    Plot->legend->setVisible(true);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    Plot->legend->setFont(legendFont);
    Plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:
    Plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);
*/
    // create graph and assign data to it: Blood Sugar
    Plot->addGraph();
    Plot->graph(0)->setPen(QPen(Qt::black));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setName("Blutzucker Tagestrend");
    Plot->graph(0)->setData(x,y);

    //Maximal Wert
    Plot->addGraph();
    QPen redDotPen;
    redDotPen.setColor(QColor(Qt::red));
    redDotPen.setStyle(Qt::DotLine);
    redDotPen.setWidthF(4);
    Plot->graph(1)->setPen(redDotPen);
    Plot->graph(1)->setBrush(QBrush(QColor(202,255,112,50)));
    Plot->graph(1)->setName("Maximalwert");
    Plot->graph(1)->setData(x,z);

    //Minimal Wert
    Plot->addGraph();
    Plot->graph(2)->setPen(redDotPen);
    Plot->graph(2)->setBrush(QBrush(QColor(255,48,48,70)));
    Plot->graph(2)->setName("Minimalwert");
    Plot->graph(2)->setData(x,p);

    Plot->graph(1)->setChannelFillGraph(Plot->graph(2));

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
    QVector<double> x(31), y(31), z(31), p(31);

    int max= 200;
    int min= 70;

    for (int i=0; i<31; ++i)
    {
      x[i] = i; // x goes from 0 to 30
      y[i] = x[i]*x[i];
      z[i] = max;
      p[i] = min;
    }

    // create graph and assign data to it: Blood Sugar
    Plot->addGraph();
    Plot->graph(0)->setPen(QPen(Qt::black));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setName("Blutzucker Monatstrend");
    Plot->graph(0)->setData(x,y);

    //Maximal Wert
    Plot->addGraph();
    QPen redDotPen;
    redDotPen.setColor(QColor(Qt::red));
    redDotPen.setStyle(Qt::DotLine);
    redDotPen.setWidthF(4);
    Plot->graph(1)->setPen(redDotPen);
    Plot->graph(1)->setBrush(QBrush(QColor(202,255,112,50)));
    //Plot->graph(1)->setChannelFillGraph(Plot->graph(2));
    Plot->graph(1)->setName("Maximalwert");
    Plot->graph(1)->setData(x,z);

    //Minimal Wert
    Plot->addGraph();
    Plot->graph(2)->setPen(redDotPen);
    Plot->graph(2)->setBrush(QBrush(QColor(255,48,48,70)));
    Plot->graph(2)->setName("Minimalwert");
    Plot->graph(2)->setData(x,p);

    Plot->graph(1)->setChannelFillGraph(Plot->graph(2));

    // give the axes some labels
    Plot->xAxis->setLabel("Tage");
    Plot->yAxis->setLabel("Blutzucker mg/dl");
    // set axes ranges
    Plot->xAxis->setRange(0, 30);
    Plot->yAxis->setRange(0, 300);
}

void UserWindow::setupQuartalsansicht(QCustomPlot *Plot)
{
    // generate quartals data:
    QVector<double> x(91), y(91), p(91), z(91);

    int max= 200;
    int min= 70;

    for (int i=0; i<91; ++i)
    {
      x[i] = i/30; // x goes from 0 to 90
      y[i] = 120+20*x[i];
      z[i] = max;
      p[i] = min;
    }

    // create graph and assign data to it: Blood Sugar
    Plot->addGraph();
    Plot->graph(0)->setPen(QPen(Qt::black));
    Plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    Plot->graph(0)->setName("Blutzucker Quartalstrend");
    Plot->graph(0)->setData(x,y);

    //Maximal Wert
    Plot->addGraph();
    QPen redDotPen;
    redDotPen.setColor(QColor(Qt::red));
    redDotPen.setStyle(Qt::DotLine);
    redDotPen.setWidthF(4);
    Plot->graph(1)->setPen(redDotPen);
    Plot->graph(1)->setBrush(QBrush(QColor(202,255,112,50)));
    //Plot->graph(1)->setChannelFillGraph(Plot->graph(2));
    Plot->graph(1)->setName("Maximalwert");
    Plot->graph(1)->setData(x,z);

    //Minimal Wert
    Plot->addGraph();
    Plot->graph(2)->setPen(redDotPen);
    Plot->graph(2)->setBrush(QBrush(QColor(255,48,48,70)));
    Plot->graph(2)->setName("Minimalwert");
    Plot->graph(2)->setData(x,p);

    Plot->graph(1)->setChannelFillGraph(Plot->graph(2));

    // give the axes some labels:
    Plot->xAxis->setLabel("Monate");
    Plot->yAxis->setLabel("Blutzucker mg/dl");
    // set axes ranges
    Plot->xAxis->setRange(0, 3);
    Plot->yAxis->setRange(0, 300);
}

//Mouse Interactions
void UserWindow::mousePressTag()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  if (ui->Tagesplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Tagesplot->axisRect()->setRangeDrag(ui->Tagesplot->xAxis->orientation());
  else if (ui->Tagesplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Tagesplot->axisRect()->setRangeDrag(ui->Tagesplot->yAxis->orientation());
  else
    ui->Tagesplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void UserWindow::mouseWheelTag()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  if (ui->Tagesplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Tagesplot->axisRect()->setRangeZoom(ui->Tagesplot->xAxis->orientation());
  else if (ui->Tagesplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Tagesplot->axisRect()->setRangeZoom(ui->Tagesplot->yAxis->orientation());
  else
    ui->Tagesplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void UserWindow::mousePressMonat()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  if (ui->Monatplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Monatplot->axisRect()->setRangeDrag(ui->Monatplot->xAxis->orientation());
  else if (ui->Monatplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Monatplot->axisRect()->setRangeDrag(ui->Monatplot->yAxis->orientation());
  else
    ui->Monatplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void UserWindow::mouseWheelMonat()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  if (ui->Monatplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Monatplot->axisRect()->setRangeZoom(ui->Monatplot->xAxis->orientation());
  else if (ui->Monatplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Monatplot->axisRect()->setRangeZoom(ui->Monatplot->yAxis->orientation());
  else
    ui->Monatplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void UserWindow::mousePressQuartal()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  if (ui->Quartalsplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Quartalsplot->axisRect()->setRangeDrag(ui->Quartalsplot->xAxis->orientation());
  else if (ui->Quartalsplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Quartalsplot->axisRect()->setRangeDrag(ui->Quartalsplot->yAxis->orientation());
  else
    ui->Quartalsplot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void UserWindow::mouseWheelQuartal()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  if (ui->Quartalsplot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Quartalsplot->axisRect()->setRangeZoom(ui->Quartalsplot->xAxis->orientation());
  else if (ui->Quartalsplot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->Quartalsplot->axisRect()->setRangeZoom(ui->Quartalsplot->yAxis->orientation());
  else
    ui->Quartalsplot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

//Menübar Interactions
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
    ui->Tagesplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    setupTagesansicht(ui->Tagesplot);
    //connect(ui->Tagesplot, SIGNAL(mousePressTag(QMouseEvent*)), this, SLOT(mousePressTag()));
    //connect(ui->Tagesplot, SIGNAL(mouseWheelTag(QWheelEvent*)), this, SLOT(mouseWheelTag()));
    ui->mainStackWidget->setCurrentIndex(2);
}

void UserWindow::on_actionMonatsansicht_triggered()
{
    ui->Monatplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    setupMonatsansicht(ui->Monatplot);
    //connect(ui->Monatplot, SIGNAL(mousePressMonat(QMouseEvent*)), this, SLOT(mousePressMonat()));
    //connect(ui->Monatplot, SIGNAL(mouseWheelMonat(QWheelEvent*)), this, SLOT(mouseWheelMonat()));
    ui->mainStackWidget->setCurrentIndex(3);
}

void UserWindow::on_actionQuartalsansicht_triggered()
{
    ui->Quartalsplot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                      QCP::iSelectLegend | QCP::iSelectPlottables);
    setupQuartalsansicht(ui->Quartalsplot);
    //connect(ui->Quartalsplot, SIGNAL(mousePressQuartal(QMouseEvent*)), this, SLOT(mousePressQuartal()));
    //connect(ui->Quartalsplot, SIGNAL(mouseWheelQuartal(QWheelEvent*)), this, SLOT(mouseWheelQuartal()));
    ui->mainStackWidget->setCurrentIndex(4);
}

void UserWindow::on_actionLogout_triggered()
{
    this->close();
}
