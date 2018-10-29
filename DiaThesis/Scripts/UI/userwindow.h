#ifndef USERWINDOW_H
#define USERWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QWidget>
#include <QWidgetItem>


namespace Ui {
class UserWindow;
}

class UserWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserWindow(QWidget *parent = nullptr);
    ~UserWindow();
    //QTabBar tb;


private slots:
    void setupTagesansicht(QCustomPlot *customPlot);
    void setupMonatsansicht(QCustomPlot *customPlot);
    void setupQuartalsansicht(QCustomPlot *customPlot);

    void on_actionProfileinstellungen_triggered();

    void on_actionDaten_aktualisieren_triggered();

    void on_actionTagesansicht_triggered();

    void on_actionMonatsansicht_triggered();

    void on_actionQuartalsansicht_triggered();

    void on_actionLogout_triggered();

private:
    Ui::UserWindow *ui;
};

#endif // USERWINDOW_H
