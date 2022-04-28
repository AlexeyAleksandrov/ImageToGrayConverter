//#include "uidatasaver.h"
#include "src/UiHandlers/uidatasaver.h"

#define appendToList(widget, list) if(widget) { list.append(widget); }
#define setStateFromString(type, list, setStateFunc) if(seletedFileLine.startsWith(QString(type))) { seletedFileLine = seletedFileLine.remove(QString(type) + lineDataSeparator); loadFromSaveString(seletedFileLine, list, setStateFunc);}

UiDataSaver::UiDataSaver(QObject *parent)  : QObject(parent)
{
//    //log.writter = "UiDataSaver";
    setPresetsDir(QDir::currentPath() + "/presents/");
}

UiDataSaver::~UiDataSaver()
{
//    qDebug() << "вызван деструктор UiDataSaver";
//    saveProgramData(); // при выходе обязательно сохраняем данные
}

void UiDataSaver::saveProgramData()
{
    QString fileDir = QDir::currentPath() + saveFile;
    saveToFile(fileDir);    // выполняем сохранение
}

void UiDataSaver::loadProgramData()
{
    QString fileDir = QDir::currentPath() + saveFile;
    loadFromFile(fileDir);
}

void UiDataSaver::saveToFile(QString fileName)
{
    if(!loaded)
    {
//        qDebug() << "Сохранение не выполнено! Программа не загружена";
        return;
    }
//    qDebug() << "SAVE вызвана функция сохранения данных в файл";
    // получаем данные о виджетах, для сохранения
    QStringList savingList; // список информации о виджетах. 1 строка = информации об 1 типе виджетов
    savingList.append(createSaveString("lineEdit", lineEditList, getLineEditState)); // добавляем данные о lineEdit
    savingList.append(createSaveString("checkBox", checkBoxesList, getCheckBoxState)); // добавляем данные о checkBox
    savingList.append(createSaveString("comboBox", comboBoxList, getComboBoxState)); // добавляем данные о checkBox
    savingList.append(createSaveString("spinBox", spinBoxList, getSpinBoxState)); // добавляем данные о checkBox
    savingList.append(createSaveString("doubleSpinBox", doubleSpinBoxList, getDoubleSpinBoxState)); // добавляем данные о checkBox
    savingList.append(createSaveString("sliderList", sliderList, getSliderState)); // добавляем данные о checkBox
    savingList.append(createSaveString("mainWindow", mainWindowList, getMainWindowState));
    savingList.append(createSaveString("radioButton", radioButtonList, getRadioButtonState));
    // для добавления информации о другом типе виджета, например, listWidget, создать функцию обработки одного виджета, и занести все необходимые для обработки виджеты в список из указателей
    // для добавления нового виджета, для которого уже реализована функция сохранения, просто использовать void add(QWidget widgetName), (функция перегружена)

    // формируем записываемый текст
    QString savingText; // текст, который будет записан в файл
    for(auto &&saveLine : savingList) // перебираем все строки информации о виджетах
    {
        savingText.append(saveLine + "\r\n"); // добавляем по строкам
    }

//    QString defaultCert;
//    QString lastCert;

//    defaultCert = pres->getDefaultCertificate();
//    lastCert = pres->getCurrentCertificate();

    savingText.append("defaultCert" + lineDataSeparator + defaultCert + "\r\n");
    savingText.append("lastCert" + lineDataSeparator + lastCert + "\r\n");
#ifdef DEBUGGING
    qDebug() << "записываемый текст" << savingText;
#endif

    // записываем данные в файл
//    QString fileDir = QDir::currentPath() + saveFile;
//    if(fileName != "")
//    {
//        fileDir = fileName;
//    }
    QFile file(fileName);
//    //log.addToLog("Начата запись параметров в файл");
#ifdef DEBUGGING
    qDebug() << "open savefile" << file.fileName();
#endif
    if(file.open(QFile::WriteOnly))
    {
        file.write(savingText.toUtf8());
        file.close();
        //log.addToLog("Параметры записаны в файл " + file.fileName());
    }
    else
    {
        //log.addToLog("Ошибка записи в файл " + file.fileName());
    }
}

