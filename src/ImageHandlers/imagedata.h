#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "imagedataexception.h"
#include <QImage>
#include <QObject>
#include <QDebug>

class ImageData : public QObject
{
    Q_OBJECT
public:
    explicit ImageData(QObject *parent = nullptr);
    ImageData(ImageData const &copy);

    ImageData(QImage image);

    ~ImageData();

    int **getGrayScaleMatrix() const;

    const QImage getImage() const;

    int getHeight() const;

    int getWidth() const;

    void setImage(const QImage &newImage);

    ImageData operator=(ImageData &copy);

    void printData();
    void printLine(int i);

private:
    int **grayScaleMatrix = nullptr; // матрица серых пикселей
    int height = 0; // высота матрицы
    int width = 0;    // ширина матрицы
//    QImage image;   // изображение, данные которого мы храним

};

#endif // IMAGEDATA_H
