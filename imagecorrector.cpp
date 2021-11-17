#include "imagecorrector.h"

ImageCorrector::ImageCorrector(QObject *parent) : QObject(parent)
{

}

void ImageCorrector::subtractObjectImage()
{
    for (int i=0; i<imageOriginal.width(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.height(); j++)    // проходим по высоте
        {
            int blackOriginal = imageOriginal.pixelColor(i, j).black(); // уровень черного в оригинале
            int blackObject = imageObject.pixelColor(i, j).black(); // уровень черного в объекте

            int blackSub = abs(blackOriginal - blackObject);    // вычитаем фон
            QColor color = blackColor(blackSub);    // формируем новый цвет пикселя

            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::clipNoise(int clippingNoiseValue)
{
    for (int i=0; i<imageOriginal.width(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.height(); j++)    // проходим по высоте
        {
            int black = resultImage.pixelColor(i, j).black();   // получаем уровень черного
            if(black < clippingNoiseValue)   // обрезаем шумы в соответсвии с указанным уровнем
            {
                black = 0;
            }
            QColor color = blackColor(black);    // формируем новый цвет пикселя
            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::enchanceBlackColor(int blackEnchancement)
{
    for (int i=0; i<imageOriginal.width(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.height(); j++)    // проходим по высоте
        {
            int black = resultImage.pixelColor(i, j).black();   // получаем уровень черного
            if(black > blackEnchancement)    // усиливаем черный цвет
            {
                black = 255;
            }
            QColor color = blackColor(black);    // формируем новый цвет пикселя
            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::invertPixels()
{
    resultImage.invertPixels();
}

void ImageCorrector::setPixelColor(QImage &image, int i, int j, QColor color)
{
    if(color.isValid())
    {
        image.setPixelColor(i, j, color); // устанавливаем нвоый цвет
    }
    else
    {
        qDebug() << "Not Valid Color: " << color << color.black();
    }
}

QColor ImageCorrector::blackColor(int grayValue)
{
    return QColor(grayValue, grayValue, grayValue);
}

QImage ImageCorrector::getResultImage() const
{
    return resultImage;
}

void ImageCorrector::setImageObject(const QImage &value)
{
    imageObject = value;
}

void ImageCorrector::setImageOriginal(const QImage &value)
{
    imageOriginal = value;
    resultImage = imageOriginal;
}
