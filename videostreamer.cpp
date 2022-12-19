#include "videostreamer.h"


VideoStreamer::VideoStreamer()
{
  mode_streamer = 0;
  stream_timer = new QTimer(this);
  connect(stream_timer,&QTimer::timeout,this,&VideoStreamer::stream);
  stream_timer->start(50);
}

VideoStreamer::~VideoStreamer()
{
  cap.release();
  qDebug()<<"videostreamer: closevideo";
}
void VideoStreamer::onMainWindow()
{
  open_video();
  qDebug()<<"videostreamer: openvideo";
  mode_streamer = 1; 
}
void VideoStreamer::onFaceDetect()
{
  mode_streamer = 2;  
}
void VideoStreamer::onQRCodeScan()
{
  open_video();
  mode_streamer = 3;  
}
void VideoStreamer::onStopCamera()
{
  qDebug()<<"videostreamer: closevideo";
  cap.release();
  mode_streamer = 0;  
}
void VideoStreamer::open_video()
{
  //cap.open("/dev/video0",cv::CAP_V4L2);
  cap.open("/dev/video11");
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 240);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 320);
  if (!cap.isOpened()) {
    qDebug()<< "ERROR: Unable to open the camera";
  }
}
void VideoStreamer::onCaptureFrame()
{
  qDebug()<<"videostreamer: capture";
  mode_streamer = 4; 
}
void VideoStreamer::stream()
{
  switch (mode_streamer)
  {
    case 0:
    { 
      break;
    }
    case 1:
    {
      cap>>Frame;
      QImage img = QImage((uchar*)Frame.data,Frame.cols,Frame.rows,QImage::Format_RGB888).rgbSwapped();
      emit newImage(img);
      break;
    }
    case 2:
    {
      cap>>Frame;
      QImage img = QImage((uchar*)Frame.data,Frame.cols,Frame.rows,QImage::Format_RGB888).rgbSwapped();
      emit newImage(img);
      break;
    }
    case 3:
    {
      cap>>Frame;
      string config_wifi ="";
      QFile file("../wpa_supplicant");
      vector<decodedObject> decodedObjects;
      config_wifi = decode(Frame, decodedObjects);
      display(Frame, decodedObjects);
      if(config_wifi != "" && file.open(QIODevice::WriteOnly))
      {
        QTextStream stream(&file);
        stream << QString::fromStdString(config_wifi) << endl;    
        file.close();
        // open status wifi, lock file
        int fd_status_json;
        if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
          qDebug()<<"video_streamer: open status file failed";
        }

        if(flock(fd_status_json,LOCK_EX)==-1)
        {
          qDebug()<<"video_streamer: can't lock status file";
        }
        struct json_object *status_json_obj= json_object_from_fd(fd_status_json);

        json_object *wifi_configured = json_object_object_get(status_json_obj,"wifi_configured");
        json_object_set_int(wifi_configured, 1);
        lseek(fd_status_json,0,SEEK_SET);
        if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
        {
          qDebug()<<"video_streamer: fail config wifi";
        } 
        json_object_put(status_json_obj);   
        close(fd_status_json); 
        emit config_wifi_success();       
        mode_streamer = 1 ;        
      }
      break;
    }
    case 4:
    {
      cap>>Frame;
      bool check = imwrite("../person.jpg", Frame);  
      // if the image is not saved
      if (check == false) {
        qDebug()<<"video_streamer: error save image";
      }
      else
      {
        mode_streamer = 1 ;
      }
      QImage img = QImage((uchar*)Frame.data,Frame.cols,Frame.rows,QImage::Format_RGB888).rgbSwapped();
      emit newImage(img);
      break;
    }
    default:
    {
      break;
    }
  }

}
string VideoStreamer::decode(Mat &im, vector<decodedObject>&decodedObjects)
{
  decodedObject obj;
  string a;
  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
    // enable qr
  scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1);
  // Convert image to grayscale
  Mat imGray;
  cvtColor(im, imGray, COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);
  // Print results
  for(Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
  {
    decodedObject obj;
    obj.type = symbol->get_type_name();
    obj.data = symbol->get_data();
    a = obj.data;
    for(int i = 0; i< symbol->get_location_size(); i++)
    {
      obj.location.push_back(Point(symbol->get_location_x(i),symbol->get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
  return a;
}

// Display barcode and QR code location
void VideoStreamer::display(Mat &im, vector<decodedObject>&decodedObjects)
{
  // Loop over all decoded objects
  for(int i = 0; i < decodedObjects.size(); i++)
  {
    vector<Point> points = decodedObjects[i].location;
    vector<Point> hull;

    // If the points do not form a quad, find convex hull
    if(points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for(int j = 0; j < n; j++)
    {
      line(im, hull[j], hull[ (j+1) % n], Scalar(255,0,0), 3);
    }

  }

  // Display results
//   waitKey(1);
  QImage img = QImage((uchar*)im.data,im.cols,im.rows,QImage::Format_RGB888).rgbSwapped();
  emit newImage(img);
}



