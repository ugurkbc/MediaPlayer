import QtQuick
import App 1.0

Item {
    anchors.fill: parent
    required property VideoControl videoControl

    VideoItem {
        id: videoItem
        anchors.fill: parent
    }

    Component.onCompleted: videoControl.videoItem = videoItem
}
