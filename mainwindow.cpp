#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    uiDataSaver.add(ui->lineEdit_imageOriginal);
    uiDataSaver.add(ui->lineEdit_imageObject);

    uiDataSaver.add(ui->horizontalSlider_blackEnchancementValue);
    uiDataSaver.add(ui->horizontalSlider_clippingNoiseValue);

    uiDataSaver.add(ui->radioButton_original);
    uiDataSaver.add(ui->radioButton_object);
    uiDataSaver.add(ui->radioButton_result);

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

    ImageCorrector imageCorrecor;
    imageCorrecor.setImageOriginal(imageOriginal);
    imageCorrecor.setImageObject(imageObject);

    imageCorrecor.subtractObjectImage();    // вычитаем изображение
    imageCorrecor.clipNoise(clippingNoiseValue);    // простое удаление шума
    imageCorrecor.enchanceBlackColor(blackEnchancement);    // усиление черного цвета

    if(ui->checkBox_colorInversion->isChecked())
    {
       imageCorrecor.invertPixels(); // инвертируем цвет, т.к. при вычитании получается негатив
    }

    resultImage = imageCorrecor.getResultImage();   // получаем обработанное изображение

    // устанавливаем границы для Label
    int widht = ui->label_image->width();
    int height = ui->label_image->height();

    ui->label_image->setMaximumWidth(widht);
    ui->label_image->setMaximumHeight(height);

    ui->label_image->setScaledContents(true);
    ui->label_image->setPixmap(QPixmap::fromImage(resultImage));

    // переводим radioButton
    ui->radioButton_result->setChecked(true);

}


void MainWindow::on_radioButton_original_clicked()
{
    if(!imageOriginal.isNull())
    {
        ui->label_image->setPixmap(QPixmap::fromImage(QImage(imageOriginal)));
    }
}


void MainWindow::on_radioButton_object_clicked()
{
    if(!imageObject.isNull())
    {
        ui->label_image->setPixmap(QPixmap::fromImage(QImage(imageObject)));
    }
}


void MainWindow::on_radioButton_result_clicked()
{
    if(!resultImage.isNull())
    {
        ui->label_image->setPixmap(QPixmap::fromImage(QImage(resultImage)));
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

