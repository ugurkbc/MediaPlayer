import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Windows
import App 1.0
import "screens/leftscreen"
import "screens/rightscreen"
import "screens/topscreen"
import "screens/bottomscreen"
import "screens/mainscreen"

ApplicationWindow  {
    visible: true
    title: qsTr("Uğur KABACAOĞLU")
    flags: Qt.FramelessWindowHint | Qt.Window
    visibility: Window.Maximized

    property string colorName: "black"
    VideoControl { id: vc }

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
                    sourceComponent: LeftStartUpScreen { videoControl: vc }
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
                            sourceComponent: TopStartUpScreen {}
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
                            sourceComponent: MainScreen { videoControl: vc }
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
                            sourceComponent: BottomStartUpScreen {}
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
                    sourceComponent: RightStartUpScreen { videoControl: vc }
                }
            }
        }
    }
}
