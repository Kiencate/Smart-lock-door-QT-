#include "appcore.h"

AppCore::AppCore(QObject* parent) : QObject(parent)
{
}

void AppCore::receiveFromQml(int i)
{
    // We increase the counter and send a signal with its value
    ++m_counter;
    qDebug()<<"ok"<<i;
    emit sendToQml(m_counter);
}
