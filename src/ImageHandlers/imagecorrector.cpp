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
            int blackOriginal = imageOriginalMatrix[i][j];  // уровень черного в оригинале
            int blackObject = imageObjectMatrix[i][j];  // уровень черного в объекте

            int blackSub = abs(blackOriginal - blackObject);    // вычитаем фон
            imageResultMatrix[i][j] = blackSub; // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::clipNoise(int clippingNoiseValue)
{
    if(clippingNoiseValue == -1)
    {
        clippingNoiseValue = this->clippingNoiseValue;
    }
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
        int black = imageResultMatrix[i][j];   // получаем уровень черного
        if(black < clippingNoiseValue)   // обрезаем шумы в соответсвии с указанным уровнем
        {
            black = 0;
        }

        imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
    };
    distributeToThreads(0, imageOriginal.getWidth(), 0, imageOriginal.getHeight(), function);

//    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
//    {
//        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
//        {
//            int black = imageResultMatrix[i][j];   // получаем уровень черного
//            if(black < clippingNoiseValue)   // обрезаем шумы в соответсвии с указанным уровнем
//            {
//                black = 0;
//            }

//            imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
//        }
//    }
}

void ImageCorrector::enchanceBlackColor(int blackEnchancement)
{
    if(blackEnchancement == -1)
    {
        blackEnchancement = this->blackEnchancement;
    }
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
        int black = imageResultMatrix[i][j];   // получаем уровень черного
        if(black > blackEnchancement)    // усиливаем черный цвет
        {
            black = 255;
        }

        imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
    };

    distributeToThreads(0, imageOriginal.getWidth(), 0, imageOriginal.getHeight(), function);   // распределяем по потокам

//    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
//    {
//        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
//        {
//            int black = imageResultMatrix[i][j];   // получаем уровень черного
//            if(black > blackEnchancement)    // усиливаем черный цвет
//            {
//                black = 255;
//            }

//            imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
//        }
//    }
}

void ImageCorrector::invertPixels()
{
    QImage img = resultImage.getImage();
    img.invertPixels();
    resultImage.setImage(img);
}

void ImageCorrector::hardClipNoise(int border, NoiseDeleteTypes type, NoiseDeleteColors colorType)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
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
            auto correctMatrix = getCorrectDataAroundPixels(imageResultMatrix, i, j, border, copmarator);   // получаем матрицу коорректности

            // слабое удаление (8 пикселей, белый)
            if(type == LOW)
            {
                if(correctMatrix[0][0] && correctMatrix[0][1] && correctMatrix[0][2]
                         && correctMatrix[1][0]  && correctMatrix[1][2]
                         && correctMatrix[2][0] && correctMatrix[2][1] && correctMatrix[2][2])  // если под условия подходят все значения вокруг
                {
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
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                }
                // плюсом
                if(correctMatrix[0][1]
                         && correctMatrix[1][0]  && correctMatrix[1][2]
                         && correctMatrix[2][1])  // если под условия подходят все значения вокруг, расположенные слева, справа, сверху и снизу
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                }
            }

            // сильное удаление (2 пикселя, белый)
            if(type == HIGH)
            {
                // горизонталь
                if(correctMatrix[1][0] && correctMatrix[1][2])  // если под условия подходят все значения, расположенные слева и справа
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                }
                // вертикаль
                if(correctMatrix[0][1] && correctMatrix[2][1])  // если под условия подходят все значения, расположенные сверху и снизу
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
                }
            }

            memoryDoubleArrayFree(correctMatrix, 3);    // очищаем память
        }
    };

    distributeToThreads(1, imageOriginal.getWidth()-1, 1, imageOriginal.getHeight()-1, function);   // распределяем по потокам

//    for (int i=1; i<imageOriginal.getWidth()-1; i++) // проходим по ширине
//    {
//        for (int j=1; j<imageOriginal.getHeight()-1; j++)    // проходим по высоте
//        {
//            int black = imageResultMatrix[i][j];    // получаем уровень черного

