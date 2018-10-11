#ifndef IMAGESCALING_H
#define IMAGESCALING_H

#include <QObject>

class ImageScaling : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int width READ getwidth WRITE setwidth NOTIFY widthChanged)
    Q_PROPERTY(int height READ getheight WRITE setheight NOTIFY heightChanged)

    int width;
    int height;

public:
    explicit ImageScaling(QObject *parent = 0);

    void setwidth(int w);
    void setheight(int h);

    int getwidth();
    int getheight();

signals:
    void widthChanged();
    void heightChanged();

public slots:


};

#endif // IMAGESCALING_H
