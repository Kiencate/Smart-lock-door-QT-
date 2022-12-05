#include "videostreamer.h"
const char *status_wifi_json_path = "/home/kiencate/Documents/benzenx_job/Rfid_rc522_i2c_linux/status.json";
VideoStreamer::VideoStreamer()
{
  mode_streamer = 0;
  stream_timer = new QTimer();
  connect(stream_timer,&QTimer::timeout,this,&VideoStreamer::stream);
  stream_timer->start(50);
}

VideoStreamer::~VideoStreamer()
{
    cap.release();
}
void VideoStreamer::onMainWindow()
{
  open_video();
  qDebug()<<"videostreamer: on main window";
  mode_streamer = 1; 
}
void VideoStreamer::onFaceDetect()
{
  open_video();
  mode_streamer = 2;  
}
void VideoStreamer::onQRCodeScan()
{
  open_video();
  mode_streamer = 3;  
}
void VideoStreamer::onStopCamera()
{
  cap.release();
  mode_streamer = 0;  
}
void VideoStreamer::open_video()
{
  cap.open("/dev/video0",cv::CAP_V4L2);
  cap.set(cv::CAP_PROP_FRAME_WIDTH, 240);
  cap.set(cv::CAP_PROP_FRAME_HEIGHT, 320);
  if (!cap.isOpened()) {
    std::cout << "ERROR: Unable to open the camera" << endl;
  }
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
        mode_streamer = 1 ;     
        file.close();
        // open status wifi
        std::ifstream file_status_read;
        file_status_read.open(status_wifi_json_path);
        while (!file_status_read) 
        {
          qDebug()<<"videostreamer: open status file failed";
        }
        nlohmann::json status = nlohmann::json::parse(file_status_read);
        file_status_read.close();
        std::ofstream file_status_write;
        file_status_write.open(status_wifi_json_path);
        while (!file_status_write) 
        {
          qDebug()<<"videostreamer: open status file failed";
        }

        // change status wifi
        status["wifi_configured"] = 1;
        file_status_write << status << std::endl;
        file_status_write.close();
        emit config_wifi_success();
      }
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
    // Print type and data
    //cout << "Type : " << obj.type << endl;
    //cout << obj.data << endl;
    //string a = string(obj.data);
    // Obtain location
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



