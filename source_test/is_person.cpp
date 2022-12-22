#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/file.h>
#include <fstream>
#include <json.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
const char *status_wifi_json_path = "../status.json";
int main( int argc, char *argv[])
{
    // bat ir chay ./ir 1
    // tat ir chay ./ir 0
    int mode;
    mode = std::stoi(argv[1]);
    std::cout<<mode;
    int fd_status_json;
    if((fd_status_json=open(status_wifi_json_path, O_RDWR)) == -1) { 
        std::cout<<"is_person: open status file failed";
    }

    if(flock(fd_status_json,LOCK_EX)==-1)
    {
        std::cout<<"is_person: can't lock status file";
    }
    struct json_object *status_json_obj= json_object_from_fd(fd_status_json);

    json_object *is_person = json_object_object_get(status_json_obj,"wifi_connected");
    json_object_set_int(is_person, mode);
    lseek(fd_status_json,0,SEEK_SET);
    if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
    {
        std::cout<<"is_person: fail to open door";
    } 
    json_object_put(status_json_obj);   
    close(fd_status_json); 
    return 0;
}
