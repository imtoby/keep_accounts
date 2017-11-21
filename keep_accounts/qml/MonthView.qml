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
        ListView{
            anchors.fill: parent
            model: 200
            clip: true
            ScrollDecorator{
                flickableItem: parent
            }
            delegate: Item{
                width: background.width - Config.margin * 2
                height: Config.cellHeight
                Text {
                    id: content
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    verticalAlignment: Text.AlignBottom
                    text: qsTr("Test")
                }
                Text {
                    id: dateTxt
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    anchors.bottom: parent.bottom
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("2016-08-08")
                }
                Text {
                    id: amountTxt
                    width: parent.width*2/5
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: Config.margin * 2
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    text: "12.34"
                }
                ///////line//////
                Rectangle{
                    implicitHeight: Config.lineWidth
                    anchors{
                        left: parent.left; leftMargin: Config.margin
                        right: parent.right; rightMargin: Config.margin
                        bottom: parent.bottom
                    }
                    color: Config.lineColor
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
