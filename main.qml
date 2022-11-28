import QtQuick 2.15
import QtQuick.Window 2.15

Window {
    visible: true
    width: 240
    height: 320
    title: qsTr("QML Signals and Slots")

    /* Using the Connections Object
     * Establish a connection with the application core object
     * */
    property var button:[]
    Connections {
        target: backEnd // Specify the target to connect
        /* Declare and implement the function as a parameter
         * object and with a name similar to the name of the signal
         * The difference is that we add on at the beginning and then write
         * capitalized
         * */
        onSendToQml_button:
        {
            if (type == 1)
            {
                if (button_id == 2)
                {
                    button[button_id].x+=5
                    button[button_id].y+=2
                    button[button_id].width = 50
                }
                else
                {
                    button[button_id].x+=10
                    button[button_id].y+=5
                    button[button_id].width = 190
                }

            }
            else if (type == -1)
            {
                if (button_id == 2)
                {
                    button[button_id].x-=5
                    button[button_id].y-=2
                    button[button_id].width = 60
                }
                else
                {
                    button[button_id].x-=10
                    button[button_id].y-=5
                    button[button_id].width =210
                }

            }
        }

        onSendChangeWindow:
        {
            if (type == 0)
            {
                inotify_text_window.visible=false
                notify_window.visible=false
                inotify.text = ""
                button_ok.visible=false
                loading_icon.visible = false
            }
            else if (type == 1)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.x = 28
                inotify.text = "Vui lòng kết nối\nvới thiết bị bluetooth \ncó tên '"+name_device_bluetooth+"'"
                inotify.color= "steelblue"
                button_ok.visible=false
                loading_icon.visible = true
            }
            else if (type == 2)
            {
                inotify.x = 40
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.text = "Kết nối thành công\n đang nhận wifi"
                button_ok.visible=false
                loading_icon.visible = true
            }
            else if (type == 3)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.x = 74
                inotify.text = "Cài đặt Wifi \nthành công"
                inotify.color= "Green"
                button_ok.visible=true
                loading_icon.visible = false
            }
        }
    }

    MouseArea {

        anchors.fill: parent
           hoverEnabled: true
           acceptedButtons: Qt.LeftButton | Qt.RightButton
           onPressed:
           {
               backEnd.handle_touch_event(1,mouseX,mouseY);
           }
           onReleased:
           {
               backEnd.handle_touch_event(-1,mouseX,mouseY);
           }
       }


    Item {
        x : 15
        y : 50
        id: button_qrcode
        width: 210
        height: 100
        Image {
            id: button_qrcode_image
            source: "qrc:/icon/QRcode.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
     }

    Item {
        x : 10
        y : 160
        id: button_bluetooth
        width: 300
        height: 100
        Image {
            id: button_bluetooth_image
            source: "qrc:/icon/Bluetooth.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
     }

    Image {
        x:10
        y:30
        id: notify_window
        source: "qrc:/icon/notify.png"
        sourceSize: Qt.size(220,250)
        smooth: true
        visible: false
    }
    Text {
        x : 40
        y : 110
        id: inotify
        text: ""
        font.family: "Vietnamese"
        font.pointSize: 11
        font.bold: true
        color: "steelblue"
        horizontalAlignment: Text.AlignHCenter
    }
    Text {
        x : 100
        y : 77
        id: inotify_text_window
        text: "Thông báo"
        font.family: "Vietnamese"
        font.pointSize: 13
        font.bold: true
        visible: false
        color: "steelblue"
    }
    Item {
        x : 100
        y : 160
        id: loading_icon_item
        width: 20
        height: 20
        AnimatedImage {
            id: loading_icon
            source: "qrc:/icon/loading.gif"
            visible: false
        }
     }
    Item {
        x : 95
        y : 150
        id: button_ok
        width: 60
        height: 100
        visible: false
        Image {
            id: button_ok_image
            source: "qrc:/icon/button_ok.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: true
        }
     }
    Component.onCompleted: {
        button.push(button_qrcode, button_bluetooth, button_ok)
    }

}
