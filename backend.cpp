#include "backend.h"

BackEnd::BackEnd(bool is_wifi_done, QObject* parent) : QObject(parent)
{
    // GPIOExport(GPIO1_3);
    // GPIODirection(GPIO1_3, OUT);
    // GPIOWrite(GPIO1_3, LOW);

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
    QFile file("../password/password.txt");
    while(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    right_password = in.readLine();
    file.close();
}

void BackEnd::handle_touch_event(int type, int x, int y)
{
    // qDebug()<<"backend:"<<"x"<<x<<"y"<<y;
    if (window_type == 0) // config_wifi
    {
        if (type == 1) //press
        {
            if (x > 20 && x <220)
            {
                if (y > 55 && y<140) //button qrcode
                {
                    pressing_button_id = 15;
                    emit sendToQml_Button(1, 15);

                }

                if (y > 162 && y<256) //button bluetooth
                {
                    pressing_button_id = 16;
                    emit sendToQml_Button(1, 16);
                }
            }

        }
        else if (type == -1) //release
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
    else if (window_type == 4) //config wifi success
    {
        if (type == 1) //press
        {
            if (x > 95 && x <140 && y > 155 && y<200) //button ok
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1)//release
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

    else if (window_type == 5) // main window unlock door
    {
        if (type == 1) // press
        {
            if (x > 40 && x <190 && y > 90 && y < 150) // password mode
            {
                pressing_button_id =14;
                emit sendToQml_Button(1, pressing_button_id);
            }
            if (x > 40 && x <200 && y > 190 && y < 250) // ai mode
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
                emit sendToQml_ChangeWindow(6,"",wrong_left); // switch to face recognition window
                start_face_detect();
                emit switch_to_face_detect();              
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
    else if (window_type == 7) //enter password
    {
        if (type == 1) //press
                {
                    if (x > 17 && x <74)
                    {
                        if (y > 110 && y<140) //button1
                        {
                            pressing_button_id = 1;
                            emit sendToQml_Button(1, 1);
                        }
                        if (y > 160 && y<190) //button4
                        {
                            pressing_button_id = 4;
                            emit sendToQml_Button(1, 4);
                        }
                        if (y > 210 && y<240) //button7
                        {
                            pressing_button_id = 7;
                            emit sendToQml_Button(1, 7);
                        }
                        else if (y > 260 && y < 290) //button del
                        {
                            pressing_button_id = 10;
                            emit sendToQml_Button(1, 10);
                        }
                    }
                    else if(x > 92 && x < 149) 
                    {
                        if (y > 110 && y<140) //button 2
                        {
                            pressing_button_id = 2; 
                            emit sendToQml_Button(1, 2);
                        }
                        if (y > 160 && y<190) //button 5
                        {
                            pressing_button_id = 5;
                            emit sendToQml_Button(1, 5);
                        }
                        if (y > 210 && y<240) //button 8
                        {
                            pressing_button_id = 8;
                            emit sendToQml_Button(1, 8);
                        }
                        else if (y > 260 && y < 290) //button 0
                        {
                            pressing_button_id = 0;
                            emit sendToQml_Button(1, 0);
                        }
                    }
                    else if(x > 167 && x < 224)
                    {
                        if (y > 110 && y<140) //button 3
                        {
                            pressing_button_id = 3;
                            emit sendToQml_Button(1, 3);
                        }
                        if (y > 160 && y<190) //button 6
                        {
                            pressing_button_id = 6;
                            emit sendToQml_Button(1, 6);
                        }
                        if (y > 210 && y<240) //button 9
                        {
                            pressing_button_id = 9;
                            emit sendToQml_Button(1, 9);
                        }
                        else if (y > 260 && y < 290) //button enter
                        {
                            pressing_button_id = 11;
                            emit sendToQml_Button(1, 11);
                        }
                    }

                }
        else if (type == -1) //release
        {
            if (pressing_button_id != -1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 10 && password.size() > 0)
            {
               password = password.left(password.size() - 1);
               qDebug()<<password;
               emit sendToQml_Password(password.size());
            }
            else if (pressing_button_id == 11)
            {
                if (password.size() < 6)
                {
                    emit sendToQml_ChangeWindow(8,"",wrong_left);
                    window_type=8;
                }
                else if (password != right_password)
                {

                    if (--wrong_left == 0)
                    {
                        password="";
                        emit sendToQml_ChangeWindow(10,"",wrong_left);
                        window_type=10;
                    }
                    else
                    {
                        password="";
                        emit sendToQml_ChangeWindow(9,"",wrong_left);
                        window_type=9;
                    }


                }
                else if (password == right_password)
                {
                    emit sendToQml_ChangeWindow(11,"",wrong_left);
                    is_right_password = true;
                    window_type = 11;

                }
            }
            else if (pressing_button_id > -1 && pressing_button_id <10)
            {
                if (password.size() < 6)
                {
                    password+=QString::number(pressing_button_id);
                     qDebug()<<password;
                     emit sendToQml_Password(password.size());
                }
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 8) //password is short 
    {
        if (type == 1) //press
        {
            if (x > 95 && x <140 && y > 155 && y<200) //button ok
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1) // release
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
    else if (window_type == 9) //wrong password
    {
        if (type == 1) //press
        {
            if (x > 95 && x <140 && y > 155 && y<200) //button ok
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1) //release
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
    else if (window_type == 10) //wrong password over 5 times
    {
        if (type == 1) //press 
        {
            if (x > 95 && x <140 && y > 155 && y<200) //button ok
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1) //release
        {
            if (pressing_button_id!=-1) emit sendToQml_Button(-1, pressing_button_id);
            if (pressing_button_id == 12)
            {
                wrong_left = 5;
                password="";
                emit sendToQml_Password(password.size());
                emit switch_to_main_window();
                
                sendToQml_ChangeWindow(5,"",wrong_left);
                window_type = 5;
            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 11) //right password 
    {
        if (type == 1) //press
        {
            if (x > 95 && x <140 && y > 155 && y<200) //button ok
            {
                    pressing_button_id = 12;
                    emit sendToQml_Button(1, 12);
            }

        }
        else if (type == -1) //release
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
    emit sendToQml_ChangeWindow(3,"",wrong_left);
    window_type = 3;
    QTimer::singleShot(5000, this, SLOT(onConfigWifiSuccess())); 
    // open status wifi, lock file
    
}
void BackEnd::onConfigWifiSuccess()
{
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"video_streamer: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        qDebug()<<"video_streamer: can't lock status file";
    }
    status_json_obj= json_object_from_fd(fd_status_json);
    json_object *wifi_configured = json_object_object_get(status_json_obj,"wifi_configured");
    json_object_set_int(wifi_configured, 1);
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        qDebug()<<"video_streamer: fail config wifi";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json); 
}
void BackEnd::sleepQt()
{
    emit stopCamera();
    sendToQml_ChangeWindow(13,"",wrong_left);
}

void BackEnd::onJsonStatusChange(bool _is_person, bool _is_wifi_configured, bool _is_door_closed, bool _is_face_detected, bool _is_password_right, bool _is_rfid_success, bool is_start_face_detect)
{
    qDebug()<<"backend: status change: pesron:"<<_is_person<<"  wifi:"<<_is_wifi_configured<<"  door close:"<<_is_door_closed<<"  face detected:"<<_is_face_detected<<"  start_face:"<<is_start_face_detect<<" pass:"<<_is_password_right<<"  rfid"<<_is_rfid_success;
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
            emit captureFrame();
            open_and_close_door_after_3s();
        }
        else if(is_rfid_success)
        {
            open_and_close_door_after_3s();
            emit switch_to_main_window();
        }
        else if (is_start_face_detect)
        {
            sendToQml_ChangeWindow(6,"",wrong_left);
            window_type = 6;
        }
        else if(is_wifi_configured )
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
    else if(!is_person)
    {
        
        sleepQt();
        reset_backend();
    }
}

void BackEnd::open_and_close_door_after_3s()
{
    sendToQml_ChangeWindow(12,"",wrong_left);
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
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
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
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
    password="";
    emit sendToQml_Password(password.size());
    sendToQml_ChangeWindow(5,"",wrong_left);
}

void BackEnd::start_face_detect()
{
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
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
        qDebug()<<"backend: fail to start face detect";
    }   
    close(fd_status_json); 
    json_object_put(status_json_obj);  
}

void BackEnd::onPasswordFolderChange()
{
    sleepQt();
    //reset backend
    reset_backend();
    
}

void BackEnd::reset_backend()
{
    QFile file("../password/password.txt");
    while(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    right_password = in.readLine();
    file.close();
    if(is_wifi_configured) 
    {
        window_type = 5;
    }
    else
    {
        window_type = 0;
    }
    wrong_left = 5;
    is_right_password = false;
    pressing_button_id = -1;
    wrong_left = 5;
    password="";
    emit sendToQml_Password(password.size());
    int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"backend: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        qDebug()<<"backend: can't lock status file";
    }
    status_json_obj = json_object_from_fd(fd_status_json);

    json_object *is_closed_door = json_object_object_get(status_json_obj,"is_closed_door");
    json_object_set_int(is_closed_door, 1);
    json_object *is_charged = json_object_object_get(status_json_obj,"is_charged");
    json_object_set_int(is_charged, 1);
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        qDebug()<<"backend: fail to open door";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json); 
}