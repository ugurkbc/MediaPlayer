import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Windows

ApplicationWindow  {

    visible: true
    title: qsTr("Hello World")
    width: 640
    height: 480

    Rectangle {
            width: parent.width
            height: parent.height
            color: "black"

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    Layout.preferredWidth: parent.width * 0.1
                    Layout.maximumWidth: 100
                    Layout.fillHeight: true
                    color: "black"

                    Loader {
                            id: leftPageLoader
                            anchors.fill: parent
                            source: "Screens/LeftScreen/LeftStartUpScreen.qml"
                        }
                }

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    color: "black"


                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredHeight: parent.height * 0.1
                            Layout.maximumHeight: 80
                            color: "black"

                            Loader {
                                    id: topPageLoader
                                    anchors.fill: parent
                                    source: "Screens/TopScreen/TopStartUpScreen.qml"
                                }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.minimumHeight: parent.height * 0.8
                            color: "black"


                            Loader {
                                    id: mainPageLoader
                                    anchors.fill: parent
                                    source: "Screens/MainScreen/MainScreen.qml"
                                }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            Layout.preferredHeight: parent.height * 0.1
                            Layout.maximumHeight: 80
                            color: "black"

                            Loader {
                                    id: bottomPageLoader
                                    anchors.fill: parent
                                    source: "Screens/BottomScreen/BottomStartUpScreen.qml"
                                }
                        }
                    }
                }

                Rectangle {
                    Layout.preferredWidth: parent.width * 0.1
                    Layout.maximumWidth: 100
                    Layout.fillHeight: true
                    color: "black"

                    Loader {
                            id: rightPageLoader
                            anchors.fill: parent
                            source: "Screens/RightScreen/RightStartUpScreen.qml"
                        }
                }
            }
    }
}
