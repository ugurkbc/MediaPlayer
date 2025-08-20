import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Window 2.15 
import Utils 1.0
import "../../customqmlcomponents"

Rectangle{
    anchors.fill: parent
    color: "#1e1e1e"
    
    ColumnLayout{
        anchors.fill: parent
        spacing: 0

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/setting.svg"
            buttonText: "Setting"
            reverse: true
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/video-record-play.svg"
            buttonText: "Record Video"
            reverse: true
            onClicked: {
                onClicked: videoControl.startRecord()
            }
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/video-record-play.svg"
            buttonText: "Stop Record Video"
            reverse: true
            onClicked: {
                onClicked: videoControl.closeRecord()
            }
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/video-records.svg"
            buttonText: "Video Records"
            reverse: true
        }

        Item { Layout.fillHeight: true }

        CustomButton{
            Layout.alignment: Qt.AlignHCenter
            iconSource: "qrc:/icons/close.svg"
            buttonText: "Close App"
            reverse: true
            onClicked: {
                onClicked: Qt.quit()
            }
        }

        Item { Layout.fillHeight: true }
    }
}


