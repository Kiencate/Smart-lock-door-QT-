import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0



Window {
    visible: true
    width: 640
    height: 480
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
        onSendToQml: {
            labelCount.text = count1 // Set the counter to the text label
        }
    }

    MouseArea {

        anchors.fill: parent
           hoverEnabled: true
           acceptedButtons: Qt.LeftButton | Qt.RightButton
           onPressed:
           {
               button_1_1.x = 110
               button_1_1.y = 110
               console.log("mouse pressed the area ",mouseX )
               button_1_1.width = 280
               button_1_1.height = 280
               button1.brightness = 0.3
           }
           onReleased:
           {
               button_1_1.x = 100
               button_1_1.y = 100
               button_1_1.width = 300
               button_1_1.height = 300
               button1.brightness=0
               console.log("mouse release the area ",mouseX)
           }
       }
    Label {
        id: labelCount
        text: "0"

        anchors.bottom: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 15
    }

    Item {
        x : 100
        y : 100
        id: button_1_1
        width: 300
        height: 300
        Image {
            id: bug
            source: "button.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
        BrightnessContrast {
              id: button1
              anchors.fill: bug
              source: bug
              brightness: 0/*1*/
//              contrast: 0.5
          }
     }



}
