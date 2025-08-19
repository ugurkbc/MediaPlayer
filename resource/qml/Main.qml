import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Windows

ApplicationWindow  {
    visible: true
    title: qsTr("Uğur KABACAOĞLU")
    flags: Qt.FramelessWindowHint | Qt.Window
    visibility: Window.Maximized

    property string colorName: "black"

    Rectangle {
        width: parent.width
        height: parent.height
        color: colorName

        RowLayout {
            anchors.fill: parent
            spacing: 0

            Rectangle {
                Layout.maximumWidth: 140
                Layout.minimumWidth: 140
                Layout.fillHeight: true
                color: colorName

                Loader {
                    id: leftPageLoader
                    anchors.fill: parent
                    source: "screens/leftscreen/LeftStartUpScreen.qml"
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                color: colorName

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 0

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: parent.height * 0.1
                        Layout.maximumHeight: 80
                        Layout.minimumHeight: 80
                        color: colorName

                        Loader {
                            id: topPageLoader
                            anchors.fill: parent
                            source: "screens/topscreen/TopStartUpScreen.qml"
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: parent.height * 0.8
                        color: colorName


                        Loader {
                            id: mainPageLoader
                            anchors.fill: parent
                            source: "screens/mainscreen/MainScreen.qml"
                        }
                    }

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: parent.height * 0.1
                        Layout.maximumHeight: 80
                        Layout.minimumHeight: 80
                        color: colorName

                        Loader {
                            id: bottomPageLoader
                            anchors.fill: parent
                            source: "screens/bottomscreen/BottomStartUpScreen.qml"
                        }
                    }
                }
            }

            Rectangle {
                Layout.maximumWidth: 140
                Layout.minimumWidth: 140
                Layout.fillHeight: true
                color: colorName

                Loader {
                    id: rightPageLoader
                    anchors.fill: parent
                    source: "screens/rightscreen/RightStartUpScreen.qml"
                }
            }
        }
    }
}
