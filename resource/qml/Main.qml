import QtQuick
import QtQuick.Layouts
import QtQuick.Controls 2.15
import QtQuick.Controls.Windows
import Utils 1.0
import App 1.0
import "screens/leftscreen"
import "screens/rightscreen"
import "screens/topscreen"
import "screens/bottomscreen"
import "screens/mainscreen"
import "model"         

ApplicationWindow  {
    visible: true
    title: qsTr("Uğur KABACAOĞLU")
    flags: Qt.FramelessWindowHint | Qt.Window
    visibility: Window.Maximized

    property bool overlayVisible: false
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
                    sourceComponent: RightStartUpScreen {
                        videoControl: vc
                        onOpenRecordsOverlay: overlayVisible = true
                    }
                }
            }
        }
    }

    Item {
        id: overlay
        anchors.fill: parent
        visible: overlayVisible
        z: 999

        Rectangle {
            anchors.fill: parent
            color: "#66222222"   // ~40% visible dark gray
            z: 0
        }

        MouseArea {
            anchors.fill: parent
            acceptedButtons: Qt.AllButtons
            hoverEnabled: true
            z: 0
        }

        Loader {
            id: recordsLoader
            z: 1
            width: Math.min(parent.width * 0.6, 900)
            height: Math.min(parent.height * 0.6, 600)
            anchors.centerIn: parent

            // use the type from import "model"
            sourceComponent: VideoRecordsView {
                anchors.fill: parent

                onAccepted: (path, name, size, mod) => {
                    console.log("Chosen:", path)
                    overlayVisible = false
                    // handle selection here if needed
                }
                onClosed: overlayVisible = false
            }
        }
    }
}
