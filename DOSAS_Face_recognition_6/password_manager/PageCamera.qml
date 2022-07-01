import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12 //для настройки стиля Material
import QtQuick.Dialogs 1.3
import QtMultimedia 5.15

Page {
    header: Rectangle {
        id: header
        color: "black"
        y: 0
        height: 45

        Label {
            color: "white"
            text: "Lab 2: менеджер паролей."
            font.weight: "DemiBold"
            font.pixelSize: 15
            anchors.verticalCenter: parent.verticalCenter
            x: 4
        }
        Rectangle{ //нижняя граница заголовка
            height: 3
            color: "#31b1da"
            anchors.top: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
        }

    }

    Camera {
        id: camera
        videoRecorder.mediaContainer: "mp4"
        deviceId: QtMultimedia.availableCameras[1].deviceId
        Component.onCompleted: cameraHelper.qcamera = camera
    }

    RowLayout {
        id: buttons
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
            text: "capture"
            onClicked: {
//                opencv_btn_clicked()
                cameraHelper.capture();
            }
        }

    }


    ColumnLayout {
        anchors.top: buttons.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        ColumnLayout {
            Rectangle {
                color: "black"
                //anchors.fill: parent
                Layout.fillHeight: true
                Layout.fillWidth: true
                VideoOutput {
                    id: cameraOutput
                    source: camera
                    anchors.fill: parent
                }
            }
        }

        RowLayout{
            Layout.margins: 12
            Label {
//                id: username
                text: "Username: " + username
                font.pixelSize: 18
                color: "#31b1da"
            }
            Label {
//                id: confidence
                Layout.leftMargin: 12
                text: "Confidence: " + confidence
                font.pixelSize: 18
                color: "#31b1da"
            }
        }

    }



}
