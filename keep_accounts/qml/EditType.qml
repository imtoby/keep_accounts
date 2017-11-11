import QtQuick 2.3
import "Config"
import "Tools"

Item {
    id: classifyEdit

    readonly property bool isExpenses: typeContainer.isExpensesType
    readonly property int typeValue:
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

            listModel: infoManager.typeModel(typeValue, Config.topTypeId)

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

            property string lastTypeUuid: ""

//            function initTopItems(){
//                console.log("initTopItems")
//                clear();
//                var a = typeManager.getTypeInfo(typeValue, Config.topTypeId);

//                for(var i=0; i<a.length; ++i){

//                    var item = a[i];
//                    var uuid = item.substring(0, Config.typeIdLength)
//                    var name = item.substring(Config.typeIdLength)
//                    addType(name, uuid);
//                }

//                if(a.length > 0){
//                    currentIndex = 0
//                    typeChildrenCount = initChildItems(
//                                a[0].substring(0, Config.typeIdLength) )
//                }else{
//                    childClassifyListView.clear()
//                }
//            }

//            function initChildItems(tUuid){
//                childClassifyListView.clear()
//                var a = typeManager.getTypeInfo(typeValue, tUuid);
//                for(var i=0; i<a.length; ++i){
//                    var item = a[i];
//                    var uuid = item.substring(0, Config.typeIdLength)
//                    var name = item.substring(Config.typeIdLength)
//                    childClassifyListView.addType(name, uuid);
//                }

//                if(a.length > 0){
//                    childClassifyListView.currentIndex = 0
//                }

//                return a.length
//            }

            onHeaderTitleShown: {
                childClassifyListView.hideHeaderTitle()
            }

            onAccepted: {
//                typeManager.addType(text, typeValue, Config.topTypeId);
            }

            onTypeChanged: {
                if(lastTypeUuid !== typeUuid){
                    childClassifyListView.currentIndex = 0
                }

                lastTypeUuid = typeUuid
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
            listModel: infoManager.typeModel(typeValue,
                                             topClassifyListView.typeUuid)

            onAccepted: {
//                typeManager.addType(text, typeValue,
//                                    topClassifyListView.typeUuid);
            }
        }
    }

//    Connections {
//        target: typeManager
//        ignoreUnknownSignals: true
//        onInitTypeInfoFinished : {
//            topClassifyListView.initTopItems()
//        }
//        onAddTopTypeFinished : {
//            topClassifyListView.addType(typeName, typeId);
//        }
//        onAddChildTypeFinished : {
//            childClassifyListView.addType(typeName, typeId);
//        }
//    }

//    onTypeValueChanged: {
//        typeManager.initData();
//    }

//    Component.onCompleted: {
//        typeManager.initData();
//    }

}
