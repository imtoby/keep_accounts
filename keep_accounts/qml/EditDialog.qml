import QtQuick 2.3
import "Config"
import "Tools"
import "Dialog"

MouseArea {
    id: editDialog
    anchors.fill: parent
    enabled: false

    readonly property bool isAddRecord: backgroundObj.isAddRecord

    readonly property bool isExpenses: typeContainer.isExpensesType

    readonly property int inOrOutType:
        isExpenses ? Config.out_type : Config.in_type

    property alias dateText: dateTxt.text

    function show(isAddRecord, recordIndex, params) {
        backgroundObj.isAddRecord
                = (typeof isAddRecord != "undefined") ? isAddRecord : true;
        if (typeof recordIndex != "undefined") {
            backgroundObj.recordIndex = recordIndex;
        } else {
            backgroundObj.recordIndex = -1;
        }
        if (typeof params != "undefined") {
            typeContainer.isExpensesType = (params.type === Config.out_type);
            dateTxt.text = (params.year + Config.dateSeparator
                            + params.month + Config.dateSeparator
                            + params.day);
            amountInput.text = params.amount;
            remarkInput.text = params.note;
            typeContent.text = params.childType;
            backgroundObj.millonSecs = params.millonSecs;
            typeContent.parentId = params.parentType;
            typeContent.typeId = params.childType;
        }
        clearInputFocus();
        state = "show";
    }

    function hide() {
        state = "";
    }

    function clearInputFocus(){
        remarkInput.focus = false
        amountInput.focus = false
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: backgroundObj; opacity: 0.3 }
            PropertyChanges { target: contentBackground; anchors.verticalCenterOffset: 0 }
            PropertyChanges { target: editDialog; enabled: true }
        },State {
            name: ""
            PropertyChanges { target: editDialog; enabled: false }
            PropertyChanges { target: contentBackground; anchors.verticalCenterOffset: height }
            PropertyChanges { target: backgroundObj; opacity: 0 }
        }
    ]

    transitions: [
        Transition {
            from: ""; to: "show"
            NumberAnimation { properties: "opacity"; duration: 400 }
            NumberAnimation { properties: "anchors.verticalCenterOffset"; duration: 400 }
        },Transition {
            from: "show"; to: ""
            NumberAnimation { properties: "anchors.verticalCenterOffset"; duration: 400 }
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    ]

    Rectangle{
        id: backgroundObj
        property bool isAddRecord: true
        property double millonSecs: 0
        property int recordIndex: -1
        property string childType: ""
        property string parentType: ""
        anchors.fill: parent
        color: "black"
        opacity: 0
    }

    BorderImage {
        id: contentBackground
        source: "qrc:/res/00.png"
        width: parent.width*4/5
        height: parent.height/2
        anchors.centerIn: parent
        anchors.verticalCenterOffset: parent.height
        border.left: 5; border.top: 5
        border.right: 5; border.bottom: 5
        horizontalTileMode: BorderImage.Repeat
        verticalTileMode: BorderImage.Repeat
        clip: true

        MouseArea {
            id: kernalArea
            width: parent.width
            height: parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0

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

                onClearInputFocus:{
                    editDialog.clearInputFocus()
                }
            }

            Item{
                id: amountContainer
                width: parent.width
                height: parent.height/7
                anchors.top: typeContainer.bottom

                TextInput{
                    id: amountInput
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 4*Config.margin
                    anchors.rightMargin: 4*Config.margin
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter
                    color: Config.amountColor
                    font.pixelSize: 32
                    validator: RegExpValidator{
                        regExp: /^[1-9]\d{0,8}([.]\d{0,2})?$/
                    }

                    Text {
                        id: placeholderTextComponent
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        opacity: !amountInput.text.length && !amountInput.activeFocus ? 1 : 0
                        color: amountInput.color
                        font: amountInput.font
                        horizontalAlignment: amountInput.horizontalAlignment
                        verticalAlignment: amountInput.verticalAlignment
                        clip: true
                        text: "0"
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

            Item{
                id: contentContainer
                width: parent.width
                height: parent.height/7
                anchors.top: amountContainer.bottom

                KButton{
                    anchors.fill: parent
                    anchors.leftMargin: Config.margin
                    anchors.rightMargin: Config.margin
                    onClicked: {
                        selectTypeDialog.open()
                    }

                    Text{
                        id: typeContent
                        property string typeId: ""
                        property string parentId: ""

                        anchors.fill: parent
                        text: "分类"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Config.newAccountColor
                    }

                    KButton {
                        width: parent.height
                        height: width
                        anchors.right: parent.right
                        Image {
                            width: parent.width/2
                            height: width
                            anchors.centerIn: parent
                            sourceSize: Qt.size(parent.width/2, parent.width/2)
                            source: "qrc:/res/setting.png"
                        }
                        onClicked: {
                            editTypDialog.open();
                        }
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

            Item{
                id: dateContainer
                width: parent.width
                height: parent.height/7
                anchors.top: contentContainer.bottom

                KButton{
                    anchors.fill: parent
                    anchors.leftMargin: Config.margin
                    anchors.rightMargin: Config.margin
                    onClicked: {
                        clearInputFocus()
                        var currentDate = new Date(dateTxt.text);
                        calendar.year = currentDate.getFullYear();
                        calendar.month = currentDate.getMonth() + 1;
                        calendar.day = currentDate.getDate();
                        contentBackground.state = "calendar"
                    }

                    Text{
                        id: dateTxt
                        anchors.fill: parent
                        text: (new Date()).toLocaleDateString(Config.locale,
                                                              Config.dateFormat)
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Config.newAccountColor
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

            Item{
                id: remarkContainer
                width: parent.width
                height: parent.height/7
                anchors.top: dateContainer.bottom

                TextInput{
                    id: remarkInput
                    anchors.fill: parent
                    clip: true
                    anchors.leftMargin: 4*Config.margin
                    anchors.rightMargin: 4*Config.margin
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: TextInput.AlignVCenter

                    Text {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        opacity: !remarkInput.text.length && !remarkInput.activeFocus ? 1 : 0
                        color: "#666666"
                        font: remarkInput.font
                        horizontalAlignment: remarkInput.horizontalAlignment
                        verticalAlignment: remarkInput.verticalAlignment
                        clip: true
                        text: "描述"
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

            Item{
                width: parent.width
                height: parent.height/7
                anchors.bottom: parent.bottom
                anchors.bottomMargin: Config.margin

                KButton{
                    width: parent.width/2 - Config.margin
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: Config.margin
                    onClicked: {
                        hide()
                    }

                    Text{
                        anchors.fill: parent
                        text: "取消"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Config.newAccountColor
                    }
                }

                Rectangle{
                    implicitHeight: 20
                    implicitWidth: Config.lineWidth
                    anchors.centerIn: parent
                    color: Config.lineColor
                }

                KButton{
                    width: parent.width/2 - Config.margin
                    height: parent.height
                    anchors.right: parent.right
                    anchors.rightMargin: Config.margin
                    onClicked: {
                        hide();

                        if (isAddRecord) {
                            infoManager.addRecord(inOrOutType,
                                                  typeContent.parentId,
                                                  typeContent.typeId,
                                                  dateTxt.text,
                                                  amountInput.text * 1.0,
                                                  remarkInput.text);
                        } else {
                            infoManager.updateRecord(backgroundObj.recordIndex,
                                                     inOrOutType,
                                                     typeContent.parentId,
                                                     typeContent.typeId,
                                                     dateTxt.text,
                                                     amountInput.text * 1.0,
                                                     remarkInput.text);
                        }
                    }

                    Text{
                        anchors.fill: parent
                        text: "保存"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Config.newAccountColor
                    }
                }
            }
        }

        MouseArea{
            id: calendarArea
            width: parent.width
            height: parent.height
            anchors.left: kernalArea.right
            opacity: 0

            YearMonthDayPicker {
                id: calendar
                width: 250
                height: 220
                anchors.fill: undefined
                anchors.centerIn: parent
            }

            KButton {
                id: cancelBtn
                width: parent.width/2
                height: 50
                anchors.bottom: parent.bottom
                Text {
                    anchors.fill: parent
                    text: qsTr("取消")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    contentBackground.state = "";
                }
            }

            KButton {
                id: okBtn
                width: parent.width/2
                height: 50
                anchors.bottom: parent.bottom
                anchors.left: cancelBtn.right
                Text {
                    anchors.fill: parent
                    text: qsTr("确定")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }
                onClicked: {
                    var selectedDate = new Date(calendar.year, calendar.month - 1, calendar.day);
                    dateTxt.text = selectedDate.toLocaleDateString(Config.locale, Config.dateFormat);
                    contentBackground.state = "";
                }
            }
        }

        states: [
            State {
                name: "calendar"
                PropertyChanges { target: kernalArea; anchors.leftMargin: -contentBackground.width }
                PropertyChanges { target: calendarArea; opacity: 1 }
                PropertyChanges { target: kernalArea; opacity: 0 }
            },State {
                name: ""
                PropertyChanges { target: kernalArea; anchors.leftMargin: 0 }
                PropertyChanges { target: calendarArea; opacity: 0 }
                PropertyChanges { target: kernalArea; opacity: 1 }
            }
        ]

        transitions: [
            Transition {
                from: ""; to: "calendar"
                NumberAnimation { properties: "opacity"; duration: 400 }
                NumberAnimation { properties: "anchors.leftMargin"; duration: 400 }
                ScriptAction{
                    script: {
                        contentBackground.clip = false;
                        kernalArea.visible = false;
                    }
                }
            },Transition {
                from: "calendar"; to: ""
                NumberAnimation { properties: "opacity"; duration: 400 }
                NumberAnimation { properties: "anchors.leftMargin"; duration: 400 }
                ScriptAction{
                    script: {
                        contentBackground.clip = true;
                        kernalArea.visible = true;
                    }
                }
            }
        ]

    }

    onClicked: {
        if(contentBackground.state === "calendar"){
            contentBackground.state = ""
        }else{
            hide()
        }
    }

    KSelectTypeDialog {
        id: selectTypeDialog
        inOrOutType: editDialog.inOrOutType
        onClicked: {
            typeContent.typeId = infoManager.getTypeName(typeId);
            var name = typeContent.typeId;
            if (parentId != Config.topTypeId) {
                typeContent.parentId = infoManager.getTypeName(parentId);
                name = typeContent.parentId;
                name = name + "-" + typeContent.typeId;
            } else {
                typeContent.parentId = Config.topTypeId;
            }
            typeContent.text = name;
        }
    }

    KContainerDialog {
        id: editTypDialog
        dialogWidth: parent.width*4/5
        dialogHeight: parent.height/2
        content: EditType {
        }

        onClosed: {
            infoManager.refreshSelectTypeData();
        }
    }

}
