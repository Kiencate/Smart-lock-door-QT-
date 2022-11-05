#include "backend.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    pressing_button_id = -1;
}

void BackEnd::handle_touch_event(int type, int x, int y)
{
    qDebug()<<"x"<<x<<"y"<<y;
    if (type == 1)
    {
        if (x > 20 && x <70)
        {
            if (y > 110 && y<140)
            {
                pressing_button_id = 1;
                emit sendToQml_button(1, 1);
            }
            else if (y > 260 && y < 290)
            {
                qDebug()<<"10";
                pressing_button_id = 10;
                emit sendToQml_button(1, 10);
            }


        }

    }
    else if (type == -1)
    {
        emit sendToQml_button(-1, pressing_button_id);
        if (pressing_button_id == 10 && _password.size() > 0)
        {
           _password = _password.left(_password.size() - 1);
           qDebug()<<_password;
           emit sendToQml_password(_password.size());
        }
        else if (pressing_button_id == 11) qDebug()<<"ent";
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
