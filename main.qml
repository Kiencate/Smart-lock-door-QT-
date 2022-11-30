import QtQuick 2.11
import QtQuick.Window 2.11
//import QtQuick.Controls 2.0

Window {
    width: 240
    height: 320
    visible: true
    title: qsTr("OpencvToQml")

    Rectangle{
        id: imageRect
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 240
        height: 320

        color: "transparent"
        border.color: "black"
        border.width: 3

        Image{

            id: opencvImage
            anchors.fill: parent
            fillMode: Image.Stretch
            property bool counter: false
            visible: false
            source: "image://live/"
            asynchronous: false
            cache: false

            function reload()
            {
            
                counter = !counter
                source = "image://live/" + counter
            }

        }
    }

   MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            VideoStreamer.openVideoCamera("")
            opencvImage.visible = true
        }
    }


  

    Connections{
        target: liveImageProvider

        onImageChanged:
        {
            opencvImage.reload()
        }

    }
}
