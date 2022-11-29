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
    void sendChangeWindow(int type, int wrong_left);
    /* signal for send Notify to user in frontend
     * type:
     *      0 -> in password window
     *      1 -> password short
     *      2 -> wrong password
     *      3 -> wrong password over 5 times
     *      4 -> right password, openning window
     *      5 -> in face recognition window
     * wrong_left:
     *      so lan sai con lai(toi da 5)
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
    int window_type;
    /* window_type
     * 0 : password
     * 1 : notify
     * 2 : start main window
     * 3 : in face recognition
     */
    int wrong_left; // so lan sai con lai (toi da 5 lan)
    int pressing_button_id;
    bool is_right_password;
    QString _password; //password user entered
    QString _right_password1; // right password
    QString _right_password2; // right password
};

#endif // BACKEND_H
