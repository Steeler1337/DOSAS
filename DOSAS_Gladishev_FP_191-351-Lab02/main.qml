import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.12
import QtQuick.Layouts 1.0
import "MaterialDesign.js" as MD


ApplicationWindow {
    id: window
    width: 700
    height: 500
    visible: true
    title: qsTr("Lab №2 Gladishev 191-351")
    background: Rectangle {
                color: "#b3d5ff"
            }

    property int recordId: -1
    property int isPassword: -1
    property string pin_code: ""


    Connections {
            target: cryptoController // указываем целевой класс
            onSendMessageToQml: {
                dialog.open()
                dialogText.text = message
            }
            onSendDbToQml: {
                listmodel1.clear()
                let json_db = JSON.parse(db_decrypted)
                for(let i = 0; i < json_db["sites"].length; i++) { // распарс данных из json в переменные
                    let json_db_model = {}
                    json_db_model["site"] = json_db["sites"][i]["site"]
                    json_db_model["login"] = json_db["sites"][i]["login"]
                    json_db_model["password"] = json_db["sites"][i]["password"]
                    listmodel1.append(json_db_model)
                    listmodel2.append(json_db_model)
                }
            }
    }


    ListModel {
        id: listmodel2 // постоянный список, хранящий ВСЕ сайты
    }

    function finder() {
        listmodel1.clear() // очищаем все видимые элементы
            for(var i = 0; i < listmodel2.count; ++i) {
                if (listmodel2.get(i).site.includes(edtSearch.text)) { // если в listmodel2 есть текст из поисковой строки, то значение из listmodel2 добавляются в listmodel1
                    listmodel1.append(listmodel2.get(i))
                }
            }
        }

    function get_password(crypt_password, pin) {
            cryptoController.text_to_clipboard(crypt_password, pin)
        }

    function getLogin(crypt_login, pin){
        cryptoController.text_to_clipboard(crypt_login, pin) // в text_to_clipboard передается зашифрованный логин
    }


    function crypt_controller(password) {
            let is_correct_passwd = cryptoController.check_password(password) // true - если верный код введен, false - неверный
            if(is_correct_passwd) {
                pin_code=password // в pin_code кладется пароль в чистом виде чтобы потом расшифровывать логин и пароль
                cryptoController.decrypt_db_file(password) // расшифровка db_crypt файла (зашифрованного уже вторым слоем)
                stackView.push(pageMain) // если пароль верный, показывает список
            } else {
                stackView.push(pageError) // если пароль неверный показывает страницу с ошибкой
            }
    }


    FontLoader {
        id: iconFont
        source: "../fonts/MaterialIcons-Regular.ttf"
    }


    StackView {
        anchors.fill: parent
        id: stackView

        initialItem: Page {
            id: pageLogin
            background: Rectangle {
                        color: "#b3d5ff"
                    }
            GridLayout {
                anchors.fill: parent
                rowSpacing: 10
                rows: 4
                flow: GridLayout.TopToBottom

                Item { // Для заполнения пространства
                    Layout.row: 0
                    Layout.fillHeight: true
                }

                TextField {
                    id: password_code
                    echoMode: TextField.Password
                    passwordCharacter: "●"
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                }

                Button {
                    id: login_button
                        text: qsTr("Login")
                        Layout.row: 2
                        Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                        onClicked:{
                            crypt_controller(password_code.text)
                            password_code.text = ""
                        }
                }

                Item { // Для заполнения пространства
                    Layout.row: 4
                    Layout.fillHeight: true
                }
            }
        }

        Page {
            id: pageError
            visible: false
            background: Rectangle {
                        color: "#b3d5ff"
                    }
            GridLayout {
                anchors.fill: parent
                rowSpacing: 10
                rows: 3
                flow: GridLayout.TopToBottom

                Item { // Для заполнения пространства
                    Layout.row: 0
                    Layout.fillHeight: true
                }
                Label {
                    id: error_code
                    text: "Wrong code!"
                    Layout.row: 1
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                }

                Button {
                    id: back_to_login_page
                    text: qsTr("Go back")
                    Layout.row: 2
                    Layout.alignment: Qt.AlignHCenter || Qt.AlignVCenter
                    onClicked:{
                        if( stackView.depth > 1 ) {
                            stackView.pop() // возварт обратно к вводу пинкода
                        }
                    }
                }
                Item { // Для заполнения пространства
                    Layout.row: 3
                    Layout.fillHeight: true
                }
            }
        }

        Page {
            id: pageMain
            visible: false
            background: Rectangle {
                        color: "#b3d5ff"
                    }
            GridLayout {
                anchors.fill: parent

                RowLayout {
                    TextField {
                        id: edtSearch
                        Layout.column: 1
                        Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                        Layout.leftMargin: 20
                        Layout.rightMargin: 20
                        rightPadding: height
                        Layout.fillWidth: true
                        placeholderText: "Search in Manager..."
                    }

                    Button {
                        text: "Find"
                        Layout.rightMargin: 10
                        onClicked: {
                            finder()
                        }
                    }
                }

                ListView {
                    id: listView
                    Layout.preferredWidth: parent.width
                    Layout.fillHeight: true
                    Layout.row: 2
                    height: 200
                    spacing: 2
                    clip: true
                    delegate: Rectangle {
                        width: listView.width
                        height: 50
                        radius: 0
                        border.color: "black"
                        border.width: 2
                        RowLayout {
                            anchors.fill: parent
                            Label {
                                text: site
                                Layout.minimumWidth: 150
                                Layout.leftMargin: 40
                            }
                            TextField {
                                                            id: edtLogin
                                                            Layout.minimumWidth: 0
                                                            Layout.maximumWidth: 59 // ограничение по кол-ву выводимых точек
                                                            Layout.leftMargin: 80
                                                            Layout.column: 0
                                                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                                            echoMode: TextField.Password
                                                            passwordCharacter: "●"
                                                            text: login
                                                            readOnly: true
                                                            background: Item {

                                                            }
                                                            MouseArea {
                                                                anchors.fill: parent
                                                                onClicked: {
                                                                    getLogin(edtLogin.text, pin_code) // pin_code хранит пинкод в чистом виде, нужна чтобы расшифр. логин и пароль
                                                                }
                                                            }
                                                        }
                            TextField {
                                                            id: edtPassword
                                                            Layout.minimumWidth: 0
                                                            Layout.maximumWidth: 59 // ограничение по кол-ву выводимых точек
                                                            Layout.leftMargin: 80
                                                            Layout.column: 0
                                                            width: 6
                                                            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
                                                            echoMode: TextField.Password
                                                            passwordCharacter: "●"
                                                            text: password
                                                            readOnly: true
                                                            background: Item {

                                                            }
                                                            MouseArea {
                                                                anchors.fill: parent
                                                                onClicked: {
                                                                    // Копирует в буфер обмена расшифрованное значение пароля
                                                                    get_password(edtPassword.text, pin_code)
                                                                }
                                                            }
                                                        }
                            Item {
                                Layout.fillWidth: true
                            }
                        }
                    }
                    model:ListModel {
                        id: listmodel1 // видимые элементы списка паролей
                    }
                }
            }

        }
    }
}
