#include "backend.h"

using namespace cv;
using namespace std;
using namespace zbar;

BackEnd::BackEnd(bool is_wifi_done, QObject* parent) : QObject(parent)
{
    if(is_wifi_done) 
    {
        qDebug()<<"wifi is setted";
        window_type = 5;
    }
    else
    {
        qDebug()<<"wifi is not setted";
        window_type = 0;
    }
        
    wrong_left = 5;
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
    qDebug()<<"x"<<x<<"y"<<y;
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
                qDebug()<<"start qrcode process";
                emit switch_to_qrcode_scan();
                emit sendToQml_ChangeWindow(1,"",wrong_left);
//                window_type = 1;
                qDebug()<<"start qrcode process";
            }
            else if (pressing_button_id ==16) //bluetooth
            {
                configured_with_bluetooth = true;
                qDebug()<<"start bluetooth process";
                serversocket = new ServerSocket();
                serversocket ->start();
                agent = new Agent();
                myAdaptor = new MyQDusAdaptor(agent);
                if(QDBusConnection::systemBus().registerObject("/pairing/agent",agent)){
                    qDebug() << "registerObject was Succesfull!";
                } else {
                    qDebug() << "registerObject was not Succesfull!";
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
        qDebug()<<"khong cho mo nua";
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
                exit(0);
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

void BackEnd::openWithFaceSuccess()
{
    emit sendToQml_ChangeWindow(12,"",wrong_left);
    window_type = 12;
}
