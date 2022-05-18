#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QThread>
#include <QTime>
#include "thread"

// осталось убрать все входные параметры у функций-фильтров
// добавить привязку обработки к области действия фильтра

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
//    on_radioButton_imageEmitter_imageFromFile_clicked();    // применяем выбор
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

//    uiDataSaver.add(ui->horizontalSlider_blackEnchancementValue);
//    uiDataSaver.add(ui->horizontalSlider_clippingNoiseValue);
//    uiDataSaver.add(ui->horizontalSlider_deleteNoise);
//    uiDataSaver.add(ui->horizontalSlider_aliasing_blackBorder);
//    uiDataSaver.add(ui->horizontalSlider_aliasing_whiteBorder);

    uiDataSaver.add(ui->radioButton_original);
    uiDataSaver.add(ui->radioButton_object);
    uiDataSaver.add(ui->radioButton_result);

//    uiDataSaver.add(ui->checkBox_colorInversion);
//    uiDataSaver.add(ui->checkBox_deleteNoise);
//    uiDataSaver.add(ui->checkBox_aliasing);
//    uiDataSaver.add(ui->checkBox_aliasingVisualisation);
//    uiDataSaver.add(ui->checkBox_medianFilter);
//    uiDataSaver.add(ui->checkBox_averageFilter);
//    uiDataSaver.add(ui->checkBox_substractObject);
    uiDataSaver.add(ui->checkBox_drawFilterRect);

//    uiDataSaver.add(ui->comboBox_deleteType);
    uiDataSaver.add(ui->comboBox_presets);

//    uiDataSaver.add(ui->spinBox_aliasingRadius);
//    uiDataSaver.add(ui->spinBox_medianFilter_radius);
//    uiDataSaver.add(ui->spinBox_averageFilter_radiusValue);
//    uiDataSaver.add(ui->spinBox_collisionRepeatCount);

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

    imageLabel = new QLabel;
    ui->tableWidget_image->setCellWidget(0, 0, imageLabel); // выводим label в tableWidget
    imageLabel->setText("Изображение");

    uiDataSaver.loadProgramData();
    loadFilterLayers();

    qDebug() << "Выводим фильтры: " << filterLayers.size();
    for(int i=0; i<filterLayers.size(); i++)
    {
        qDebug() << filterLayers.at(i).filterName;
    }

    if(filterLayers.size() == 0)    // если не были загружены фильтры, добавляем фильтр по умолчанию
    {
        on_pushButton_addFilterLayer_clicked(); // добавляем фильтр главного слоя
    }

    qDebug() << "Выводим фильтры: " << filterLayers.size();
    for(int i=0; i<filterLayers.size(); i++)
    {
        qDebug() << filterLayers.at(i).filterName;
    }

    ui->lineEdit_layerName->clear();

    // соединяем сигналы для обновления фильтров согласно изменению состояния на форме
    connect(ui->horizontalSlider_filter_min_x, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->horizontalSlider_filter_max_x, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->verticalSlider_filter_min_y, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->verticalSlider_filter_max_y, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->horizontalSlider_clippingNoiseValue, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->horizontalSlider_blackEnchancementValue, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->horizontalSlider_deleteNoise, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->horizontalSlider_aliasing_blackBorder, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->horizontalSlider_aliasing_whiteBorder, &QSlider::valueChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->checkBox_deleteNoise, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->checkBox_hardNoiseClipping_deleteBlack, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->checkBox_hardNoiseClipping_deleteWhite, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->checkBox_colorInversion, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->checkBox_substractObject, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->checkBox_aliasing, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->checkBox_aliasingVisualisation, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->checkBox_averageFilter, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);
    connect(ui->checkBox_medianFilter, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);

//    connect(ui->checkBox_drawFilterRect, &QCheckBox::stateChanged, this, &MainWindow::updateFilterLayerParams);

    connect(ui->spinBox_aliasingRadius, SIGNAL(valueChanged(int)), this, SLOT(updateFilterLayerParams(int)));
    connect(ui->spinBox_averageFilter_radiusValue, SIGNAL(valueChanged(int)), this, SLOT(updateFilterLayerParams(int)));
    connect(ui->spinBox_collisionRepeatCount, SIGNAL(valueChanged(int)), this, SLOT(updateFilterLayerParams(int)));
    connect(ui->spinBox_medianFilter_radius, SIGNAL(valueChanged(int)), this, SLOT(updateFilterLayerParams(int)));

    connect(ui->comboBox_deleteType, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterLayerParams(int)));
    connect(ui->comboBox_presets, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterLayerParams(int)));
    connect(ui->comboBox_threadsCount, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFilterLayerParams(int)));

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
    qDebug() << "Нажата кнопка рассчёта";
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

    originalResultImage = imageOriginal;

    processImageFilters(imageOriginal, imageObject, originalResultImage);   // применяем фильтры

    // выводим картинку
    setImageResultToOutputLabel();

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
    if(!originalResultImage.isNull())
    {
        setImageResultToOutputLabel();
    }
}


