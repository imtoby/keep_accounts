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

        ClassifyListView {
            id: topClassifyListView

            width: parent.width
            state: count > 0 ? "showChild" : ""
            inOrOutType: classifyEdit.inOrOutType

            states: [
                State {
                    name: "showChild"
                    PropertyChanges {
                        target: topClassifyListView; width: parent.width/2
                    }
                    PropertyChanges { target: divLine; opacity: 1 }
                    PropertyChanges {
                        target: childClassifyListView; opacity: 1
                    }
                },State {
                    name: ""
                    PropertyChanges {
                        target: topClassifyListView; width: parent.width
                    }
                    PropertyChanges { target: divLine; opacity: 0 }
                    PropertyChanges {
                        target: childClassifyListView; opacity: 0
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
                childClassifyListView.hideHeaderTitle()
            }

            onAccepted: {
                infoManager.addType(text, inOrOutType, Config.topTypeId);
            }

            onTypeChanged: {
                if(lastTypeId !== typeId){
                    childClassifyListView.currentIndex = 0
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

        ClassifyListView{
            id: childClassifyListView
            enabled: topClassifyListView.currentIndex > -1
            headerTitle: qsTr("子项目")
            onHeaderTitleShown: {
                topClassifyListView.hideHeaderTitle()
            }
            opacity: 0
            inOrOutType: classifyEdit.inOrOutType
            onAccepted: {
                infoManager.addType(text, inOrOutType,
                                    topClassifyListView.typeId);
            }
        }
    }

    Connections {
        target: infoManager
        ignoreUnknownSignals: true
        onInitTypeFinished : {
            topClassifyListView.listModel =
                    Qt.binding(function(){
                        return infoManager.typeModel(inOrOutType,
                                                     Config.topTypeId) })
            childClassifyListView.listModel =
                    Qt.binding(function(){
                        return infoManager.typeModel(inOrOutType,
                                                     topClassifyListView.typeId) })

        }

        onAddTypeFinished : {
            if (Config.topTypeId == parentId) {
                topClassifyListView.currentIndex = size - 1;
            } else {
                childClassifyListView.currentIndex = size - 1;
            }
        }
    }

    Component.onCompleted: {
        infoManager.initData();
    }

}
