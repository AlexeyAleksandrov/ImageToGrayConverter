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


void MainWindow::on_pushButton_choseInputImage_clicked()
{
    QString inputImageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");

    QImage image = QImage(inputImageDir);
    image.convertTo(QImage::Format_Grayscale16);
    image.invertPixels();

    int widht = ui->label_image->width();
    int height = ui->label_image->height();

    ui->label_image->setMaximumWidth(widht);
    ui->label_image->setMaximumHeight(height);

    ui->label_image->setScaledContents(true);
    ui->label_image->setPixmap(QPixmap::fromImage(image));

    QFile file("pixels.txt");
    file.open(QIODevice::WriteOnly);
    for (int i=0; i<image.width(); i++)
    {
        for (int j=0; j<image.height(); j++)
        {
            QColor color = image.pixel(i, j);
            file.write(QString(QString::number(color.black()) + "\t").toUtf8());
        }
        file.write("\r\n");
    }
    file.close();
    qDebug() << "Готово!";
}
