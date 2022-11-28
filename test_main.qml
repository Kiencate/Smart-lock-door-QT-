import QtQuick 2.11
//import QtQuick.Controls 2.2

import QtQuick.Window 2.3

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("EyeGaze")

    property var i: 0

    Loader {id: pageLoader}
    MouseArea {
        anchors.fill: parent
        onClicked:
            {

            pageLoader.source = "main.qml"
            if (i == 0)
            {
                camera.stop()
                i=1
            }
            else
            {
                camera.start()
                i=0
            }

            console.log("ok")
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
               visible: true
           }
        }
}
