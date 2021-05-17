import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts
import QtQuick.Controls


ApplicationWindow {
    visible: true
    width: 400
    height: 300
    color: "#FF252525"
    title: qsTr("Qt QuickType")


    GridLayout {
        anchors.fill: parent
        Layout.fillHeight: true
        Layout.fillWidth: true
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
                width: parent.width *0.9
                text:backEnd.command
                onTextChanged: {backEnd.command = text}
                anchors.centerIn: r1
            }
        }

        Rectangle {
            id: r2
            Layout.row: 1
            color: "transparent"
            Layout.fillWidth: true
            anchors.top: r1.bottom
            height: 30
            Label {
                width: parent.width *0.9
                anchors.centerIn: r2
                text : backEnd.command
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
                height: parent.height
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: backEnd.options
            }
        }


    }
}
