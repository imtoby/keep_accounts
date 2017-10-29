import QtQuick 2.3
import "../Config"

Item {
    id: typeContainer
    width: parent.width
    height: parent.height/7
    anchors.top: parent.top
    anchors.topMargin: Config.margin
    property bool isExpensesType: true
    signal clearInputFocus()
    KButton{
        id: expensesBtn
        width: parent.width/2 - Config.margin
        height: parent.height
        anchors.left: parent.left
        anchors.leftMargin: Config.margin
        onClicked: {
            clearInputFocus()
            isExpensesType = true
        }
        Rectangle{
            color: "transparent"
            border.color: Config.lineColor
            border.width: Config.lineWidth
            width: typeExpensesText.paintedHeight
            height: width
            radius: width/2
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: - typeExpensesText.paintedWidth
            Rectangle{
                anchors.fill: parent
                anchors.margins: 2
                radius: width/2
                color: Config.expensesColor
                opacity: isExpensesType
                scale: isExpensesType
                Behavior on opacity {
                    NumberAnimation{duration: 200}
                }
                Behavior on scale {
                    NumberAnimation{duration: 200}
                }
            }
        }
        Text{
            id: typeExpensesText
            anchors.fill: parent
            text: "支出"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Config.expensesColor
        }
    }
    KButton{
        width: parent.width/2 - Config.margin
        height: parent.height
        anchors.right: parent.right
        anchors.rightMargin: Config.margin
        onClicked: {
            clearInputFocus()
            typeContainer.isExpensesType = false
        }
        Rectangle{
            color: "transparent"
            border.color: Config.lineColor
            border.width: Config.lineWidth
            width: typeIncomeText.paintedHeight
            height: width
            radius: width/2
            anchors.centerIn: parent
            anchors.horizontalCenterOffset: - typeIncomeText.paintedWidth
            Rectangle{
                anchors.fill: parent
                anchors.margins: 2
                radius: width/2
                color: Config.incomeColor
                opacity: !isExpensesType
                scale: !isExpensesType
                Behavior on opacity {
                    NumberAnimation{duration: 200}
                }
                Behavior on scale {
                    NumberAnimation{duration: 200}
                }
            }
        }
        Text{
            id: typeIncomeText
            anchors.fill: parent
            text: "收入"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Config.incomeColor
        }
    }
    Rectangle{
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        color: Config.lineColor
        implicitHeight: Config.lineWidth
        implicitWidth: parent.width - Config.margin*4
    }
}