void UiDataSaver::loadFromFile(QString fileName)
{
#ifdef DEBUGGING
    qDebug() << "LOAD вызвана функция загрузки данных из файла";
#endif
    // открываем файл для чтения
//    QString fileDir = QDir::currentPath() + saveFile;
//    if(fileName != "")
//    {
//        fileDir = fileName;
//    }
    QFile file(fileName);
    //log.addToLog("Открываем файл параметров");
#ifdef DEBUGGING
    qDebug() << "open loadfile" << file.fileName();
#endif
    if(file.open(QFile::ReadOnly))
    {
        // читаем данные из файла
        QString text = file.readAll();
        file.close();
        // дробим на строки
#ifdef DEBUGGING
        qDebug() << "file = " << file.fileName();
        qDebug() << "cчитанные данные из файла - " << text;
#endif
        QStringList list = text.split("\r\n", SPLITTER);
#ifdef DEBUGGING
        for(int i=0; i<list.size(); i++)
        {
            qDebug() << "list " << i << list.at(i);
        }
#endif
        for(int i=0; i<list.size(); i++)
        {
            // обрабатываем каждую строку как информацию о конкретном типе виджетов
            // тут главное чтобы последовательность обработки была правильной!!! если данные о LineEdit были записаны в 0 строку, её нужно читать нулевой. Если 1й, то первой и т.д.
            QString seletedFileLine = list.at(i); // выбранная строка из файла

            setStateFromString("lineEdit", lineEditList, setLineEditState)
            setStateFromString("checkBox", checkBoxesList, setCheckBoxState)
            setStateFromString("comboBox", comboBoxList, setComboBoxState)
            setStateFromString("spinBox", spinBoxList, setSpinBoxState)
            setStateFromString("doubleSpinBox", doubleSpinBoxList, setDoubleSpinBoxState)
            setStateFromString("sliderList", sliderList, setSliderState)
            setStateFromString("mainWindow", mainWindowList, setMainWindowState)
            setStateFromString("radioButton", radioButtonList, setRadioButtonState)

            else if(seletedFileLine.startsWith("defaultCert"))
            {
                seletedFileLine = seletedFileLine.remove("defaultCert" + lineDataSeparator);
                defaultCert = seletedFileLine;
            }
            else if(seletedFileLine.startsWith("lastCert"))
            {
                seletedFileLine = seletedFileLine.remove("lastCert" + lineDataSeparator);
                lastCert = seletedFileLine;
            }
            // по аналогии можно сделать обработку других типов виджетов


        }
#ifdef DEBUGGING
        qDebug() << "считанные значения - дефолтный сертификат - " << defaultCert << "последний " << lastCert;
#endif
        //log.addToLog("Параметры установлены");
    }
    loaded = true;
}

void UiDataSaver::loadPresets()
{
//    QDirIterator filesInDir(presetsDir, QDir::Files);
//    while (filesInDir.hasNext())
//    {
//        QFile file("filesInDir.next()");               //файл-источник, который находится по адресу ItR
//        QString fileDir = file.fileName();
//        QString fileName = QFileInfo(fileDir).fileName();
//        presets.append(fileName);
//    }

    QDirIterator ligIter(presetsDir,{"*.ini"},QDir::Files);
    while (ligIter.hasNext())
    {
        // Берем следующий файл в списке, подходящий по параметрам
        QFile logFile(ligIter.next());
        QFileInfo inf(logFile); // Эта переменная нужна для возвращения имени файла без полного пути
        QString logFileName = inf.baseName(); // Сохраняем только имя файла без расширения
        presets.append(logFileName);
    }
    presets.sort(); // сортируем пресеты
}

void UiDataSaver::applyPreset(QString presetName)
{
    QString presetFile = getPresetFileName(presetName);
    loadFromFile(presetFile);   // загружаем программу из файла
}

void UiDataSaver::savePreset(QString presetName)
{
    QDir presetsDirectory(presetsDir);
    if(!presetsDirectory.exists())  // если директория не создана
    {
        presetsDirectory.mkdir(presetsDir);   // создаём директоию для сохранения
    }
    if(!presets.contains(presetName))
    {
        presets.append(presetName);
    }
    QString presetFile = getPresetFileName(presetName);
    saveToFile(presetFile); // сохраняем текущее состояние в файл
}

void UiDataSaver::removePreset(QString presetName)
{
    QString presetFile = getPresetFileName(presetName);
    QFile(presetFile).remove(); // удаляем файл пресета
    presets.removeAll(presetName);  // удаляем из списка
}

QString UiDataSaver::getPresetFileName(QString presetName)
{
    QString presetFile = presetsDir + presetName + ".ini";  // генерируем строку с путём к файлу
    return presetFile;
}

void UiDataSaver::add(QCheckBox *checkBox) // добавление checkBox в список
{
    appendToList(checkBox, checkBoxesList);
    connect(checkBox, &QCheckBox::pressed, this, &UiDataSaver::saveProgramData);
}

