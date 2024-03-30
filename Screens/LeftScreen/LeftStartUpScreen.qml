import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs
import Utils 1.0
import "../../CustomQMLComponents"

ColumnLayout{
    anchors.fill: parent
    spacing: 20

    CustomButton{
        Layout.topMargin : 20
        Layout.alignment: Qt.AlignHCenter
        buttonText: "Open Video"
        onClicked: {
            onClicked: fileDialog.open()
        }
    }

    CustomButton{
        Layout.alignment: Qt.AlignHCenter
        buttonText: "Play"
        onClicked: {
            onClicked: fileDialog.open()
        }
    }

    CustomButton{
        Layout.alignment: Qt.AlignHCenter
        buttonText: "Pause"
        onClicked: {
            onClicked: fileDialog.open()
        }
    }

    CustomButton{
        Layout.alignment: Qt.AlignHCenter
        buttonText: "Close"
        onClicked: {
            onClicked: fileDialog.open()
        }
    }

    Rectangle{
        Layout.fillHeight: true
        Layout.fillWidth: true
        color: "black"
    }

    FileDialog {
            id: fileDialog
            title: "Select a .mp4 File"
            nameFilters: ["MP4 Files (*.mp4)"]
            onAccepted: {
                var filePath = Utils.fileDialogConverter(selectedFile.toString());
                videoControl.setUrl(filePath);
            }
        }
}
