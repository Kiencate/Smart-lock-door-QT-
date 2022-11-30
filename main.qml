import QtQuick 2.12
import QtQuick.Window 2.12
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
        width: 800
        height: 600

        color: "transparent"
        border.color: "black"
        border.width: 3

        Image{
            width:240
            height:320
            id: opencvImage
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            property bool counter: false
            visible: false
            source: "image://live/image"
            asynchronous: false
            cache: false

            function reload()
            {
                counter = !counter
                source = "image://live/image?id=" + counter
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

        function onImageChanged()
        {
            opencvImage.reload()
        }

    }
}
