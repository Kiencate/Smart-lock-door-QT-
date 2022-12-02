#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "backend.h"
#include "opencvimageprovider.h"
#include "videostreamer.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext *context = engine.rootContext();

    qRegisterMetaType<cv::Mat>("cv::Mat");

    //create thread stream video
    QThread* threadStreamer = new QThread();
    VideoStreamer *videoStreamer = new VideoStreamer();
    videoStreamer->moveToThread(threadStreamer);

    //
    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);
    context->setContextProperty("VideoStreamer",videoStreamer);
    context->setContextProperty("liveImageProvider",liveImageProvider);
    engine.addImageProvider("live",liveImageProvider);
 
    //check if wifi congigured
    QFile file("../wifi.txt");
    while(!file.open(QIODevice::ReadOnly)) 
    {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    QString wifi_status = in.readLine();
    if (wifi_status == "1")
    {
        BackEnd backEnd(true);
        context->setContextProperty("backEnd", &backEnd);

        
        const QUrl url(QStringLiteral("qrc:/enter_password.qml"));
        
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
                                if (!obj && url == objUrl) QCoreApplication::exit(-1);
                            }, Qt::QueuedConnection);
                engine.load(url);
            //
        QObject::connect(threadStreamer,&QThread::started,videoStreamer,&VideoStreamer::onMainWindow);
        threadStreamer->start();
        QObject::connect(videoStreamer,&VideoStreamer::newImage,liveImageProvider,&OpencvImageProvider::updateImage);
        QObject::connect(&backEnd,&BackEnd::switch_to_qrcode_scan,videoStreamer,&VideoStreamer::onQRCodeScan);
        QObject::connect(&backEnd,&BackEnd::switch_to_main_window,videoStreamer,&VideoStreamer::onMainWindow);
        QObject::connect(&backEnd,&BackEnd::switch_to_face_detect,videoStreamer,&VideoStreamer::onFaceDetect);
        QObject::connect(&backEnd,&BackEnd::stopCamera,videoStreamer,&VideoStreamer::onStopCamera);
        QObject::connect(videoStreamer,&VideoStreamer::config_wifi_success,&backEnd,&BackEnd::onReceivedWifi);
        QObject::connect(videoStreamer,&VideoStreamer::open_with_face_success,&backEnd,&BackEnd::openWithFaceSuccess);
        return app.exec();
    

        
    }
    else
    {
        BackEnd backEnd(false);
        context->setContextProperty("backEnd", &backEnd);

        const QUrl url(QStringLiteral("qrc:/config_wifi.qml"));
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
                                if (!obj && url == objUrl) QCoreApplication::exit(-1);
                            }, Qt::QueuedConnection);
                engine.load(url);
            //
        QObject::connect(threadStreamer,&QThread::started,videoStreamer,&VideoStreamer::onStopCamera);
        threadStreamer->start();
        QObject::connect(videoStreamer,&VideoStreamer::newImage,liveImageProvider,&OpencvImageProvider::updateImage);
        QObject::connect(&backEnd,&BackEnd::switch_to_qrcode_scan,videoStreamer,&VideoStreamer::onQRCodeScan);
        QObject::connect(&backEnd,&BackEnd::switch_to_main_window,videoStreamer,&VideoStreamer::onMainWindow);
        QObject::connect(&backEnd,&BackEnd::switch_to_face_detect,videoStreamer,&VideoStreamer::onFaceDetect);
        QObject::connect(&backEnd,&BackEnd::stopCamera,videoStreamer,&VideoStreamer::onStopCamera);
        QObject::connect(videoStreamer,&VideoStreamer::config_wifi_success,&backEnd,&BackEnd::onReceivedWifi);
        QObject::connect(videoStreamer,&VideoStreamer::open_with_face_success,&backEnd,&BackEnd::openWithFaceSuccess);
        return app.exec();

    }

   
}
