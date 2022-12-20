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
#include "sys/ioctl.h"
#include "linux/fb.h"
#include "check_password_folder.h"

const char *status_json_path = "../status.json";
static void fbclear()
{
   char dev[256] = "/dev/fb0";
   struct fb_var_screeninfo var_info;
   int fd = open(dev, O_RDWR);
   int line_size;
   int buffer_size;
   void *buffer = NULL;
   if (fd < 0) {
       printf("failed to open %s display device\n", dev);
       return;
   }
   //get display size
   ioctl (fd, FBIOGET_VSCREENINFO, &var_info);
   line_size = var_info.xres * var_info.bits_per_pixel / 8;
   buffer_size = line_size * var_info.yres;
   //malloc buffer and set to 0
   buffer = malloc(buffer_size);
   memset(buffer, 0, buffer_size);
   //write zeros to display
   write(fd, buffer, buffer_size);
   //printf("haha dcm\n");
   free(buffer);
   close(fd);
   return;
}
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
    // touchEvent->start();

    //create thread to track password folder (password and uuid rfid)
    CheckStatusPasswordFolder *checkstatuspass = new CheckStatusPasswordFolder();
    checkstatuspass->start();

    // create video thread show on qml
    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);
    context->setContextProperty("VideoStreamer",videoStreamer);
    context->setContextProperty("liveImageProvider",liveImageProvider);
    engine.addImageProvider("live",liveImageProvider);
 
    //create file descriptor and json object to read status json file
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"main: open status file failed";
    }
    if(flock(fd_status_json,LOCK_SH)==-1)
    {
        qDebug()<<"main: can't lock status file";
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
    QObject::connect(&backEnd,&BackEnd::captureFrame,videoStreamer,&VideoStreamer::onCaptureFrame);
    QObject::connect(videoStreamer,&VideoStreamer::config_wifi_success,&backEnd,&BackEnd::onReceivedWifi);
    QObject::connect(checkStatus, &CheckStatus::JsonChangestatus, &backEnd, &BackEnd::onJsonStatusChange);
    QObject::connect(touchEvent, &TouchEvent::new_touch_event, &backEnd, &BackEnd::handle_touch_event);
    QObject::connect(checkstatuspass, &CheckStatusPasswordFolder::PasswordFolderChange, &backEnd, &BackEnd::onPasswordFolderChange);
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QGuiApplication::quit);
    //backEnd.sendToQml_ChangeWindow(13,"",0); // turn off frame 
    //loop at restart 
    restart_app: 
    while(!checkStatus->is_person || (checkStatus->is_person && !checkStatus->is_door_closed))
    {
        usleep(10000);
    }
    //engine.load(url);
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
    fbclear();
    qDebug()<<"main: quit app";
    goto restart_app;
    return 0;
}
