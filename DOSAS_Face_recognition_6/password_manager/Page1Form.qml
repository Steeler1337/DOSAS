import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12 //для настройки стиля Material
import QtQuick.Dialogs 1.3


Page {// ЛР 2. Разработка защищённого менеджера паролей.
//    Connections {
//            target: cryptoController // Указываем целевое соединение

//            onSendToQml: {
//                encrypted_textarea1.text += qsTr(decrypted_text)
//            }
//    }

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
    ColumnLayout{
        anchors.fill: parent
        anchors.margins: 5
        anchors.topMargin: 10

        Button {
            text: "face recognition"
            Layout.fillWidth: true
            onClicked: {
                swipeView.currentIndex = 2
            }
        }

        Text{
            text: "Input a password:"
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignBottom
            Layout.topMargin: 0
            Layout.bottomMargin: 0
            color: "white"
            font.pixelSize: 16
        }
        RowLayout{
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignTop
            Layout.topMargin: 0
            TextField{
                id: passwd
                Layout.fillWidth: true
                echoMode: TextInput.Password
                color: "#31b1da"
                maximumLength: 32
                placeholderText: "Input a password"
            }
            Button{
                onClicked: {
                    passwd.echoMode == TextInput.Password ? passwd.echoMode=TextInput.Normal : passwd.echoMode = TextInput.Password
                }
                icon.source: "img/eye.png"
                icon.color: "#31b1da"
            }
        }

        RowLayout{
            Layout.alignment: Qt.AlignHCenter
            Button{
                text: "OK"
                onClicked: {
                    crypt_controller(passwd.text)
                    passwd.text = ""

                }
            }
        }
    }
}
