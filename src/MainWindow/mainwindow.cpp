#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QThread>
#include <QTime>
#include "thread"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    int *mas = new int [100000];
//    int mas2 = *mas;
//    gmas = &mas2;

//    qDebug() << "PixelRatio: " << QGuiApplication::primaryScreen()->devicePixelRatio();
    QThread th; // класс потоков, для получения кол-ва возможных, для использования
    int maxThreadsCount = th.idealThreadCount();    // получаем максимальное кол-во потоков
    for(int i=1; i<=maxThreadsCount; i++)
    {
        ui->comboBox_threadsCount->addItem(QString::number(i)); // добавлем цифру кол-ва потов в список
    }
    ui->comboBox_threadsCount->setCurrentIndex(maxThreadsCount-1);  // по умолчанию ставим, что выбрано максимальное кол-во потоков
//    qDebug() << "Threads Count = " << th.idealThreadCount();
    screen = QGuiApplication::primaryScreen();  // получаем указатель на главный экран
    ui->radioButton_imageEmitter_imageFromFile->setChecked(true);
    ui->radioButton_captureDevice_camera->setChecked(true);
    on_radioButton_imageEmitter_imageFromFile_clicked();    // применяем выбор
    on_radioButton_captureDevice_camera_clicked();  // применяем выбор на камеру

//    auto screens = QGuiApplication::screens();  // поулчаем список экранов, подключенных к ПК
//    for(QScreen *screenItem : qAsConst(screens))
//    {
//        ui->comboBox_choseScreen->addItem(screenItem->name());  // добавляем каждый дисплей в список
//    }

    // FiltersManager

//    SubstructImageFilter *substructImageFilter = new SubstructImageFilter;
//    substructImageFilter->setImageCorrector(&imageCorrector);

//    ClipNoiseFilter *clipNoiseFilter = new ClipNoiseFilter;
//    clipNoiseFilter->setImageCorrector(&imageCorrector);
//    clipNoiseFilter->setSlider(ui->horizontalSlider_clippingNoiseValue);

//    EnchanceBlackColorFilter *enchanceBlackColorFilter = new EnchanceBlackColorFilter;
//    enchanceBlackColorFilter->setImageCorrector(&imageCorrector);
//    enchanceBlackColorFilter->setSlider(ui->horizontalSlider_blackEnchancementValue);

//    HardClipNoiseFilter *hardClipNoiseFilter = new HardClipNoiseFilter;
//    hardClipNoiseFilter->setImageCorrector(&imageCorrector);
//    hardClipNoiseFilter->setSlider(ui->horizontalSlider_deleteNoise);
//    hardClipNoiseFilter->setComboBox(ui->comboBox_deleteType);

//    QList<IFilter> *filtersList = filtersManager.getFilters();
//    filtersList->append(*substructImageFilter);
//    filtersList->append(*clipNoiseFilter);
//    filtersList->append(*enchanceBlackColorFilter);
////    filtersList->append(hardClipNoiseFilter);

    // ==

    uiDataSaver.add(ui->lineEdit_imageOriginal);
    uiDataSaver.add(ui->lineEdit_imageObject);
    uiDataSaver.add(ui->lineEdit_presets_name);
    uiDataSaver.add(ui->lineEdit_imageOriginal_video);

    uiDataSaver.add(ui->radioButton_imageEmitter_imageFromFile);
    uiDataSaver.add(ui->radioButton_imageEmitter_videoCaptureFromScreen);
    uiDataSaver.add(ui->radioButton_captureDevice_monitor);
    uiDataSaver.add(ui->radioButton_captureDevice_camera);

    uiDataSaver.add(ui->horizontalSlider_blackEnchancementValue);
    uiDataSaver.add(ui->horizontalSlider_clippingNoiseValue);
    uiDataSaver.add(ui->horizontalSlider_deleteNoise);
//    uiDataSaver.add(ui->horizontalSlider_aliasingBorder);
    uiDataSaver.add(ui->horizontalSlider_aliasing_blackBorder);
    uiDataSaver.add(ui->horizontalSlider_aliasing_whiteBorder);

    uiDataSaver.add(ui->radioButton_original);
    uiDataSaver.add(ui->radioButton_object);
    uiDataSaver.add(ui->radioButton_result);

    uiDataSaver.add(ui->checkBox_colorInversion);
    uiDataSaver.add(ui->checkBox_deleteNoise);
    uiDataSaver.add(ui->checkBox_aliasing);
    uiDataSaver.add(ui->checkBox_aliasingVisualisation);
    uiDataSaver.add(ui->checkBox_medianFilter);
    uiDataSaver.add(ui->checkBox_averageFilter);
    uiDataSaver.add(ui->checkBox_substractObject);

    uiDataSaver.add(ui->comboBox_deleteType);
    uiDataSaver.add(ui->comboBox_presets);

    uiDataSaver.add(ui->spinBox_aliasingRadius);
    uiDataSaver.add(ui->spinBox_medianFilter_radius);
    uiDataSaver.add(ui->spinBox_averageFilter_radiusValue);
    uiDataSaver.add(ui->spinBox_collisionRepeatCount);

    uiDataSaver.loadPresets();

    QStringList presets = uiDataSaver.getPresets();
    ui->comboBox_presets->addItems(presets);    // выводим пресеты

