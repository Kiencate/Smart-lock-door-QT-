#include "backend.h"
#include <unistd.h>
#include <iostream>
#include <chrono>
#include <string>
#include <opencv2/opencv.hpp>
#include <stdint.h> // for uint32_t
#include <sys/ioctl.h> // for ioctl
#include <linux/fb.h> // for fb_
#include <fcntl.h> // for O_RDWR
#include <fstream> // for std::ofstream
#include <thread>
#include <string>
// #include <wiringPi.h>
#include <time.h>
#include <pthread.h>

const int showWidth          = 240;
const int showHeight         = 320;

const int iconWidth          = 60;
const int iconHeight         = 60;
const int x_offset           = 0;
const int y_offset           = 0;
char* person_name            = NULL;
bool is_unlocked             = false;

volatile int detectCount = 0;

const char*  rgb_cam_index            = "/dev/video9";
using namespace std;
using namespace cv;
using namespace std::chrono;
cv::VideoCapture cap(rgb_cam_index);

static cv::Mat locked_icon_4_chan          = cv::imread("/home/linaro/Rfid_rc522_i2c_linux/symbol/locked.png", cv::IMREAD_UNCHANGED);
static cv::Mat unlocked_icon_4_chan        = cv::imread("/home/linaro/Rfid_rc522_i2c_linux/symbol/unlocked.png", cv::IMREAD_UNCHANGED);
static cv::Mat black_screen                = cv::imread("/home/linaro/Rfid_rc522_i2c_linux/symbol/black.png");
static cv::Mat locked_icon;
static cv::Mat unlocked_icon;
static cv::Mat alpha_locked_icon;
static cv::Mat alpha_unlocked_icon;


const char* output_screen = "/dev/fb0";
float ScaleX, ScaleY;
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//  Computing the cosine distance between input feature and ground truth feature
//----------------------------------------------------------------------------------------

//////////////////////////////////
bool checkPerson=true;
//////////////////////////////////
// void setup(){
//   wiringPiSetup() ;
//   pinMode(2, OUTPUT);
//   pinMode(5, INPUT);
// }
struct framebuffer_info { 
    uint32_t bits_per_pixel; uint32_t xres_virtual; 
};
struct framebuffer_info get_framebuffer_info(const char* framebuffer_device_path) {
    struct framebuffer_info info;
    struct fb_var_screeninfo screen_info;
    int fd = -1;
    fd = open(framebuffer_device_path, O_RDWR);
    if (fd >= 0) {
        if (!ioctl(fd, FBIOGET_VSCREENINFO, &screen_info)) {
            info.xres_virtual = screen_info.xres_virtual;
            info.bits_per_pixel = screen_info.bits_per_pixel;
        }
    }
    return info;
};

void init_icon(){
    cv::resize(locked_icon_4_chan, locked_icon_4_chan, Size(iconWidth,iconHeight),INTER_LINEAR);
    cv::resize(unlocked_icon_4_chan, unlocked_icon_4_chan, Size(iconWidth,iconHeight),INTER_LINEAR);
    cv::resize(black_screen, black_screen, Size(showWidth,showHeight),INTER_LINEAR);

    vector<cv::Mat> layers_unlocked, layers_locked;
    cv::split(unlocked_icon_4_chan, layers_unlocked);
    cv::split(locked_icon_4_chan, layers_locked);

    cv::Mat alpha_locked[] = {layers_locked[3], layers_locked[3], layers_locked[3]};
    cv::Mat alpha_unlocked[] = {layers_unlocked[3], layers_unlocked[3], layers_unlocked[3]};
    cv::Mat rgb_locked[] = {layers_locked[0], layers_locked[1], layers_locked[2]};
    cv::Mat rgb_unlocked[] = {layers_unlocked[0], layers_unlocked[1], layers_unlocked[2]};
    
    cv::merge(alpha_locked, 3, alpha_locked_icon);
    cv::merge(alpha_unlocked, 3, alpha_unlocked_icon);
    cv::merge(rgb_unlocked, 3, unlocked_icon);
    cv::merge(rgb_locked, 3, locked_icon);

    cv::threshold(alpha_locked_icon, alpha_locked_icon, 250, 255, 0);
    cv::threshold(alpha_unlocked_icon, alpha_unlocked_icon, 250, 255, 0);

    alpha_locked_icon.convertTo(alpha_locked_icon, CV_32FC3, 1.0/255);
    alpha_unlocked_icon.convertTo(alpha_unlocked_icon, CV_32FC3, 1.0/255);

    unlocked_icon.convertTo(unlocked_icon, alpha_unlocked_icon.type());
    cv::multiply(alpha_unlocked_icon, unlocked_icon, unlocked_icon);

    locked_icon.convertTo(locked_icon, alpha_locked_icon.type());
    cv::multiply(alpha_locked_icon, locked_icon, locked_icon);
}

