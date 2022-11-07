#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QDebug>
#include <QFile>

class BackEnd : public QObject
{
    Q_OBJECT
public:
    explicit BackEnd(QObject *parent = nullptr);

signals:
    void sendToQml_button(int type, int button_id);
    /* signal for set button animation in frontend
     * type:
     *      1 -> press
     *      -1 -> release
     *
     *id:
     *      0-9 -> button 0-9
     *      10 -> del
     *      11 -> ent
     */
    void sendToQml_password(int num);
    /*signal for set icon * in frontend
     * num: number of password character (0-6)
     */
    void sendNotify(int type);
    /* signal for send Notify to user in frontend
     * type:
     *      1 -> password short
     *      2 -> wrong password
     */
public slots:
    void handle_touch_event(int type, int x, int y);
    /* slot handle event from ili9341
     * type:
     *      1 -> press
     *      -1 -> release
     * x,y:
     *      x,y position of touch in ili9341
     */

private:
    int pressing_button_id;
    QString _password; //password user entered
    QString _right_password; // right password
};

#endif // BACKEND_H
