import QtQuick 2.15

Rectangle {
    id: customButton
    width: 120
    height: 40
    radius: 6
    border.width: 0
    color: normalColor

    // themeable properties
    property color normalColor: "#2d2d2d"
    property color hoverColor: "#3c3c3c"
    property color pressedColor: "#555555"

    property url iconSource: ""
    property string buttonText: ""
    property bool showIcon: true
    property bool reverse: false

    signal clicked()

    MouseArea {
        anchors.fill: parent
        onClicked: customButton.clicked()
        hoverEnabled: true
        onPressed: customButton.color = pressedColor
        onReleased: customButton.color = containsMouse ? hoverColor : normalColor
        onEntered: customButton.color = hoverColor
        onExited: customButton.color = normalColor
    }

    Row {
        id: contentRow
        anchors.centerIn: parent
        spacing: 8

        // text before icon when reverse == false
        Text {
            text: customButton.buttonText
            font.pixelSize: 12
            color: "white"
            visible: text.length > 0 && !customButton.reverse
        }

        Image {
            source: customButton.iconSource
            width: 24
            height: 20
            fillMode: Image.PreserveAspectFit
            visible: customButton.showIcon
        }

        // text after icon when reverse == true
        Text {
            text: customButton.buttonText
            font.pixelSize: 12
            color: "white"
            visible: text.length > 0 && customButton.reverse
        }
    }
}
