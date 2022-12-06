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
int main( int argc, char *argv[])
{
    // bat ir chay ./ir 1
    // tat ir chay ./ir 0
    int mode;
    mode = std::stoi(argv[1]);
    std::cout<<mode;
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
        std::ofstream file_status_write;
        file_status_write.open(status_wifi_json_path);
        while (!file_status_write) 
        {
            std::cout<<"videostreamer: open status file failed"<<std::endl;
        }
        if(mode==1){
            status["is_person"] = 1;
        }
        else if (mode == 0 )
        {
            status["is_person"] = 0;
        }       
        file_status_write << status << std::endl;
        file_status_write.close(); 
        
    }
    catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl;}       
    close(fd);   

    return 0;
}
