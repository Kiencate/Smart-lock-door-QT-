#include "agent.h"

Agent::Agent() {
    agentManager = new QDBusInterface("org.bluez","/org/bluez","org.bluez.AgentManager1",QDBusConnection::systemBus(),this);
    agent = new QDBusInterface("org.bluez","/org/bluez/agent","org.bluez.Agent1",QDBusConnection::systemBus(),this);
    agentManagerprops = new QDBusInterface("org.bluez","/org/bluez/hci0","org.bluez.Adapter1",QDBusConnection::systemBus(),this);
    agentManagerprops->setProperty("Powered",true);
    agentManagerprops->setProperty("Discoverable",true);
    agentManagerprops->setProperty("Pairable",true);
    agentManagerprops->setProperty("DiscoverableTimeout",quint32(0));
    agentManagerprops->setProperty("PairableTimeout",quint32(0));

    QDBusReply<void> reply;

    reply = agentManager->call("RegisterAgent", QVariant::fromValue(QDBusObjectPath("/pairing/agent")), "KeyboardDisplay");
    if (!reply.isValid()) {
        qDebug() << reply.error().message() << reply.error().name();
    }

    reply = agentManager->call("RequestDefaultAgent", QVariant::fromValue(QDBusObjectPath("/pairing/agent")));
    if (!reply.isValid()) {
        qDebug() << reply.error().message() << reply.error().name();
    }


}

QString Agent::getDeviceName()
{
    agentManagerprops = new QDBusInterface("org.bluez","/org/bluez/hci0","org.bluez.Adapter1",QDBusConnection::systemBus(),this);
    return agentManagerprops->property("Name").toString();
}

Agent::~Agent()
{
    qDebug()<<"ok";
    agentManagerprops = new QDBusInterface("org.bluez","/org/bluez/hci0","org.bluez.Adapter1",QDBusConnection::systemBus(),this);
    agentManagerprops->setProperty("Powered",false);
    QDBusReply<void> reply;
    reply = agentManager->call("UnregisterAgent", QVariant::fromValue(QDBusObjectPath("/pairing/agent")));
    if (!reply.isValid()) {
        qDebug() << reply.error().message() << reply.error().name();
    }
}
