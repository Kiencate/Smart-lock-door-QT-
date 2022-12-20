#include "check_password_folder.h"
const char *password_folder_path = "../password";
CheckStatusPasswordFolder::CheckStatusPasswordFolder()
{
    qDebug()<<"check stt folder"<<password_folder_path;
/*creating the INOTIFY instance*/
  fd_inotify = inotify_init();

  /*checking for error*/
  if ( fd_inotify < 0 ) {
    perror( "inotify_init" );
  }

  /*adding the “../password” directory into watch list. Here, the suggestion is to validate the existence of the directory before adding into monitoring list.*/
  wd_inotify = inotify_add_watch( fd_inotify, password_folder_path, IN_MODIFY | IN_CREATE | IN_DELETE );
}

void CheckStatusPasswordFolder::run()
{
    int length, i; // length is length of event read, i is th index of event in list event
    loop:
    i=0;
    length = read( fd_inotify, buffer, EVENT_BUF_LEN ); 
    qDebug()<<"new pass change";
    /*checking for error*/
    if ( length < 0 ) {
        perror( "read" );
    }
    while ( i < length ) {     
        struct inotify_event *event = ( struct inotify_event * ) &buffer[ i ];  
        i += EVENT_SIZE + event->len;       
    }
    //lock and open file
    emit PasswordFolderChange();

    goto loop;
}
CheckStatusPasswordFolder::~CheckStatusPasswordFolder()
{
    inotify_rm_watch( fd_inotify, wd_inotify );

    /*closing the INOTIFY instance*/
    close( fd_inotify );
}