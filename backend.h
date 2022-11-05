#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);

signals:
    void sendToQml_button(int type, int button_id);
    /*type:
     1 -> press
     -1 -> release
     */
    /*id:
     10 -> del
     11 -> ent
     */
    void sendToQml_password(int num); // send number of password * icon to qml

public slots:
    void handle_touch_event(int type, int x, int y);
    /*type:
     1 -> press
     -1 -> release
     */

private:
    int pressing_button_id;
    QString _password;
};

#endif // BACKEND_H
