#include "checkstatus_ir.h"
const char *status_json_check_path = "/home/kiencate/Documents/benzenx_job/Rfid_rc522_i2c_linux/status.json";
CheckStatus::CheckStatus()
{
/*creating the INOTIFY instance*/
  fd = inotify_init();

  /*checking for error*/
  if ( fd < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd = inotify_add_watch( fd, status_json_check_path, IN_MODIFY );
  is_person = false;
}

void CheckStatus::run()
{
    char data_status_json_file[100];
    char data_status_json_file1[100];
    int length, i; // length is length of event read, i is th index of event in list event
    loop:
    i=0;
    length = read( fd, buffer, EVENT_BUF_LEN ); 
    std::cout<<"check status: new change file: ";
    /*checking for error*/
    if ( length < 0 ) {
        perror( "read" );
    }
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
        i += EVENT_SIZE + event->len;
        std::ifstream file_status;
        file_status.open(status_json_check_path);
        while(!file_status)
        {
            qDebug("error while opening status json path");
        }
        try{
            nlohmann::json status = nlohmann::json::parse(file_status);
            file_status.close();
            bool new_person = std::stoi(status["is_person"].dump()) == 1? true:false;
            bool close_door = std::stoi(status["is_closed_door"].dump()) == 1? true:false;
            bool wifi_config = std::stoi(status["wifi_configured"].dump()) == 1? true:false;
            bool face_detect = std::stoi(status["is_face_detected"].dump()) == 1? true:false;
            std::cout<<"checkstatus: person:  "<<new_person;
            if(new_person) 
            {
                is_person = true;                
            }
            else 
            {
                is_person = false;
            }
            emit JsonChangestatus(new_person,wifi_config,close_door,face_detect); 
        }   
        catch(nlohmann::json::parse_error& ex){ std::cerr << "parse error at byte " << ex.byte << std::endl;} 
    }
    
    goto loop;
}
CheckStatus::~CheckStatus()
{
    inotify_rm_watch( fd, wd );

    /*closing the INOTIFY instance*/
    close( fd );
}