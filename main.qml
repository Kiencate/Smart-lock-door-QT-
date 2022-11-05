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


    Component.onCompleted: {
        button.push(button_qrcode, button_bluetooth)
        color.push(button_qrcode_color, button_bluetooth_color)
    }

}
