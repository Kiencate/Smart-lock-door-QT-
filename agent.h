#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QtDBus/QDBusObjectPath>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include "serversocket.h"

class Agent : public QObject
{
    Q_OBJECT

public:
    Agent();
    ~Agent();
    QString getDeviceName();
    void turnOffBluetooth();
private:
    QDBusInterface *agentManager;
    QDBusInterface *agent;
    QDBusInterface *agentManagerprops;
};



#endif // AGENT_H
