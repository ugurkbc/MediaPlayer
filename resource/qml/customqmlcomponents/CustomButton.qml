import QtQuick 2.15

Rectangle {
    id: customButton
    width: 120
    height: 40
    radius: 6
    border.width: 0

    property color normalColor: "#2d2d2d"
    property color hoverColor:  "#3c3c3c"
    property color pressedColor:"#555555"
    property color activeColor: "#228B22"  

    property url iconSource: ""
    property string buttonText: ""
    property bool showIcon: true
    property bool reverse: false

    property bool active: false

    property color _currentColor: normalColor
    color: _currentColor

    onActiveChanged:      _currentColor = active ? activeColor : normalColor
    onNormalColorChanged: if (!active) _currentColor = normalColor
    onActiveColorChanged: if (active)  _currentColor = activeColor

    signal clicked()

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: customButton.clicked()

        onPressed:  { if (!customButton.active) customButton._currentColor = customButton.pressedColor }
        onReleased: { if (!customButton.active) customButton._currentColor = containsMouse ? customButton.hoverColor : customButton.normalColor }
        onEntered:  { if (!customButton.active) customButton._currentColor = customButton.hoverColor }
        onExited:   { if (!customButton.active) customButton._currentColor = customButton.normalColor }
    }

    Row {
        id: contentRow
        anchors.centerIn: parent
        spacing: 8

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

        Text {
            text: customButton.buttonText
            font.pixelSize: 12
            color: "white"
            visible: text.length > 0 && customButton.reverse
        }
    }
}
