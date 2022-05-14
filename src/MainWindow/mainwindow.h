#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <qdebug.h>
#include <QRgb>
#include <QMessageBox>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCloseEvent>
#include <QTime>
#include <QTableWidgetItem>
#include <QResizeEvent>
#include <QPainter>


#include "src/UiHandlers/uidatasaver.h"
#include "src/ImageHandlers/imagecorrector.h"
#include "src/ImageHandlers/imagetransformer.h"
#include "src/ImageFilters/imagecorrectrfilterparams.h"

#include <QJsonDocument>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_choseImageOriginal_clicked();

    void on_pushButton_choseImageObject_clicked();

    void on_pushButton_calculate_clicked();

    void on_radioButton_original_clicked();

    void on_radioButton_object_clicked();

    void on_radioButton_result_clicked();

    void on_pushButton_saveResult_clicked();

    void on_horizontalSlider_clippingNoiseValue_valueChanged(int value);

    void on_horizontalSlider_blackEnchancementValue_valueChanged(int value);

    void on_horizontalSlider_deleteNoise_valueChanged(int value);

    void on_pushButton_presets_accept_clicked();

    void on_pushButton_presets_addNew_clicked();

    void on_pushButton_presets_updateCurrent_clicked();

    void on_pushButton_presets_deleteChosed_clicked();

    void on_pushButton_screen_clicked();

    void on_pushButton_runVideo_clicked();

    void on_pushButton_choseImageObject_video_clicked();

public:
    void setImageToOutputLabel(QImage image);  // вывести картинку
    void setImageResultToOutputLabel();     // вывести результирующее изображение
    void updateLabelImageSize();    // перерасчитать азмеры label с изображением
    void processImageFilters(QImage imageOriginal, QImage imageObject, QImage &resultImage);  // функция рассчёта фильтров
    QImage* colliseImages(QImage &imageDown, QImage &imageUpper);  // коллизия 2х изображений

    ImageCorrectrFilterParams createFilterParams(QString filterName); // создать набор параметров на основе данных на интерфейсе
    void applyFilterParams(ImageCorrectrFilterParams filterParams); // установить на интерфейс выбранный фильтр

    void loadFilterLayers();    // загрузка списка фильтров из файла
    void saveFiterLayers();     // сохранение списка слоёв в файл

public slots:
    void updateFilterLayerParams(int value); // слот обновления параметров слоя

private:
    void saveImageToFileWithDialog(QImage *image);    // сохранить изображение в файл с отображением диалога выбора
    void redrawImageFilterRect(); // перерисовать линии грницы фильтра для изображения

private:
    // экраны
    QStringList getScreensList();   // получить список мониторов

    // камера
    QStringList getCamerasList();   // получить список камер, подключенных к ПК

    QCamera *camera = nullptr;    // объект камеры
    QCameraViewfinder *viewfinder = nullptr;  // обработчик камеры
    QCameraImageCapture *imageCapture = nullptr;  // обработчик снимков камеры
    QList<QCameraInfo> cameras; // список доступных камер

private slots:
    void cameraImageCaptured(int id, const QImage &preview);    // получаение изображения
    void cameraReadyForCaptureChanged(bool ready); // готовы дальше получать изображения

    void on_radioButton_captureDevice_monitor_clicked();

    void on_radioButton_captureDevice_camera_clicked();

    void on_pushButton_choseScreen_clicked();

    void on_checkBox_deleteNoise_stateChanged(int arg1);

    void on_radioButton_imageEmitter_videoCaptureFromScreen_clicked(bool checked);

    void on_radioButton_imageEmitter_imageFromFile_clicked(bool checked);

    void on_checkBox_aliasing_stateChanged(int arg1);

    void on_horizontalSlider_aliasing_blackBorder_valueChanged(int value);

    void on_horizontalSlider_aliasing_whiteBorder_valueChanged(int value);

    void on_toolButton_saveOriginalImage_clicked();

    void on_toolButton_saveObjectImage_clicked();

    void on_toolButton_saveResultImage_clicked();

    void on_pushButton_showFilters_clicked();

    void on_pushButton_showSettingsBlock_clicked();

    void on_verticalSlider_filter_min_y_valueChanged(int value);

    void on_horizontalSlider_filter_min_x_valueChanged(int value);

    void on_verticalSlider_filter_max_y_valueChanged(int value);

    void on_horizontalSlider_filter_max_x_valueChanged(int value);

    void on_pushButton_addFilterLayer_clicked();

    void on_pushButton_removeFilterLayer_clicked();

    void on_comboBox_layers_currentIndexChanged(int index);

    void on_checkBox_drawFilterRect_stateChanged(int arg1);

    void on_toolButton_updateLayerConfiguration_clicked();

private:
    Ui::MainWindow *ui;

    QImage imageOriginal;
    QImage imageObject;
    QImage originalResultImage;
    QImage resultImageWithDrawFilter;

    QLabel *imageLabel= nullptr;    // label, в который будет выводиться изображение

    ImageCorrector imageCorrector;   // обработчик изображений

    UiDataSaver uiDataSaver;

    QScreen *screen = nullptr;  // экран, с которого мы будем брать изображение
    bool isRunning = false;
    bool isScreening = false;
    bool isShowingFiltersBlock = true;  // флаг показа блока фильтров
    bool isShowingSettingsBlock = true; // флаг показа блока настроек

    QList<ImageCorrectrFilterParams> filterLayers;  // список слоёв фильтров
    int lastComboBoxFilterLayersIndex = 0;  // последний выбранный индекс в comboBox
    bool noUpdateLayersParams = false;

    void closeEvent(QCloseEvent *event)
    {
        qDebug() << "close";
        onExit();
        std::exit(EXIT_SUCCESS);
        event->accept();
    };

    void resizeEvent(QResizeEvent *event)
    {
        updateLabelImageSize();
        event->accept();
    };

    void onExit();  // выполняется при выходе
};
#endif // MAINWINDOW_H
