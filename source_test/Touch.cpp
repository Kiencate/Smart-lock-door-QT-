#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/file.h>
#include <json.h>
#include <chrono>
#include "signal.h"

const char *touch_json_path = "../touch.json";
int getpidui()
{
    char line_face[30];
    FILE *cmd_face = popen("pidof button", "r");
    fgets(line_face, 30, cmd_face);
    pid_t pid_ui = strtoul(line_face, NULL, 10);
    pclose(cmd_face);
    return pid_ui;
}
int main( int argc, char *argv[])
{
    int type, x ,y;
    type = std::stoi(argv[1]);
    x = std::stoi(argv[2]);
    y = std::stoi(argv[3]);

    int fd_touch_json;
    if((fd_touch_json=open(touch_json_path, O_RDWR)) == -1) { 
        std::cout<<"touch event: open touch file failed";
    }

    if(flock(fd_touch_json,LOCK_EX)==-1)
    {
        std::cout<<"touch event: can't lock touch file";
    }
    struct json_object *touch_json_obj= json_object_from_fd(fd_touch_json);

    json_object *type_json = json_object_object_get(touch_json_obj,"type");
    json_object_set_int(type_json, type);
    json_object *x_json = json_object_object_get(touch_json_obj,"x");
    json_object_set_int(x_json, x);
    json_object *y_json = json_object_object_get(touch_json_obj,"y");
    json_object_set_int(y_json, y);
    lseek(fd_touch_json,0,SEEK_SET);
    if(write(fd_touch_json,json_object_get_string(touch_json_obj),strlen(json_object_get_string(touch_json_obj)))<0)
    {
        std::cout<<"is_person: fail to open door";
    } 
    json_object_put(touch_json_obj);   
    close(fd_touch_json);
    kill(getpidui(), SIGUSR1); 
    return 0;
}