void showing_frame(){
    // init_icon();
    time_t timeSensor, timeSensorx;
    timeSensorx = time(NULL);
    // setup();
    init_icon();


    cv::Mat locked_icon_temp = locked_icon.clone();

    // writing frame to fb
   // screen device to write out
    std::ofstream ofs(output_screen);
    framebuffer_info fb_info = get_framebuffer_info(output_screen);

    // cv::VideoCapture cap(rgb_cam_index);
    // cv::VideoCapture cap("IMG_2206.MOV");

    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return;
    }

    cv::Mat frame;

    while (1){
        // cout << "show: " << is_unlocked << "  , check person: " << checkPerson << endl;
        // if(digitalRead(5)==1){
        //     timeSensorx=time(NULL);
        //     checkPerson = true;
        // }
        // if((time(NULL) - timeSensorx)>0){
        //     checkPerson = false;
        // }
        
        // if(is_unlocked){
        //     timeSensor = time(NULL);
        //     digitalWrite(2, HIGH);
        // }
        // else{
        //     digitalWrite(2, LOW);
        // }
        if (checkPerson){
            cap >> frame;
            if (frame.empty()) {
                cerr << "End of movie" << endl;
                break;
            }

            cv::resize(frame, frame, Size(showWidth,showHeight),INTER_LINEAR);
            cv::Mat insertImage(frame, Rect(x_offset, y_offset, iconWidth, iconHeight));
            insertImage.convertTo(insertImage, CV_32FC3);
            if (is_unlocked){
                cv::multiply(cv::Scalar::all(1.0)-alpha_unlocked_icon, insertImage, insertImage);
                add(unlocked_icon, insertImage, insertImage);

                if (person_name != NULL){
                    cv::putText(frame, person_name, cv::Point(x_offset + iconWidth + 10, y_offset + 20),cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 0, 0));
                }
            }
            else{
                cv::multiply(cv::Scalar::all(1.0)-alpha_locked_icon, insertImage, insertImage);
                add(locked_icon, insertImage, insertImage);
            }
            insertImage.copyTo(frame(Rect(x_offset, y_offset, iconWidth, iconHeight)));
        }
        // show black frame if cam is not opened
        else{
            frame = black_screen;
        }

        // show output
        int framebuffer_width = fb_info.xres_virtual;
        int framebuffer_depth = fb_info.bits_per_pixel;
        cv::Size2f frame_size = frame.size();
        cv::Mat framebuffer_compat;

        switch (framebuffer_depth) {
            case 16:
                cv::cvtColor(frame, framebuffer_compat, cv::COLOR_BGR2BGR565);
                for (int y = 0; y < frame_size.height ; y++) {
                    ofs.seekp(y*framebuffer_width*2);
                    ofs.write(reinterpret_cast<char*>(framebuffer_compat.ptr(y)),frame_size.width*2);
                }
            break;
            case 32: {
                    std::vector<cv::Mat> split_bgr;
                    cv::split(frame, split_bgr);
                    split_bgr.push_back(cv::Mat(frame_size, CV_8UC1,cv::Scalar(255)));
                    cv::merge(split_bgr, framebuffer_compat);
                    for (int y = 0; y < frame_size.height ; y++) {
                        ofs.seekp(y*framebuffer_width*4);
                        ofs.write(reinterpret_cast<char*>(framebuffer_compat.ptr(y)),frame_size.width*4);
                    }
                } break;
            default:
                std::cerr << "Unsupported depth of framebuffer." << std::endl;
        }
    }
}
BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    is_right_password = false;
    window_type = 0;
    wrong_left = 5;
    pressing_button_id = -1;
    // get right password from txt file
    QFile file("/home/linaro/Rfid_rc522_i2c_linux/password.txt");
    while(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    _right_password1 = in.readLine();
    _right_password2 = in.readLine();
    file.close();
//    qDebug()<<_right_password1<<_right_password2;
}

