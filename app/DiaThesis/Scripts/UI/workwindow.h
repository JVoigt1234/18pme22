#ifndef WORKWINDOW_H
#define WORKWINDOW_H

#include <QDialog>

namespace Ui {
class workwindow;
}

class workwindow : public QDialog
{
    Q_OBJECT

public:
    explicit workwindow(QWidget *parent = nullptr);
    ~workwindow();

private slots:
    void on_diary_btn_clicked();

    void on_patient_btn_clicked();

    void on_diary_btn_2_clicked();

private:
    Ui::workwindow *ui;
};

#endif // WORKWINDOW_H
