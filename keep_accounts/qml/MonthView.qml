import QtQuick 2.3
import "Config"
import "Tools"

Item {
    id: background
    HeaderView{
        id: headBackground
        onClicked: {
            switch(index){
            case Config.indexNewAccount:
                editDialog.clearInputFocus()
                editDialog.show()
                break;
            }
        }
    }
    Rectangle{
        id: viewBackground
        anchors{
            left: parent.left; leftMargin: Config.margin
            right: parent.right; rightMargin: Config.margin
            top: headBackground.bottom; topMargin: Config.margin
            bottom: parent.bottom; bottomMargin: Config.margin
        }
        color: "transparent"
        border{
            color: Config.lineColor
            width: Config.lineWidth
        }
        ListView {
            id: monthView
            anchors.fill: parent
            anchors.margins: Config.lineWidth
            model: infoManager.recordModel()
            clip: true
            ScrollDecorator{
                flickableItem: parent
            }
            Text {
                id: tipInfo
                anchors.centerIn: parent
                opacity: monthView.count === 0
                Behavior on opacity {
                    NumberAnimation { duration: 200 }
                }
                text: qsTr("本月暂无记录，点击 \"新增记录\" 添加新的记录")
            }
            delegate: SlideLeftToDeleteDelegate {
                width: background.width - Config.margin * 2
                height: Config.cellHeight
                deleteButtonColor: Config.deleteColor
                deleteButtonPressedColor: Config.deletePressedColor
                Text {
                    id: content
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    verticalAlignment: Text.AlignBottom
                    text: model.modelData.note
                }
                Text {
                    id: dateTxt
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    anchors.bottom: parent.bottom
                    verticalAlignment: Text.AlignVCenter
                    text: model.modelData.year + Config.dateSeparator +
                          model.modelData.month + Config.dateSeparator +
                          model.modelData.day
                }
                Text {
                    id: amountTxt
                    width: parent.width*2/5
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: Config.margin * 2
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    text: model.modelData.amount
                }
                ///////line//////
                Rectangle{
                    implicitHeight: Config.lineWidth
                    anchors{
                        left: parent.left
                        right: parent.right
                    }
                    color: Config.lineColor
                }
                Rectangle{
                    implicitHeight: Config.lineWidth
                    anchors{
                        left: parent.left
                        right: parent.right
                        bottom: parent.bottom
                    }
                    color: Config.lineColor
                }

                onDeleteButtonClicked: {
                    infoManager.deleteRecord(model.modelData.millonSecs)
                }
            }
            onMovementStarted: {
                posY = contentY
            }
            onContentYChanged: {
                if(contentY > posY){
                    // move down
                    headBackground.hide()
                }else{
                    // move up
                    headBackground.show()
                }
            }
            property real posY: contentY
        }

    }
    EditDialog{
        id: editDialog
    }
}