//    uiDataSaver.loadProgramData();

    on_horizontalSlider_blackEnchancementValue_valueChanged(ui->horizontalSlider_blackEnchancementValue->value());
    on_horizontalSlider_clippingNoiseValue_valueChanged(ui->horizontalSlider_clippingNoiseValue->value());

    QTableWidgetItem *imageItem = new QTableWidgetItem; // создаем item для вывода изображения
    ui->tableWidget_image->setRowCount(1);
    ui->tableWidget_image->setColumnCount(1);
    ui->tableWidget_image->setItem(0, 0, imageItem);    // устанавливаем item

//    QRect tableGeometry = ui->tableWidget_image->geometry();
//    ui->tableWidget_image->setColumnWidth(0, tableGeometry.width());
//    ui->tableWidget_image->setRowHeight(0, tableGeometry.height());

    imageLabel = new QLabel;
    ui->tableWidget_image->setCellWidget(0, 0, imageLabel); // выводим label в tableWidget
    imageLabel->setText("Изображение");

//    ui->groupBox_medianFilter->setVisible(false);   // отключаем отображение медианного фильтра

    uiDataSaver.loadProgramData();
}

MainWindow::~MainWindow()
{

    delete ui;

}

void MainWindow::on_pushButton_choseImageOriginal_clicked()
{
    QString imageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");
    ui->lineEdit_imageOriginal->setText(imageDir);
}


void MainWindow::on_pushButton_choseImageObject_clicked()
{
    QString imageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");
    ui->lineEdit_imageObject->setText(imageDir);
}


void MainWindow::on_pushButton_calculate_clicked()
{
    QString imageOriginalDir = ui->lineEdit_imageOriginal->text();
    QString imageObjectDir = ui->lineEdit_imageObject->text();

    if(imageOriginalDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение оригинала!");
        return;
    }
    if(imageObjectDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение объекта!");
        return;
    }

    imageOriginal = QImage(imageOriginalDir);
    imageObject = QImage(imageObjectDir);

    imageOriginal.convertTo(QImage::Format_Grayscale16);    // конвертируем в ч/б изображение
    imageObject.convertTo(QImage::Format_Grayscale16);  // конвертируем в ч/б изображение

    resultImage = imageOriginal;

    processImageFilters(imageOriginal, imageObject, resultImage);   // применяем фильтры

    // выводим картинку
    setImageToOutputLabel(resultImage);

    // переводим radioButton
    ui->radioButton_result->setChecked(true);

}


void MainWindow::on_radioButton_original_clicked()
{
    if(!imageOriginal.isNull())
    {
        setImageToOutputLabel(imageOriginal);
    }
}


void MainWindow::on_radioButton_object_clicked()
{
    if(!imageObject.isNull())
    {
        setImageToOutputLabel(imageObject);
    }
}


void MainWindow::on_radioButton_result_clicked()
{
    if(!resultImage.isNull())
    {
//        ui->label_image->setPixmap(QPixmap::fromImage(QImage(resultImage)));
        setImageToOutputLabel(resultImage);
    }
}


void MainWindow::on_pushButton_saveResult_clicked()
{
    QString saveDir = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Images (*.jpg)");
    if(saveDir != "")
    {
        resultImage.save(saveDir);
    }
}


void MainWindow::on_horizontalSlider_clippingNoiseValue_valueChanged(int value)
{
    ui->label_clippingNoiseValue->setNum(value);
}


void MainWindow::on_horizontalSlider_blackEnchancementValue_valueChanged(int value)
{
    ui->label_blackEnchancementValue->setNum(value);
}


void MainWindow::on_horizontalSlider_deleteNoise_valueChanged(int value)
{
    ui->label_deleteNoise_value->setNum(value);
}

