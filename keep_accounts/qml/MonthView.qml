import QtQuick 2.3
import "Config"
import "Tools"
import "Dialog"

Item {
    id: background
    HeaderView {
        id: headBackground
        onClicked: {
            switch(index){
            case Config.indexNewAccount:
                editDialog.show();
                break;
            case Config.indexMonth:
                datePickerDialog.startPicker(year, month);
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
            property int year: -1
            property int month: -1
            anchors.fill: parent
            anchors.margins: Config.lineWidth
            model: infoManager.recordModel(year, month)
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
                text: qsTr("本月暂无记录，点击 \"<a href=\"www.toby.com\">新增记录</a>\" 添加新的记录")
                onLinkActivated: {
                    editDialog.show();
                }
            }

            delegate: SlideLeftToDeleteDelegate {
                width: background.width - Config.margin * 2
                height: Config.cellHeight
                deleteButtonColor: Config.deleteColor
                deleteButtonPressedColor: Config.deletePressedColor

                Text {
                    id: dateTxt
                    width: 50
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    verticalAlignment: Text.AlignBottom
                    font.pointSize: 16
                    text: model.modelData.day + "日"
                    color: model.modelData.type == Config.out_type ?
                               Config.expensesColor : Config.incomeColor
                }
                Text {
                    id: weekday
                    width: dateTxt.width
                    height: parent.height/2
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin * 2
                    anchors.bottom: parent.bottom
                    verticalAlignment: Text.AlignVCenter
                    color: Config.describeColor
                    text: Config.weekdayString(model.modelData.year,
                                               model.modelData.month,
                                               model.modelData.day)
                }

                Rectangle {
                    id: dateDiv
                    width: Config.lineWidth
                    height: parent.height - Config.margin * 2
                    color: Config.lineColor
                    anchors.left: dateTxt.right
                    anchors.verticalCenter: parent.verticalCenter
                }

                Text {
                    id: content
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: dateDiv.right
                    anchors.leftMargin: Config.margin * 2
                    verticalAlignment: Text.AlignBottom
                    font.pointSize: 16
                    text: ((model.modelData.parentType !== Config.topTypeId) ?
                              (model.modelData.parentType + "-") : "") +
                              model.modelData.childType
                }
                Text {
                    id: describe
                    width: parent.width*3/5
                    height: parent.height/2
                    anchors.left: dateDiv.right
                    anchors.leftMargin: Config.margin * 2
                    anchors.bottom: parent.bottom
                    verticalAlignment: Text.AlignVCenter
                    color: Config.describeColor
                    text: model.modelData.note
                }
                Text {
                    id: amountTxt
                    width: parent.width*2/5
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: Config.margin * 2
                    horizontalAlignment: Text.AlignRight
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 18
                    color: model.modelData.type == Config.out_type ?
                               Config.expensesColor : Config.incomeColor
                    text: model.modelData.amount.toFixed(2)
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

                onContentRectangleClicked: {
                    editDialog.show(false, index, {
                                        "millonSecs":
                                        model.modelData.millonSecs,
                                        "amount": model.modelData.amount,
                                        "type": model.modelData.type,
                                        "year": model.modelData.year,
                                        "month": model.modelData.month,
                                        "day": model.modelData.day,
                                        "parentType":
                                        model.modelData.parentType,
                                        "childType": model.modelData.childType,
                                        "note": model.modelData.note,
                                        "icon": model.modelData.icon
                                    });
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
    KYearMonthPickerDialog {
        id: datePickerDialog
        parent: rootPage

        onAccepted: {
            headBackground.year = year;
            headBackground.month = month;
            monthView.year = year;
            monthView.month = month;
        }

        onRejected: {
        }
    }
}
