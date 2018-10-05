#include "imagescaling.h"

ImageScaling::ImageScaling(QObject *parent) : QObject(parent)
{
    width = 0;
    height = 0;
}

void ImageScaling::setwidth(int w){

    if(width == w)
        return;

    width = w;
    emit widthChanged();
}

void ImageScaling::setheight(int h){

    if(height == h)
        return;

    height = h;
    emit heightChanged();
}

int ImageScaling::getwidth(){

    return width;
}

int ImageScaling::getheight(){

    return height;
}