void MainWindow::setImageToOutputLabel(QImage image)
{
//    int labelWidht = ui->label_image->width();
//    int labelHeight = ui->label_image->height();

//    int labelWidht = imageLabel->width();
//    int labelHeight = imageLabel->height();
    int labelWidht = ui->tableWidget_image->columnWidth(0);
    int labelHeight = ui->tableWidget_image->rowHeight(0);

    double imgWidht = image.width();
    double imgHeight = image.height();

    double widthFactor = labelWidht/imgWidht;
    double heightFactor = labelHeight/imgHeight;

    if(widthFactor < heightFactor)
    {
        imgWidht *= widthFactor;
        imgHeight *= widthFactor;
    }
    else
    {
        imgWidht *= heightFactor;
        imgHeight *= heightFactor;
    }

    image = ImageTransformer::getTransformedImage(image, imgWidht, imgHeight);
//    ui->label_image->setScaledContents(true);
//    ui->label_image->setPixmap(QPixmap::fromImage(image));
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::updateLabelImageSize()
{
    QApplication::processEvents();
    // подгоняем размеры item
    QRect tableGeometry = ui->tableWidget_image->geometry();
    ui->tableWidget_image->setColumnWidth(0, tableGeometry.width());
    ui->tableWidget_image->setRowHeight(0, tableGeometry.height());

    // выводим соответсвующее отмасштабирвоанное изображение
    if(ui->radioButton_original->isChecked())
    {
        setImageToOutputLabel(imageOriginal);
    }
    else if(ui->radioButton_object->isChecked())
    {
        setImageToOutputLabel(imageObject);
    }
    else if(ui->radioButton_result->isChecked())
    {
        setImageToOutputLabel(resultImage);
    }
}

//void MainWindow::processImageFilters(QImage &imageOriginal, QImage &imageObject, QImage &resultImage)
//{
//    QImage tempOriginalImage;
//    if(ui->checkBox_substractObject->isChecked())
//    {
//        imageCorrector.setImageOriginal(imageOriginal);
//    }
//    else
//    {
////        QImage *image = new QImage(imageObject.size(), imageObject.format());
////        image->fill(Qt::white);  // заполняем белым цветом
//        tempOriginalImage = QImage(imageObject.size(), imageObject.format());
//        tempOriginalImage.fill(Qt::white);  // заполняем белым цветом
//        imageCorrector.setImageOriginal(tempOriginalImage);
//    }
////    imageCorrector.setImageOriginal(imageOriginal);
//    imageCorrector.setImageObject(imageObject);

//    int clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
//    int blackEnchancement = 255 - ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)

//    imageCorrector.setClippingNoiseValue(clippingNoiseValue);
//    imageCorrector.setBlackEnchancement(blackEnchancement);

//    int threadsCount = ui->comboBox_threadsCount->currentText().toInt();    // получаем количество потоков, которое мы можем использовать
//    imageCorrector.setThreadsCount(threadsCount);    // устанавливаем количество потоков, которое будет использовать программа

//    int repeatOffset = ui->spinBox_collisionRepeatCount->value(); // смещение для радиуса при применении фильтра несколько раз

//    for(int offset=0; offset<=repeatOffset; offset++)
//    {
//        if(ui->checkBox_substractObject->isChecked())
//        {
//            imageCorrector.substractObjectImage();    // вычитаем изображение
//        }

//        if(ui->checkBox_deleteNoise->isChecked())
//        {
//            int deleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов
//            int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
//            ImageCorrector::NoiseDeleteTypes type = ImageCorrector::NoiseDeleteTypes(deleteType);

//            if(ui->checkBox_hardNoiseClipping_deleteWhite->isChecked()) // если нужно удалять черный цвет
//            {
//               imageCorrector.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
//            }
//            if(ui->checkBox_hardNoiseClipping_deleteBlack->isChecked()) // если нкжно удалять белый цвет
//            {
//                imageCorrector.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
//            }

//            qDebug() << "Удаление выполнено!";
//        }

//    //    imageCorrector.enchanceBlackColor();    // усиление черного цвета
//        imageCorrector.clipNoise();    // простое удаление шума
//        imageCorrector.enchanceBlackColor();    // усиление черного цвета

//        if(ui->checkBox_medianFilter->isChecked())
//        {
//            int radius = ui->spinBox_medianFilter_radius->value() + offset;  // радиус медианного фильтра
//            imageCorrector.medianRadiusFilter(radius);  // применяем медианный фильтр нового типа (старый вариант остался на всякий случай)
//        }
//        if(ui->checkBox_averageFilter->isChecked())
//        {
//            int radius = ui->spinBox_averageFilter_radiusValue->value() + offset;  // радиус среднеарифметического фильтра
//            imageCorrector.averageFilter(radius);   // среднеарифметичсекий фильтр
//        }

//        if(ui->checkBox_aliasing->isChecked())
//        {
//            int aliasingRadius = ui->spinBox_aliasingRadius->value() + offset;
//    //        int aliasingBorder = ui->horizontalSlider_aliasingBorder->value();
//            int aliasingBorder = 126; // 255/2
//            int blackBorder = 100 - ui->horizontalSlider_aliasing_blackBorder->value();
//            int whiteBorder = 100 - ui->horizontalSlider_aliasing_whiteBorder->value();
//            imageCorrector.aliasing(aliasingRadius, aliasingBorder, blackBorder, whiteBorder);
//    //        imageCorrector.aliasing(aliasingRadius+1, aliasingBorder);
//        }

//        QImage result = imageCorrector.getResultImage();   // получаем обработанное изображение

//        if(ui->checkBox_colorInversion->isChecked())
//        {
//           result.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
//        }

//        if(offset > 0)  // если есть предыдущее изображение
//        {
//            resultImage = *colliseImages(resultImage, result);  // объединяем методом Исключающего ИЛИ
//        }
//        else    // иначе, если это первая итерация, то просто сохраняем
//        {
//            resultImage = result;
//        }
//    }


//    if(ui->checkBox_aliasingVisualisation->isChecked())
//    {
//        QPainter *painter = new QPainter(&resultImage);
//        QPen *pen = new QPen;
//        pen->setWidth(1);
//        pen->setColor(Qt::red);
//        painter->setPen(*pen);

//        int radius = ui->spinBox_aliasingRadius->value();
//        for (int i=radius*2; i<resultImage.width()-radius; i += radius*2) // проходим по ширине
//        {
//            painter->drawLine(i, radius, i, resultImage.height()-radius);
//        }

//        for (int j=radius*2; j<resultImage.height()-radius; j += radius*2)    // проходим по высоте
//        {
//            painter->drawLine(radius, j, resultImage.width()-radius, j);
//        }

//        delete painter;
//        delete pen;
//    }
//}

void MainWindow::processImageFilters(QImage &imageOriginal, QImage &imageObject, QImage &resultImage)
{
    ImageCorrectrFilterParams filter = createFilterParams();  // получаем параметры фильтров

    QImage tempOriginalImage;
    if(filter.needSubstructImage)
    {
        imageCorrector.setImageOriginal(imageOriginal);
    }
    else
    {
        tempOriginalImage = QImage(imageObject.size(), imageObject.format());
        tempOriginalImage.fill(Qt::white);  // заполняем белым цветом
        imageCorrector.setImageOriginal(tempOriginalImage);
    }
    imageCorrector.setImageObject(imageObject);

    imageCorrector.setClippingNoiseValue(filter.clippingNoiseValue);
    imageCorrector.setBlackEnchancement(255 - filter.blackEnchancement);

    imageCorrector.setThreadsCount(filter.threadsCount);    // устанавливаем количество потоков, которое будет использовать программа

    int repeatOffset = filter.repeatOffset; // смещение для радиуса при применении фильтра несколько раз

    for(int offset=0; offset<=repeatOffset; offset++)
    {
        if(filter.needSubstructImage)
        {
            imageCorrector.substractObjectImage();    // вычитаем изображение
        }

        if(filter.needHardDeleteNoise)
        {
//            int deleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов
//            int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
//            ImageCorrector::NoiseDeleteTypes type = ImageCorrector::NoiseDeleteTypes(deleteType);

            if(filter.needHardDeleteWhiteColor) // если нужно удалять черный цвет
            {
               imageCorrector.hardClipNoise(filter.hardDeleteNoiseBorder, filter.hardDeleteNoiseDeleteType, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
            }
            if(filter.needHardDeleteBlackColor) // если нкжно удалять белый цвет
            {
                imageCorrector.hardClipNoise(filter.hardDeleteNoiseBorder, filter.hardDeleteNoiseDeleteType, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
            }

            qDebug() << "Удаление выполнено!";
        }

    //    imageCorrector.enchanceBlackColor();    // усиление черного цвета
        imageCorrector.clipNoise();    // простое удаление шума
        imageCorrector.enchanceBlackColor();    // усиление черного цвета

        if(filter.needMedianFilter)
        {
            int radius = filter.medianFilter_radius + offset;  // радиус медианного фильтра
            imageCorrector.medianRadiusFilter(radius);  // применяем медианный фильтр нового типа (старый вариант остался на всякий случай)
        }
        if(filter.needAverageFilter)
        {
            int radius = filter.averageFilter_radius + offset;  // радиус среднеарифметического фильтра
            imageCorrector.averageFilter(radius);   // среднеарифметичсекий фильтр
        }

        if(filter.needAliasing)
        {
            int aliasingRadius = filter.aliasingRadius + offset;
    //        int aliasingBorder = ui->horizontalSlider_aliasingBorder->value();
            int aliasingBorder = 126; // 255/2
            int blackBorder = 100 - filter.aliasingBlackBorder;
            int whiteBorder = 100 - filter.aliasingWhiteBorder;
            imageCorrector.aliasing(aliasingRadius, aliasingBorder, blackBorder, whiteBorder);
    //        imageCorrector.aliasing(aliasingRadius+1, aliasingBorder);
        }

        QImage result = imageCorrector.getResultImage();   // получаем обработанное изображение

        if(filter.needColorInversion)
        {
           result.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
        }

        if(offset > 0)  // если есть предыдущее изображение
        {
            resultImage = *colliseImages(resultImage, result);  // объединяем методом Исключающего ИЛИ
        }
        else    // иначе, если это первая итерация, то просто сохраняем
        {
            resultImage = result;
        }
    }


    if(filter.needAliasingVisualisation)
    {
        QPainter *painter = new QPainter(&resultImage);
        QPen *pen = new QPen;
        pen->setWidth(1);
        pen->setColor(Qt::red);
        painter->setPen(*pen);

        int radius = ui->spinBox_aliasingRadius->value();
        for (int i=radius*2; i<resultImage.width()-radius; i += radius*2) // проходим по ширине
        {
            painter->drawLine(i, radius, i, resultImage.height()-radius);
        }

        for (int j=radius*2; j<resultImage.height()-radius; j += radius*2)    // проходим по высоте
        {
            painter->drawLine(radius, j, resultImage.width()-radius, j);
        }

        delete painter;
        delete pen;
    }
}

QImage *MainWindow::colliseImages(QImage &imageDown, QImage &imageUpper)
{
    QImage *imageOut = new QImage(imageDown);    // создаем изображение, идентичное по параметрам входному

    for(int i=0; i<imageDown.width(); i++)
    {
        for(int j=0; j<imageDown.height(); j++)
        {
            if((imageDown.pixelColor(i, j).black() > 100 && imageUpper.pixelColor(i, j).black() <= 100)
                    || (imageDown.pixelColor(i, j).black() <= 100 && imageUpper.pixelColor(i, j).black() > 100))   // условие инверсности пикселей
            {
                imageOut->setPixelColor(i, j, Qt::white);
            }
        }
    }

    return imageOut;
}

ImageCorrectrFilterParams MainWindow::createFilterParams()
{
    ImageCorrectrFilterParams params;   // создаем объект параметров

    // обрабатываем флаги
    params.needSubstructImage = ui->checkBox_substractObject->isChecked();
    params.needHardDeleteNoise = ui->checkBox_deleteNoise->isChecked();
    params.needHardDeleteWhiteColor = ui->checkBox_hardNoiseClipping_deleteWhite->isChecked();
    params.needHardDeleteBlackColor = ui->checkBox_hardNoiseClipping_deleteBlack->isChecked();
    params.needMedianFilter = ui->checkBox_medianFilter->isChecked();
    params.needAverageFilter = ui->checkBox_averageFilter->isChecked();
    params.needAliasing = ui->checkBox_aliasing->isChecked();
    params.needColorInversion = ui->checkBox_colorInversion->isChecked();
    params.needAliasingVisualisation = ui->checkBox_aliasingVisualisation->isChecked();

    // обрабатываем параметры
    params.clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
    params.blackEnchancement = ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)
    params.threadsCount = ui->comboBox_threadsCount->currentText().toInt();    // получаем количество потоков, которое мы можем использовать
    params.repeatOffset = ui->spinBox_collisionRepeatCount->value(); // смещение для радиуса при применении фильтра несколько раз
    params.hardDeleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов

    int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
    params.hardDeleteNoiseDeleteType = ImageCorrector::NoiseDeleteTypes(deleteType);

    params.medianFilter_radius = ui->spinBox_medianFilter_radius->value();  // радиус медианного фильтра
    params.averageFilter_radius = ui->spinBox_averageFilter_radiusValue->value();  // радиус среднеарифметического фильтра
    params.aliasingRadius = ui->spinBox_aliasingRadius->value();
    params.aliasingBorder = 126; // 255/2
    params.aliasingBlackBorder = ui->horizontalSlider_aliasing_blackBorder->value();
    params.aliasingWhiteBorder = ui->horizontalSlider_aliasing_whiteBorder->value();

    // обрабатываем параметры изображения
    params.heightStart = 0;
    params.widthStart = 0;
    params.heightEnd = imageObject.height();
    params.widthEnd = imageObject.width();

    return params;
}

void MainWindow::saveImageToFileWithDialog(QImage *image)
{
    if(image == nullptr)
    {
        QMessageBox::warning(this, "Ошибка", "Изображение пустое");
        return;
    }
    if(image->width() == 0 || image->height() == 0)
    {
        QMessageBox::warning(this, "Ошибка", "Изображение содержит 0 высоту или ширину");
        return;
    }
    QString imageDir = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Images (*.jpg)");
    if(imageDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Вы не выбрали файл!");
        return;
    }
    if(!image->save(imageDir))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить изображение!");
        return;
    }
}

QStringList MainWindow::getScreensList()
{
    auto screens = QGuiApplication::screens();  // поулчаем список экранов, подключенных к ПК
    QStringList screensList;
    for(QScreen *screenItem : qAsConst(screens))
    {
//        ui->comboBox_choseScreen->addItem(screenItem->name());  // добавляем каждый дисплей в список
        screensList.append(screenItem->name()); // добавляем каждый дисплей в список
    }
    return screensList;
}

QStringList MainWindow::getCamerasList()
{
    cameras = QCameraInfo::availableCameras(); // получаем список всех камер
    QStringList camerasNamesList;
    for (const QCameraInfo &cameraInfo : qAsConst(cameras))
    {
        camerasNamesList.append(cameraInfo.description());
    }
    return camerasNamesList;
}

void MainWindow::cameraImageCaptured(int id, const QImage &preview)
{
//    qDebug() << "photo";
    Q_UNUSED(id);

    if(isScreening) // если делаем скриншот для фона
    {
//        imageOriginal = preview.convertToFormat(QImage::Format_Grayscale16);
        imageOriginal = preview;
        imageOriginal = imageOriginal.convertToFormat(QImage::Format_Grayscale16);
        setImageToOutputLabel(imageOriginal);
//        ui->lineEdit_imageOriginal_video->setText(QString("Сохранено ") + QTime().currentTime().hour() + ":" + QTime().currentTime().minute() + ":" + QTime().currentTime().second());
        ui->lineEdit_imageOriginal_video->setText(QString("Сохранено ") + QTime().currentTime().toString());
        ui->radioButton_original->setChecked(true);
        isScreening = false;
        return;
    }

    // обрабатываем полученный снимок
    if(ui->radioButton_object->isChecked()) // если над опоказать оригинал с камеры
    {
        setImageToOutputLabel(preview.convertToFormat(QImage::Format_Grayscale16));
    }
    else if(ui->radioButton_result->isChecked())    // если нужно показать полученное изображение
    {
    //    setImageToOutputLabel(preview);
        imageObject = preview;
        processImageFilters(imageOriginal, imageObject, resultImage);

        // выводим полученный снимок
        setImageToOutputLabel(resultImage);
    }

    QApplication::processEvents();
}

void MainWindow::cameraReadyForCaptureChanged(bool ready)
{
    if(ready && isRunning)  // если можно продолжать
    {
//            qDebug() << "run";
        // выводим счётчик FPS
//        int times = frameTime.elapsed();    // получаем сколько прошло времени между кадрами
//        ui->label_FpsCount->setText(QString::number(times));    // выводим, сколько времени между кадрами прошло
//        frameTime.restart();    // перезапускаем счётчик
//        QApplication::processEvents();

        // делаем снимок и ожидаем его получения
        camera->searchAndLock();
        imageCapture->capture();
        camera->unlock();
    }
}


void MainWindow::on_pushButton_presets_accept_clicked()
{
    QString currentPresetName = ui->comboBox_presets->currentText(); // получаем название выбранного пресета
    int index = ui->comboBox_presets->currentIndex();   // получаем индекс выбранного пресета
    uiDataSaver.applyPreset(currentPresetName); // применяем пресет
    ui->lineEdit_presets_name->setText(currentPresetName);  // выводим название
    ui->comboBox_presets->setCurrentIndex(index);   // применяем индекс пресета, чтобы данные не съезжали
}

void MainWindow::on_pushButton_presets_addNew_clicked()
{
    QString presetName = ui->lineEdit_presets_name->text(); // получаем название пресета
    uiDataSaver.savePreset(presetName); // сохряняем
    ui->comboBox_presets->addItem(presetName);  // добавляем в список
    int index = ui->comboBox_presets->count() - 1;  // получаем номер последнего элемента
    ui->comboBox_presets->setCurrentIndex(index);   // прокручиваем но нового
}

void MainWindow::on_pushButton_presets_updateCurrent_clicked()
{
    QString presetName = ui->lineEdit_presets_name->text(); // получаем название пресета
    uiDataSaver.savePreset(presetName); // сохряняем
    QString lastPresetName = ui->comboBox_presets->currentText();   // получаем текущее название пресета
    if(presetName != lastPresetName)    // если название изменилось
    {
        uiDataSaver.removePreset(lastPresetName);   // старый пресет удаляем
        int currentIndex = ui->comboBox_presets->currentIndex();    // получаем текущий индекс
        ui->comboBox_presets->removeItem(currentIndex); // удаляем старый элемент
        ui->comboBox_presets->addItem(presetName);  // добавляем новое название
    }
}

void MainWindow::on_pushButton_presets_deleteChosed_clicked()
{
    QString presetName = ui->comboBox_presets->currentText(); // получаем название пресета
    uiDataSaver.removePreset(presetName);   // удаляем пресет
    int currentIndex = ui->comboBox_presets->currentIndex();    // получаем текущий индекс
    ui->comboBox_presets->removeItem(currentIndex); // удаляем элемент
}

void MainWindow::on_pushButton_screen_clicked()
{
    // получить изображение экрана
    if(ui->radioButton_captureDevice_monitor->isChecked())
    {
        QScreen *screen = QGuiApplication::primaryScreen();

        QPixmap pixmap = QPixmap (); // Каждый раз присваиваем нулевое значение pixmap
        int currentScreenNumber = ui->comboBox_choseScreen->currentIndex(); // получаем номер дисплея, с которого будем получать изображение
        pixmap = screen->grabWindow (currentScreenNumber); // Снимок экрана
    //    pixmap.save("C:/Users/ASUS/Pictures/qtscreen.jpg");
        setImageToOutputLabel(pixmap.toImage());    // выводим скриншот на экран
        ui->lineEdit_imageOriginal_video->setText(QString("Сохранено ") + QTime().currentTime().hour() + ":" + QTime().currentTime().minute() + ":" + QTime().currentTime().second());
        imageOriginal = pixmap.toImage();   // сохраняем изображение как фон
    }
    if(ui->radioButton_captureDevice_camera->isChecked())
    {
        if(camera == nullptr)
        {
            QMessageBox::warning(this, "Ошибка!", "Не выбрана камера!");
            return;
        }
        isScreening = true;     // указываем, что делаем скрин
//        int currentCameraIndex = ui->comboBox_choseScreen->currentIndex();  // получаем выбранную камеру
//        QCameraInfo cameraInfo = cameras[currentCameraIndex];   // получаем камеру, с которой будем производить снятие видео
//        camera = new QCamera(cameraInfo);
//        viewfinder = new QCameraViewfinder;
//        camera->setViewfinder(viewfinder);
//        viewfinder->show();

//        imageCapture = new QCameraImageCapture(camera);
//        imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);  // буферизируем
//        connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::cameraImageCaptured);
//        connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::cameraReadyForCaptureChanged);

//        camera->setCaptureMode(QCamera::CaptureStillImage);
//        camera->start();

        // делаем снимок и ожидаем его получения
        camera->searchAndLock();
        imageCapture->capture();
        camera->unlock();
    }
}


void MainWindow::on_radioButton_imageEmitter_imageFromFile_clicked()
{
////    ui->groupBox_imageEmitter_imageFromFile->show();
////    ui->groupBox_imageEmitter_videoCaptureFromScreen->hide();

//    ui->groupBox_imageEmitter_imageFromFile->setEnabled(true);
//    ui->groupBox_imageEmitter_videoCaptureFromScreen->setEnabled(false);

//    ui->pushButton_calculate->setEnabled(true);
//    ui->pushButton_saveResult->setEnabled(true);
//    ui->pushButton_runVideo->setEnabled(false);
//    ui->pushButton_screen->setEnabled(false);
}


void MainWindow::on_radioButton_imageEmitter_videoCaptureFromScreen_clicked()
{
////    ui->groupBox_imageEmitter_imageFromFile->hide();
////    ui->groupBox_imageEmitter_videoCaptureFromScreen->show();

//    ui->groupBox_imageEmitter_imageFromFile->setEnabled(false);
//    ui->groupBox_imageEmitter_videoCaptureFromScreen->setEnabled(true);

//    ui->pushButton_calculate->setEnabled(false);
//    ui->pushButton_saveResult->setEnabled(false);
//    ui->pushButton_runVideo->setEnabled(true);
//    ui->pushButton_screen->setEnabled(true);
}


void MainWindow::on_pushButton_runVideo_clicked()
{
    isRunning = !isRunning; // инвертируем значение
    if(!isRunning)
    {
        ui->pushButton_runVideo->setText("Run Video");
    }
    if(isRunning)
    {
        QString imageOriginalDir = ui->lineEdit_imageOriginal_video->text();    // получаем изображение фона

        if((imageOriginalDir == "" || imageOriginal.width() == 0 || imageOriginal.height() == 0) && ui->checkBox_substractObject->isChecked())
        {
            QMessageBox::warning(this, "Ошибка", "Выберите изображение оригинала!");
            return;
        }

        if(ui->checkBox_substractObject->isChecked())
        {
            imageOriginal.convertTo(QImage::Format_Grayscale16);    // конвертируем в ч/б изображение
        }
//        imageOriginal.convertTo(QImage::Format_Grayscale16);    // конвертируем в ч/б изображение

//        resultImage = imageOriginal;
        resultImage = imageObject;

        ui->pushButton_runVideo->setText("Stop Video");

        if(ui->radioButton_captureDevice_monitor->isChecked())  // если выполняется обработка изображения с монитора
        {
            while(isRunning)    // запускаем бесконечный цикл, пока не будет сигнал остановки
            {
                // получить изображение экрана
                QScreen *screen = QGuiApplication::primaryScreen(); // получаем главный экран
                QPixmap pixmap = QPixmap (); // Каждый раз присваиваем нулевое значение pixmap
                int currentScreenNumber = ui->comboBox_choseScreen->currentIndex(); // получаем номер дисплея, с которого будем получать изображение
                pixmap = screen->grabWindow(currentScreenNumber); // снимок экрана
        //        setImageToOutputLabel(pixmap.toImage());    // выводим скриншот на экран

                // обработка изображения
                imageObject = pixmap.toImage(); // переводим в картинку
                imageObject.convertTo(QImage::Format_Grayscale16);  // конвертируем в ч/б изображение

                processImageFilters(imageOriginal, imageObject, resultImage);   // применяем фильтры

                // выводим картинку
                setImageToOutputLabel(resultImage);

                // обработка интерфейса
                QApplication::processEvents();
            }
        }
        else if(ui->radioButton_captureDevice_camera->isChecked())
        {
            connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::cameraReadyForCaptureChanged);

            // делаем снимок и ожидаем его получения
//            frameTime.restart();
            camera->searchAndLock();
            imageCapture->capture();
            camera->unlock();
        }
    }
}


