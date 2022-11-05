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
    Connections {
        target: appCore // Specify the target to connect
        /* Declare and implement the function as a parameter
         * object and with a name similar to the name of the signal
         * The difference is that we add on at the beginning and then write
         * capitalized
         * */
        function onSendToQml(count) {
//            labelCount.text = count // Set the counter to the text label
        }
    }

    MouseArea {

        anchors.fill: parent
           hoverEnabled: true
           acceptedButtons: Qt.LeftButton | Qt.RightButton
           onPressed:
           {
               appCore.receiveFromQml(mouseX);
//               button_1_1.x = 110
//               button_1_1.y = 110
//               console.log("mouse pressed the area ",mouseX )
//               button_1_1.width = 80
//               button_1_1.height = 80
//               button1.brightness = 0.3
           }
           onReleased:
           {
//               button_1_1.x = 100
//               button_1_1.y = 100
//               button_1_1.width = 100
//               button_1_1.height = 100
//               button1.brightness=0
//               console.log("mouse release the area ",button_1_1.width)
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
            visible: false
        }
        BrightnessContrast {
              id: password_bolder_color
              anchors.fill: password_bolder_image
              source: password_bolder_image
              brightness: 0
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



}
