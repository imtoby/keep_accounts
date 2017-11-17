import QtQuick 2.3
import "Config"
import "Tools"
import "Dialog"

ListView{
    id: topClassifyView
    width: parent.width/2
    height: parent.height
    clip: true

    readonly property string tag: "ClassifyListView"

    property string headerTitle: qsTr("一级项目")
    property string headerTitleRemark: qsTr("(点击添加)")

    readonly property string typeName:
        (listModel !== null) && (listModel.get(currentIndex) !== null) ?
            listModel.get(currentIndex).typeName : ""

    readonly property string typeId:
        (listModel !== null) && (listModel.get(currentIndex) !== null) ?
            listModel.get(currentIndex).typeId : ""

    readonly property string parentId:
        (listModel !== null) && (listModel.get(currentIndex) !== null) ?
            listModel.get(currentIndex).parentId : ""

    property int typeChildrenCount: 0

    property QtObject listModel: null

    property int inOrOutType: Config.out_type

    function hideHeaderTitle(){
        headerItem.hide()
        rejected()
    }

    signal headerTitleShown()
    signal accepted(string text)
    signal rejected()
    signal typeChanged()

    model: listModel

    delegate: KButton {
        width: topClassifyView.width
        height: 40
        enabled: currentIndex !== index

        readonly property string typeId: model.modelData.typeId
        readonly property string type: model.modelData.type
        readonly property string parentId: model.modelData.parentId
        readonly property string typeName: model.modelData.typeName

        Text {
            anchors.fill: parent
            anchors.leftMargin: 2*Config.margin
            verticalAlignment: Text.AlignVCenter
            text: model.modelData.typeName
            opacity: currentIndex !== index
        }

        Rectangle{
            width: topClassifyView.width - 2*Config.margin
            height: Config.lineWidth
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: Config.lineColor
        }

        onClicked: {
            currentIndex = index
            typeChanged()
        }
    }

    highlight: Item {
        width: topClassifyView.width
        height: 40
        y: topClassifyView.currentItem.y

        Behavior on y {
            SpringAnimation {
                spring: 5
                damping: 0.2
            }
        }

        Rectangle {
            anchors.fill: parent
            anchors.margins: 6
            color: Config.lineColor
        }

        TextInput {
            id: typeEditInput
            anchors.fill: parent
            clip: true
            anchors.leftMargin: Config.margin*2
            anchors.rightMargin: Config.margin*2
            horizontalAlignment: TextInput.AlignLeft
            verticalAlignment: TextInput.AlignVCenter
            color: Config.balanceColor
            text: topClassifyView.currentItem.typeName

            property string sourceText: text

            onFocusChanged: {
                if (focus) {
                    sourceText = text
                    dialog.currentEditIndex = currentIndex
                    dialog.currentEditType = topClassifyView.currentItem.type
                    dialog.currentEditTypeId
                            = topClassifyView.currentItem.typeId
                    dialog.currentEditParentId
                            = topClassifyView.currentItem.parentId
                }
            }

            onEditingFinished: {
                console.log(tag,"onEditingFinished");
                if (text.length > 0) {
                    focus = false
                    if (sourceText !== text) {
                        console.log(tag,"accept edit");
                        infoManager.setTypeName(inOrOutType, typeId,
                                                parentId, text);
                    }
                } else if ((text.length == 0) && focus) {
                    dialog.open()
                }
            }
        }

        KDialog {
            id: dialog
            parent: rootPage
            message: qsTr("清空文字内容将会删除当前类别<\ br>" +
                          "（包括其子类别），<\ br>确定要删除它吗？")

            property int currentEditIndex: -1
            property int currentEditType: Config.out_type
            property string currentEditTypeId: ""
            property string currentEditParentId: ""

            onAccepted: {
                infoManager.deleteType(currentEditIndex, currentEditType,
                                       currentEditTypeId, currentEditParentId)
            }

            onRejected: {
                typeEditInput.text = Qt.binding(function(){
                    return topClassifyView.currentItem.typeName})
                typeEditInput.focus = false
            }

        }

        Connections {
            target: infoManager
            onDeleteTypeFinished: {
                dialog.close()
            }
        }
    }

    footer: Item {
        width: topClassifyView.width
        height: Config.lineWidth

        Rectangle{
            width: topClassifyView.width - 2*Config.margin
            height: Config.lineWidth
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            color: Config.lineColor
            opacity: count > 0
        }
    }

    header: Item {
        id: topBtn
        width: topClassifyView.width
        height: 50

        function hide(){
            topBtn.state = ""

            topClassifyView.model = Qt.binding(function(){return listModel})
        }

        function show(){
            topBtn.state = "show"

            topClassifyView.model = null
        }

        KButton {
            id: topTxt
            width: topClassifyView.width
            height: 50
            Text {
                id: topLabel
                anchors.fill: parent
                text: headerTitle + headerTitleRemark
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
            }

            onClicked: {
                headerTitleShown()
                topBtn.show()
            }

            Rectangle{
                id: topLine
                width: 0
                height: Config.lineWidth
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                color: Config.lineColor
            }
        }

        Item{
            id: addTopArea
            anchors.fill: parent
            anchors.topMargin: topTxt.height - Config.margin
            opacity: 0

            Item{
                width: parent.width
                height: parent.height/9
                anchors.centerIn: parent

                TextInput{
                    id: classifyInput
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 4*Config.margin
                    anchors.rightMargin: 4*Config.margin
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    color: Config.balanceColor

                    Text {
                        id: placeholderTextComponent
                        anchors.fill: parent
                        opacity: !classifyInput.text.length && !classifyInput.activeFocus ? 1 : 0
                        color: Config.lineColor
                        font: classifyInput.font
                        horizontalAlignment: classifyInput.horizontalAlignment
                        verticalAlignment: classifyInput.verticalAlignment
                        elide: Text.ElideRight
                        text: "新" + headerTitle
                        Behavior on opacity { NumberAnimation { duration: 200 } }
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

            KButton {
                id: topCancelBtn
                width: parent.width/2
                height: 50
                anchors.bottom: parent.bottom
                enabled: addTopArea.opacity
                Text {
                    anchors.fill: parent
                    text: qsTr("取消")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    topBtn.hide()
                    rejected()
                }
            }

            KButton {
                id: topOkBtn
                width: parent.width/2
                height: 50
                anchors.bottom: parent.bottom
                anchors.left: topCancelBtn.right
                enabled: addTopArea.opacity
                Text {
                    anchors.fill: parent
                    text: qsTr("确定")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    topBtn.hide()
                    if(classifyInput.length > 0){
                        accepted(classifyInput.text)
                        classifyInput.text = ""
                    }
                }
            }

        }

        states: [
            State {
                name: "show"
                PropertyChanges {
                    target: topBtn;
                    height: topClassifyView.height
                }
                PropertyChanges {
                    target: topLine;
                    width: topLine.parent.width - Config.margin*2
                }
                PropertyChanges { target: addTopArea; opacity: 1 }
                PropertyChanges { target: topTxt; enabled: false }
                PropertyChanges { target: topLabel; text: headerTitle }
            },State {
                name: ""
                PropertyChanges { target: topLine; width: 0 }
                PropertyChanges {
                    target: topLabel;
                    text: headerTitle + headerTitleRemark
                }
                PropertyChanges { target: topTxt; enabled: true }
                PropertyChanges { target: addTopArea; opacity: 0 }
                PropertyChanges { target: topBtn; height: 50 }
            }
        ]

        transitions: [
            Transition {
                from: ""; to: "show"
                NumberAnimation { properties: "height"; duration: 400 }
                NumberAnimation { properties: "opacity"; duration: 400 }
                NumberAnimation { properties: "width"; duration: 400 }
                ScriptAction { script: classifyInput.focus = false }
            },Transition {
                from: "show"; to: ""
                NumberAnimation { properties: "width"; duration: 400 }
                NumberAnimation { properties: "opacity"; duration: 400 }
                NumberAnimation { properties: "height"; duration: 400 }
            }
        ]

    }

    add: Transition {
        NumberAnimation { properties: "x,y"; from: 0; duration: 200 }
    }

    remove: Transition {
        ParallelAnimation {
            NumberAnimation { property: "opacity"; to: 0; duration: 200 }
            NumberAnimation { properties: "x,y"; to: 0; duration: 200 }
        }
    }

    highlightFollowsCurrentItem: true
}

