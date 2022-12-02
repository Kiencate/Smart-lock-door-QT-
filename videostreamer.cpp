#include "videostreamer.h"

// code for comunication with face detect process
int getpidface()
{
  char line_ui[30];
  FILE *cmd_ui = popen("pidof Face", "r");
  fgets(line_ui, 30, cmd_ui);
  pid_t pid_face = strtoul(line_ui, NULL, 10);
  pclose(cmd_ui);
  return pid_face;
}
volatile int is_right_face;

void usrui(int sig)
{
  is_right_face = 1;
}
VideoStreamer::VideoStreamer()
{
  if (signal(SIGUSR2, usrui)==SIG_ERR){
    perror("\nSIGUSR2");
    exit(4);
  }
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
  mode_streamer = 1; 
}
void VideoStreamer::onFaceDetect()
{
  open_video();
  mode_streamer = 2;  
  kill(getpidface(), SIGUSR1);
}
void VideoStreamer::onQRCodeScan()
{
  open_video();
  mode_streamer = 3;  
}
void VideoStreamer::onStopCamera()
{
  mode_streamer = 0;  
}
void VideoStreamer::open_video()
{
  cap.open("/dev/video0");
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
      cap.release();
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
      if (is_right_face == 1)
      {
        emit open_with_face_success();
      }
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
        emit config_wifi_success();
        file.close();
        QFile file_wifi("../wifi.txt");
        while(!file_wifi.open(QIODevice::WriteOnly))
        {
            qDebug()<<file.errorString();
        }
        QTextStream write(&file_wifi);
        write<<"1"<<endl;
        file_wifi.close();
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



