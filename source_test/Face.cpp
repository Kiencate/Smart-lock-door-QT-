#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <time.h>
#include <nlohmann/json.hpp>
#include <fcntl.h>
#include <sys/file.h>
#include <fstream>
#include <json.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
const char *status_wifi_json_path = "../status.json";
int main( )
{
    int length, i = 0;
    int fd_notify_file_json;
    int fd_status_json;
    struct json_object *status_json_obj;
    int watch_file_json;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd_notify_file_json = inotify_init();

    /*checking for error*/
    if ( fd_notify_file_json < 0 ) {
        perror( "inotify_init" );
    }

    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    watch_file_json = inotify_add_watch( fd_notify_file_json, status_wifi_json_path, IN_MODIFY );
    /*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 
    loop:
    i=0;
    length = read( fd_notify_file_json, buffer, EVENT_BUF_LEN ); 
    /*checking for error*/
    if ( length < 0 ) {
        perror( "read" );
    }
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
        i += EVENT_SIZE + event->len;
    }
    
    if((fd_status_json=open(status_wifi_json_path, O_RDWR)) == -1) { 
        std::cout<<"face: open status file failed"<<std::endl;
    }

    if(flock(fd_status_json,LOCK_SH)==-1)
    {
        std::cout<<"face: can't lock status file"<<std::endl;
    }
    status_json_obj = json_object_from_fd(fd_status_json);   
    close(fd_status_json);
    if (json_object_get_int(json_object_object_get(status_json_obj,"start_face_recognize_process")) == 1)
    {
        if((fd_status_json=open(status_wifi_json_path, O_RDWR)) == -1) { 
        std::cout<<"face: open status file failed"<<std::endl;
        }

        if(flock(fd_status_json,LOCK_EX)==-1)
        {
            std::cout<<"face: can't lock status file"<<std::endl;
        }
        status_json_obj = json_object_from_fd(fd_status_json);   
        json_object *is_face_detected = json_object_object_get(status_json_obj,"is_face_detected");
        json_object_set_int(is_face_detected, 1);
        json_object *start_face_recognize_process = json_object_object_get(status_json_obj,"start_face_recognize_process");
        json_object_set_int(start_face_recognize_process, 0);
        lseek(fd_status_json,0,SEEK_SET);
        if(write(fd_status_json,json_object_get_string(status_json_obj),strlen(json_object_get_string(status_json_obj)))<0)
        {
            std::cout<<"face: fail to detect"<<std::endl;
        } 
        close(fd_status_json);
    }
    json_object_put(status_json_obj);
  
    goto loop;
    /*removing the “/tmp” directory from the watch list.*/
    inotify_rm_watch( fd_notify_file_json, watch_file_json );

    /*closing the INOTIFY instance*/
    close( fd_notify_file_json );
    return 0;
}
