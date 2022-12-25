#include "checkstatus.h"
CheckStatus::CheckStatus()
{
    qDebug()<<"check stt"<<status_json_path;
/*creating the INOTIFY instance*/
  fd_inotify = inotify_init();

  /*checking for error*/
  if ( fd_inotify < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “/tmp” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd_inotify = inotify_add_watch( fd_inotify, status_json_path, IN_MODIFY );

  int fd_status_json;
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"checkstatus: open status file failed";
    }

    if(flock(fd_status_json,LOCK_SH)==-1)
    {
        qDebug()<<"checkstatus: can't lock status file";
    } 
    status_json_obj = json_object_from_fd(fd_status_json);
    is_person = json_object_get_int(json_object_object_get(status_json_obj,"is_person")) == 1? true:false;
    is_door_closed = json_object_get_int(json_object_object_get(status_json_obj,"is_closed_door")) == 1? true:false;
    close(fd_status_json);
    json_object_put(status_json_obj); 
}

void CheckStatus::run()
{
    int fd_status_json;
    int length, i; // length is length of event read, i is th index of event in list event
    loop:
    i=0;
    length = read( fd_inotify, buffer, EVENT_BUF_LEN ); 
    /*checking for error*/
    if ( length < 0 ) {
        perror( "read" );
    }
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
        i += EVENT_SIZE + event->len;       
    }
    //lock and open file
    if((fd_status_json=open(status_json_path, O_RDWR)) == -1) { 
        qDebug()<<"checkstatus: open status file failed";
    }

    if(flock(fd_status_json,LOCK_SH)==-1)
    {
        qDebug()<<"checkstatus: can't lock status file";
    }  
    status_json_obj = json_object_from_fd(fd_status_json);
    is_person = json_object_get_int(json_object_object_get(status_json_obj,"is_person")) == 1? true:false;
    is_door_closed = json_object_get_int(json_object_object_get(status_json_obj,"is_closed_door")) == 1? true:false;
    wifi_config = json_object_get_int(json_object_object_get(status_json_obj,"wifi_configured")) == 1? true:false;
    face_detect = json_object_get_int(json_object_object_get(status_json_obj,"is_face_detected")) == 1? true:false;
    right_password = json_object_get_int(json_object_object_get(status_json_obj,"is_password_success")) == 1? true:false;
    rfid_success = json_object_get_int(json_object_object_get(status_json_obj,"is_rfid_success")) == 1? true:false;
    is_start_face_detect = json_object_get_int(json_object_object_get(status_json_obj,"start_face_recognize_process")) == 1? true:false;
    is_wifi_connected = json_object_get_int(json_object_object_get(status_json_obj,"wifi_connected")) == 1? true:false;
    is_start_config_wifi = json_object_get_int(json_object_object_get(status_json_obj,"start_config_wifi_qr")) == 1? true:false;
    close(fd_status_json);
    emit JsonChangestatus(is_person, wifi_config, is_door_closed, face_detect, right_password, rfid_success,is_start_face_detect,is_wifi_connected, is_start_config_wifi); 
    json_object_put(status_json_obj); 
    goto loop;
}
CheckStatus::~CheckStatus()
{
    inotify_rm_watch( fd_inotify, wd_inotify );

    /*closing the INOTIFY instance*/
    close( fd_inotify );
}