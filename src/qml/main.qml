import QtQuick 2.0
import QtQuick.Window 
import QtQuick.Layouts
import QtQuick.Controls 2.0


ApplicationWindow {
    visible: true
    width: 600
    height: 300
    color: "#FF252525"
    title: qsTr("Qt QuickType")


    GridLayout {
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.topMargin : 15
        columns: 1
        rows: 3

        Rectangle {
            id: r1
            color: "transparent"
            Layout.row: 0
            anchors.top: parent.top
            Layout.fillWidth: true
            height: 30
            TextField {
                width: parent.width *0.95
                text:backEnd.command
                font.family: "Consolas"
                onTextChanged: {backEnd.command = text}
                anchors.centerIn: r1
                Keys.onDownPressed : {
                    backEnd.selectedIndex++
                }
                Keys.onUpPressed : {
                    backEnd.selectedIndex--
                }
                 Keys.onReturnPressed : {
                    backEnd.ExecCurrentCmd();
                }
                
            }
        }

        Item {
            id: r2
            Layout.row: 1
            Layout.fillWidth: true
            anchors.top: r1.bottom
            height: 30
            Label {
                width: parent.width *0.95
                anchors.centerIn: r2
                text : backEnd.currentProcess
                color : "white"
            }
        }

        Rectangle {
            id: r3
            Layout.row: 2
            anchors.top: r2.bottom
            color: "transparent"
            Layout.fillWidth: true
            Layout.fillHeight: true
            ResultList {
                id: resultList
                height: parent.height
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: backEnd.options
                viewModel : backEnd
            }
        }


    }
}