//            if(black <= border) // если цвет попадает под границу для сглаживания шума
//            {
//                bool (*copmarator)(int comparedValue, int borderValue) = nullptr;   // функция сравнения значения с границей
//                int replasePixelColorGrayLevel = 0; // цвет, на который будет заменён цвет пикселя в случае сглаживания
//                if(colorType == WHITE)  // условие для белового цвета
//                {
//                    copmarator = [](int comparedValue, int borderValue)
//                    {
//                        return comparedValue <= borderValue;    // значение сранивается на <=
//                    };
////                    replasePixelColorGrayLevel = WHITE_GRAY_LEVEL;
//                    replasePixelColorGrayLevel = BLACK_GRAY_LEVEL;
//                }
//                else // условие сравнения для чёрного цвета
//                {
//                    copmarator = [](int comparedValue, int borderValue)
//                    {
//                        return comparedValue > borderValue; // значения сравниваются в большую сторону
//                    };
////                    replasePixelColorGrayLevel = BLACK_GRAY_LEVEL;
//                    replasePixelColorGrayLevel = WHITE_GRAY_LEVEL;
//                }
//                auto correctMatrix = getCorrectDataAroundPixels(imageResultMatrix, i, j, border, copmarator);   // получаем матрицу коорректности

//                // слабое удаление (8 пикселей, белый)
//                if(type == LOW)
//                {
//                    if(correctMatrix[0][0] && correctMatrix[0][1] && correctMatrix[0][2]
//                             && correctMatrix[1][0]  && correctMatrix[1][2]
//                             && correctMatrix[2][0] && correctMatrix[2][1] && correctMatrix[2][2])  // если под условия подходят все значения вокруг
//                    {
//                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
//                    }
//                }

//                // среднее удаление (4 пикселя, белый)
//                if(type == MEDIUM)
//                {
//                    // крестом
//                    if(correctMatrix[0][0] && correctMatrix[0][2]
//                             && correctMatrix[2][0] && correctMatrix[2][2])  // если под условия подходят все значения вокруг, расположенные по диагонале
//                    {
//                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
//                    }
//                    // плюсом
//                    if(correctMatrix[0][1]
//                             && correctMatrix[1][0]  && correctMatrix[1][2]
//                             && correctMatrix[2][1])  // если под условия подходят все значения вокруг, расположенные слева, справа, сверху и снизу
//                    {
//                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
//                    }
//                }

//                // сильное удаление (2 пикселя, белый)
//                if(type == HIGH)
//                {
//                    // горизонталь
//                    if(correctMatrix[1][0] && correctMatrix[1][2])  // если под условия подходят все значения, расположенные слева и справа
//                    {
//                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
//                    }
//                    // вертикаль
//                    if(correctMatrix[0][1] && correctMatrix[2][1])  // если под условия подходят все значения, расположенные сверху и снизу
//                    {
//                        imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем белый цвет пикселю
//                    }
//                }
//            }
//        }
//    }
}

//void ImageCorrector::medianFilter()
//{
//    auto sourseImageMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения
////    QImage sourseImage = QImage(resultImage);   // копируем исходную картинку

//    for(int i=1; i<resultImage.getWidth()-1; i++)
//    {
//        for(int j=1; j<resultImage.getHeight()-1; j++)
//        {
//            auto pixels = getMatrixAroundPixel(sourseImageMatrix, i, j);    // получаем матрицу интенсивности 3х3 вокруг пикселя
//            QList<int> sortedPixelsList;  // одномерный список для сортировки массива
//            for(int k=0; k<3; k++)
//            {
//                for(int h=0; h<3; h++)
//                {
//                    sortedPixelsList.append(pixels[k][h]);  // добавляем значение в список
//                }
//            }
//            QVector<int> sortedPixelsVector = sortedPixelsList.toVector();    // переводим список в вектор для ускорения работы программы
//            qSort(sortedPixelsVector);  // сортируем
//            int size = sortedPixelsVector.size();   // поулчаем кол-во элементов
//            int mid = ((size - 1) / 2); //  9 -1 = 8 ; 8 / 2 = 4 ; 4 - серединный элемент
//            int medianBlackValue = 255 - sortedPixelsVector[mid]; // получаем медианное значение

//            sourseImageMatrix[i][j] = medianBlackValue;   // устанавливаем белый цвет пикселю
//            memoryDoubleArrayFree(pixels, 3);   // очищаем память
//        }
//    }
//}

