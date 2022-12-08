#include "backend.h"

using namespace cv;
using namespace std;
using namespace zbar;
const char *status_password_json_path = "../status.json";
int getpidface()
{
  char line_ui[30];
  FILE *cmd_ui = popen("pidof Face", "r");
  fgets(line_ui, 30, cmd_ui);
  pid_t pid_face = strtoul(line_ui, NULL, 10);
  pclose(cmd_ui);
  return pid_face;
}
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
    wrong_left = 5;
    is_right_password = false;
    pressing_button_id = -1;
    configured_with_bluetooth= false;
    is_wifi_configured_before = is_wifi_configured;
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
                qDebug()<<"start face";
                // kill(getpidface(), SIGUSR1);
                start_face_detect();
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
                emit switch_to_main_window(); 
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
    // emit sendToQml_ChangeWindow(4,"",wrong_left);
    // window_type = 4;
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
    if(is_person &&  is_door_closed)
    {
         // open camera
        if(is_face_detected) 
        {
            open_and_close_door_after_3s();
        }
        else if(is_rfid_success)
        {
            open_and_close_door_after_3s();
            emit switch_to_main_window();
        }
        else
        {
            if(is_wifi_configured )
            {
                if(is_wifi_configured_before)
                {
                    sendToQml_ChangeWindow(5,"",wrong_left);
                    window_type = 5;
                }
                else
                {
                    emit sendToQml_ChangeWindow(4,"",wrong_left);
                    window_type = 4;
                    is_wifi_configured_before = true;
                    qDebug()<<"backend: wificonfig";
                }
                               
            }
            else
            {
                sendToQml_ChangeWindow(0,"",wrong_left);
                window_type = 0;
            }
        }       
    }
    else if(!is_person)
    {
        sleepQt();
    }
}

void BackEnd::open_and_close_door_after_3s()
{
    sendToQml_ChangeWindow(12,"",wrong_left);
    int fd_status_json;
    if((fd_status_json=open(status_password_json_path, O_RDWR)) == -1) { 
        qDebug()<<"backend: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        qDebug()<<"backend: can't lock status file";
    }
    status_json_obj = json_object_from_fd(fd_status_json);

    json_object *is_closed_door = json_object_object_get(status_json_obj,"is_closed_door");
    json_object_set_int(is_closed_door, 0);
    json_object *is_charged = json_object_object_get(status_json_obj,"is_charged");
    json_object_set_int(is_charged, 0);
    if (is_right_password)
    {
        json_object *is_password_success = json_object_object_get(status_json_obj,"is_password_success");
        json_object_set_int(is_password_success, 1);
    } 
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        qDebug()<<"backend: fail to open door";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json); 
    QTimer::singleShot(3000, this, SLOT(closeDoor()));  
}

void BackEnd::closeDoor()
{
    int fd_status_json;
    if((fd_status_json=open(status_password_json_path, O_RDWR)) == -1) { 
        qDebug()<<"videostreamer: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        qDebug()<<"videostreamer: can't lock status file";
    }
    status_json_obj = json_object_from_fd(fd_status_json);
    json_object *is_closed_door = json_object_object_get(status_json_obj,"is_closed_door");
    json_object_set_int(is_closed_door, 1);
    json_object *is_charged = json_object_object_get(status_json_obj,"is_charged");
    json_object_set_int(is_charged, 1);
    json_object *is_face_detected = json_object_object_get(status_json_obj,"is_face_detected");
    json_object_set_int(is_face_detected, 0);
    json_object *is_password_success = json_object_object_get(status_json_obj,"is_password_success");
    json_object_set_int(is_password_success, 0);
    json_object *is_rfid_success = json_object_object_get(status_json_obj,"is_rfid_success");
    json_object_set_int(is_rfid_success, 0);  
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        qDebug()<<"backend: fail to close door";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json);
    /// reset password
    wrong_left = 5;
    _password="";
    emit sendToQml_Password(_password.size());
    sendToQml_ChangeWindow(5,"",wrong_left);
}

void BackEnd::start_face_detect()
{
    int fd_status_json;
    if((fd_status_json=open(status_password_json_path, O_RDWR)) == -1) { 
        qDebug()<<"backend: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        qDebug()<<"backend: can't lock status file";
    }
    status_json_obj = json_object_from_fd(fd_status_json);

    json_object *start_face_recognize_process = json_object_object_get(status_json_obj,"start_face_recognize_process");
    json_object_set_int(start_face_recognize_process, 1);
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        qDebug()<<"backend: fail to open door";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json); 
}