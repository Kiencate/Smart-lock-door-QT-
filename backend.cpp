#include "backend.h"

using namespace cv;
using namespace std;
using namespace zbar;

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    pressing_button_id = -1;
}


int rgb_cam_index = 0;
// decodedObject obj;
// Find and decode barcodes and QR codes
string decode(Mat &im, vector<decodedObject>&decodedObjects)
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
void display(Mat &im, vector<decodedObject>&decodedObjects)
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
  imshow("Results", im);
  waitKey(1);

}

void BackEnd::handle_touch_event(int type, int x, int y)
{/*
    qDebug()<<"x"<<x<<"y"<<y;*/
    if (type == 1)
    {
        if (x > 20 && x <220)
        {
            if (y > 55 && y<140)
            {
                pressing_button_id = 0;
                emit sendToQml_button(1, 0);

            }

            if (y > 162 && y<256)
            {
                pressing_button_id = 1;
                emit sendToQml_button(1, 1);
            }
        }

    }
    else if (type == -1)
    {
        if (pressing_button_id!=-1) emit sendToQml_button(-1, pressing_button_id);
        if (pressing_button_id == 0) //qrcode
        {
            qDebug()<<"start qrcode process";
            Mat inputImage;
              //VideoCapture cap("test_qr.mp4");   //Movie
            VideoCapture cap(rgb_cam_index);
            cap.set(CAP_PROP_FRAME_WIDTH, 640);
            cap.set(CAP_PROP_FRAME_HEIGHT, 480);
            if (!cap.isOpened()) {
              cerr << "ERROR: Unable to open the camera" << endl;
              return;
            }
            cout << "Start grabbing, press ESC on TLive window to terminate" << endl;

            while(1){
            cap >> inputImage;

            // Variable for decoded objects
            vector<decodedObject> decodedObjects;

            // Find and decode barcodes and QR codes
            string config_wifi = decode(inputImage, decodedObjects);
            cout << config_wifi << endl;
            QFile file("../wpa_supplicant");
            if (file.open(QIODevice::ReadWrite)) {
                QTextStream stream(&file);
                stream << QString::fromStdString(config_wifi) << endl;
            }
            if (config_wifi!="") exit(0);
            // Display location
            display(inputImage, decodedObjects);
            }
            //code for qrcode process

        }
        else if (pressing_button_id ==1) //bluetooth
        {
            qDebug()<<"start bluetooth process";   
            //code for bluetooth process
            //exit(0);
        }
        pressing_button_id = -1;

        //code to start process qrcode or bluetooth
    }

}
