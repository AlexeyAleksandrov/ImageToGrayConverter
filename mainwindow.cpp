#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QImage image = QImage("C:/Users/ASUS/Downloads/cat.jpg");
//    image.convertTo(QImage::Format_Grayscale16);
//    ui->label_image->setPixmap(QPixmap::fromImage(image));
//    ui->label_image->setScaledContents(true);

    QColor color = Qt::red;
    int blackValue = color.black();
    color.setRgb(blackValue, blackValue, blackValue);
    QPalette palette = ui->label_image->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->label_image->setPalette(palette);
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

    int clippingNoiseValue = ui->horizontalSlider_clippingNoiseValue->value();  // граница шума, уровень ниже этой границы будет отрезан ( = 0)
    int blackEnchancement = ui->horizontalSlider_blackEnchancementValue->value();   // усиление черного, значения выше этой границы будут увеличены до максимума ( = 255)

    for (int i=0; i<imageOriginal.width(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.height(); j++)    // проходим по высоте
        {
            int blackOriginal = imageOriginal.pixelColor(i, j).black(); // уровень черного в оригинале
            int blackObject = imageObject.pixelColor(i, j).black(); // уровень черного в объекте

            int blackSub = abs(blackOriginal - blackObject);    // вычитаем фон
            if(blackSub < clippingNoiseValue)   // обрезаем шумы
            {
                blackSub = 0;
            }

            if(blackSub > blackEnchancement)    // усиливаем черный цвет
            {
                blackSub = 255;
            }

            QColor color = QColor(blackSub, blackSub, blackSub);    // формируем новый цвет пикселя
            if(color.isValid())
            {
                resultImage.setPixelColor(i, j, color); // устанавливаем нвоый цвет
            }
            else
            {
                qDebug() << "Not Valid Color: " << color << blackSub;
            }
        }
    }

    resultImage.invertPixels(); // инфвертируем цвет, т.к. при вычитании получается негатив

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

