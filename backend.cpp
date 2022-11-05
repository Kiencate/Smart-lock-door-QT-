#include "backend.h"

BackEnd::BackEnd(QObject* parent) : QObject(parent)
{
    pressing_button_id = -1;
}

void BackEnd::handle_touch_event(int type, int x, int y)
{/*
    qDebug()<<"x"<<x<<"y"<<y;*/
    if (type == 1)
    {
        if (x > 20 && x <220)
        {
            if (y > 55 && y<140)
            {
                pressing_button_id = 0;
                emit sendToQml_button(1, 0);
            }

            if (y > 162 && y<256)
            {
                pressing_button_id = 1;
                emit sendToQml_button(1, 1);
            }
        }

    }
    else if (type == -1)
    {
        emit sendToQml_button(-1, pressing_button_id);
        pressing_button_id = -1;

        //code to start process qrcode or bluetooth
    }

}
