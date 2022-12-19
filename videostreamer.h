#ifndef VIDEOSTREAMER_H
#define VIDEOSTREAMER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QThread>
#include <QTimer>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <unistd.h>
#include <iostream>
#include <zbar.h>
#include <sys/file.h>
#include <json.h>

using namespace cv;
using namespace std;
using namespace zbar;

extern const char *status_json_path;

typedef struct
{
  string type;
  string data;
  vector <Point> location;
} decodedObject;

class VideoStreamer: public QThread
{
    Q_OBJECT
public:
    VideoStreamer();
    ~VideoStreamer();

public:
    string decode(Mat &im, vector<decodedObject>&decodedObjects);
    void display(Mat &im, vector<decodedObject>&decodedObjects);

public slots:
    void onQRCodeScan(); // switch to scan qrcode
    void onMainWindow(); // show camera normal
    void onFaceDetect(); // show camera normal
    void onStopCamera(); // close camera
    void onCaptureFrame(); //capture frame and save image

private:
    cv::Mat Frame;
    VideoCapture cap;
    int mode_streamer;
    /*
    * 0 -> stop_mode
    * 1 -> main_window mode
    * 2 -> face_detect mode
    * 3 -> qrcode_scan mode
    */
    QTimer *stream_timer;
    void stream();
    /*
    * create video with fps
    */
    void open_video();
    /*
    * open device camera
    */
signals:
    void newImage(const QImage &);
    /*
    * emit to imageprovider new image captured
    */
    void config_wifi_success();
    /*
    * emit to backend when config wifi with qr success
    */
};

#endif //VIDEOSTREAMER_H