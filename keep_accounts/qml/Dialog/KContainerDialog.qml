import QtQuick 2.3
import "../Tools"
import "../Config"

KBaseDialog {
    id: baseDialog
    anchors.fill: parent

    property int dialogWidth: baseDialog.width * 0.8
    property int dialogHeight: dialogWidth * 0.8
    property string title: qsTr("")
    property Component content: null

    signal accepted()
    signal rejected()

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

        Item {
            id: contentTxt
            anchors.top: titleTxt.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: cancelBtn.top
            anchors.margins: 20

            Loader {
                width: parent.width
                height: parent.height
                sourceComponent: content
            }
        }

        KButton {
            id: cancelBtn
            width: parent.width/2
            height: 50
            anchors.bottom: parent.bottom
            Text {
                anchors.fill: parent
                text: qsTr("取消")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            onClicked: {
                baseDialog.close()
                rejected()
            }
        }

        KButton {
            id: okBtn
            width: parent.width/2
            height: 50
            anchors.bottom: parent.bottom
            anchors.left: cancelBtn.right
            Text {
                anchors.fill: parent
                text: qsTr("确定")
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }
            onClicked: {
                baseDialog.close()
                accepted()
            }
        }
    }
}
