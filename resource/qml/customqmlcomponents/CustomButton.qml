import QtQuick 2.15

Rectangle {
    id: customButton
    color: "green"
    width: Math.max(textMetrics.width + 20, 80)
    height: Math.max(textMetrics.height + 10, 20)
    border.width: 0

    property string buttonText: "Custom Button"
    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: customButton.clicked()
        hoverEnabled: true
        onEntered: parent.color = "lightblue"
        onExited: parent.color = "green"
    }

    TextMetrics {
           id: textMetrics
           text: customButton.buttonText
           font.pixelSize: 10
       }

    Text {
        anchors.centerIn: parent
        text: customButton.buttonText
        font.pixelSize: 10
        clip: true
        color: "black"
    }
}
