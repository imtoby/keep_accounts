import QtQuick 2.3
import "../Tools"
import "../Config"

KBaseDialog {
    id: baseDialog
    anchors.fill: parent

    property string title: qsTr("类别选择")
    property int dialogWidth: baseDialog.width / 2.0
    property int dialogHeight: dialogWidth * 0.8

    contextSourceComponent: Item {
        width: dialogWidth
        height: dialogHeight

        BorderImage {
            source: "qrc:/res/00.png"
            anchors.fill: parent
            border.left: 5; border.top: 5
            border.right: 5; border.bottom: 5
            horizontalTileMode: BorderImage.Repeat
            verticalTileMode: BorderImage.Repeat
        }

        MouseArea {
            anchors.fill: parent
        }

        Text {
            id: titleTxt
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            font.pointSize: 18
            text: title
        }

        ListView {
            id: contentTxt
            anchors.top: titleTxt.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20

            model: 20

            delegate: KButton {
                width: contentTxt - (2 * Config.margin)
                height: 40
                Text {
                    anchors.fill: parent
                    text: qsTr("取消")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                }
            }
        }


    }
}
