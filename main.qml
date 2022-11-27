import QtQuick 1.1
//import QtQuick.Control 1.3

Rectangle {
    visible: true
    width: 240
    height: 320
    //title: qsTr("QML Signals and Slots")

    /* Using the Connections Object
     * Establish a connection with the application core object
     * */
    property variant password_num : 0
    property variant button:[]
    property variant button_color: []
    property variant pass_icon: []

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
                button[button_id].x+=5
                button[button_id].y+=2
                button[button_id].width = 50
                //button_color[button_id].brightness = 0.3
            }
            else if (type == -1)
            {
                button[button_id].x-=5
                button[button_id].y-=2
                button[button_id].width = 60
               // button_color[button_id].brightness = 0
            }
        }
        function onSendToQml_password(num)
        {
            password_num = num
            for (var i=0;i<6;i++)
            {
                pass_icon[i].visible = false
            }
            for (var i=0;i<num;i++)
            {
                pass_icon[i].visible = true
            }
        }
        function onSendChangeWindow(type, wrong_left)
        {
            if (type == 0)
            {
                inotify_text_window.visible=false
                notify_window.visible=false
                inotify.text = ""
                button_ok.visible=false
            }
            else if (type == 1)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.x = 40
                inotify.y = 115
                inotify.color = "red"
                inotify.text = "Mật khẩu quá ngắn"
                button_ok.visible = true
            }
            else if (type == 2)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                password_num=0
                inotify.x = 65
                inotify.y = 110
                inotify.color = "red"
                inotify.text = "Mật khẩu sai\nCòn "+wrong_left +" lần thử"
                for (var i=0;i<6;i++)
                {
                    pass_icon[i].visible = false
                }
                button_ok.visible = true
            }
            else if (type ==3)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.x = 45
                inotify.y = 110
                inotify.color = "red"
                inotify.text = "Bạn đã nhập sai\nquá 5 lần\nVui lòng liên hệ lễ\n tân để mở khóa"
            }
            else if (type ==4)
            {
                inotify_text_window.visible=true
                notify_window.visible=true
                inotify.x = 38
                inotify.y = 110
                inotify.color = "green"
                inotify.text = "Mật khẩu chính xác!\n Đang mở cửa"
                button_ok.visible = true
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
        y : 40
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
//        BrightnessContrast {
//              id: button1_color
//              anchors.fill: button1_image
//              source: button1_image
//              brightness: 0
//        }
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
//        BrightnessContrast {
//              id: button2_color
//              anchors.fill: button2_image
//              source: button2_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button3_color
//              anchors.fill: button3_image
//              source: button3_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button4_color
//              anchors.fill: button4_image
//              source: button4_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button5_color
//              anchors.fill: button5_image
//              source: button5_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button6_color
//              anchors.fill: button6_image
//              source: button6_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button7_color
//              anchors.fill: button7_image
//              source: button7_image
//              brightness: 0
//          }
     }

    Item {
        x : 90
        y : 200
        id: button8
        width: 60
        height: 100
        Image {
            id: button8_image
            source: "qrc:/icon/button8.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
//        BrightnessContrast {
//              id: button8_color
//              anchors.fill: button8_image
//              source: button8_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button9_color
//              anchors.fill: button9_image
//              source: button9_image
//              brightness: 0
//          }
     }

    Item {
        x : 15
        y : 250
        id: button_del
        width: 60
        height: 100
        Image {
            id: button_del_image
            source: "qrc:/icon/button_del.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
//        BrightnessContrast {
//              id: button_del_color
//              anchors.fill: button_del_image
//              source: button_del_image
//              brightness: 0
//        }
     }

    Item {
        x : 165
        y : 250
        id: button_ent
        width: 60
        height: 100
        Image {
            id: button_ent_image
            source: "qrc:/icon/button_ent.png"
            sourceSize: Qt.size(parent.width, parent.height)
            smooth: true
            visible: false
        }
//        BrightnessContrast {
//              id: button_ent_color
//              anchors.fill: button_ent_image
//              source: button_ent_image
//              brightness: 0
//          }
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
//        BrightnessContrast {
//              id: button0_color
//              anchors.fill: button0_image
//              source: button0_image
//              brightness: 0
//          }
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
            visible: false
        }
//        BrightnessContrast {
//              id: button_ok_color
//              anchors.fill: button_ok_image
//              source: button_ok_image
//              brightness: 0
//        }
     }
    Image {
        x:5
        y:26
        id: pass1
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:35
        y:26
        id: pass2
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:65
        y:26
        id: pass3
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:95
        y:26
        id: pass4
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:125
        y:26
        id: pass5
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Image {
        x:155
        y:26
        id: pass6
        source: "qrc:/icon/password.png"
        sourceSize: Qt.size(100,100)
        smooth: true
        visible: false
    }
    Component.onCompleted: {
        pass_icon.push(pass1, pass2, pass3, pass4, pass5, pass6)
        button.push(button0, button1, button2, button3, button4, button5, button6, button7, button8, button9, button_del, button_ent, button_ok)
        //button_color.push(button0_color, button1_color, button2_color, button3_color, button4_color, button5_color, button6_color, button7_color, button8_color, button9_color, button_del_color, button_ent_color,button_ok_color)
        button_color.push(button0, button1, button2, button3, button4, button5, button6, button7, button8, button9, button_del, button_ent, button_ok)

    }

}
