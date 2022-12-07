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

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )
const char *status_wifi_json_path = "../status.json";
int main( )
{
    int length, i = 0;
    int fd_file_json;
    int wd;
    char buffer[EVENT_BUF_LEN];

    /*creating the INOTIFY instance*/
    fd_file_json = inotify_init();

    /*checking for error*/
    if ( fd_file_json < 0 ) {
        perror( "inotify_init" );
    }

    /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
    wd = inotify_add_watch( fd_file_json, status_wifi_json_path, IN_MODIFY );
    /*read to determine the event change happens on “/tmp” directory. Actually this read blocks until the change event occurs*/ 
    loop:
    i=0;
    length = read( fd_file_json, buffer, EVENT_BUF_LEN ); 

    std::cout<<"signal";
    /*checking for error*/
    if ( length < 0 ) {
        perror( "read" );
    }
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
        i += EVENT_SIZE + event->len;
        int fd;
        if((fd=open(status_wifi_json_path, O_RDWR)) == -1) { 
            std::cout<<"videostreamer: open status file failed"<<std::endl;
        }

        if(flock(fd,LOCK_EX)==-1)
        {
            std::cout<<"videostreamer: can't lock status file"<<std::endl;
        }
        std::ifstream file_status_read;
        file_status_read.open(status_wifi_json_path);
        while (!file_status_read) 
        {
            std::cout<<"videostreamer: open status file failed"<<std::endl;
        }
        try
        {
            nlohmann::json status = nlohmann::json::parse(file_status_read);       
            file_status_read.close();
            std::cout<<"face"<<status["start_face_recognize_process"].dump();
            if(std::stoi(status["start_face_recognize_process"].dump())==1)
            {
                std::ofstream file_status_write;
                file_status_write.open(status_wifi_json_path);
                while (!file_status_write) 
                {
                    std::cout<<"videostreamer: open status file failed"<<std::endl;
                }
                status["start_face_recognize_process"] = 0;
                status["is_face_detected"] = 1;
                file_status_write << status << std::endl;
                file_status_write.close(); 
            }
        }
        catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl;}       
        close(fd);   
    }
  
    goto loop;
    /*removing the “/tmp” directory from the watch list.*/
    inotify_rm_watch( fd_file_json, wd );

    /*closing the INOTIFY instance*/
    close( fd_file_json );
    return 0;
}
