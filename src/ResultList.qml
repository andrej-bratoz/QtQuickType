import QtQuick 2.0
import QtQuick.Layouts 2.1
import QtQuick.Controls 2.15

Item {
    id: root
    width: parent.width
    Layout.fillWidth: true
    Layout.fillHeight: true
    property var model
    property var viewModel
    
    ListView {
        id: listView
        model: root.model
        implicitWidth: parent.width * 0.95
        anchors.centerIn: parent
        implicitHeight: 200
        topMargin: 5
        clip: true
        delegate: Component {
            Item {
                width: listView.width
                height: 20
                Column {
                    Text { 
                        text: modelData
                        color : "white"    
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listView.currentIndex = index
                    }
                    onDoubleClicked: {
                        listView.currentIndex = index
                        root.viewModel.selectedIndex = index
                    }
                }
            }
        }
        highlight: Rectangle {
            color: 'grey'
            Text {
                anchors.centerIn: parent
            }
        }
        focus: true
        onCurrentItemChanged: console.log(listView.currentIndex)
    }
}
