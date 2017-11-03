import QtQuick 2.3
import com.imtoby.keep_accounts 1.0
import "Config"
import "Tools"

Item {
    id: classifyEdit

    readonly property bool isExpenses: typeContainer.isExpensesType
    readonly property int typeValue: isExpenses ? Config.expensesClassify
                                         : Config.incomeClassify

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

        ClassifyListView{
            id: topClassifyListView

            width: parent.width

            state: count > 0 ? "showChild" : ""

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

            function initTopItems(){
                clear();
                var a = typeManager.getTypeInfo(typeValue, "");

                for(var i=0; i<a.length; ++i){
                    var item = a[i];
                    var uuid = item.substring(0, 36)
                    var name = item.substring(36)
                    addType(name, uuid);
                }

                if(a.length > 0){
                    currentIndex = 0
                    typeChildrenCount = initChildItems(a[0].substring(0, 36) )
                }else{
                    childClassifyListView.clear()
                }
            }

            function initChildItems(tUuid){
                childClassifyListView.clear()
                var a = typeManager.getTypeInfo(typeValue, tUuid);
                for(var i=0; i<a.length; ++i){
                    var item = a[i];
                    var uuid = item.substring(0, 36)
                    var name = item.substring(36)
                    childClassifyListView.addType(name, uuid);
                }

                if(a.length > 0){
                    childClassifyListView.currentIndex = 0
                }

                return a.length
            }

            onHeaderTitleShown: {
                childClassifyListView.hideHeaderTitle()
            }

            onAccepted: {
                var uuid = typeManager.addType(text, typeValue, "");

                addType(text, uuid);
            }

            onTypeChanged: {
                if(lastTypeUuid !== typeUuid){
                    typeChildrenCount = initChildItems(typeUuid)
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

            onAccepted: {
                var uuid = typeManager.addType(text, typeValue,
                                               topClassifyListView.typeUuid);

                addType(text, uuid);
            }
        }
    }

    onTypeValueChanged: {
        topClassifyListView.initTopItems()
    }

}
