#include <QApplication>
#include <QQmlApplicationEngine>
#include <QCryptographicHash>
#include "cryptocontroller.h"
#include <QObject>
#include <QQmlContext>
#include <QMessageBox>
#include <Windows.h>
#include <Qt>
#include <QProcess>
#include <iostream>
#include <QFile>
#include "camerahelper.h"
#include "cv_controller.h"


typedef unsigned long long QWORD;

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    /// БЛОК ЗАЩИТЫ ОТ ОТЛАДКИ МЕТОДОМ САМООТЛАДКИ
//    QProcess *satelite_process = new QProcess();
//    int pid = QApplication::applicationPid();
//    qDebug() << "pid = " << pid;
//    QStringList arguments = {QString::number(pid)};
//    qDebug() << "arguments = " << arguments;

//    satelite_process->start("DebugProtector.exe", arguments);
//    qDebug() << satelite_process->errorString();
//    bool protector_started = satelite_process->waitForStarted(1000);
//    qDebug() << "protector_started = " << protector_started;


//    /// БЛОК САМОПРОВЕРКИ ХЭШ-СУММЫ
//    //1) определить, где в памяти начало сегмента .text
//    QWORD moduleBase = (QWORD)GetModuleHandle(NULL); //нач. адрес, с которого приложение размещается в виртуальной памяти
//    QWORD text_segment_start = moduleBase + 0x1000; //адрес сегмента .text
//    //2) определить, какой он длины
//    PIMAGE_DOS_HEADER pIDH = reinterpret_cast<PIMAGE_DOS_HEADER>(moduleBase);
//    PIMAGE_NT_HEADERS pIHN = reinterpret_cast<PIMAGE_NT_HEADERS>(moduleBase + pIDH->e_lfanew);
//    QWORD size_of_text = pIHN->OptionalHeader.SizeOfCode; //размер сегмента .text
//    //3) от бинарного блока в диапазоне start...(start+size) посчитать хэш
//    QByteArray text_segment_contents = QByteArray((char *)text_segment_start, size_of_text);
//    QByteArray current_hash = QCryptographicHash::hash(text_segment_contents, QCryptographicHash::Sha256);
//    QByteArray current_hash_base64 = current_hash.toBase64();
//    //4) сравнить полученный хэш с эталонным
//    const QByteArray hash0_base64 = QByteArray("CyZaTzu6gjJMlLAE92BcIQE57IyBzyuWGhMqpnUAdaE=");

//    qDebug() << "moduleBase: " << moduleBase;
//    qDebug() << "text_segment_start: " << text_segment_start;
//    qDebug() << "size_of_text" << size_of_text;
////    qDebug() << "hash_base64" << Qt::hex << text_segment_contents.first(100);
//    qDebug() << current_hash_base64;

//    //5) реакция на расхождение хэшей
//    bool check_result = (current_hash_base64 == hash0_base64);
//    qDebug() << "check_result: " << check_result;
//    if(!check_result) {
//        int result = QMessageBox::critical(nullptr, "Внимание!", "Приложение модифицировано");
//        return -1;
//    }


    QQmlApplicationEngine engine;                                               // создание объекта движка/интерпретатора QML

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    cryptoController crp;
    QQmlContext *context = engine.rootContext();
    context->setContextProperty("cryptoController", &crp);

    cv_controller ctr;
    CameraHelper camera_helper;
    QObject::connect(&camera_helper, SIGNAL(imageChanged(const QImage &)), &ctr, SLOT(face_recognition(const QImage &)));
    context->setContextProperty("cvController", &ctr);
    engine.rootContext()->setContextProperty("cameraHelper", &camera_helper);


    /// БЛОК ПРОСТЕЙШЕГО ОБНАРУЖЕНИЯ ОТЛАДЧИКА
//    bool debugger_found = IsDebuggerPresent();
//    if(debugger_found) {
//        qDebug() << "Debugger found, terminating PassManager!!!";
//        std::cout << "Debugger found, terminating PassManager!!!";
//        return -1;
//    }


    engine.load(url);

//    QObject *rootItem = engine.rootObjects()[0];

//    QObject::connect(rootItem, SIGNAL(opencv_btn_clicked()), &ctr, SLOT(myfunc()));
    context->setContextProperty("cvController", &ctr);



    return app.exec();
}
