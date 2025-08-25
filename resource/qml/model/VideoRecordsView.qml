import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Utils 1.0
import App 1.0  

Rectangle {
    id: root
    anchors.fill: parent

    radius: 14
    color: "#99222222"
    border.width: 1
    border.color: "#66FFFFFF"

    property int colSizeW: 100
    property int colModifiedW: 240

    signal accepted(string filePath, string fileName, var fileSize, var fileModified)
    signal closed()

    RecordModel {
        id: recordModel
        folder: Utils.recordsFolderPath()
    }

    function formatSize(bytes) {
        const units = ["B","KB","MB","GB","TB"]; let i = 0; let n = Number(bytes);
        while (n >= 1024 && i < units.length - 1) { n /= 1024; ++i; }
        return (i === 0 ? n : n.toFixed(1)) + " " + units[i];
    }
    function formatDate(dt) { return dt.toLocaleString(Qt.locale()); }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 10

        RowLayout {
            Layout.fillWidth: true
            spacing: 8
            Label { text: "Records"; font.pixelSize: 18; color: "white" }
            Label {
                Layout.fillWidth: true
                text: recordModel.folder
                color: "#AAAAAA"
                elide: Text.ElideLeft
                horizontalAlignment: Text.AlignRight
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.leftMargin: 4
            Layout.rightMargin: 4
            spacing: 16

            Label {                     
                text: "Name"
                color: "#DDDDDD"
                Layout.fillWidth: true
            }
            Label {                    
                text: "Size"
                color: "#DDDDDD"
                Layout.preferredWidth: root.colSizeW
                horizontalAlignment: Text.AlignRight
            }
            Label {                    
                text: "Modified"
                color: "#DDDDDD"
                Layout.preferredWidth: root.colModifiedW
                horizontalAlignment: Text.AlignRight
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            radius: 8
            color: "#33222222"
            clip: true

            Label {
                anchors.centerIn: parent
                visible: list.count === 0
                text: "No recordings found in:\n" + recordModel.folder
                color: "#BBBBBB"
                horizontalAlignment: Text.AlignHCenter
                z: 1
            }

            ListView {
                id: list
                anchors.fill: parent
                model: recordModel
                clip: true
                currentIndex: -1
                keyNavigationWraps: true
                focus: true

                highlight: Rectangle { color: "#22336699"; radius: 6 }
                highlightFollowsCurrentItem: true

                ScrollBar.vertical: ScrollBar { }

                delegate: Rectangle {
                    id: row

                    required property string fileName
                    required property string filePath
                    required property var    fileSize
                    required property var    fileModified
                    required property int    rowIndex

                    width: list.width 
                    height: 36
                    color: "transparent"

                    RowLayout {
                        anchors.fill: parent
                        anchors.leftMargin: 8
                        anchors.rightMargin: 8
                        spacing: 16

                        Label { 
                            text: row.fileName
                            color: "white"
                            elide: Text.ElideMiddle
                            Layout.fillWidth: true
                        }
                        Label { 
                            text: root.formatSize(row.fileSize)
                            color: "#CCCCCC"
                            Layout.preferredWidth: root.colSizeW
                            horizontalAlignment: Text.AlignRight
                        }
                        Label {
                            text: root.formatDate(row.fileModified)
                            color: "#CCCCCC"
                            Layout.preferredWidth: root.colModifiedW
                            horizontalAlignment: Text.AlignRight
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked:       list.currentIndex = rowIndex
                        onDoubleClicked: root.accepted(row.filePath, row.fileName, row.fileSize, row.fileModified)
                    }
                }

                onCountChanged: if (count > 0 && currentIndex < 0) currentIndex = 0
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.rightMargin: 4
            spacing: 8

            Item { Layout.fillWidth: true } // spacer

            Button { text: "Close"; onClicked: root.closed() }
            Button {
                text: "OK"
                enabled: list.currentIndex >= 0
                onClicked: {
                    const it = list.currentItem
                    if (it) root.accepted(it.filePath, it.fileName, it.fileSize, it.fileModified)
                }
            }
        }
    }

    Keys.onPressed: (e) => {
        if ((e.key === Qt.Key_Return || e.key === Qt.Key_Enter) && list.currentItem) {
            accepted(list.currentItem.filePath, list.currentItem.fileName, list.currentItem.fileSize, list.currentItem.fileModified)
            e.accepted = true
        } else if (e.key === Qt.Key_Escape) {
            closed()
            e.accepted = true
        }
    }
}
