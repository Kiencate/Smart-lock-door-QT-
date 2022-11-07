#include "backend.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    pressing_button_id = -1;
    // get right password from txt file
    QFile file("/home/kiencate/Documents/benzenx_job/Rfid_rc522_i2c_linux/password.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug()<<file.errorString();
    }
    QTextStream in(&file);
    _right_password = in.readLine();
}

void BackEnd::handle_touch_event(int type, int x, int y)
{
    qDebug()<<"x"<<x<<"y"<<y;
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
            if (_password.size() < 6) emit sendNotify(1);
            else if (_password != _right_password) emit sendNotify(2);
            else
            {
                qDebug()<<"true password, end this process, start other process";
                /*
                 * code for start other process
                 */
                exit(0);
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