void MainWindow::on_pushButton_choseImageObject_video_clicked()
{
    QString imageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");
    ui->lineEdit_imageOriginal_video->setText(imageDir);
}


void MainWindow::on_radioButton_captureDevice_monitor_clicked()
{
    QStringList screens = getScreensList();
    ui->comboBox_choseScreen->clear();
    ui->comboBox_choseScreen->addItems(screens);
}


void MainWindow::on_radioButton_captureDevice_camera_clicked()
{
    QStringList camerasList = getCamerasList();
    ui->comboBox_choseScreen->clear();
    ui->comboBox_choseScreen->addItems(camerasList);
}


void MainWindow::on_pushButton_choseScreen_clicked()
{
    if(ui->radioButton_captureDevice_camera->isChecked())
    {
        int currentCameraIndex = ui->comboBox_choseScreen->currentIndex();  // получаем выбранную камеру
        QCameraInfo cameraInfo = cameras[currentCameraIndex];   // получаем камеру, с которой будем производить снятие видео
        camera = new QCamera(cameraInfo);
        viewfinder = new QCameraViewfinder;
        camera->setViewfinder(viewfinder);
        viewfinder->show();

        imageCapture = new QCameraImageCapture(camera);
        imageCapture->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);  // буферизируем
        connect(imageCapture, &QCameraImageCapture::imageCaptured, this, &MainWindow::cameraImageCaptured);
        connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::cameraReadyForCaptureChanged);

        camera->setCaptureMode(QCamera::CaptureStillImage);
        camera->start();
    }
}


