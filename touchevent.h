#ifndef TOUCH_EVENT_H
#define TOUCH_EVENT_H
#include <QObject>
#include <QDebug>
#include <QThread>
#include <time.h>
#include "json.h"
#include <fcntl.h>
#include <sys/file.h>
#include "signal.h"
#include "unistd.h"

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
    struct  json_object *touch_json_obj; 
    int type,x,y;
};

#endif // TOUCH_EVENT_H