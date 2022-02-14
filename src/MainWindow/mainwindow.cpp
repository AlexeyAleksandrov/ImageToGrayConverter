#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScreen>
#include <QThread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    on_radioButton_imageEmitter_imageFromFile_clicked();    // применяем выбор

    auto screens = QGuiApplication::screens();  // поулчаем список экранов, подключенных к ПК
    for(QScreen *screenItem : qAsConst(screens))
    {
        ui->comboBox_choseScreen->addItem(screenItem->name());  // добавляем каждый дисплей в список
    }

    uiDataSaver.add(ui->lineEdit_imageOriginal);
    uiDataSaver.add(ui->lineEdit_imageObject);
    uiDataSaver.add(ui->lineEdit_presets_name);

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
    uiDataSaver.saveProgramData();
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

    int clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
    int blackEnchancement = 255 - ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)

    int threadsCount = ui->comboBox_threadsCount->currentText().toInt();    // получаем количество потоков, которое мы можем использовать

    ImageCorrector imageCorrecor;
    imageCorrecor.setThreadsCount(threadsCount);    // устанавливаем количество потоков, которое будет использовать программа
    imageCorrecor.setImageOriginal(imageOriginal);
    imageCorrecor.setImageObject(imageObject);

    imageCorrecor.substractObjectImage();    // вычитаем изображение
    imageCorrecor.clipNoise(clippingNoiseValue);    // простое удаление шума

    if(ui->checkBox_deleteNoise->isChecked())
    {
        int deleteNoiseBorder = ui->horizontalSlider_deleteNoise->value();  // граница продвинутого удаления шумов
        int deleteType = ui->comboBox_deleteType->currentIndex();   // выбранный тип
        ImageCorrector::NoiseDeleteTypes type = ImageCorrector::NoiseDeleteTypes(deleteType);

        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
        imageCorrecor.hardClipNoise(deleteNoiseBorder, type, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
    }

    imageCorrecor.enchanceBlackColor(blackEnchancement);    // усиление черного цвета

    if(ui->checkBox_medianFilter->isChecked())
    {
        imageCorrecor.medianFilter();
    }
//    imageCorrecor.medianFilter();   // применение медианного фильтра

    if(ui->checkBox_colorInversion->isChecked())
    {
       imageCorrecor.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
    }

    resultImage = imageCorrecor.getResultImage();   // получаем обработанное изображение

    if(ui->checkBox_colorInversion->isChecked())
    {
       resultImage.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
    }

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
    QScreen *screen = QGuiApplication::primaryScreen();

    QPixmap pixmap = QPixmap (); // Каждый раз присваиваем нулевое значение pixmap
    pixmap = screen->grabWindow (0); // Снимок экрана
//    pixmap.save("C:/Users/ASUS/Pictures/qtscreen.jpg");
    setImageToOutputLabel(pixmap.toImage());    // выводим скриншот на экран
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
    if(isRunning)
    {
        ui->pushButton_runVideo->setText("Stop Video");
    }
    else
    {
        ui->pushButton_runVideo->setText("Run Video");
    }
    while(isRunning)    // запускаем бесконечный цикл, пока не будет сигнал остановки
    {
        // получить изображение экрана
        QScreen *screen = QGuiApplication::primaryScreen(); // получаем главный экран
        QPixmap pixmap = QPixmap (); // Каждый раз присваиваем нулевое значение pixmap
        int currentScreenNumber = ui->comboBox_choseScreen->currentIndex(); // получаем номер дисплея, с которого будем получать изображение
        pixmap = screen->grabWindow(currentScreenNumber); // снимок экрана
        setImageToOutputLabel(pixmap.toImage());    // выводим скриншот на экран
        QApplication::processEvents();
    }
}

