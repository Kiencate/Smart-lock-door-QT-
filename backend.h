#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <zbar.h>
#include <signal.h>
#include "serversocket.h"
#include "agent.h"
#include "dbusadaptor.h"
#include <fcntl.h>
#include <sys/file.h>
#include <fstream>
#include <json.h>

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(bool is_wifi_done, QObject *parent = nullptr);
    void sleepQt();
    /*
    * sleep gui qt and camera to save power
    */
    void open_and_close_door_after_3s();  // opendoor and set timer to call slot close door after 3s
    void start_face_detect(); //wake up process face detect
    /* variable in file status json */
    bool is_wifi_configured; // true if wifi is configured
    bool is_person; // true if ir sensor detected person
    bool is_door_closed; //true if door is closed
    bool is_face_detected; //true if face detection process deteted person's face
    bool is_rfid_success; //true if rfid scan success
signals:
    void sendToQml_Button(int type, int button_id);
    /*type:
     * 1 -> press
     * -1 -> release
     */
    /*id:
     * 0 -> button 0
     * 1 -> button 1
     * 2 -> button 2
     * 3 -> button 3
     * 4 -> button 4
     * 5 -> button 5
     * 6 -> button 6
     * 7 -> button 7
     * 8 -> button 8
     * 9 -> button 9
     * 10 -> button delete
     * 11 -> button enter
     * 12 -> button ok
     * 13 -> button ai mode
     * 14 -> button password mode
     * 15 -> button qrcode
     * 16 -> button bluetooth
     */
    void sendToQml_ChangeWindow(int type, QString name_device_bluetooth, int wrong_left);
    /*type:
     * 0 -> config wifi
     * 1 -> scaning qr code
     * 2 -> connecting bluetooth
     * 3 -> connected bluetooh
     * 4 -> config wifi success
     * 5 -> main window unlock door
     * 6 -> detecting face
     * 7 -> entering password
     * 8 -> password is short
     * 9 -> wrong password
     * 10 -> wrong password over 5 times
     * 11 -> right password, opening window
     * 12 -> right face, opening window
     * 13 -> in sleep mode, don't show frame
     */
    
    void sendToQml_Password(int num);
    /*signal for set number of icon * in frontend
     * num: number of password character (0-6)
     */
    
    void switch_to_main_window();
    /*
    * switch videostreamer to main window
    */
    void switch_to_qrcode_scan();
    /*
    * switch videostreamer to scan qr code
    */
    void switch_to_face_detect();
    /*
    * switch videostreamer to detect face
    */
    void stopCamera();
    /*
     * stop using camera (power saved)
     */

public slots:
    void handle_touch_event(int type, int x, int y);
    /*type:
     1 -> press
     -1 -> release
     */
    void onConectedBluetooth();
    /*
    * bluetooth connected with android app
    */
    void onReceivedWifi();
    /*
    * receive ssid and password success
    */
    void onJsonStatusChange(bool _is_person, bool _is_wifi_configured, bool _is_door_closed, bool _is_face_detected, bool _is_password_right, bool _is_rfid_success);
    /*
    * when file status json changed detected person
    * Có 4 trường hợp:
    *   -TH1: Không có người hoặc có người && mở cửa-> tắt giao diện, không quan tâm đến các tín hiệu khác
    *   -TH2: Có người, cửa đang đóng, wifi chưa config -> Giao diện config wifi
    *   -TH3: Có người, cửa đang đóng, wifi đã config -> Giao diện đang mở khóa (nhập mật khẩu và mở bằng khuôn mặt)
    *   -TH4: Có người, cửa đang đóng, phát hiện khuôn mặt thành công hoặc mở bằng mật khẩu thành công hoặc mở bằng thẻ rfid thành công-> Giao diện đã mở khóa (hiện camera và icon mở khóa)
    */
    void closeDoor(); // close the door after 3s
private:
    
    int window_type; // type of window (see signal sendToQml_ChangeWindow)
    int pressing_button_id; // id of pressed button (see sendToQml_button)
    // socket, agent, adaptor dbus bluetooth
    bool configured_with_bluetooth; // check if staff config wifi with bluetooth
    bool is_wifi_configured_before; // check if this is first time config wifi
    ServerSocket *serversocket;
    Agent *agent;
    MyQDusAdaptor* myAdaptor;
    QString name_device_bluetooth; // name of bluetooth device lockdoor

    // using in entering password window
    int wrong_left; // wrong times remain (max 5 times)
    bool is_right_password; // check if user entered right password
    QString _password; //password user entered
    QString _right_password; //right password
    struct json_object *status_json_obj; //json object for status json file


};

// variable and function for qr code



#endif // BACKEND_H
