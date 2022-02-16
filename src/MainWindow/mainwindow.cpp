#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QThread>
#include <QTime>

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

    uiDataSaver.add(ui->radioButton_original);
    uiDataSaver.add(ui->radioButton_object);
    uiDataSaver.add(ui->radioButton_result);

    uiDataSaver.add(ui->checkBox_colorInversion);
    uiDataSaver.add(ui->checkBox_deleteNoise);

    uiDataSaver.add(ui->comboBox_deleteType);
    uiDataSaver.add(ui->comboBox_presets);

    uiDataSaver.loadPresets();

    QStringList presets = uiDataSaver.getPresets();
    ui->comboBox_presets->addItems(presets);    // выводим пресеты

    uiDataSaver.loadProgramData();

    on_horizontalSlider_blackEnchancementValue_valueChanged(ui->horizontalSlider_blackEnchancementValue->value());
    on_horizontalSlider_clippingNoiseValue_valueChanged(ui->horizontalSlider_clippingNoiseValue->value());
}

MainWindow::~MainWindow()
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

//    int clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
//    int blackEnchancement = 255 - ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)

//    int threadsCount = ui->comboBox_threadsCount->currentText().toInt();    // получаем количество потоков, которое мы можем использовать

//    imageCorrecor.setClippingNoiseValue(clippingNoiseValue);
//    imageCorrecor.setBlackEnchancement(blackEnchancement);
//    imageCorrecor.setThreadsCount(threadsCount);

    processImageFilters(imageOriginal, imageObject, resultImage);
//    ImageCorrector imageCorrecor;
//    imageCorrecor.setThreadsCount(threadsCount);    // устанавливаем количество потоков, которое будет использовать программа
//    imageCorrecor.setImageOriginal(imageOriginal);
//    imageCorrecor.setImageObject(imageObject);

//    imageCorrecor.substractObjectImage();    // вычитаем изображение
//    imageCorrecor.clipNoise(clippingNoiseValue);    // простое удаление шума

//    if(ui->checkBox_deleteNoise->isChecked())
//    {
//        int deleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов
//        int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
//        ImageCorrector::NoiseDeleteTypes type = ImageCorrector::NoiseDeleteTypes(deleteType);

//        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
//        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
//    }

//    imageCorrecor.enchanceBlackColor(blackEnchancement);    // усиление черного цвета

//    if(ui->checkBox_medianFilter->isChecked())
//    {
//        imageCorrecor.medianFilter();
//    }
////    imageCorrecor.medianFilter();   // применение медианного фильтра

//    if(ui->checkBox_colorInversion->isChecked())
//    {
//       imageCorrecor.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
//    }

//    resultImage = imageCorrecor.getResultImage();   // получаем обработанное изображение

//    if(ui->checkBox_colorInversion->isChecked())
//    {
//       resultImage.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
//    }

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
    int labelWidht = ui->label_image->width();
    int labelHeight = ui->label_image->height();

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
    ui->label_image->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::processImageFilters(QImage &imageOriginal, QImage &imageObject, QImage &resultImage)
{
    imageCorrecor.setImageOriginal(imageOriginal);
    imageCorrecor.setImageObject(imageObject);

    int clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
    int blackEnchancement = 255 - ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)

    imageCorrecor.setClippingNoiseValue(clippingNoiseValue);
    imageCorrecor.setBlackEnchancement(blackEnchancement);

    int threadsCount = ui->comboBox_threadsCount->currentText().toInt();    // получаем количество потоков, которое мы можем использовать
    imageCorrecor.setThreadsCount(threadsCount);    // устанавливаем количество потоков, которое будет использовать программа

    imageCorrecor.substractObjectImage();    // вычитаем изображение
    imageCorrecor.clipNoise();    // простое удаление шума

    if(ui->checkBox_deleteNoise->isChecked())
    {
        int deleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов
        int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
        ImageCorrector::NoiseDeleteTypes type = ImageCorrector::NoiseDeleteTypes(deleteType);

        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
    }

    imageCorrecor.enchanceBlackColor();    // усиление черного цвета

    if(ui->checkBox_medianFilter->isChecked())
    {
        imageCorrecor.medianFilter();
    }

//    if(ui->checkBox_colorInversion->isChecked())
//    {
//       imageCorrecor.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
//    }

    resultImage = imageCorrecor.getResultImage();   // получаем обработанное изображение

    if(ui->checkBox_colorInversion->isChecked())
    {
       resultImage.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
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
        setImageToOutputLabel(imageOriginal);
//        ui->lineEdit_imageOriginal_video->setText(QString("Сохранено ") + QTime().currentTime().hour() + ":" + QTime().currentTime().minute() + ":" + QTime().currentTime().second());
        ui->lineEdit_imageOriginal_video->setText(QString("Сохранено ") + QTime().currentTime().toString());
        isScreening = false;
        return;
    }

    // обрабатываем полученный снимок
    if(ui->radioButton_object->isChecked()) // если над опоказать оригинал с камеры
    {
        setImageToOutputLabel(preview.convertToFormat(QImage::Format_Grayscale16));
    }
    else if(ui->radioButton_result->isChecked())    // если нужн опоказать полученное изображение
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
    uiDataSaver.applyPreset(currentPresetName); // применяем пресет
    ui->lineEdit_presets_name->setText(currentPresetName);  // выводим название
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
    ui->groupBox_imageEmitter_imageFromFile->show();
    ui->groupBox_imageEmitter_videoCaptureFromScreen->hide();
}


void MainWindow::on_radioButton_imageEmitter_videoCaptureFromScreen_clicked()
{
    ui->groupBox_imageEmitter_imageFromFile->hide();
    ui->groupBox_imageEmitter_videoCaptureFromScreen->show();
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

        if(imageOriginalDir == "")
        {
            QMessageBox::warning(this, "Ошибка", "Выберите изображение оригинала!");
            return;
        }

        imageOriginal.convertTo(QImage::Format_Grayscale16);    // конвертируем в ч/б изображение

        resultImage = imageOriginal;

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