void UiDataSaver::add(QLineEdit *lineEdit) // добавление lineEdit в список
{
    appendToList(lineEdit, lineEditList);
    connect(lineEdit, &QLineEdit::editingFinished, this, &UiDataSaver::saveProgramData);
}

void UiDataSaver::add(QComboBox *comboBox)
{
    appendToList(comboBox, comboBoxList);
//    connect(comboBox, SIGNAL(currentIndexChanged(int index)), this, SLOT(saveActivated(int i)));
}

void UiDataSaver::add(QSpinBox *spinBox)
{
    appendToList(spinBox, spinBoxList);
//    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(saveActivated(int)));
}

void UiDataSaver::add(QDoubleSpinBox *doubleSpinBox)
{
    appendToList(doubleSpinBox, doubleSpinBoxList);
//    connect(doubleSpinBox, SIGNAL(valueChanged(int)), this, SLOT(saveActivated(int)));
}

void UiDataSaver::add(QSlider *slider)
{
    appendToList(slider, sliderList);
//    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(saveActivated(int)));
}

void UiDataSaver::add(QMainWindow *mainWindow)
{
    appendToList(mainWindow, mainWindowList);
}

void UiDataSaver::add(QRadioButton *radioButton)
{
    appendToList(radioButton, radioButtonList);
    connect(radioButton, &QRadioButton::clicked, this, &UiDataSaver::saveProgramData);
}

void UiDataSaver::setDefaultCert(const QString &value)
{
    defaultCert = value;
}

void UiDataSaver::setLastCert(const QString &value)
{
    lastCert = value;
}

QString UiDataSaver::getDefaultCert() const
{
    return defaultCert;
}

QString UiDataSaver::getLastCert() const
{
    return lastCert;
}

void UiDataSaver::setSaveFile(const QString &value)
{
    saveFile = value;
}

void UiDataSaver::saveActivated(int i)
{
    Q_UNUSED(i);
    saveProgramData();
}

const QStringList &UiDataSaver::getPresets() const
{
    return presets;
}

void UiDataSaver::setPresetsDir(const QString &newPresetsDir)
{
    presetsDir = newPresetsDir;
}

//preset *UiDataSaver::getPres() const
//{
//    return pres;
//}

//void UiDataSaver::setPres(preset *value)
//{
//    pres = value;
//}

QString UiDataSaver::getLineEditState(QLineEdit *lineEdit)
{
    QString state = lineEdit->text();
    QString currentPath = QDir::currentPath(); // получаем текущую директорию
    if(state == currentPath)
    {
        state = "currentPath";
    }
    return state;
}

QString UiDataSaver::getCheckBoxState(QCheckBox *checkBox)
{
    return QString::number(checkBox->isChecked());
}

QString UiDataSaver::getComboBoxState(QComboBox *comboBox)
{
    return QString::number(comboBox->currentIndex());
}

QString UiDataSaver::getSpinBoxState(QSpinBox *spinBox)
{
    return QString::number(spinBox->value());
}

QString UiDataSaver::getDoubleSpinBoxState(QDoubleSpinBox *doubleSpinBox)
{
    return QString::number(doubleSpinBox->value());
}

QString UiDataSaver::getSliderState(QSlider *slider)
{
    return QString::number(slider->value());
}

QString UiDataSaver::getMainWindowState(QMainWindow *mainWindow)
{
    objectProperty objectInfo; // создаем структуру свойств
    objectInfo.addProperty("height", QString::number(mainWindow->height()));
    objectInfo.addProperty("width", QString::number(mainWindow->width()));
    return objectInfo.getPropertySaveString(); // получаем строку по свойствам
}

QString UiDataSaver::getRadioButtonState(QRadioButton *radioButton)
{
    return QString::number(radioButton->isChecked());
}

void UiDataSaver::setLineEditState(QLineEdit *lineEdit, QString state)
{
    if(state == "currentPath") // если сохраненное состояние текущая директория
    {
        state = QDir::currentPath(); // устанавливаем текущую директорию, т.к. программа может быть перемещена
    }
    lineEdit->setText(state); // устанавливаем ему сохранённое состояние
}

void UiDataSaver::setCheckBoxState(QCheckBox *checkBox, QString state)
{
    //    qDebug() << "Изменяется статус " << checkBox->objectName() << " на " << state;
    checkBox->setChecked(state.toInt()); // устанавливаем ему сохранённое состояние
    emit checkBox->stateChanged(state.toInt()); // отправляем сигнал об обнолении
}

void UiDataSaver::setComboBoxState(QComboBox *comboBox, QString state)
{
    comboBox->setCurrentIndex(state.toInt());
}

