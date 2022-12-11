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
#include "checkstatus.h"
#include "touchevent.h"

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

    //create thread check file status.json
    CheckStatus *checkStatus = new CheckStatus();
    checkStatus->start();
    
    //create thread to send touch event
    TouchEvent *touchEvent = new TouchEvent();
    touchEvent->start();

    // create video thread show on qml
    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);
    context->setContextProperty("VideoStreamer",videoStreamer);
    context->setContextProperty("liveImageProvider",liveImageProvider);
    engine.addImageProvider("live",liveImageProvider);
 
    //create file descriptor and json object to read status json file
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"videostreamer: open status file failed";
    }
    if(flock(fd_status_json,LOCK_SH)==-1)
    {
        qDebug()<<"videostreamer: can't lock status file";
    }
    struct json_object *status_json_obj = json_object_from_fd(fd_status_json);
    bool wifi_configured = json_object_get_int(json_object_object_get(status_json_obj,"wifi_configured")) == 1? true:false;
    close(fd_status_json);
    json_object_put(status_json_obj);

    //create backend and connect classes
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
    QObject::connect(touchEvent, &TouchEvent::new_touch_event, &backEnd, &BackEnd::handle_touch_event);
    backEnd.sendToQml_ChangeWindow(13,"",0); // turn off frame 
    //loop at restart 
    restart_app:
    while(!checkStatus->is_person || (checkStatus->is_person && !checkStatus->is_door_closed))
    {
        usleep(500000);
    }
    backEnd.switch_to_main_window();
    qDebug()<<"main: start app";
    if(backEnd.is_wifi_configured)
    {
        backEnd.sendToQml_ChangeWindow(5,"",0);
    }
    else
    {
        backEnd.sendToQml_ChangeWindow(0,"",0);
    }
    
    app.exec();
    qDebug()<<"main: quit app";
    goto restart_app;
    return 0;
}
