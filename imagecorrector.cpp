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
            int blackOriginal = getPixelBlackValue(imageOriginal, i, j); // уровень черного в оригинале
            int blackObject = getPixelBlackValue(imageObject, i, j); // уровень черного в объекте

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
            int black = getPixelBlackValue(resultImage, i, j);   // получаем уровень черного
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
            int black = getPixelBlackValue(resultImage, i, j);   // получаем уровень черного
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

void ImageCorrector::hardClipNoise(int border, NoiseDeleteTypes type)
{
    for (int i=1; i<imageOriginal.width()-1; i++) // проходим по ширине
    {
        for (int j=1; j<imageOriginal.height()-1; j++)    // проходим по высоте
        {
            int black = getPixelBlackValue(resultImage, i, j);   // получаем уровень черного

            if(black <= border) // если цвет попадает под границу для сглаживания шума
            {
                auto correctMatrix = getCorrectDataAroundPixels(resultImage, i, j, border); // получаем матрицу коорректности

                // слабое удаление (8 пикселей, белый)
                if(type == LOW)
                {
                    if(correctMatrix[0][0] && correctMatrix[0][1] && correctMatrix[0][2]
                             && correctMatrix[1][0]  && correctMatrix[1][2]
                             && correctMatrix[2][0] && correctMatrix[2][1] && correctMatrix[2][2])  // если под условия подходят все значения вокруг
                    {
                        setPixelColor(resultImage, i, j, blackColor(WHITE_GRAY_LEVEL));    // устанавливаем белый цвет пикселю
                    }
                }

                // среднее удаление (4 пикселя, белый)
                if(type == MEDIUM)
                {
                    // крестом
                    if(correctMatrix[0][0] && correctMatrix[0][2]
                             && correctMatrix[2][0] && correctMatrix[2][2])  // если под условия подходят все значения вокруг, расположенные по диагонале
                    {
                        setPixelColor(resultImage, i, j, blackColor(WHITE_GRAY_LEVEL));    // устанавливаем белый цвет пикселю
                    }
                    // плюсом
                    if(correctMatrix[0][1]
                             && correctMatrix[1][0]  && correctMatrix[1][2]
                             && correctMatrix[2][1])  // если под условия подходят все значения вокруг, расположенные слева, справа, сверху и снизу
                    {
                        setPixelColor(resultImage, i, j, blackColor(WHITE_GRAY_LEVEL));    // устанавливаем белый цвет пикселю
                    }
                }


                // сильное удаление (2 пикселя, белый)
                if(type == HIGH)
                {
                    // горизонталь
                    if(correctMatrix[1][0] && correctMatrix[1][2])  // если под условия подходят все значения, расположенные слева и справа
                    {
                        setPixelColor(resultImage, i, j, blackColor(WHITE_GRAY_LEVEL));    // устанавливаем белый цвет пикселю
                    }
                    // вертикаль
                    if(correctMatrix[0][1] && correctMatrix[2][1])  // если под условия подходят все значения, расположенные сверху и снизу
                    {
                        setPixelColor(resultImage, i, j, blackColor(WHITE_GRAY_LEVEL));    // устанавливаем белый цвет пикселю
                    }
                }
            }
        }
    }
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

int ImageCorrector::getPixelBlackValue(QImage &image, int i, int j)
{
    return image.pixelColor(i, j).black();
}

QColor ImageCorrector::blackColor(int grayValue)
{
    return QColor(grayValue, grayValue, grayValue);
}

QVector<QVector<bool>> ImageCorrector::getCorrectDataAroundPixels(QImage &image, int i, int j, int border)
{
    QVector<QVector<bool>> matrix;  // матрица соответствия
    matrix.resize(3);   // задаем кол-во строк
    for(auto &&item : matrix)
    {
        item.resize(3); // задаем кол-во столбцов
    }

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            // i + (h-1) даст нам значения -1 0 +1
            int black = getPixelBlackValue(image, i + (k-1), j + (h-1));    // берем информацию о пикселях вокруг
            bool isCorrect = (black <= border);  // результат корректности
            matrix[k][h] = isCorrect;   // заносим данные в матрицу
        }
    }
    return matrix;
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
