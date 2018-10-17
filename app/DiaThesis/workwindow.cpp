#include "workwindow.h"
#include "ui_workwindow.h"

workwindow::workwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workwindow)
{
    ui->setupUi(this);
}

workwindow::~workwindow()
{
    delete ui;
}
