#include "backend.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    is_right_password = false;
    window_type =2 ;
    wrong_left = 5;
    pressing_button_id = -1;
    // get right password from txt file
    QFile file("../password.txt");
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
                    exit(0);
                }
                else
                {
                    window_type=0;
                    emit sendChangeWindow(0,wrong_left);
                }

            }
            pressing_button_id = -1;
        }
    }
    else if (window_type == 2) // start main window
    {
        if (type == 1) // press
        {
            if (x > 95 && x <150 && y > 190 && y < 210)
            {
                pressing_button_id =13;
                emit sendToQml_button(1, pressing_button_id);
            }
            if (x > 95 && x <150 && y > 90 && y < 110)
            {
                pressing_button_id =14;
                emit sendToQml_button(1, pressing_button_id);
            }
        }
        else if(type == -1) //release
        {
            if (pressing_button_id != -1) emit sendToQml_button(-1, pressing_button_id);
            if (pressing_button_id == 13) // ai mode
            {
                qDebug()<<"in ai mode";

            }
            else if (pressing_button_id == 14) //password mode
            {
                window_type =0;
                emit sendChangeWindow(0,wrong_left);
            }
            pressing_button_id = -1;
        }
    }

}
