#include "imagecorrector.h"

ImageCorrector::ImageCorrector(QObject *parent) : QObject(parent)
{

}

void ImageCorrector::substractObjectImage()
{
    auto imageOriginalMatrix = imageOriginal.getGrayScaleMatrix();  // матрица пикселей исходного изображения
    auto imageObjectMatrix = imageObject.getGrayScaleMatrix();  // матрица пикселей изображения объекта
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
        {
//            int blackOriginal = getPixelBlackValue(imageOriginal, i, j); // уровень черного в оригинале
//            int blackObject = getPixelBlackValue(imageObject, i, j); // уровень черного в объекте

            int blackOriginal = imageOriginalMatrix[i][j];  // уровень черного в оригинале
            int blackObject = imageObjectMatrix[i][j];  // уровень черного в объекте

            int blackSub = abs(blackOriginal - blackObject);    // вычитаем фон

//            QColor color = blackColor(blackSub);    // формируем новый цвет пикселя
//            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя

            imageResultMatrix[i][j] = blackSub; // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::clipNoise(int clippingNoiseValue)
{
//    auto imageOriginalMatrix = imageOriginal.getGrayScaleMatrix();  // матрица пикселей исходного изображения
//    auto imageObjectMatrix = imageObject.getGrayScaleMatrix();  // матрица пикселей изображения объекта
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
        {
            int black = imageResultMatrix[i][j];   // получаем уровень черного
//            if(i >= 230 && i <= 240)
//            {
//                int a = black;
//                resultImage.printLine(i);
//            }
            if(black < clippingNoiseValue)   // обрезаем шумы в соответсвии с указанным уровнем
            {
                black = 0;
            }

            imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя

//            QColor color = blackColor(black);    // формируем новый цвет пикселя
//            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::enchanceBlackColor(int blackEnchancement)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
        {
            int black = imageResultMatrix[i][j];   // получаем уровень черного
            if(black > blackEnchancement)    // усиливаем черный цвет
            {
                black = 255;
            }

//            QColor color = blackColor(black);    // формируем новый цвет пикселя
//            setPixelColor(resultImage, i, j, color);    // устанавливаем цвет пикселя

            imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::invertPixels()
{
    QImage img = resultImage.getImage();
    img.invertPixels();
    resultImage.setImage(img);

//    resultImage.invertPixels();
}

void ImageCorrector::hardClipNoise(int border, NoiseDeleteTypes type, NoiseDeleteColors colorType)
{
//    QImage startResultImage = resultImage; // сохраняем
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=1; i<imageOriginal.getWidth()-1; i++) // проходим по ширине
    {
        for (int j=1; j<imageOriginal.getHeight()-1; j++)    // проходим по высоте
        {
//            int black = getPixelBlackValue(startResultImage, i, j);   // получаем уровень черного
            int black = imageResultMatrix[i][j];    // получаем уровень черного

            if(black <= border) // если цвет попадает под границу для сглаживания шума
            {
                bool (*copmarator)(int comparedValue, int borderValue) = nullptr;   // функция сравнения значения с границей
                int replasePixelColorGrayLevel = 0; // цвет, на который будет заменён цвет пикселя в случае сглаживания
                if(colorType == WHITE)  // условие для белового цвета
                {
                    copmarator = [](int comparedValue, int borderValue)
                    {
                        return comparedValue <= borderValue;    // значение сранивается на <=
                    };
//                    replasePixelColorGrayLevel = WHITE_GRAY_LEVEL;
                    replasePixelColorGrayLevel = BLACK_GRAY_LEVEL;
                }
                else // условие сравнения для чёрного цвета
                {
                    copmarator = [](int comparedValue, int borderValue)
                    {
                        return comparedValue > borderValue; // значения сравниваются в большую сторону
                    };
//                    replasePixelColorGrayLevel = BLACK_GRAY_LEVEL;
                    replasePixelColorGrayLevel = WHITE_GRAY_LEVEL;
                }
//                auto correctMatrix = getCorrectDataAroundPixels(resultImage, i, j, border, copmarator); // получаем матрицу коорректности
                auto correctMatrix = getCorrectDataAroundPixels(imageResultMatrix, i, j, border, copmarator);   // получаем матрицу коорректности

                // слабое удаление (8 пикселей, белый)
                if(type == LOW)
                {
                    if(correctMatrix[0][0] && correctMatrix[0][1] && correctMatrix[0][2]
                             && correctMatrix[1][0]  && correctMatrix[1][2]
                             && correctMatrix[2][0] && correctMatrix[2][1] && correctMatrix[2][2])  // если под условия подходят все значения вокруг
                    {
//                        setPixelColor(resultImage, i, j, blackColor(replasePixelColorGrayLevel));    // устанавливаем белый цвет пикселю
                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                    }
                }

                // среднее удаление (4 пикселя, белый)
                if(type == MEDIUM)
                {
                    // крестом
                    if(correctMatrix[0][0] && correctMatrix[0][2]
                             && correctMatrix[2][0] && correctMatrix[2][2])  // если под условия подходят все значения вокруг, расположенные по диагонале
                    {
//                        setPixelColor(resultImage, i, j, blackColor(replasePixelColorGrayLevel));    // устанавливаем белый цвет пикселю
                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                    }
                    // плюсом
                    if(correctMatrix[0][1]
                             && correctMatrix[1][0]  && correctMatrix[1][2]
                             && correctMatrix[2][1])  // если под условия подходят все значения вокруг, расположенные слева, справа, сверху и снизу
                    {
//                        setPixelColor(resultImage, i, j, blackColor(replasePixelColorGrayLevel));    // устанавливаем белый цвет пикселю
                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                    }
                }


                // сильное удаление (2 пикселя, белый)
                if(type == HIGH)
                {
                    // горизонталь
                    if(correctMatrix[1][0] && correctMatrix[1][2])  // если под условия подходят все значения, расположенные слева и справа
                    {
//                        setPixelColor(resultImage, i, j, blackColor(replasePixelColorGrayLevel));    // устанавливаем белый цвет пикселю
                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                    }
                    // вертикаль
                    if(correctMatrix[0][1] && correctMatrix[2][1])  // если под условия подходят все значения, расположенные сверху и снизу
                    {
//                        setPixelColor(resultImage, i, j, blackColor(replasePixelColorGrayLevel));    // устанавливаем белый цвет пикселю
                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                    }
                }
            }
        }
    }
}

void ImageCorrector::medianFilter()
{
    auto sourseImageMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения
//    QImage sourseImage = QImage(resultImage);   // копируем исходную картинку

    for(int i=1; i<resultImage.getWidth()-1; i++)
    {
        for(int j=1; j<resultImage.getHeight()-1; j++)
        {
//            auto pixels = getMatrixAroundPixel(sourseImage, i, j);  // получаем матрицу интенсивности 3х3 вокруг пикселя
            auto pixels = getMatrixAroundPixel(sourseImageMatrix, i, j);    // получаем матрицу интенсивности 3х3 вокруг пикселя
            QList<int> sortedPixelsList;  // одномерный список для сортировки массива
            for(int k=0; k<3; k++)
            {
                for(int h=0; h<3; h++)
                {
                    sortedPixelsList.append(pixels[k][h]);  // добавляем значение в список
                }
            }
            QVector<int> sortedPixelsVector = sortedPixelsList.toVector();    // переводим список в вектор для ускорения работы программы
            qSort(sortedPixelsVector);  // сортируем
            int size = sortedPixelsVector.size();   // поулчаем кол-во элементов
            int mid = ((size - 1) / 2); //  9 -1 = 8 ; 8 / 2 = 4 ; 4 - серединный элемент
            int medianBlackValue = 255 - sortedPixelsVector[mid]; // получаем медианное значение

//            setPixelColor(resultImage, i, j, medianBlackValue); // устанавливаем цвет
            sourseImageMatrix[i][j] = medianBlackValue;   // устанавливаем белый цвет пикселю
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

QVector<QVector<bool>> ImageCorrector::getCorrectDataAroundPixels(QImage &image, int i, int j, int border, bool (*compare)(int, int))
{
    QVector<QVector<bool>> matrix;  // матрица соответствия
    matrix.resize(3);   // задаем кол-во строк
    for(auto &&item : matrix)
    {
        item.resize(3); // задаем кол-во столбцов
    }

    auto pixsels = getMatrixAroundPixel(image, i, j);   // получаем матрицу пикселей

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            int black = pixsels[k][h];  // получаем значение интенсивности пикселя
            bool isCorrect = compare(black, border);  // результат корректности
            matrix[k][h] = isCorrect;   // заносим данные в матрицу
        }
    }
    return matrix;
}

bool **ImageCorrector::getCorrectDataAroundPixels(int **grayScaleMatrix, int i, int j, int border, bool (*compare)(int, int))
{
    bool **correctMatrix = new bool *[3];
    for(int i=0; i<3; i++)
    {
        correctMatrix[i] = new bool [3];
    }

    auto pixsels = getMatrixAroundPixel(grayScaleMatrix, i, j);   // получаем матрицу пикселей

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
//            int black = pixsels[k][h];  // получаем значение интенсивности пикселя
//            bool isCorrect = compare(black, border);  // результат корректности
//            correctMatrix[k][h] = isCorrect;   // заносим данные в матрицу

            // i + (h-1) даст нам значения -1 0 +1
//            int black = grayScaleMatrix[i + (k-1)][j + (h-1)];  // получаем значение интенсивности пикселя
            int black = pixsels[k][h];
            bool isCorrect = compare(black, border);  // результат корректности
            correctMatrix[k][h] = isCorrect;    // заносим данные в матрицу
        }
    }
    return correctMatrix;
}


QVector<QVector<int> > ImageCorrector::getMatrixAroundPixel(QImage &image, int i, int j)
{
    QVector<QVector<int>> matrix;  // матрица соответствия
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
            matrix[k][h] = black;   // заносим данные в матрицу
        }
    }
    return matrix;
}

bool **ImageCorrector::getMatrixAroundPixel(int **grayScaleMatrix, int i, int j)
{
    bool **matrix = new bool *[3];
    for(int i=0; i<3; i++)
    {
        matrix[i] = new bool [3];
    }

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            // i + (h-1) даст нам значения -1 0 +1
            int black = grayScaleMatrix[i + (k-1)][j + (h-1)];  // берем информацию о пикселях вокруг

//            int black = getPixelBlackValue(image, i + (k-1), j + (h-1));    // берем информацию о пикселях вокруг

            matrix[k][h] = black;   // заносим данные в матрицу
        }
    }
    return matrix;
}

QImage ImageCorrector::getResultImage() const
{
    return resultImage.getImage();
}

void ImageCorrector::setImageObject(const QImage &value)
{
    imageObject.setImage(value);
}

void ImageCorrector::setImageOriginal(const QImage &value)
{
    imageOriginal.setImage(value);
    resultImage.setImage(value);
}
