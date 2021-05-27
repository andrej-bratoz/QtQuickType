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
                text:quickType.command
                font.family: "Consolas"
                anchors.centerIn: r1
                onTextChanged: {quickType.command = text}
                Keys.onDownPressed : {quickType.selectedIndex++}
                Keys.onUpPressed : {quickType.selectedIndex--}
                Keys.onReturnPressed : {quickType.execCurrentCmd();}   
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
                text : quickType.currentProcess
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
            ResultView {
                id: resultList
                height: parent.height
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: quickType.options
                viewModel : quickType
            }
        }
    }
}
