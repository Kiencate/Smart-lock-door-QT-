#ifndef CHECKSTATUS_H
#define CHECKSTATUS_H
#include <QObject>
#include <QDebug>
#include <QThread>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <fcntl.h>
#include <sys/file.h>
#include <json.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


class CheckStatus : public QThread
{
    Q_OBJECT

public:
    CheckStatus();
    ~CheckStatus();
    // variable for fields in status.json
    bool is_person;
    bool is_door_closed;
    bool wifi_config ;
    bool face_detect ;
    bool right_password ;
    bool rfid_success ;
    bool is_start_face_detect; //true if started face detect
signals:
    void JsonChangestatus(bool _is_person, bool _is_wifi_configured, bool _is_door_closed, bool _is_face_detected, bool right_password, bool is_rfid_success,bool is_start_face_detect);
    // emit when status.json is changed
protected:
    void run();
private:
    int fd_inotify; //inotify instance
    int wd_inotify; //inotify watch
    struct json_object *status_json_obj; //json object for status json file
    char buffer[EVENT_BUF_LEN]; // save event with file 
};

#endif // CHECKSTATUS_H