void MainWindow::on_checkBox_deleteNoise_stateChanged(int arg1)
{
    ui->groupBox_hardNoiseDeletingParams->setEnabled(arg1);
}


void MainWindow::on_radioButton_imageEmitter_videoCaptureFromScreen_clicked(bool checked)
{
    if(checked)
    {
        //    ui->groupBox_imageEmitter_imageFromFile->hide();
        //    ui->groupBox_imageEmitter_videoCaptureFromScreen->show();

        ui->groupBox_imageEmitter_imageFromFile->setEnabled(false);
        ui->groupBox_imageEmitter_videoCaptureFromScreen->setEnabled(true);

        ui->pushButton_calculate->setEnabled(false);
        ui->pushButton_saveResult->setEnabled(false);
        ui->pushButton_runVideo->setEnabled(true);
        ui->pushButton_screen->setEnabled(true);
    }
}


void MainWindow::on_radioButton_imageEmitter_imageFromFile_clicked(bool checked)
{
    if(checked)
    {
        //    ui->groupBox_imageEmitter_imageFromFile->show();
        //    ui->groupBox_imageEmitter_videoCaptureFromScreen->hide();

        ui->groupBox_imageEmitter_imageFromFile->setEnabled(true);
        ui->groupBox_imageEmitter_videoCaptureFromScreen->setEnabled(false);

        ui->pushButton_calculate->setEnabled(true);
        ui->pushButton_saveResult->setEnabled(true);
        ui->pushButton_runVideo->setEnabled(false);
        ui->pushButton_screen->setEnabled(false);
    }
}

