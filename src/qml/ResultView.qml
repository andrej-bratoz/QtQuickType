import QtQuick
import QtQuick.Layouts 2.1
import QtQuick.Controls 2.15

Frame {
    id: root
    width: parent.width * 0.95
    anchors.centerIn: parent
    Layout.fillWidth: true
    Layout.fillHeight: true
    property var model
    property var viewModel
    
    ListView {
        id: listView
        model: root.model
        implicitWidth: parent.width * 0.95
        anchors.centerIn: parent
        currentIndex : quickType.selectedIndex
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
                Connections{
                    
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
        highlightFollowsCurrentItem: true
        onCurrentItemChanged:  console.log("SELECTED_INDEX " + listView.currentIndex)
    }
}
