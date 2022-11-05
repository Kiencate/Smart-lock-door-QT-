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
    property var press: []
    property var release: []
    property var pass_icon: []
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
//                press[button_id]()
                button[button_id].x+=5
                button[button_id].y+=5
                button[button_id].width = 50
            }
            else if (type == -1)
            {
                release[button_id]()
            }
        }
        function onSendToQml_password(num)
        {
            for (var i=0;i<6;i++)
            {
                pass_icon[i].visible = false
            }
            for (var i=0;i<num;i++)
            {
                pass_icon[i].visible = true
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
        y : 30
        id: password_bolder
        width: 210
        height: 100
        Image {
            id: password_bolder_image
            source: "qrc:/icon/password_bolder.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: true
        }
        function press(){

            x+=20
        }
     }

    Item {
        x : 15
        y : 100
        id: button1
        width: 60
        height: 100
        Image {
            id: button1_image
            source: "qrc:/icon/button1.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button1_color
              anchors.fill: button1_image
              source: button1_image
              brightness: 0
        }
        function press(){
            x = 20
            y = 105
            width = 50
            button1_color.brightness = 0.3
        }
        function release(){
            x = 15
            y = 100
            width = 60
            button1_color.brightness = 0
        }
     }

    Item {
        x : 90
        y : 100
        id: button2
        width: 60
        height: 100
        Image {
            id: button2_image
            source: "qrc:/icon/button2.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button2_color
              anchors.fill: button2_image
              source: button2_image
              brightness: 0
          }
     }

    Item {
        x : 165
        y : 100
        id: button3
        width: 60
        height: 100
        Image {
            id: button3_image
            source: "qrc:/icon/button3.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button3_color
              anchors.fill: button3_image
              source: button3_image
              brightness: 0
          }
     }

    Item {
        x : 15
        y : 150
        id: button4
        width: 60
        height: 100
        Image {
            id: button4_image
            source: "qrc:/icon/button4.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button4_color
              anchors.fill: button4_image
              source: button4_image
              brightness: 0
          }
     }

    Item {
        x : 90
        y : 150
        id: button5
        width: 60
        height: 100
        Image {
            id: button5_image
            source: "qrc:/icon/button5.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button5_color
              anchors.fill: button5_image
              source: button5_image
              brightness: 0
          }
     }

    Item {
        x : 165
        y : 150
        id: button6
        width: 60
        height: 100
        Image {
            id: button6_image
            source: "qrc:/icon/button6.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button6_color
              anchors.fill: button6_image
              source: button6_image
              brightness: 0
          }
     }

    Item {
        x : 15
        y : 200
        id: button7
        width: 60
        height: 100
        Image {
            id: button7_image
            source: "qrc:/icon/button7.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button7_color
              anchors.fill: button7_image
              source: button7_image
              brightness: 0
          }
     }

    Item {
        x : 90
        y : 200
        id: button8
        width: 60
        height: 100
        Image {
            id: button8_image
            source: "qrc:/icon/button7.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button8_color
              anchors.fill: button8_image
              source: button8_image
              brightness: 0
          }
     }

    Item {
        x : 165
        y : 200
        id: button9
        width: 60
        height: 100
        Image {
            id: button9_image
            source: "qrc:/icon/button9.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button9_color
              anchors.fill: button9_image
              source: button9_image
              brightness: 0
          }
     }

    Item {
        x : 10
        y : 251
        id: button_del
        width: 72
        height: 100
        Image {
            id: button_del_image
            source: "qrc:/icon/button_del.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button_del_color
              anchors.fill: button_del_image
              source: button_del_image
              brightness: 0
        }
        function press(){
            x = 15
            y = 256
            width = 62
            button_del_color.brightness = 0.3
        }
        function release(){
            x = 10
            y = 251
            width = 72
            button_del_color.brightness = 0
        }
     }

    Item {
        x : 160
        y : 251
        id: button_ent
        width: 72
        height: 100
        Image {
            id: button_ent_image
            source: "qrc:/icon/button_ent.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button_ent_color
              anchors.fill: button_ent_image
              source: button_ent_image
              brightness: 0
          }
     }

    Item {
        x : 90
        y : 250
        id: button0
        width: 60
        height: 100
        Image {
            id: button0_image
            source: "qrc:/icon/button0.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button0_color
              anchors.fill: button0_image
              source: button0_image
              brightness: 0
          }
     }
    Image {
        x:5
        y:16
        id: pass1
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:35
        y:16
        id: pass2
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:65
        y:16
        id: pass3
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:95
        y:16
        id: pass4
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:125
        y:16
        id: pass5
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:155
        y:16
        id: pass6
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Component.onCompleted: {
        press.push(button0.press, button1.press, button2.press, button3.press, button4.press, button5.press, button6.press, button7.press, button8.press, button9.press, button_del.press, button_ent.press)
        release.push(button0.release, button1.release, button2.release, button3.release, button4.release, button5.release, button6.release, button7.release, button8.release, button9.release, button_del.release, button_ent.release)
        pass_icon.push(pass1, pass2, pass3, pass4, pass5, pass6)
        button.push(button0, button1, button2, button3, button4, button5, button6, button7, button8, button9, button_del, button_ent)
        console.log(pass_icon)
    }

}
