import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.12
import QtGraphicalEffects 1.12

Page {
    header: Label {
        text: qsTr("Passwords")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Connections {
            target: cryptoController

            onSendDbToQml: {
                listmodel1.clear()

                label_no_data.visible = false
//                console.log(db_decrypted)
                let json_db = JSON.parse(db_decrypted)

                for(let i = 0; i < json_db["sites"].length; i++) {
//                    console.log(json_db["sites"][i]["site"])
                    let json_db_model = {}
                    json_db_model["site"] = json_db["sites"][i]["site"]
                    json_db_model["login"] = "******"
                    json_db_model["password"] = "******"
                    listmodel1.append(json_db_model)
                }
            }
    }

    Label {
        id: label_no_data
        text: qsTr("No data.")
        anchors.centerIn: parent
    }

    ListModel { //модель для ListView
        id: listmodel1
    }


    ColumnLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.fill: parent

        GridView{
            id: gridview1
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.topMargin: 5
            Layout.leftMargin: 10
            clip: true
            model: listmodel1
            cellWidth: width

            delegate: Item{
                width: gridview1.cellWidth
                height: 80
                property int recordIndex: index

                Rectangle {
                    width: gridview1.cellWidth - 10
                    height: 80
                    color: "#222222"

                    RowLayout{
                        anchors.fill: parent

                        Text {
                            Layout.preferredWidth: 150
                            Layout.leftMargin: 15
                            Layout.alignment: Qt.AlignLeft
                            horizontalAlignment: Text.AlignLeft
                            text: site
                            font.pixelSize: 14
                            font.weight: "DemiBold"
                            color: '#31b1da'

                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.leftMargin: 15
                            Layout.alignment: Qt.AlignHCenter
                            horizontalAlignment: Text.AlignHCenter
                            text: login
                            font.pixelSize: 14
                            font.weight: "DemiBold"
                            color: '#31b1da'
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
//                                    console.log("login clicked")
                                    recordId = recordIndex
                                    isPassword = 0
                                    cryptoController.decrypt_db_file(recordId, isPassword)
//                                    swipeView.currentIndex = 0
                                }
                            }

                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.rightMargin: 15
                            Layout.alignment: Qt.AlignRight
                            horizontalAlignment: Text.AlignRight
                            text: password
                            font.pixelSize: 14
                            font.weight: "DemiBold"
                            color: '#31b1da'
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
//                                    console.log(recordIndex)
                                    recordId = recordIndex
                                    isPassword = 1
                                    cryptoController.decrypt_db_file(recordId, isPassword)
//                                    swipeView.currentIndex = 0
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}
