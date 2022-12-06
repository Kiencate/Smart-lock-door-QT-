#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include "string.h"
#include "backend.h"
#include "opencvimageprovider.h"
#include "videostreamer.h"
#include "checkstatus_ir.h"
const char *status_json_path = "../status.json";
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
    CheckStatus *checkStatus = new CheckStatus();
    checkStatus->start();

    // create video thread show on qml
    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);
    context->setContextProperty("VideoStreamer",videoStreamer);
    context->setContextProperty("liveImageProvider",liveImageProvider);
    engine.addImageProvider("live",liveImageProvider);
 
    //create ifstream to read status json file
    std::ifstream file_status;
    file_status.open(status_json_path);
    while(!file_status)
    {
        qDebug("error while opening status json path");
    }
    bool wifi_configured;
    try
    {
        nlohmann::json status = nlohmann::json::parse(file_status);
        file_status.close();
        wifi_configured = std::stoi(status["wifi_configured"].dump()) == 1?true:false;
    }
    catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl; return 0;}

    BackEnd backEnd(wifi_configured);
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
    QObject::connect(checkStatus, &CheckStatus::JsonChangestatus, &backEnd, &BackEnd::onJsonStatusChange);
    backEnd.sendToQml_ChangeWindow(13,"",0); // turn off frame
    loop:
    while(!checkStatus->is_person || (checkStatus->is_person && !checkStatus->is_door_closed))
    {
        // qDebug()<<"check =0";
        usleep(500000);
    }
    backEnd.switch_to_main_window();
    qDebug()<<"start app";
    if(backEnd.is_wifi_configured)
    {
        backEnd.sendToQml_ChangeWindow(5,"",0);
    }
    else
    {
        backEnd.sendToQml_ChangeWindow(0,"",0);
    }
    
    app.exec();
    qDebug()<<"quit";
    goto loop;

    // }  
}
