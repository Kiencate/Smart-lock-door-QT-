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
#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


class CheckStatus : public QThread
{
    Q_OBJECT

public:
    CheckStatus();
    ~CheckStatus();
    bool is_person;
signals:
    void JsonChangestatus(bool _is_person, bool _is_wifi_configured, bool _is_door_closed, bool _is_face_detected);
protected:
    void run();
private:
    int fd; //inotify instance
    int wd; //inotify watch
    char buffer[EVENT_BUF_LEN]; // save event with file 
};

#endif // CHECKSTATUS_H