void ImageCorrector::aliasing(int radius, int border, int blackBorderPercent, int whiteBorderPercent)
{
//    auto imageOriginalMatrix = imageOriginal.getGrayScaleMatrix();  // матрица пикселей исходного изображения
//    auto imageObjectMatrix = imageObject.getGrayScaleMatrix();  // матрица пикселей изображения объекта
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=radius; i<imageOriginal.getWidth()-radius; i += radius*2) // проходим по ширине
    {
        for (int j=radius; j<imageOriginal.getHeight()-radius; j += radius*2)    // проходим по высоте
        {
            if(i % radius == 0 && j % radius == 0)  // Если попадаем в радиус
            {
                int countBiggerBorder = 0;

                for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
                {
                    for(int g=j-radius; g<j+radius; g++)
                    {
                        if(imageResultMatrix[k][g] >= border)   // если значение пикселя больше, чем граница
                        {
                            countBiggerBorder++;
                        }
                    }
                }

                double maxPixels = radius * (radius/2);    // считаем максимальное кол-во пикселей
                double blackPercent = (((double)countBiggerBorder) / maxPixels) * 100.0;   // считаем процент пикселей
                double whitePercent = 100.0 - blackPercent; // процент белых пикселей
                if(blackPercent < (double)whiteBorderPercent)   // если кол-во темных пикселей больше чем половина, то весь квадрат заполняем церным цветом
                {
                    for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
                    {
                        for(int g=j-radius; g<j+radius; g++)
                        {
                            imageResultMatrix[k][g] = BLACK_GRAY_LEVEL;
                        }
                    }
                }
                /*else */
                if (whitePercent < (double)blackBorderPercent)
                {
                    for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
                    {
                        for(int g=j-radius; g<j+radius; g++)
                        {
                            imageResultMatrix[k][g] = WHITE_GRAY_LEVEL;
                        }
                    }
                }
            }
        }
    }
}

void ImageCorrector::medianRadiusFilter(int radius)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    int i_start = radius;
    int i_end = imageOriginal.getWidth()-radius;
    int i_step = 1;

    int j_start = radius;
    int j_end = imageOriginal.getHeight()-radius;
    int j_step = 1;

    int **pixelsNewMas = new int* [imageOriginal.getWidth()];
    for(int i = 0; i<imageOriginal.getWidth(); i++)
    {
        pixelsNewMas[i] = new int [imageOriginal.getHeight()];
    }

    for (int i=i_start; i<i_end; i += i_step) // проходим по ширине
    {
        for (int j=j_start; j<j_end; j += j_step)    // проходим по высоте
        {
            QList<int> pixelsAround;   // массив всех пикселей

            for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
            {
                for(int g=j-radius; g<j+radius; g++)
                {
                    pixelsAround.append(imageResultMatrix[k][g]);
                }
            }

            std::sort(pixelsAround.begin(), pixelsAround.end());  // сортируем массив (быстрая сортировка)
            int medianValue = pixelsAround.size() / 2;  // получаем номер серединного (медианного) элемента

            pixelsNewMas[i][j] = pixelsAround[medianValue]; // сохраняем полученное значение в новый массив, чтобы не затирать старые значения
        }
    }
    for (int i=i_start; i<i_end; i += i_step) // проходим по ширине
    {
        for (int j=j_start; j<j_end; j += j_step)    // проходим по высоте
        {

            imageResultMatrix[i][j] = pixelsNewMas[i][j];
        }
    }

    for(int i=0; i<imageOriginal.getWidth(); i++)
    {
        delete [] pixelsNewMas [i];
    }
    delete [] pixelsNewMas;
}

void ImageCorrector::averageFilter(int radius)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    int i_start = radius;
    int i_end = imageOriginal.getWidth()-radius;
    int i_step = 1;

    int j_start = radius;
    int j_end = imageOriginal.getHeight()-radius;
    int j_step = 1;

    int **pixelsNewMas = new int* [imageOriginal.getWidth()];
    for(int i = 0; i<imageOriginal.getWidth(); i++)
    {
        pixelsNewMas[i] = new int [imageOriginal.getHeight()];
    }

    for (int i=i_start; i<i_end; i += i_step) // проходим по ширине
    {
        for (int j=j_start; j<j_end; j += j_step)    // проходим по высоте
        {
            QList<int> pixelsAround;   // массив всех пикселей

            for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
            {
                for(int g=j-radius; g<j+radius; g++)
                {
                    pixelsAround.append(imageResultMatrix[k][g]);
                }
            }

            int sum = 0; // сумма значений пикселей
            for(int i=0; i<pixelsAround.size(); i++)
            {
                sum += pixelsAround[i];
            }
            double avg = sum;
            avg /= (double)pixelsAround.size(); // считаем среднее арифметическое
            int value = (int)avg;   // приводим к целому значению для изображения

            pixelsNewMas[i][j] = value; // сохраняем полученное значение в новый массив, чтобы не затирать старые значения
        }
    }
    for (int i=i_start; i<i_end; i += i_step) // проходим по ширине
    {
        for (int j=j_start; j<j_end; j += j_step)    // проходим по высоте
        {

            imageResultMatrix[i][j] = pixelsNewMas[i][j];
        }
    }

    for(int i=0; i<imageOriginal.getWidth(); i++)
    {
        delete [] pixelsNewMas [i];
    }
    delete [] pixelsNewMas;
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

    memoryDoubleArrayFree(pixsels, 3);

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