void MainWindow::onExit()
{
    qDebug() << "exit";
    uiDataSaver.saveProgramData();
    if(camera != nullptr)
    {
        camera->stop();
        delete camera;
    }
    if(viewfinder != nullptr)
    {
        viewfinder->close();
        delete viewfinder;
    }
    if(imageCapture != nullptr)
    {
        delete imageCapture;
    }
}


void MainWindow::on_checkBox_aliasing_stateChanged(int arg1)
{
//    ui->label_aliasingBorder->setEnabled(arg1);
    ui->label_aliasingRadius->setEnabled(arg1);
    ui->spinBox_aliasingRadius->setEnabled(arg1);
//    ui->horizontalSlider_aliasingBorder->setEnabled(arg1);
}


void MainWindow::on_horizontalSlider_aliasing_blackBorder_valueChanged(int value)
{
    ui->label_aliasing_blackBorder_value->setText(QString::number(value) + "%");
}


void MainWindow::on_horizontalSlider_aliasing_whiteBorder_valueChanged(int value)
{
    ui->label_aliasing_whiteBorder_value->setText(QString::number(value) + "%");
}


void MainWindow::on_toolButton_saveOriginalImage_clicked()
{
    saveImageToFileWithDialog(&imageOriginal);
}


void MainWindow::on_toolButton_saveObjectImage_clicked()
{
    saveImageToFileWithDialog(&imageObject);
}