void MainWindow::on_pushButton_saveResult_clicked()
{
    QString saveDir = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Images (*.jpg)");
    if(saveDir != "")
    {
        originalResultImage.save(saveDir);
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
    imageLabel->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::setImageResultToOutputLabel()
{
    if(resultImageWithDrawFilter.width() != 0 && resultImageWithDrawFilter.height() != 0 && ui->checkBox_drawFilterRect->isChecked())
    {
        setImageToOutputLabel(resultImageWithDrawFilter);
    }
    else
    {
        setImageToOutputLabel(originalResultImage);
    }
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
//        setImageToOutputLabel(resultImage);
        setImageResultToOutputLabel();
    }
}

void MainWindow::processImageFilters(QImage imageOriginal, QImage imageObject, QImage &resultImage)
{
    if(imageObject.isNull())
    {
        qDebug() << "processImageFilters: ImageObject is invalid!";
        QMessageBox::critical(this, "", "ImageObject is invalid!");
        return;
    }

    if(filterLayers.size() == 0)
    {
        qDebug() << "Не задан ниодин слой фильтра!";
        return;
    }

//    qDebug() << "Необходимость вычита изображения: " << filterLayers.at(0).needSubstructImage << filterLayers.at(0).clippingNoiseValue << filterLayers.at(0).blackEnchancement << filterLayers.at(0).threadsCount;

    if(filterLayers.at(0).needSubstructImage)
    {
        if(imageOriginal.isNull())
        {
            qDebug() << "processImageFilters: ImageOriginal is invalid!";
            QMessageBox::critical(this, "", "ImageOriginal is invalid!");
            return;
        }
    }

    QImage tempOriginalImage;
    if(filterLayers.at(0).needSubstructImage)
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

    if(filterLayers.at(0).needSubstructImage)
    {
        imageCorrector.substractObjectImage();    // вычитаем изображение
    }

//    qDebug() << "";
//    qDebug() << "=================";
    for(int filterNumber=0; filterNumber<filterLayers.size(); filterNumber++)
    {
//        qDebug() << "Номер фильтра: " << filterNumber+1 << " из " << filterLayers.size();

        ImageCorrectrFilterParams filter = filterLayers.at(filterNumber);
        imageCorrector.setFilter(filter);   // задаем фильтр

        int repeatOffset = filter.repeatOffset; // смещение для радиуса при применении фильтра несколько раз

        for(int offset=0; offset<=repeatOffset; offset++)
        {
//            qDebug() << "Повтор для смещения: " << offset+1 << " из " << repeatOffset;
            if(filter.needHardDeleteNoise)
            {
//                qDebug() << "Выполняется усиленное удаление шумов";

                if(filter.needHardDeleteWhiteColor) // если нужно удалять черный цвет
                {
                    imageCorrector.hardClipNoise(filter.hardDeleteNoiseBorder, filter.hardDeleteNoiseDeleteType, ImageCorrectorEnums::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
                }
                if(filter.needHardDeleteBlackColor) // если нкжно удалять белый цвет
                {
                    imageCorrector.hardClipNoise(filter.hardDeleteNoiseBorder, filter.hardDeleteNoiseDeleteType, ImageCorrectorEnums::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
                }

//                qDebug() << "Удаление выполнено!";
            }

            //    imageCorrector.enchanceBlackColor();    // усиление черного цвета
            if(filter.clippingNoiseValue > 0)
            {
//                qDebug() << "Выполняется простое удаление шума: " << filter.clippingNoiseValue;
                imageCorrector.clipNoise();    // простое удаление шума
            }

            if(filter.blackEnchancement > 0)
            {
//                qDebug() << "Выполняется усиление чёрного цвета: " << filter.blackEnchancement;
                imageCorrector.enchanceBlackColor();    // усиление черного цвета
            }

            if(filter.needMedianFilter)
            {
                int radius = filter.medianFilter_radius + offset;  // радиус медианного фильтра
                imageCorrector.medianRadiusFilter(radius);  // применяем медианный фильтр нового типа (старый вариант остался на всякий случай)
//                qDebug() << "Выполняется медианный фильтр: " << radius;
            }
            if(filter.needAverageFilter)
            {
                int radius = filter.averageFilter_radius + offset;  // радиус среднеарифметического фильтра
                imageCorrector.averageFilter(radius);   // среднеарифметичсекий фильтр
//                qDebug() << "Выполняется среднеарифметический фильтр: " << radius;
            }

            if(filter.needAliasing)
            {
                int aliasingRadius = filter.aliasingRadius + offset;
                //        int aliasingBorder = ui->horizontalSlider_aliasingBorder->value();
                int aliasingBorder = 126; // 255/2
                int blackBorder = /*100 - */filter.aliasingBlackBorder;
                int whiteBorder = /*100 - */filter.aliasingWhiteBorder;
                imageCorrector.aliasing(aliasingRadius, aliasingBorder, blackBorder, whiteBorder);
                //        imageCorrector.aliasing(aliasingRadius+1, aliasingBorder);

//                qDebug() << "Выполняется увеличение резкости: " << aliasingRadius << aliasingBorder << blackBorder << whiteBorder;
            }

            QImage result = imageCorrector.getResultImage();   // получаем обработанное изображение

//            if(filterNumber == filterLayers.size()-1 && filterLayers.at(0).needColorInversion)
//            {
//                result.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
//            }

            if(offset > 0)  // если есть предыдущее изображение
            {
                resultImage = *colliseImages(resultImage, result);  // объединяем методом Исключающего ИЛИ
//                qDebug() << "Применяется Исключающее ИЛИ: " << offset << "/" << repeatOffset;
            }
            else    // иначе, если это первая итерация, то просто сохраняем
            {
                resultImage = result;
//                qDebug() << "Передаём изображение в результат ";
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

        imageCorrector.setImageObject(resultImage); // в качесте опорного изображения берём предыдущее
    }

    if(filterLayers.at(0).needColorInversion)
    {
        resultImage.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
        qDebug() << "Инвертируем пиксели";
    }

    ui->horizontalSlider_filter_min_x->setMaximum(resultImage.width());
    ui->horizontalSlider_filter_max_x->setMaximum(resultImage.width());
    ui->verticalSlider_filter_min_y->setMaximum(resultImage.height());
    ui->verticalSlider_filter_max_y->setMaximum(resultImage.height());

    redrawImageFilterRect();
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

ImageCorrectrFilterParams MainWindow::createFilterParams(QString filterName)
{
    ImageCorrectrFilterParams params;   // создаем объект параметров

    params.filterName = filterName;

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
    params.hardDeleteNoiseDeleteType = ImageCorrectorEnums::NoiseDeleteTypes(deleteType);

    params.medianFilter_radius = ui->spinBox_medianFilter_radius->value();  // радиус медианного фильтра
    params.averageFilter_radius = ui->spinBox_averageFilter_radiusValue->value();  // радиус среднеарифметического фильтра
    params.aliasingRadius = ui->spinBox_aliasingRadius->value();
    params.aliasingBorder = 126; // 255/2
    params.aliasingBlackBorder = ui->horizontalSlider_aliasing_blackBorder->value();
    params.aliasingWhiteBorder = ui->horizontalSlider_aliasing_whiteBorder->value();

    // обрабатываем параметры изображения
    params.heightStart = ui->verticalSlider_filter_min_y->value();
    params.widthStart = ui->horizontalSlider_filter_min_x->value();
    params.heightEnd = ui->verticalSlider_filter_max_y->value();
    params.widthEnd = ui->horizontalSlider_filter_max_x->value();

    qDebug() << "Сохранение настроек фильтра: " << params.filterName << params.widthStart << params.widthEnd << params.heightStart << params.heightEnd;

    return params;
}

void MainWindow::applyFilterParams(ImageCorrectrFilterParams filterParams)
{
//    ui->lineEdit_layerName->setText(filterParams.filterName);

    // обрабатываем флаги
    ui->checkBox_substractObject->setChecked(filterParams.needSubstructImage);
    ui->checkBox_deleteNoise->setChecked(filterParams.needHardDeleteNoise);
    ui->checkBox_hardNoiseClipping_deleteWhite->setChecked(filterParams.needHardDeleteWhiteColor);
    ui->checkBox_hardNoiseClipping_deleteBlack->setChecked(filterParams.needHardDeleteBlackColor);
    ui->checkBox_medianFilter->setChecked(filterParams.needMedianFilter);
    ui->checkBox_averageFilter->setChecked(filterParams.needAverageFilter);
    ui->checkBox_aliasing->setChecked(filterParams.needAliasing);
    ui->checkBox_colorInversion->setChecked(filterParams.needColorInversion);
    ui->checkBox_aliasingVisualisation->setChecked(filterParams.needAliasingVisualisation);

    // обрабатываем параметры
    ui->horizontalSlider_clippingNoiseValue->setValue(filterParams.clippingNoiseValue);  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
    ui->horizontalSlider_blackEnchancementValue->setValue(filterParams.blackEnchancement);   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)
    ui->comboBox_threadsCount->setCurrentIndex(filterParams.threadsCount - 1);    // получаем количество потоков, которое мы можем использовать
    ui->spinBox_collisionRepeatCount->setValue(filterParams.repeatOffset); // смещение для радиуса при применении фильтра несколько раз
    ui->horizontalSlider_deleteNoise->setValue(filterParams.hardDeleteNoiseBorder);  // граница продвинутого удаления шумов

    int deleteType = filterParams.hardDeleteNoiseDeleteType;
    ui->comboBox_deleteType->setCurrentIndex(deleteType);


    ui->spinBox_medianFilter_radius->setValue(filterParams.medianFilter_radius);  // радиус медианного фильтра
    ui->spinBox_averageFilter_radiusValue->setValue(filterParams.averageFilter_radius);  // радиус среднеарифметического фильтра
    ui->spinBox_aliasingRadius->setValue(filterParams.aliasingRadius);
//    filterParams.aliasingBorder = 126; // 255/2
    ui->horizontalSlider_aliasing_blackBorder->setValue(filterParams.aliasingBlackBorder);
    ui->horizontalSlider_aliasing_whiteBorder->setValue(filterParams.aliasingWhiteBorder);

    // обрабатываем параметры изображения
    ui->verticalSlider_filter_max_y->setValue(filterParams.heightEnd);
    ui->horizontalSlider_filter_max_x->setValue(filterParams.widthEnd);

    ui->verticalSlider_filter_min_y->setValue(filterParams.heightStart);
    ui->horizontalSlider_filter_min_x->setValue(filterParams.widthStart);

    ui->verticalSlider_filter_max_y->setValue(filterParams.heightEnd);  // двойной вызов необходим для корректной установки значений мин и макс для соответсвующих слайдеров
    ui->horizontalSlider_filter_max_x->setValue(filterParams.widthEnd);


//    qDebug() << "Применение настроек фильтра: " << filterParams.filterName << filterParams.widthStart << filterParams.widthEnd << filterParams.heightStart << filterParams.heightEnd;
}

void MainWindow::loadFilterLayers()
{
    QFile file("filterlayers.json");
    if(!file.exists())
    {
        return;
    }
    file.open(QIODevice::ReadOnly);
    QByteArray fileText = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(fileText);  // поулчаем JSON
    QJsonArray jsonArray = doc.array(); // конвертируем его в массив

    ui->comboBox_layers->blockSignals(true);    // блокируем все сигналы для ComboBox

    if(jsonArray.size() > 0 && filterLayers.size() > 0)
    {
        filterLayers.clear();
        ui->comboBox_layers->clear();
    }

    for(int i=0; i<jsonArray.size(); i++)
    {
        QJsonObject json = jsonArray[i].toObject(); // получаем объект слоя
        ImageCorrectrFilterParams filter;   // создаём фильтр
        filter.setFilterParamsFromJsonObject(json); // задаём ему параметры из JSON
        filterLayers.append(filter);
        ui->comboBox_layers->addItem(filter.filterName);
    }

    ui->comboBox_layers->setCurrentIndex(0);    // устанавливаем исходный индекс на начальный

    ui->comboBox_layers->blockSignals(false);   // разблокируем все сигналы для ComboBox

    applyFilterParams(filterLayers.first());    // применяем сохранённый фильтр
}

void MainWindow::saveFiterLayers()
{
    QJsonDocument doc;
    QJsonArray jsonArray;

    for (int i=0; i<filterLayers.size(); i++)
    {
        QJsonObject json = filterLayers[i].getFilterJsonObject();   // получаем JSON представление фильтра
        jsonArray.append(json);
        qDebug() << "Добавляю " << filterLayers[i].filterName << json["filterName"].toString();
    }

    doc.setArray(jsonArray);

    QFile file("filterlayers.json");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
    file.close();
}

void MainWindow::updateFilterLayerParams(int value)
{
    Q_UNUSED(value);
    on_toolButton_updateLayerConfiguration_clicked();   // вызываем функцию обновления информации о слое
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

void MainWindow::redrawImageFilterRect()
{
    int max_x = ui->horizontalSlider_filter_max_x->value();
    int max_y = ui->verticalSlider_filter_max_y->value();

    int min_x = ui->horizontalSlider_filter_min_x->value();
    int min_y = ui->verticalSlider_filter_min_y->value();

    ui->horizontalSlider_filter_min_x->setMaximum(max_x);
    ui->verticalSlider_filter_min_y->setMaximum(max_y);

    ui->horizontalSlider_filter_max_x->setMinimum(min_x);
    ui->verticalSlider_filter_max_y->setMinimum(min_y);

    ui->horizontalSlider_filter_min_x->setValue(min_x);
    ui->verticalSlider_filter_min_y->setValue(min_y);

    ui->horizontalSlider_filter_max_x->setValue(max_x);
    ui->verticalSlider_filter_max_y->setValue(max_y);

    if(originalResultImage.width() != 0 && originalResultImage.height() != 0)
    {
        resultImageWithDrawFilter = QImage(originalResultImage);
        QPainter *painter = new QPainter(&resultImageWithDrawFilter);
        QPen *pen = new QPen;
        pen->setWidth(1);
        pen->setColor(Qt::red);
        painter->setPen(*pen);

        painter->drawRect(min_x, min_y, max_x - min_x, max_y - min_y);

        delete painter;
        delete pen;

        if(ui->radioButton_result->isChecked())
        {
            setImageResultToOutputLabel();
        }
    }

//    qDebug() << "x_min = " << min_x << "x_max = " << max_x << "y_min = " << min_y << "y_max" << max_y;
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
        processImageFilters(imageOriginal, imageObject, originalResultImage);

        // выводим полученный снимок
//        setImageToOutputLabel(resultImage);
        setImageResultToOutputLabel();
    }

    QApplication::processEvents();
}

void MainWindow::cameraReadyForCaptureChanged(bool ready)
{
    if(ready && isRunning)  // если можно продолжать
    {
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

        // делаем снимок и ожидаем его получения
        camera->searchAndLock();
        imageCapture->capture();
        camera->unlock();
    }
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

        originalResultImage = imageObject;

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

                // обработка изображения
                imageObject = pixmap.toImage(); // переводим в картинку
                imageObject.convertTo(QImage::Format_Grayscale16);  // конвертируем в ч/б изображение

                processImageFilters(imageOriginal, imageObject, originalResultImage);   // применяем фильтры

                // выводим картинку
                setImageResultToOutputLabel();

                // обработка интерфейса
                QApplication::processEvents();
            }
        }
        else if(ui->radioButton_captureDevice_camera->isChecked())
        {
            connect(imageCapture, &QCameraImageCapture::readyForCaptureChanged, this, &MainWindow::cameraReadyForCaptureChanged);

            // делаем снимок и ожидаем его получения
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
    ui->comboBox_layers->setCurrentIndex(0);
    on_comboBox_layers_currentIndexChanged(0);  // переключаем на нулевой слой
    saveFiterLayers();
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
    ui->label_aliasingRadius->setEnabled(arg1);
    ui->spinBox_aliasingRadius->setEnabled(arg1);
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
    saveImageToFileWithDialog(&originalResultImage);
}

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


void MainWindow::on_verticalSlider_filter_min_y_valueChanged(int value)
{
    redrawImageFilterRect();
    Q_UNUSED(value);
}


void MainWindow::on_horizontalSlider_filter_min_x_valueChanged(int value)
{
    redrawImageFilterRect();
    Q_UNUSED(value);
}


void MainWindow::on_verticalSlider_filter_max_y_valueChanged(int value)
{
    redrawImageFilterRect();
    Q_UNUSED(value);
}


void MainWindow::on_horizontalSlider_filter_max_x_valueChanged(int value)
{
    redrawImageFilterRect();
    Q_UNUSED(value);
}

void MainWindow::on_pushButton_addFilterLayer_clicked()
{
    QString layerName = ui->lineEdit_layerName->text(); // название слоя
    if(layerName == "")
    {
        int count = ui->comboBox_layers->count();   // получаем кол-во слоёв
        layerName = "Слой #" + QString::number(count);
    }

    ui->lineEdit_layerName->setText(layerName);

    ImageCorrectrFilterParams filter = createFilterParams(layerName);    // получаем текущие настройки
    int currentIndex = ui->comboBox_layers->currentIndex(); // текущий выбранный номер слоя

    filterLayers.insert(currentIndex+1, filter);    // добавлем фильтр в список
    qDebug() << "Добавлен фильтр: " << filter.filterName << layerName << filterLayers.last().filterName;

    ui->comboBox_layers->insertItem(currentIndex+1, layerName); // добавляем название слоя в список
    ui->comboBox_layers->setCurrentIndex(currentIndex+1);    // устанавливаем текущий элемент
    ui->lineEdit_layerName->clear();    // очищаем название слоя
}


void MainWindow::on_pushButton_removeFilterLayer_clicked()
{
    int currentIndex = ui->comboBox_layers->currentIndex(); // текущий выбранный номер слоя

    if(currentIndex < 1)    // не позвляем удалить главный слой
    {
        return;
    }

    filterLayers.removeAt(currentIndex);    // удалем слой из списка
    noUpdateLayersParams = true;    // временно блокируем обновление параметров
    ui->comboBox_layers->removeItem(currentIndex);  // удаляем слой из выпадающего списка
    noUpdateLayersParams = false;
}


void MainWindow::on_comboBox_layers_currentIndexChanged(int index)
{
    if(!noUpdateLayersParams)   // если нет запрета на обновление старых настроек
    {
        // обновляем настройки предыдущего слоя
        QString lastFilterLayerName = ui->comboBox_layers->itemText(lastComboBoxFilterLayersIndex); // название обновляемого фильтра
        ImageCorrectrFilterParams filter = createFilterParams(lastFilterLayerName);    // получаем текущие настройки
        filterLayers.replace(lastComboBoxFilterLayersIndex, filter);    // заменяем старые настройки на новые
    }

    // получаем настройки нового слоя
    ImageCorrectrFilterParams currentFilter = filterLayers.at(index);   // получаем текущий фильтр
    applyFilterParams(currentFilter);   // применяем выбранный фильтр

    if(index < 1)  // блокируем возможность использовать общие настройки, неприменимые к конкретным координатам
    {
        ui->checkBox_substractObject->setEnabled(true);
        ui->checkBox_colorInversion->setEnabled(true);
    }
    else
    {
        ui->checkBox_substractObject->setChecked(false);
        ui->checkBox_colorInversion->setChecked(false);
        ui->checkBox_substractObject->setEnabled(false);
        ui->checkBox_colorInversion->setEnabled(false);
    }

    lastComboBoxFilterLayersIndex = index;  // обновляем последний индекс
}


void MainWindow::on_checkBox_drawFilterRect_stateChanged(int arg1)
{
    redrawImageFilterRect();
    Q_UNUSED(arg1);
}


void MainWindow::on_toolButton_updateLayerConfiguration_clicked()
{
    int currentIndex = ui->comboBox_layers->currentIndex(); // текущий выбранный номер слоя
    QString layerName = ui->lineEdit_layerName->text(); // получаем название слоя
    if(layerName == "")
    {
        layerName = ui->comboBox_layers->currentText(); // иначе получаем текущее название
    }
    else
    {
        // если название не пустое
        ui->comboBox_layers->setItemText(currentIndex, layerName);
        ui->lineEdit_layerName->clear();
    }

    // обновляем настройки слоя
    ImageCorrectrFilterParams filter = createFilterParams(layerName);    // получаем текущие настройки
    filterLayers.replace(currentIndex, filter);    // заменяем старые настройки на новые
//    qDebug() << "Начало Х: " << filter.widthStart;

    if(!isRunning && ui->checkBox_autoReCalculate->isChecked())
    {
        on_pushButton_calculate_clicked();

        QApplication::processEvents();
    }
}


void MainWindow::on_toolButton_reloadCamerasList_clicked()
{
    if(ui->radioButton_captureDevice_camera->isChecked())
    {
        QStringList camerasList = getCamerasList();
        ui->comboBox_choseScreen->clear();
        ui->comboBox_choseScreen->addItems(camerasList);
    }
    else if(ui->radioButton_captureDevice_monitor->isChecked())
    {
        QStringList screens = getScreensList();
        ui->comboBox_choseScreen->clear();
        ui->comboBox_choseScreen->addItems(screens);
    }
    else
    {
        return;
    }
}

