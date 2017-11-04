import QtQuick 2.3
import "Config"
import "Tools"

ListView{
    id: topClassifyView
    width: parent.width/2
    height: parent.height
    clip: true
    currentIndex: -1

    property string headerTitle: qsTr("一级项目")
    property string headerTitleRemark: qsTr("(点击添加)")
    readonly property string typeName: listModel.typeName
    readonly property string typeUuid: listModel.typeUuid
    property int typeChildrenCount: 0

    function hideHeaderTitle(){
        headerItem.hide()
        rejected()
    }

    function addType(tName, tUuid) {
        if(typeName === ""){
            listModel.typeName = tName
        }
        listModel.append({"name":tName,"uuid":tUuid})
        console.log("addType: ", tName, tUuid)
    }

    function clear(){
        listModel.clear()
        listModel.typeName = ""
        listModel.typeUuid = ""
        typeChildrenCount = 0
    }

    signal headerTitleShown()
    signal accepted(string text)
    signal rejected()
    signal typeChanged()

    ListModel{
        id: listModel
        property string typeName: ""
        property string typeUuid: ""
    }

    model: listModel

    delegate: KButton {
        width: topClassifyView.width
        height: 40
        enabled: currentIndex !== index

        Text {
            anchors.fill: parent
            anchors.leftMargin: 2*Config.margin
            verticalAlignment: Text.AlignVCenter
            text: (currentIndex === index)
                  ? "" : ( (typeof name != "undefined") ? name : "" )
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

            if(typeof name != "undefined"){
                listModel.typeName = name
            }
            if(typeof uuid != "undefined"){
                listModel.typeUuid = uuid
            }

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
            anchors.margins: -1
            color: Config.lineColor
        }

        TextInput{
            id: typeEditInput
            anchors.fill: parent
            clip: true
            anchors.leftMargin: Config.margin*2
            anchors.rightMargin: Config.margin*2
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: TextInput.AlignVCenter
            color: Config.balanceColor
            text: typeName
        }

        Keys.onReleased: {
            if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter){
                typeEditInput.focus = false

                if(typeEditInput.length === 0){
                    if(typeChildrenCount > 0){
                        console.log("typeChildrenCount: ", typeChildrenCount)
                    }else{
                        console.log("to do show item dialog")
                    }
                }
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

            topClassifyView.model = listModel
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

