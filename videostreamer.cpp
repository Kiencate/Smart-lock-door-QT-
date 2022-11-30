#include "videostreamer.h"

VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
    threadStreamer->requestInterruption();
}

void VideoStreamer::streamVideo()
{

    if(frame.data)
    {
        // cv::imshow("ok",frame);
        QImage img = QImage((uchar*)frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
        emit newImage(img);
    }
}

void VideoStreamer::catchFrame(cv::Mat emittedFrame)
{
    frame = emittedFrame;
}

void VideoStreamer::openVideoCamera(QString path)
{
    // if(path.length() == 1)
    // cap.open(path.toInt());
    // else
    // cap.open(path.toStdString());
    // cap;

    // VideoCapture cap(rgb_cam_index);
    cap.open("/dev/video0");
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 240);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 320);
    if (!cap.isOpened()) {
        std::cout << "ERROR: Unable to open the camera" << endl;
        return;
    }
    // qDebug()<<cap;
    VideoStreamer* worker = new VideoStreamer();
    worker->moveToThread(threadStreamer);
    QObject::connect(threadStreamer,SIGNAL(started()),worker,SLOT(streamerThreadSlot()));
    QObject::connect(worker,&VideoStreamer::emitThreadImage,this,&VideoStreamer::catchFrame);
    threadStreamer->start();

    double fps = cap.get(cv::CAP_PROP_FPS);
    tUpdate.start(1000/fps);
}

void VideoStreamer::streamerThreadSlot()
{
    cv::Mat tempFrame;

    while (1) {
        cap>>tempFrame;
        if(tempFrame.data)
            emit emitThreadImage(tempFrame);

        if(QThread::currentThread()->isInterruptionRequested())
        {
            cap.release();
            return;
        }

    }
}
