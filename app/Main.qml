import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Layouts
import LightManager

Window {
    id: root
    objectName: "root"
    property var portNames
    property real sensorIllum: 0
    property real manual: 0
    property string mode: "sensor"
    property alias selectedPort: portCombo.currentText
    property alias pageState: rootPage.state
    signal connectToPort(string port)
    width: 480
    height: 300
    visible: true
    title: qsTr("Light manager")
    Page {
        id: rootPage
        objectName: "rootPage"
        padding: 15
        anchors.fill: parent
        states: [
            State {
                name: "disconnected"
                PropertyChanges {
                    target: connectBtn
                    enabled: true
                }
                PropertyChanges {
                    target: modeLayout
                    enabled: false
                }
            },
            State {
                name: "connecting"
                PropertyChanges {
                    target: connectBtn
                    enabled: false
                }
                PropertyChanges {
                    target: modeLayout
                    enabled: false
                }
            },
            State {
                name: "connected"
                PropertyChanges {
                    target: connectBtn
                    enabled: true
                }
                PropertyChanges {
                    target: modeLayout
                    enabled: true
                }
            }
        ]
        state: "disconnected"
        footer: Label {
            id: statusLbl
            objectName: "statusLbl"
            text: qsTr("Status:") + " " + rootPage.state
        }
        ButtonGroup {
            objectName: "modeBtnGroup"
            id: modeBtnGroup
        }
        ColumnLayout {
            anchors.fill:parent
            Row {
                spacing: 15
                Label {
                    anchors.verticalCenter: parent.verticalCenter
                    text: qsTr("Selected port:") + " "
                }
                ComboBox {
                    id: portCombo
                    objectName: "portCombo"
                    enabled: count !== 0
                    model: root.portNames
                }
                Button {
                    id: connectBtn
                    objectName: "connectBtn"
                    text: qsTr("Connect")
                    enabled: selectedPort !== ""
                    onClicked: {
                        root.connectToPort(root.selectedPort)
                    }
                }
            }
            RowLayout {
                id: modeLayout
                objectName: "modeLayout"
                GroupBox {
                    id: sensorModeGroup
                    objectName: "sensorModeGroup"
                    Layout.preferredWidth: parent.width / 2
                    Layout.fillHeight: true
                    label: RadioButton {
                        id: sensorModeRadio
                        objectName: "sensorModeRadio"
                        checked: root.mode == "sensor"
                        onCheckedChanged: if (checked)
                                              root.mode = "sensor"
                        ButtonGroup.group: modeBtnGroup
                        text: qsTr("Outdoor Illuminance")
                    }
                    Column {
                        enabled: sensorModeRadio.checked
                        anchors.centerIn: parent
                        spacing: 10
                        Image {
                            id: sensorIcon
                            objectName: "sensorIcon"
                            states: ["night", "day"]
                            state: root.sensorIllum >= 0.5 ? "day" : "night"
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: {
                                if (sensorIcon.state == "night")
                                    return Qt.resolvedUrl("moon.png")
                                else if (sensorIcon.state == "day")
                                    return Qt.resolvedUrl("sun.png")
                            }
                            sourceSize.width: 70
                            sourceSize.height: 70
                        }
                        Label {
                            id: sensorIlluminance
                            objectName: "sensorIlluminance"
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Outdoor Illuminance: ") + Math.round(
                                      root.sensorIllum * 100)
                        }
                    }
                }
                GroupBox {
                    id: manualModeGroup
                    objectName: "manualModeGroup"
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    label: RadioButton {
                        id: manualModeRadio
                        objectName: "manualModeRadio"
                        checked: root.mode == "manual"
                        onCheckedChanged: if (checked)
                                              root.mode = "manual"
                        text: qsTr("Вручную")
                        ButtonGroup.group: modeBtnGroup
                    }
                    Column {
                        anchors.centerIn: parent
                        spacing: 10
                        enabled: manualModeRadio.checked
                        Dial {
                            id: manualModeDial
                            objectName: "manualModeDial"
                            anchors.horizontalCenter: parent.horizontalCenter
                            from: 0
                            to: 1
                            stepSize: 0.05
                            snapMode: Dial.SnapAlways
                            onValueChanged: root.manual = value
                        }
                        Label {
                            id: manualIllumination
                            objectName: "manualIllumination"
                            anchors.horizontalCenter: parent.horizontalCenter
                            text: qsTr("Lighting: ") + Math.round(
                                      root.manual * 100)
                        }
                    }
                }
            }
        }
    }
}
