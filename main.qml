import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0

Window {
    visible: true
    width: 240
    height: 320
    title: qsTr("QML Signals and Slots")

    /* Using the Connections Object
     * Establish a connection with the application core object
     * */
    property var button:[]
    property var color: []
    Connections {
        target: backEnd // Specify the target to connect
        /* Declare and implement the function as a parameter
         * object and with a name similar to the name of the signal
         * The difference is that we add on at the beginning and then write
         * capitalized
         * */
        function onSendToQml_button(type, button_id) {
            if (type == 1)
            {
                button[button_id].x+=10
                button[button_id].y+=5
                button[button_id].width = 190
                color[button_id].brightness = 0.3
            }
            else if (type == -1)
            {
                button[button_id].x-=10
                button[button_id].y-=5
                button[button_id].width = 210
                color[button_id].brightness = 0
            }
        }

        function onSendChangeWindow(type) {
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
                inotify.text = "Đang kết nối"
                button_ok.visible=false
                loading_icon.visible = true
            }
            else if (type == 2)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.text = "Kết nối thành công, đang nhận wifi"
                button_ok.visible=false
                loading_icon.visible = true
            }
            else if (type == 3)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.text = "Nhận wifi thành công"
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
        BrightnessContrast {
              id: button_qrcode_color
              anchors.fill: button_qrcode_image
              source: button_qrcode_image
              brightness: 0
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
        BrightnessContrast {
              id: button_bluetooth_color
              anchors.fill: button_bluetooth_image
              source: button_bluetooth_image
              brightness: 0
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
        color: "red"
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
        BrightnessContrast {
              id: button_ok_color
              anchors.fill: button_ok_image
              source: button_ok_image
              brightness: 0
        }
     }
    Component.onCompleted: {
        button.push(button_qrcode, button_bluetooth, button_ok)
        color.push(button_qrcode_color, button_bluetooth_color, button_ok_color)
    }

}
