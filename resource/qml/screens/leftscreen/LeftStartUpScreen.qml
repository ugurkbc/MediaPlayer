import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs
import Utils 1.0
import App 1.0
import "../../customqmlcomponents"

Rectangle{
    anchors.fill: parent
    color: "#1e1e1e"

    required property VideoControl videoControl

    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/file-dialog.svg"
            buttonText: "Open Video"
            onClicked: fileDialog.open()
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/play-button.svg"
            buttonText: "Play Video"
            onClicked: videoControl.playVideoStream()
            
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/video-pause.svg"
            buttonText: "Stop Video"
            onClicked: videoControl.pauseVideoStream()
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/video-close.svg"
            buttonText: "Close Video"
            onClicked: videoControl.closeVideoStream()
        }

        Item { Layout.fillHeight: true }

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
}


