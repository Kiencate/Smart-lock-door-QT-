#ifndef TOUCH_EVENT_H
#define TOUCH_EVENT_H
#include <QObject>
#include <QDebug>
#include <QThread>
#include <time.h>
#include <fcntl.h>
#include <sys/file.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/input.h>

#define MOUSEFILE "/dev/input/event0"
class TouchEvent : public QThread
{
    Q_OBJECT

public:
    TouchEvent();
    ~TouchEvent();
signals:
    void new_touch_event(int type, int x, int y);
    /*type:
     1 -> press
     -1 -> release
     */
protected:
    void run();
private:    
    int fd_dev_event;
    struct input_event event_dev;
    int type, x, y, x_pre, y_pre;
    bool is_new_event_touch, is_new_event_release;
};

#endif // TOUCH_EVENT_H
