#ifndef CHECKSTATUSFOLDER_H
#define CHECKSTATUSFOLDER_H
#include <QObject>
#include <QDebug>
#include <QThread>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>
#include <json.h>

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )


class CheckStatusPasswordFolder : public QThread
{
    Q_OBJECT

public:
    CheckStatusPasswordFolder();
    ~CheckStatusPasswordFolder();
signals:
    void PasswordFolderChange();
    // emit when status.json is changed
protected:
    void run();
private:
    int fd_inotify; //inotify instance
    int wd_inotify; //inotify watch
    char buffer[EVENT_BUF_LEN]; // save event with file 
};

#endif // CHECKSTATUSFOLDER_H