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

private:
    Ui::workwindow *ui;
};

#endif // WORKWINDOW_H