void BackEnd::handle_touch_event(int type, int x, int y)
{
//    qDebug()<<"x"<<x<<"y"<<y;
    if (window_type == 0) // password window
    {
        if (type == 1)
        {
            if (x > 17 && x <74)
            {
                if (y > 100 && y<130)
                {
                    pressing_button_id = 1;
                    emit sendToQml_button(1, 1);
                }
                if (y > 150 && y<180)
                {
                    pressing_button_id = 4;
                    emit sendToQml_button(1, 4);
                }
                if (y > 200 && y<230)
                {
                    pressing_button_id = 7;
                    emit sendToQml_button(1, 7);
                }
                else if (y > 250 && y < 280)
                {
                    pressing_button_id = 10;
                    emit sendToQml_button(1, 10);
                }
            }
            else if(x > 92 && x < 149)
            {
                if (y > 100 && y<130)
                {
                    pressing_button_id = 2;
                    emit sendToQml_button(1, 2);
                }
                if (y > 150 && y<180)
                {
                    pressing_button_id = 5;
                    emit sendToQml_button(1, 5);
                }
                if (y > 200 && y<230)
                {
                    pressing_button_id = 8;
                    emit sendToQml_button(1, 8);
                }
                else if (y > 250 && y < 280)
                {
                    pressing_button_id = 0;
                    emit sendToQml_button(1, 0);
                }
            }
            else if(x > 167 && x < 224)
            {
                if (y > 100 && y<130)
                {
                    pressing_button_id = 3;
                    emit sendToQml_button(1, 3);
                }
                if (y > 150 && y<180)
                {
                    pressing_button_id = 6;
                    emit sendToQml_button(1, 6);
                }
                if (y > 200 && y<230)
                {
                    pressing_button_id = 9;
                    emit sendToQml_button(1, 9);
                }
                else if (y > 250 && y < 280)
                {
                    pressing_button_id = 11;
                    emit sendToQml_button(1, 11);
                }
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id != -1) emit sendToQml_button(-1, pressing_button_id);
            if (pressing_button_id == 10 && _password.size() > 0)
            {
               _password = _password.left(_password.size() - 1);
               qDebug()<<_password;
               emit sendToQml_password(_password.size());
            }
            else if (pressing_button_id == 11)
            {
                if (_password.size() < 6)
                {
                    emit sendChangeWindow(1,wrong_left);
                    window_type=1;
                }
                else if (_password != _right_password1 && _password != _right_password2)
                {
                    if (--wrong_left == 0)
                    {
                        _password="";
                        emit sendChangeWindow(3,wrong_left);
                        window_type=2;
                    }
                    else
                    {
                        _password="";
                        emit sendChangeWindow(2,wrong_left);
                        window_type=1;
                    }


                }
                else if (_password == _right_password1 || _password == _right_password2)
                {
                    emit sendChangeWindow(4,wrong_left);
                    is_right_password = true;
                    window_type = 1;

                }
            }
            else if (pressing_button_id > -1 && pressing_button_id <10)
            {
                if (_password.size() < 6)
                {
                    _password+=QString::number(pressing_button_id);
                     qDebug()<<_password;
                     emit sendToQml_password(_password.size());
                }
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 1) // notify window
    {
        if (type == 1) // press
        {
            if (x > 95 && x <140 && y > 155 && y < 200)
            {
                pressing_button_id =12;
                emit sendToQml_button(1, pressing_button_id);
            }
        }
        else if(type == -1) //release
        {
            if (pressing_button_id != -1) emit sendToQml_button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                if (is_right_password)
                {
                    qDebug()<<"true password, end this process, start other process";
                    /*
                     * code for start other process
                     */
                    showing_frame();
                    //exit(0);
                }
                else
                {
                    window_type=0;
                    emit sendChangeWindow(0,wrong_left);
                }

            }
        }
    }


}
