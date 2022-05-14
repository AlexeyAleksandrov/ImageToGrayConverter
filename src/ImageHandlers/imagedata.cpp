#include "imagedata.h"

#define blackColor(grayValue) QColor(grayValue, grayValue, grayValue)

ImageData::ImageData(QObject *parent)
    : QObject{parent}
{

}

ImageData::ImageData(const ImageData &copy)
{
    if(copy.grayScaleMatrix != nullptr)
    {
        this->grayScaleMatrix = new int *[copy.width];
        for(int i=0; i<copy.width; i++)
        {
            this->grayScaleMatrix[i] = new int [copy.height];
        }
        for(int i=0; i<copy.width; i++)
        {
            for(int j=0; j<copy.height; j++)
            {
                grayScaleMatrix[i][j] = copy.grayScaleMatrix[i][j];
            }
        }
        this->width = copy.width;
        this->height = copy.height;
    }
}

ImageData::ImageData(QImage image)
{
    setImage(image);
}

ImageData::~ImageData()
{
    if(grayScaleMatrix != nullptr)  // если матрица не пустая, то удалем её
    {
        for(int i=0; i<width; i++)
        {
            delete [] grayScaleMatrix [i];
        }
        delete [] grayScaleMatrix;
        grayScaleMatrix = nullptr;
    }
}

int **ImageData::getGrayScaleMatrix() const
{
    return grayScaleMatrix;
}

int **ImageData::getGrayScaleMatrixCopy()
{
    if(this->height <= 0 || this->width <= 0 || grayScaleMatrix == nullptr)
    {
        throw ImageDataException(QString("Некорректное значение высоты или ширины матрицы! ") + QString::number(height) + QString::number(width));
    }

    int **grayScaleMatrixCopy = new int *[width];    // выделяем память под строки
    for(int i=0; i<width; i++)
    {
        grayScaleMatrixCopy[i] = new int [height];   // выделяем память под столбцы
    }

    for(int i=0; i<width; i++) // заполняем данными из изображения
    {
        for(int j=0; j<height; j++)
        {
            grayScaleMatrixCopy[i][j] = grayScaleMatrix[i][j];
        }
    }

    return grayScaleMatrixCopy;
}

const QImage ImageData::getImage() const
{
    QImage image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);
    for(int i=0; i<width; i++)
    {
        for(int j=0; j<height; j++)
        {
            image.setPixelColor(i, j, blackColor(255 - grayScaleMatrix[i][j]));
        }
    }
    return image;
}

int ImageData::getHeight() const
{
    return height;
}

int ImageData::getWidth() const
{
    return width;
}

void ImageData::setImage(const QImage &newImage)
{
    int height = newImage.height();    // получаем данные изобржения
    int width = newImage.width();

    if(height <= 0 || width <= 0)
    {
        throw ImageDataException(QString("Некорректное значение высоты или ширины матрицы! ") + QString::number(height) + QString::number(width));
    }

    if(grayScaleMatrix != nullptr)  // если матрица не пустая, то удалем её
    {
        for(int i=0; i<width; i++)
        {
            delete [] grayScaleMatrix [i];
        }
        delete [] grayScaleMatrix;
        grayScaleMatrix = nullptr;
    }

    // если всё хорошо, то сохраняем значения
    this->height = height;
    this->width = width;

    grayScaleMatrix = new int *[width];    // выделяем память под строки
    for(int i=0; i<width; i++)
    {
        grayScaleMatrix[i] = new int [height];   // выделяем память под столбцы
    }

    for(int i=0; i<width; i++) // заполняем данными из изображения
    {
        for(int j=0; j<height; j++)
        {
            grayScaleMatrix[i][j] = newImage.pixelColor(i, j).black();
        }
    }
}

ImageData ImageData::operator=(ImageData &copy)
{
    ImageData next;
    if(copy.grayScaleMatrix != nullptr)
    {
        next.grayScaleMatrix = new int *[copy.width];
        for(int i=0; i<copy.width; i++)
        {
            next.grayScaleMatrix[i] = new int [copy.height];
        }
        for(int i=0; i<copy.width; i++)
        {
            for(int j=0; j<copy.height; j++)
            {
                next.grayScaleMatrix[i][j] = copy.grayScaleMatrix[i][j];
            }
        }
    }
    next.width = copy.width;
    next.height = copy.height;

    return next;
}

void ImageData::printData()
{
    for(int i=0; i<width; i++) // заполняем данными из изображения
    {
        qDebug() << "\r\n";
        QString debugtext = "i = " + QString::number(i) + " - ";
        for(int j=0; j<height; j++)
        {
            debugtext.append(QString::number(grayScaleMatrix[i][j]) + " ");
        }
        qDebug() << debugtext;
    }
}

void ImageData::printLine(int i)
{
    qDebug() << "\r\n";
    QString debugtext = "i = " + QString::number(i) + " - ";
    for(int j=0; j<height; j++)
    {
        debugtext.append(QString::number(grayScaleMatrix[i][j]) + " ");
    }
    qDebug() << debugtext;
}
