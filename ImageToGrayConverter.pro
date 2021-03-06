QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/ImageHandlers/imagecorrector.cpp \
    src/ImageHandlers/imagecorrectorenums.cpp \
    src/ImageHandlers/imagedata.cpp \
    src/ImageHandlers/imagedataexception.cpp \
    src/ImageHandlers/imagetransformer.cpp \
    src/MainWindow/mainwindow.cpp \
    src/UiHandlers/uidatasaver.cpp \
    src/ImageFilters/imagecorrectrfilterparams.cpp \
    src/main.cpp


HEADERS += \
    src/ImageHandlers/imagecorrector.h \
    src/ImageHandlers/imagecorrectorenums.h \
    src/ImageHandlers/imagedata.h \
    src/ImageHandlers/imagedataexception.h \
    src/ImageHandlers/imagetransformer.h \
    src/MainWindow/mainwindow.h \
    src/UiHandlers/uidatasaver.h \
    src/ImageFilters/imagecorrectrfilterparams.h


FORMS += \
    src/MainWindow/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
