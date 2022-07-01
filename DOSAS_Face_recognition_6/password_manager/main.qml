import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    id: mainWindow
    width: 500
    height: 700
    visible: true
    title: qsTr("Tabs")
    Material.accent: '#31b1da'

    property int recordId: -1
    property int isPassword: -1

    property string username: ""
    property string confidence: ""

    Connections {
            target: cryptoController
            onSendMessageToQml: {
                dialog.open()
                dialogText.text = message
            }
    }

    Connections {
            target: cvController
            onSendMessageToQml: {
                dialog.open()
                dialogText.text = message
            }
            onSendUserInfoToQml: {
                username = _username
                confidence = _confidence
            }
            onUserIdentified: {
                cryptoController.decrypt_db_file()
                swipeView.currentIndex = 1
            }
    }

    Dialog {
        id: dialog
        anchors.centerIn: parent
        width: mainWindow.width/1.5
        height: mainWindow.height/4
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
        standardButtons: Dialog.Ok
        contentItem: Text {
            id: dialogText
            anchors.fill: parent
            horizontalAlignment: Qt.AlignHCenter
            verticalAlignment: Qt.AlignVCenter
            anchors.bottomMargin: footer.height
            text: "An error occured."
            color: "white"
            font.pointSize: 12
            wrapMode: Text.WordWrap
        }
    }


    function crypt_controller(password) {
        let is_correct_passwd = cryptoController.check_password(password)
        if(is_correct_passwd) {
//            if(recordId != -1 && isPassword != -1)
//            {
//                cryptoController.decrypt_db_file(recordId, isPassword)
//                recordId = -1
//                isPassword = -1
//            }
//            else
            cryptoController.decrypt_db_file()
            swipeView.currentIndex = 1
        } else {
            dialog.open()
            dialogText.text = "Неверный пароль."
        }

    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1Form {
        }

        Page2Form {
        }

        PageCamera {

        }
    }

    footer: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Page 1")
        }

        TabButton {
            text: qsTr("Page 2")
        }

        TabButton {
            text: qsTr("Biometry")
        }
    }
}
