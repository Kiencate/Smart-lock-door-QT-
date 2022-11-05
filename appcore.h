#ifndef APPCORE_H
#define APPCORE_H

#include <QObject>
#include <QDebug>

class AppCore : public QObject
{
    Q_OBJECT
public:
    explicit AppCore(QObject *parent = nullptr);

signals:
    void sendToQml(int count);

public slots:
    void receiveFromQml(int i);

private:
    int m_counter {0};
};

#endif // APPCORE_H
