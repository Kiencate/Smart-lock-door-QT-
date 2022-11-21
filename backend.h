#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <zbar.h>
#include "serversocket.h"
#include "agent.h"
#include "dbusadaptor.h"

using namespace cv;
using namespace std;
using namespace zbar;

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);

signals:
    void sendToQml_button(int type, int button_id);
    /*type:
     1 -> press
     -1 -> release
     */
    /*id:
     0 -> qrcode
     1 -> bluetooth
     2 -> button ok
     */
    void sendChangeWindow(int type);
    /*type:
     * 0 -> main window
     * 1 -> connecting bluetooth
     * 2 -> connected bluetooh
     * 3 -> received msg config wifi
     */


public slots:
    void handle_touch_event(int type, int x, int y);
    /*type:
     1 -> press
     -1 -> release
     */
    void onConectedBluetooth();
    void onReceivedWifi();
private:
    int window_type;
    int pressing_button_id;
    QString _password;
    ServerSocket *serversocket;
    Agent *agent;
    MyQDusAdaptor* myAdaptor;
};

// variable and function for qr code
typedef struct
{
  string type;
  string data;
  vector <Point> location;
} decodedObject;

string decode(Mat &im, vector<decodedObject>&decodedObjects);
void display(Mat &im, vector<decodedObject>&decodedObjects);


#endif // BACKEND_H
