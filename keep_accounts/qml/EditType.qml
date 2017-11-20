import QtQuick 2.3
import "Config"
import "Tools"

Item {
    id: classifyEdit

    readonly property bool isExpenses: typeContainer.isExpensesType
    readonly property int inOrOutType:
        isExpenses ? Config.out_type : Config.in_type

    Rectangle{
        anchors.fill: parent
        anchors.margins: Config.margin
        color: "transparent"
        border{
            width: Config.lineWidth
            color: Config.lineColor
        }
    }

    TypeSwitch{
        id: typeContainer
    }

    Row{
        id: classifyContainer
        anchors.fill: parent
        anchors.margins: Config.margin
        anchors.topMargin: typeContainer.height + Config.margin
        clip: true

        EditTypeView {
            id: topView

            width: parent.width
            state: count > 0 ? "showChild" : ""
            inOrOutType: classifyEdit.inOrOutType

            states: [
                State {
                    name: "showChild"
                    PropertyChanges {
                        target: topView; width: parent.width/2
                    }
                    PropertyChanges { target: divLine; opacity: 1 }
                    PropertyChanges {
                        target: childView; opacity: 1
                    }
                },State {
                    name: ""
                    PropertyChanges {
                        target: topView; width: parent.width
                    }
                    PropertyChanges { target: divLine; opacity: 0 }
                    PropertyChanges {
                        target: childView; opacity: 0
                    }
                }
            ]

            transitions: [
                Transition {
                    from: ""; to: "showChild"
                    NumberAnimation { properties: "width"; duration: 400 }
                    NumberAnimation { properties: "opacity"; duration: 400 }
                },Transition {
                    from: "showChild"; to: ""
                    NumberAnimation { properties: "opacity"; duration: 400 }
                    NumberAnimation { properties: "width"; duration: 400 }
                }
            ]

            property string lastTypeId: ""

            onHeaderTitleShown: {
                childView.hideHeaderTitle()
            }

            onAccepted: {
                delayAddTimer.startTimer(text, inOrOutType, Config.topTypeId);
            }

            onTypeChanged: {
                if(lastTypeId !== typeId){
                    childView.currentIndex = 0
                }

                lastTypeId = typeId
            }
        }

        Rectangle{
            id: divLine
            width: Config.lineWidth
            height: parent.height - Config.margin*2
            color: Config.lineColor
            anchors.verticalCenter: parent.verticalCenter
            opacity: 0
        }

        EditTypeView {
            id: childView
            enabled: topView.currentIndex > -1
            headerTitle: qsTr("子项目")
            onHeaderTitleShown: {
                topView.hideHeaderTitle()
            }
            opacity: 0
            inOrOutType: classifyEdit.inOrOutType
            onAccepted: {
                delayAddTimer.startTimer(text, inOrOutType,
                                         topView.typeId);
            }
        }
    }

    Timer {
        id: delayAddTimer
        property string __name: ""
        property int __type: inOrOutType
        property string __parentId: ""

        function startTimer(name, type, parentId) {
            __name = name
            __type = type
            __parentId = parentId
            start()
        }

        interval: 450
        onTriggered: {
            infoManager.addType(__name, __type, __parentId);
        }
    }

    Connections {
        target: infoManager
        ignoreUnknownSignals: true
        onInitTypeFinished : {
            topView.listModel =
                    Qt.binding(function(){
                        return infoManager.editTypeModel(inOrOutType,
                                                         Config.topTypeId) })
            childView.listModel =
                    Qt.binding(function(){
                        return infoManager.editTypeModel(inOrOutType,
                                                         topView.typeId) })

        }

        onAddTypeFinished : {
            if (Config.topTypeId == parentId) {
                topView.currentIndex = size - 1;
            } else {
                childView.currentIndex = size - 1;
            }
        }
    }

    Component.onCompleted: {
        infoManager.initData();
    }

}
