#ifndef SERVERSOCKET_H
#define SERVERSOCKET_H
#include <QObject>
#include <QDebug>
#include <QThread>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <QFile>



class ServerSocket : public QThread
{
    Q_OBJECT

public:
    ServerSocket();
signals:
    void Receive_wifi_success();
    /*
    * emit to backend if config wifi success
    */
protected:
    void run();
};

#endif // SERVERSOCKET_H