void UiDataSaver::setSpinBoxState(QSpinBox *spinBox, QString state)
{
    spinBox->setValue(state.toInt());
}

void UiDataSaver::setDoubleSpinBoxState(QDoubleSpinBox *doubleSpinBox, QString state)
{
    doubleSpinBox->setValue(state.toDouble());
}

void UiDataSaver::setSliderState(QSlider *slider, QString state)
{
    slider->setValue(state.toInt());
    slider->setSliderPosition(state.toInt());
}

void UiDataSaver::setMainWindowState(QMainWindow *mainWindow, QString state)
{
    objectProperty objectInfo; // создаем структуру хранения свойст объекта
    objectInfo.setPropertyList(state); // переводим считанное свойство в список
    int height = objectInfo.getPropertyInfo("height").toInt();
    int width = objectInfo.getPropertyInfo("width").toInt();
    if(height > 0 && width > 0)
    {
        auto geometry = mainWindow->geometry(); // получаем геометрию окна
        geometry.setHeight(height);
        geometry.setWidth(width);
        mainWindow->setGeometry(geometry); // устанавливаем ту, что была в файле
    }
    else
    {
        Q_UNUSED(mainWindow);
    }
}

void UiDataSaver::setRadioButtonState(QRadioButton *radioButton, QString state)
{
    bool click_state = state.toInt();
    radioButton->setChecked(click_state);
//    radioButton->setDown(click_state);
    emit radioButton->clicked(click_state);
}

template<typename T>
QString UiDataSaver::createSaveString(QString widgetType, QList<T> &list, QString (*stateFunc)(T))
{
//    QJsonArray widgetsJsonArray;
//    for (auto &&object : list)
//    {
//        QString objectName = object->objectName();
//    }

    QString saveString; // строка, которую добавим
    for (auto &&object : list)
    {
        QString objectName = object->objectName();
        QString state = stateFunc(object);
        saveString.append(widgetType + lineDataSeparator + objectName + keyDataSeparator + state + stringSeparator);
    }
    return saveString;
}

template<typename T>
void UiDataSaver::loadFromSaveString(QString savedString, QList<T> &list, void (*setStateFunc)(T, QString state))
{
    QStringList objectDataList = savedString.split("|", SPLITTER); // разбиваем на пары ключ = значение
    for (auto &&objectData : objectDataList)  // перебираем все пары
    {
        QStringList objectSavedDataInfo = objectData.split("=", SPLITTER); // разбиваем пару на ключ / значение
        if(objectSavedDataInfo.size() == 2) // проверяем, чтобы было ровно 2 элемента (ключ и значение)
        {
            QString objectName = objectSavedDataInfo.at(0); // название = ключ (0 индекс)
            QString state = objectSavedDataInfo.at(1); // состояние (нажато/не нажато) - значение (1 индекс)
            for (auto &&objectUi : list) // перебираем все object, которые добавлены в список для сохранения
            {
                if(objectUi->objectName() == objectName) // если имя совпадает с сохранённым
                {
                    setStateFunc(objectUi, state);
                }
            }
        }
        #ifdef DEBUGGING
        else
        {

            qDebug() << "Ошибка строки данных object - " << objectSavedDataInfo;
        }
        #endif
    }
}

void UiDataSaver::objectProperty::addProperty(QString propertyName, QString value)
{
    propertyList.insert(propertyName, value);
}

QString UiDataSaver::objectProperty::getPropertySaveString()
{
    QString state = "";
    QMapIterator<QString, QString> iterator(propertyList);
    while (iterator.hasNext())
    {
        if(state != "")
        {
            state.append(propertySeparator);
        }
        iterator.next(); // идём к следующему итератору
        state.append(iterator.key() + propertyInfoSeparator + iterator.value());
    }
    return state;
}

void UiDataSaver::objectProperty::setPropertyList(QString readedObjectState)
{
    propertyList.clear(); // очищаем
    QStringList valuesList = readedObjectState.split(propertySeparator); // разбиваем на свойства
    for (auto &&value : valuesList)
    {
        QStringList valuePair = value.split(propertyInfoSeparator);
        if(valuePair.size() >= 2)
        {
            QString key = valuePair.at(0);
            QString val = valuePair.at(1);
            propertyList.insert(key, val); // добавляем в мап
        }
        #ifdef DEBUGGING
        else
        {
            qDebug() << "Ошибка добавления свойства";
        }
        #endif
    }
}

QString UiDataSaver::objectProperty::getPropertyInfo(QString propertyName)
{
    return propertyList.value(propertyName);
}
