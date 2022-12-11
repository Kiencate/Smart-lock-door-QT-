#include "touchevent.h"

volatile int is_new_touch;
const char *touch_json_path = "../touch.json";
void usrui(int sig)
{
  is_new_touch = 1;
}

TouchEvent::TouchEvent()
{
    if (signal(SIGUSR1, usrui)==SIG_ERR)
    {
        perror("\nSIGUSR2");
    }
}
TouchEvent::~TouchEvent()
{

}

void TouchEvent::run()
{
    int fd_touch_json;
    loop:
    while(is_new_touch == 0)
    {
        usleep(100000);
    }
 
    if((fd_touch_json=open(touch_json_path, O_RDONLY)) == -1) { 
        qDebug()<<"touch event: open touch file failed";
    }
    
    if(flock(fd_touch_json,LOCK_SH)==-1)
    {
        qDebug()<<"touch event: can't lock touch file";
    }  
    touch_json_obj = json_object_from_fd(fd_touch_json);
    type = json_object_get_int(json_object_object_get(touch_json_obj,"type"));
    x = json_object_get_int(json_object_object_get(touch_json_obj,"x"));
    y = json_object_get_int(json_object_object_get(touch_json_obj,"y"));
    close(fd_touch_json);
    json_object_put(touch_json_obj); 
    emit new_touch_event(type,x,y);
    qDebug()<<"new touch"<<type<<x<<y;
    is_new_touch = 0;
    goto loop;

}