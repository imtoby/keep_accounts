import QtQuick 2.3
import "Config"
import "Tools"

Item {
    id: headerView

    anchors{
        left: parent.left; leftMargin: Config.margin
        right: parent.right; rightMargin: Config.margin
        top: parent.top; topMargin: Config.margin
    }
    implicitHeight: Config.headHeight

    property alias amount: amountTotalObj.text
    property alias expenses: expensesTotalObj.text
    property alias income: incomeTotalObj.text

    property int month: monthObj.initDate.getMonth() + 1
    property int year: monthObj.initDate.getFullYear()

    signal clicked(int index)

    function show(){
        if(state !== ""){
            state = ""
        }
    }

    function hide(){
        if(state !== "hide"){
            state = "hide"
        }
    }

    states: [
        State {
            name: ""
            PropertyChanges { target: headerView; anchors.topMargin: Config.margin }
        },State {
            name: "hide"
            PropertyChanges { target: headerView; anchors.topMargin: -Config.headHeight }
        }
    ]

    transitions: Transition {
        from: "hide"; to: ""; reversible: true
        NumberAnimation { properties: "anchors.topMargin"; duration: 200 }
    }


    Rectangle{
        anchors.fill: parent
        color: "transparent"
        border{
            color: Config.lineColor
            width: Config.lineWidth
        }

        Rectangle{
            implicitHeight: Config.lineWidth
            implicitWidth: parent.width - Config.margin*2
            color: Config.lineColor
            anchors.centerIn: parent
            anchors.verticalCenterOffset: Config.headHeight / 14
        }

        Item{
            anchors{
                left: parent.left; leftMargin: Config.margin
                right: parent.right; rightMargin: Config.margin
            }
            implicitHeight: Config.headHeight * 4 / 7

            KButton{
                id: incomeTotalObjBtn
                width: parent.width/3
                height: parent.height
                onClicked: headerView.clicked(Config.indexIncome)

                Text{
                    width: parent.width
                    height: parent.height/2
                    text: "收入"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    color: Config.incomeColor
                }

                Text{
                    id: incomeTotalObj
                    width: parent.width
                    height: parent.height/2
                    anchors.bottom: parent.bottom
                    text: "0.00"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    color: Config.incomeColor
                }
            }


            Rectangle{
                anchors{
                    left: incomeTotalObjBtn.right
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
            }

            KButton{
                width: parent.width/3
                height: parent.height
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: headerView.clicked(Config.indexExpenses)

                Text{
                    width: parent.width
                    height: parent.height/2
                    text: "支出"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    color: Config.expensesColor
                }

                Text{
                    id: expensesTotalObj
                    width: parent.width
                    height: parent.height/2
                    anchors.bottom: parent.bottom
                    text: "0.00"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    color: Config.expensesColor
                }
            }

            Rectangle{
                anchors{
                    right: amountTotalObjBtn.left
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
            }

            KButton{
                id: amountTotalObjBtn
                width: parent.width/3
                height: parent.height
                anchors.right: parent.right
                onClicked: headerView.clicked(Config.indexAmount)

                Text{
                    width: parent.width
                    height: parent.height/2
                    text: "结余"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignBottom
                    color: Config.amountColor
                }

                Text{
                    id: amountTotalObj
                    width: parent.width
                    height: parent.height/2
                    anchors.bottom: parent.bottom
                    text: "0.00"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                    color: Config.amountColor
                }
            }


        }

        Item{
            anchors{
                left: parent.left; leftMargin: Config.margin
                right: parent.right; rightMargin: Config.margin
                bottom: parent.bottom
            }
            implicitHeight: Config.headHeight * 3 / 7

            KButton{
                width: parent.width*3/5
                height: parent.height
                onClicked: headerView.clicked(Config.indexMonth)

                Text{
                    id: monthObj
                    property var initDate: new Date
                    anchors.fill: parent
                    text: year + "/" + Config.padZero(month, 2)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle{
                anchors{
                    right: newObjMouseArea.left
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
            }

            KButton{
                id: newObjMouseArea
                width: parent.width*2/5
                height: parent.height
                anchors.right: parent.right
                onClicked: headerView.clicked(Config.indexNewAccount)

                Text{
                    id: newObj
                    anchors.fill: parent
                    text: "新增记录"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    color: Config.newAccountColor
                }
            }


        }
    }

    Connections {
        id: infoManagerConnections
        target: infoManager
        ignoreUnknownSignals: true

        function refreshTotalData() {
            incomeTotalObj.text = infoManager.incomeTotal();
            expensesTotalObj.text = infoManager.expensesTotal();
            amountTotalObj.text = infoManager.balanceTotal();
        }

        onInitRecordFinished: {
            infoManagerConnections.refreshTotalData();
        }

        onAddRecordFinished: {
            infoManagerConnections.refreshTotalData();
        }

        onDeleteRecordFinished: {
            infoManagerConnections.refreshTotalData();
        }

        onUpdateRecordFinished: {
            infoManagerConnections.refreshTotalData();
        }
    }
}
