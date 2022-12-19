#include "touchevent.h"


TouchEvent::TouchEvent()
{
    if((fd_dev_event = open(MOUSEFILE, O_RDONLY)) == -1) 
    {
        perror("opening device");
        exit(0);
    }
}

TouchEvent::~TouchEvent()
{
    close(fd_dev_event);
}

void TouchEvent::run()
{
    while(read(fd_dev_event, &event_dev, sizeof(struct input_event))) 
    {
        // printf("Event: time %ld.%06ld, ", ie.input_event_sec, ie.input_event_usec);
        // if (event_dev.type==1 && event_dev)
        // qDebug()<<"type:"<<event_dev.type<<"  code:"<<event_dev.code<<"  value:"<<event_dev.value;
        //qDebug()<<"type:"<<event_dev.type<<"  code:"<<event_dev.code;

        if (event_dev.type == 0) //sync event
        {
            qDebug()<<"new touch event";
            if(is_new_event_touch) 
            {
                emit new_touch_event(1,x,y);
                is_new_event_touch = false;
            }
            else if(is_new_event_release) 
            {
                emit new_touch_event(-1,x,y);
                is_new_event_release = false;
            }
        }
 
        else if (event_dev.type == 1) //touch or release event
        {
            if(event_dev.code == 330 && event_dev.value == 1)
            {
                is_new_event_touch = true;
            }

            else if(event_dev.code == 330 && event_dev.value == 0)
            {
                is_new_event_release = true;
            }
        }

        else if (event_dev.type == 3) //touch or release point event
        {
            if(event_dev.code == 0) //point x
            {
                x_pre = 3800 - event_dev.value;
                x = x_pre * 240 / 3800;
                qDebug()<<"x: "<< x;
            }

            if(event_dev.code == 1) //point y
            {
                y = event_dev.value * 320 / 3800;
                qDebug()<<"y: "<< y;
            }
        }
    }
}
