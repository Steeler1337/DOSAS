QT += quick core gui widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        camerahelper.cpp \
        cryptocontroller.cpp \
        cv_controller.cpp \
        main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += multimedia

win32 {
LIBS += C:\Qt\Tools\OpenSSL\Win_x64\lib\libssl.lib \
     C:\Qt\Tools\OpenSSL\Win_x64\lib\libcrypto.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_core460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_core460d.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_videoio460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_videoio460d.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_highgui460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_highgui460d.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_imgproc460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_imgproc460d.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_objdetect460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_objdetect460d.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_face460.lib \
     C:\opencv\opencv_bin\install\x64\vc16\lib\opencv_face460d.lib \
    }

INCLUDEPATH += C:\Qt\Tools\OpenSSL\Win_x64\include\ # чтоб можно было инклудить в плюсах (в начале статьи на вики-опенссл)
INCLUDEPATH += C:\opencv\opencv_bin\install\include\

HEADERS += \
    camerahelper.h \
    cryptocontroller.h \
    cv_controller.h

QMAKE_EXTRA_TARGETS += before_build makefilehook


makefilehook.target = $(MAKEFILE)

makefilehook.depends = .beforebuild



PRE_TARGETDEPS += .beforebuild


before_build.target = .beforebuild

before_build.depends = FORCE

before_build.commands = chcp 1251
