#include "backend.h"

using namespace cv;
using namespace std;
using namespace zbar;
const char *status_password_json_path = "../status.json";
BackEnd::BackEnd(bool is_wifi_done, QObject* parent) : QObject(parent)
{
    is_wifi_configured = is_wifi_done;
    if(is_wifi_done) 
    {
        qDebug()<<"backend: wifi is setted";
        window_type = 5;
    }
    else
    {
        qDebug()<<"backend: wifi is not setted";
        window_type = 0;
    }
    wrong_left = 1;
    is_right_password = false;
    pressing_button_id = -1;
    configured_with_bluetooth= false;
    //read right password from txt file 
    QFile file("../password.txt");
    while(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    _right_password = in.readLine();
    file.close();
}


void BackEnd::handle_touch_event(int type, int x, int y)
{
    // qDebug()<<"backend:"<<"x"<<x<<"y"<<y;
    if (window_type == 0) // config_wifi
    {
        if (type == 1)
        {
            if (x > 20 && x <220)
            {
                if (y > 55 && y<140)
                {
                    pressing_button_id = 15;
                    emit sendToQml_Button(1, 15);

                }

                if (y > 162 && y<256)
                {
                    pressing_button_id = 16;
                    emit sendToQml_Button(1, 16);
                }
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 15) //qrcode
            {
                qDebug()<<"backend: start qrcode process";
                emit switch_to_qrcode_scan();
                emit sendToQml_ChangeWindow(1,"",wrong_left);
            }
            else if (pressing_button_id ==16) //bluetooth
            {
                configured_with_bluetooth = true;
                qDebug()<<"backend: start bluetooth process";
                serversocket = new ServerSocket();
                serversocket -> start();
                agent = new Agent();
                myAdaptor = new MyQDusAdaptor(agent);
                if(QDBusConnection::systemBus().registerObject("/pairing/agent",agent)){
                    qDebug() << "backend: registerObject was Succesfull!";
                } else {
                    qDebug() << "backend: registerObject was not Succesfull!";
                }
                connect(myAdaptor, &MyQDusAdaptor::Connect_success, this, &BackEnd::onConectedBluetooth);
                connect(serversocket, &ServerSocket::Receive_wifi_success, this, &BackEnd::onReceivedWifi);
                emit sendToQml_ChangeWindow(2,agent->getDeviceName(),wrong_left);
                window_type = 2;
                //exit(0);
            }
            pressing_button_id = -1;

        }
    }
    else if (window_type == 4)
    {
        if (type == 1)
        {
            if (x > 95 && x <140 && y > 155 && y<200)
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                if(configured_with_bluetooth)
                {
                    delete myAdaptor;
                    delete agent;
                }
                emit sendToQml_ChangeWindow(5,"",wrong_left);
                window_type = 5;
            }
            pressing_button_id = -1;
        }
    }

    else if (window_type == 5)
    {
        if (type == 1) // press
        {
            if (x > 40 && x <190 && y > 90 && y < 150)
            {
                pressing_button_id =14;
                emit sendToQml_Button(1, pressing_button_id);
            }
            if (x > 40 && x <200 && y > 190 && y < 250)
            {
                pressing_button_id =13;
                emit sendToQml_Button(1, pressing_button_id);
            }
        }
        else if(type == -1) //release
        {
            if (pressing_button_id != -1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 13) // ai mode
            {
                window_type = 6; // in face recognition
                emit switch_to_face_detect();
                emit sendToQml_ChangeWindow(6,"",wrong_left); // switch to face recognition window


            }
            else if (pressing_button_id == 14) //password mode
            {
                window_type =7;
                emit stopCamera();
                emit sendToQml_ChangeWindow(7,"",wrong_left);

            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 7)
    {
        if (type == 1)
                {
                    if (x > 17 && x <74)
                    {
                        if (y > 110 && y<140)
                        {
                            pressing_button_id = 1;
                            emit sendToQml_Button(1, 1);
                        }
                        if (y > 160 && y<190)
                        {
                            pressing_button_id = 4;
                            emit sendToQml_Button(1, 4);
                        }
                        if (y > 210 && y<240)
                        {
                            pressing_button_id = 7;
                            emit sendToQml_Button(1, 7);
                        }
                        else if (y > 260 && y < 290)
                        {
                            pressing_button_id = 10;
                            emit sendToQml_Button(1, 10);
                        }
                    }
                    else if(x > 92 && x < 149)
                    {
                        if (y > 110 && y<140)
                        {
                            pressing_button_id = 2;
                            emit sendToQml_Button(1, 2);
                        }
                        if (y > 160 && y<190)
                        {
                            pressing_button_id = 5;
                            emit sendToQml_Button(1, 5);
                        }
                        if (y > 210 && y<240)
                        {
                            pressing_button_id = 8;
                            emit sendToQml_Button(1, 8);
                        }
                        else if (y > 260 && y < 290)
                        {
                            pressing_button_id = 0;
                            emit sendToQml_Button(1, 0);
                        }
                    }
                    else if(x > 167 && x < 224)
                    {
                        if (y > 110 && y<140)
                        {
                            pressing_button_id = 3;
                            emit sendToQml_Button(1, 3);
                        }
                        if (y > 160 && y<190)
                        {
                            pressing_button_id = 6;
                            emit sendToQml_Button(1, 6);
                        }
                        if (y > 210 && y<240)
                        {
                            pressing_button_id = 9;
                            emit sendToQml_Button(1, 9);
                        }
                        else if (y > 260 && y < 290)
                        {
                            pressing_button_id = 11;
                            emit sendToQml_Button(1, 11);
                        }
                    }

                }
        else if (type == -1)
        {
            if (pressing_button_id != -1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 10 && _password.size() > 0)
            {
               _password = _password.left(_password.size() - 1);
               qDebug()<<_password;
               emit sendToQml_Password(_password.size());
            }
            else if (pressing_button_id == 11)
            {
                if (_password.size() < 6)
                {
                    emit sendToQml_ChangeWindow(8,"",wrong_left);
                    window_type=8;
                }
                else if (_password != _right_password)
                {

                    if (--wrong_left == 0)
                    {
                        _password="";
                        emit sendToQml_ChangeWindow(10,"",wrong_left);
                        window_type=10;
                    }
                    else
                    {
                        _password="";
                        emit sendToQml_ChangeWindow(9,"",wrong_left);
                        window_type=9;
                    }


                }
                else if (_password == _right_password)
                {
                    emit sendToQml_ChangeWindow(11,"",wrong_left);
                    is_right_password = true;
                    window_type = 11;

                }
            }
            else if (pressing_button_id > -1 && pressing_button_id <10)
            {
                if (_password.size() < 6)
                {
                    _password+=QString::number(pressing_button_id);
                     qDebug()<<_password;
                     emit sendToQml_Password(_password.size());
                }
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 8)
    {
        if (type == 1)
        {
            if (x > 95 && x <140 && y > 155 && y<200)
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                emit sendToQml_ChangeWindow(7,"",wrong_left);
                window_type = 7;
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 9)
    {
        if (type == 1)
        {
            if (x > 95 && x <140 && y > 155 && y<200)
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                emit sendToQml_ChangeWindow(7,"",wrong_left);
                window_type = 7;
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 10)
    {
        if (type == 1)
        {
            if (x > 95 && x <140 && y > 155 && y<200)
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                wrong_left = 5;
                _password="";
                emit sendToQml_Password(_password.size());
                emit switch_to_main_window();
                
                sendToQml_ChangeWindow(5,"",wrong_left);
                window_type = 5;
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 11)
    {
        if (type == 1)
        {
            if (x > 95 && x <140 && y > 155 && y<200)
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                wrong_left = 5;
                _password="";
                emit sendToQml_Password(_password.size());
                open_and_close_door_after_3s();
            }
            pressing_button_id = -1;
        }
    }

}

void BackEnd::onConectedBluetooth()
{
    emit sendToQml_ChangeWindow(3,"",wrong_left);
    window_type = 3;
}

void BackEnd::onReceivedWifi()
{
    emit sendToQml_ChangeWindow(4,"",wrong_left);
    window_type = 4;
}

void BackEnd::sleepQt()
{
    emit stopCamera();
    sendToQml_ChangeWindow(13,"",wrong_left);
}

void BackEnd::onJsonStatusChange(bool _is_person, bool _is_wifi_configured, bool _is_door_closed, bool _is_face_detected, bool _is_password_right, bool _is_rfid_success)
{
    qDebug()<<"backend: status change: pesron:"<<_is_person<<"  wifi:"<<_is_wifi_configured<<"  door close:"<<_is_door_closed<<"  face detected:"<<_is_face_detected<<" pass:"<<_is_password_right;
    is_person = _is_person;
    is_wifi_configured =_is_wifi_configured;
    is_door_closed = _is_door_closed;
    is_face_detected = _is_face_detected;
    is_rfid_success = _is_rfid_success;
    if(is_person && is_door_closed )
    {
        emit switch_to_main_window(); // open camera
        if(is_face_detected || is_rfid_success) 
        {
            open_and_close_door_after_3s();
        }
        else
        {
            if(is_wifi_configured)
            {
                sendToQml_ChangeWindow(5,"",wrong_left);
                window_type = 5;
            }
            else
            {
                sendToQml_ChangeWindow(0,"",wrong_left);
                window_type = 0;
            }
        }       
    }
    else if (!is_person)
    {
        sleepQt();
    }
}

void BackEnd::open_and_close_door_after_3s()
{
    sleepQt();
    int fd;
    if((fd=open(status_password_json_path, O_RDWR)) == -1) { 
        std::cout<<"videostreamer: open status file failed"<<std::endl;
    }

    if(flock(fd,LOCK_EX)==-1)
    {
        std::cout<<"videostreamer: can't lock status file"<<std::endl;
    }
    std::ifstream file_status_read;
    file_status_read.open(status_password_json_path);
    while (!file_status_read) 
    {
        std::cout<<"videostreamer: open status file failed"<<std::endl;
    }
    try
    {
        nlohmann::json status = nlohmann::json::parse(file_status_read);       
        file_status_read.close();
        std::ofstream file_status_write;
        file_status_write.open(status_password_json_path);
        while (!file_status_write) 
        {
            std::cout<<"videostreamer: open status file failed"<<std::endl;
        }
        if (is_right_password)
        {
            status["is_password_success"] = 1;
        }
        status["is_closed_door"] = 0;
        status["is_charged"] = 0;
            
        file_status_write << status << std::endl;
        file_status_write.close(); 
        
    }
    catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl;}       
    close(fd); 

    usleep(3000000);

    if((fd=open(status_password_json_path, O_RDWR)) == -1) { 
        std::cout<<"videostreamer: open status file failed"<<std::endl;
    }

    if(flock(fd,LOCK_EX)==-1)
    {
        std::cout<<"videostreamer: can't lock status file"<<std::endl;
    }
    file_status_read.open(status_password_json_path);
    while (!file_status_read) 
    {
        std::cout<<"videostreamer: open status file failed"<<std::endl;
    }
    try
    {
        nlohmann::json status = nlohmann::json::parse(file_status_read);       
        file_status_read.close();
        std::ofstream file_status_write;
        file_status_write.open(status_password_json_path);
        while (!file_status_write) 
        {
            std::cout<<"videostreamer: open status file failed"<<std::endl;
        }
        if (is_right_password) is_right_password = false;
        status["is_face_detected"] = 0;
        status["is_password_success"] = 0;
        status["is_rfid_success"]=0;
        status["is_closed_door"] = 1;
        status["is_charged"] = 1; 
            
        file_status_write << status << std::endl;
        file_status_write.close(); 
        
    }
    catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl;}       
    close(fd); 
}