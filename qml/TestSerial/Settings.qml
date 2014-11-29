import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Window 2.0
Window{
    id:setwindow
    width: 300
    height: 300
    Column{
        id: maincolumn
        anchors.fill: parent
        spacing: 10

        Rectangle{
            anchors.horizontalCenter: parent.horizontalCenter
            height: 1
            width: parent.width
        }
        Label{
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Set Serial Port"
            font.pointSize:12
            font.bold: true

        }
        Grid{
            id:selectgrid
            anchors.horizontalCenter: parent.horizontalCenter
            rows:6
            columns: 2
            columnSpacing: 20
            rowSpacing: 10
            Label{
                id:selectlabel
                height: 20
                text: "PortName:"
                font.pointSize:9
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id :firstcombo
                width: maincolumn.width/2
                currentIndex: 2
                model: [ "COM1", "COM2", "COM3" ,"COM4" ,"COM5" ,"COM6" ]
            }
            Label{
                text: "BaudRate:"
                height: 20
                font.pointSize:selectlabel.font.pointSize
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id: baudRate
                width:firstcombo.width
                currentIndex: 0
                model: [ "9600", "19200", "38400","115200" ]
            }
            Label{
                text: "Data bits:"
                height: 20
                font.pointSize:selectlabel.font.pointSize
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id:dataBits
                width:firstcombo.width
                currentIndex: 3
                model: [ "5", "6", "7", "8" ]
            }
            Label{
                text: "Parity:"
                height: 20
                font.pointSize:selectlabel.font.pointSize
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id:parity
                width:firstcombo.width
                currentIndex: 0
                model: [ "None", "Even", "Odd", "Mark", "Space" ]
            }
            Label{
                height: 20
                text: "Stop bits:"
                font.pointSize:selectlabel.font.pointSize
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id:stopBits
                width:firstcombo.width
                currentIndex: 0
                model: [ "1", "1.5", "2" ]
            }
            Label{
                height: 20
                text: "Flow control:"
                font.pointSize:selectlabel.font.pointSize
                horizontalAlignment : Text.AlignHCenter
                verticalAlignment :Text.AlignVCenter
            }
            ComboBox {
                id:flowControl
                currentIndex: 0
                width:firstcombo.width
                model: [ "None", "RTS/CTS", "XON/XOFF" ]
            }
        }
        Button{
            width: 60
            text: "Apply"
            anchors.horizontalCenter: parent.horizontalCenter
            onClicked: {
                serialtest.openAndSetPort(firstcombo.currentIndex,baudRate.currentIndex,dataBits.currentIndex
                                           ,parity.currentIndex,stopBits.currentIndex,flowControl.currentIndex)
                //触发此函数，由combobox控件的currentIndex作为函数变量，（所有combobox的model值和顺序都和serialtest.openAndSetPort一致，这样就可以通过传递index来获取当前设置信息）
                setwindow.visible=false
            }
        }

    }

}