void ImageCorrector::setBlackEnchancement(int newBlackEnchancement)
{
    blackEnchancement = newBlackEnchancement;
}

void ImageCorrector::setClippingNoiseValue(int newClippingNoiseValue)
{
    clippingNoiseValue = newClippingNoiseValue;
}

void ImageCorrector::setThreadsCount(int newThreadsCount)
{
    threadsCount = newThreadsCount;
}

void ImageCorrector::distributeToThreads(int startI, int endI, int startJ, int endJ, std::function<void(int, int)> function, int stepI, int stepJ)
{
    int countI = endI - startI; // считаем кол-во значений для i, если начальное значение не 0
    int countJ = endJ - startJ; // считаем кол-во значений для j, если начальное значение не 0

    if(threadsCount <= 1 || countI < threadsCount || countJ < threadsCount)   // если требуется всего 1 поток
    {
        for(int i=startI; i<endI; i += stepI)  // делаем обычный вложенный цикл
        {
            for(int j=startJ; j<endJ; j += stepJ)
            {
                function(i, j); // и выполняем действие, которое нужно сделать
            }
        }
    }
    else    // если требуется несколько потоков
    {
        int intervalI = countI / threadsCount;  // считаем интервал между i для ровного разделения на циклы
        int intervalJ = countJ / threadsCount;  // считаем интервал между j для ровного разделения на циклы

        int *masStartI = new int [threadsCount];    // массив хранения стартовых значений i для каждого потока
        int *masStartJ = new int [threadsCount];    // массив хранения стартовых значений j для каждого потока
        int *masEndI = new int [threadsCount];    // массив хранения конечных значений i для каждого потока
        int *masEndJ = new int [threadsCount];    // массив хранения конечных значений j для каждого потока

        for(int p=0; p<threadsCount; p++)   // заполняем стартовые значения
        {
            masStartI[p] = startI + intervalI * p;  // стартовое значение (константа) + интервал, умноженный на текущую позицию
            masStartJ[p] = startJ + intervalJ * p;  // стартовое значение (константа) + интервал, умноженный на текущую позицию
            masEndI[p] = startI + intervalI * (p+1);    // конечное значение равно следующему стартовому
            masEndJ[p] = startJ + intervalJ * (p+1);    // конечное значение равно следующему стартовому
        }

        masEndI[threadsCount-1] = endI; // последнее значение i равно максимальному
        masEndJ[threadsCount-1] = endJ; // последнее значение j равно максимальному

        std::thread *threads = new std::thread [threadsCount];  // создаём потоки
        for(int k=0; k<threadsCount; k++)   // заполняем действиями потоки
        {
            threads[k] = std::thread([masStartI, masStartJ, masEndI, masEndJ, function, k, startJ, endJ]()    // передаём потоку действие, которое нужно выполнить
            {
                for(int i=masStartI[k]; i<masEndI[k]; i++)  // делаем проход по i
                {
//                    for(int j=masStartJ[k]; j<masEndJ[k]; j++)  // делаем проход по j
                    for(int j=startJ; j<endJ; j++)
                    {
                        function(i, j); // и выполняем действие, которое нужно сделать
                    }
                }
            });
        }

        for(int i=threadsCount-1; i>=0; i--)    // идём в обратном порядке и дожидаемся завершения всех потоков
        {
            threads[i].join();
        }

        // удалить память !!!
        delete [] threads;
        threads = nullptr;

        delete [] masStartI;
        masStartI = nullptr;

        delete [] masStartJ;
        masStartJ = nullptr;

        delete [] masEndI;
        masEndI = nullptr;

        delete [] masEndJ;
        masEndJ = nullptr;
    }
}

template<typename T>
void ImageCorrector::memoryDoubleArrayFree(T **&array, int count1)
{
    for(int i=0; i<count1; i++)
    {
        delete [] array [i];
        array[i] = nullptr;
    }
    delete [] array;
    array = nullptr;
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
