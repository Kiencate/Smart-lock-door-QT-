#include "dbusadaptor.h"

MyQDusAdaptor::MyQDusAdaptor(QObject* parent) : QDBusAbstractAdaptor(parent) {

    setAutoRelaySignals(true);
//    check_connect_timer = new QTimer(this);
//    connect(check_connect_timer, &QTimer::timeout,this, &MyQDusAdaptor::check_connect);
//    check_connect_timer->start(1000);
}

MyQDusAdaptor::~MyQDusAdaptor() {
    deviceprop->setProperty("Trusted",false);
    QDBusInterface *agentManagerprops = new QDBusInterface("org.bluez","/org/bluez/hci0","org.bluez.Adapter1",QDBusConnection::systemBus(),this);
    agentManagerprops->call("RemoveDevice",device);
}
void MyQDusAdaptor::RequestConfirmation(const QDBusObjectPath& in0 ,quint32 passkey)
{
    device = in0;
    deviceprop = new QDBusInterface("org.bluez",in0.path(),"org.bluez.Device1",QDBusConnection::systemBus(),this);
    deviceprop->setProperty("Trusted",true);
    emit Connect_success();
    return;
}

