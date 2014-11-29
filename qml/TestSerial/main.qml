import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
Rectangle{
    width: 800
    height: 600
    color: "lightblue"
    Settings{
        id:settingwindow
        visible: false
    }

    Column{
        anchors.fill: parent
        spacing: 50
        Row{
            spacing: 50
            Button{
                width: 60
                text: "Open"
                onClicked: {
                    settingwindow.visible=true//使设置窗口可见，通过设置串口的apply按钮触发的serialtest.openAndSetPort函数打开和设置串口
                }
            }
            Button{
                width: 60
                text: "Close"
                onClicked: {
                    serialtest.closePort()//关闭串口
                    Qt.quit()
                }
            }
        }

        Grid{
            rows:2
            columns:4
            rowSpacing: 20
            columnSpacing: 40

            Label{
                height: 25
                text: "Send Data : "
                verticalAlignment :Text.AlignVCenter
            }

            TextField {
                id: textInput1
                width: 300
                height: 25
                placeholderText: qsTr("Send Data")
                font.pixelSize: 12
            }
            Label{
                height: 25
                text: "Number of Send Data: "+serialtest.sendnumber//显示发送数据计数
                verticalAlignment :Text.AlignVCenter
            }
            Button{
                id:sendData
                width: 60
                text: "Send"
                onClicked: {
                    serialtest.sendto(textInput1.text);//触发发送数据函数
                }
            }


            Label{
                height: 25
                text: "Receive Data : "
                verticalAlignment :Text.AlignVCenter
            }
            Rectangle{
                height: 300
                width: 300
                color: "lightgreen"
                radius: 10
                Label{
                    anchors.fill: parent
                    id: textreceive
                    font.pixelSize: 12
                    text:serialtest.receivedata
                }
            }

            Label{
                height: 25
                text: "Number of receive Data: "+serialtest.receivenumber//显示接收数据计数
                verticalAlignment :Text.AlignVCenter
            }
            Button{
                width: 60
                text: "Clear"
                onClicked: {//清空接收数据显示，将数据计数清零
                    serialtest.receivedata=""
                    serialtest.sendnumber="0"
                    serialtest.receivenumber="0"
                    serialtest.clearnumber();
                }
            }

        }

    }
}