void MainWindow::on_toolButton_saveResultImage_clicked()
{
    saveImageToFileWithDialog(&resultImage);
}


//void MainWindow::on_pushButton_clicked()
//{
//    QImage img1("C:/Users/ASUS/Documents/ImageToGrayConverter/build-ImageToGrayConverter-Desktop_Qt_5_15_2_MinGW_64_bit-Release/test_result_1.jpg");
//    QImage img2("C:/Users/ASUS/Documents/ImageToGrayConverter/build-ImageToGrayConverter-Desktop_Qt_5_15_2_MinGW_64_bit-Release/test_result_2.jpg");

////    for(int i=0; i<img1.width(); i++)
////    {
////        for(int j=0; j<img1.height(); j++)
////        {
////            if((img1.pixelColor(i, j).black() > 100 && img2.pixelColor(i, j).black() <= 100)
////                    || (img1.pixelColor(i, j).black() <= 100 && img2.pixelColor(i, j).black() > 100))   // условие инверсности пикселей
////            {
////                img1.setPixelColor(i, j, Qt::white);
////            }
////        }
////    }

//    QImage *img = colliseImages(img1, img2);

//    setImageToOutputLabel(*img);
//    img->save("C:/Users/ASUS/Documents/ImageToGrayConverter/build-ImageToGrayConverter-Desktop_Qt_5_15_2_MinGW_64_bit-Release/test_result_3_2.jpg");

//}


void MainWindow::on_pushButton_showFilters_clicked()
{
    isShowingFiltersBlock = !isShowingFiltersBlock;
    ui->groupBox_imageCorrectorParams->setVisible(isShowingFiltersBlock);
    updateLabelImageSize();
}


void MainWindow::on_pushButton_showSettingsBlock_clicked()
{
    isShowingSettingsBlock = !isShowingSettingsBlock;
    ui->groupBox_settings->setVisible(isShowingSettingsBlock);
    updateLabelImageSize();
}

