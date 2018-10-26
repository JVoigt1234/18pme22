#include "workwindow.h"
#include "ui_workwindow.h"

workwindow::workwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::workwindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

}

workwindow::~workwindow()
{
    delete ui;
}

void workwindow::on_diary_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void workwindow::on_patient_btn_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void workwindow::on_diary_btn_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
