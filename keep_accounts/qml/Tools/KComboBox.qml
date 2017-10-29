import QtQuick 2.3
import "../Config"

Item {
    id: comboBox

    property alias model: popView.model
    property alias delegate: popView.delegate
    property alias buttonText: contentTxt.text
    property alias buttonColor: contentTxt.color
    property int currentIndex: -1
    property color mainColor: Config.newAccountColor
    property bool borderVisible: true
    property bool dividerVisible: true

    readonly property real contentButtonWidth: contentBtn.width
    readonly property bool popuped: popView.state === "visible"

    signal clicked(int index)
    signal clearOtherPop()

    function hidePop(){
        popView.state = ""
    }

    Rectangle{
        id: bgArea
        anchors.fill: parent
        color: "transparent"
        border.width: borderVisible ? Config.lineWidth : 0
        border.color: Config.lineColor

        KButton{
            id: leftBtn
            height: parent.height
            width: height
            onClicked: {
                clearOtherPop()
                popView.state = ""
                if(0 === currentIndex){
                    return comboBox.clicked(popView.count - 1)
                }
                comboBox.clicked(Math.max(currentIndex - 1, 0))
            }

            Rectangle{
                anchors{
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
                visible: dividerVisible
            }

            Text {
                anchors.centerIn: parent
                text: "<"
                color: mainColor
            }
        }

        KButton{
            id: contentBtn
            height: parent.height
            anchors.left: leftBtn.right
            anchors.leftMargin: borderVisible ? -Config.lineWidth : 0
            anchors.right: rightBtn.left
            onClicked: {
                popView.setCurrentIndex(currentIndex)
                popView.state = "visible"
                clearOtherPop()
            }

            Text {
                id: contentTxt
                anchors.centerIn: parent
                color: mainColor
            }
        }

        PathView{
            id: popView
            width: contentButtonWidth
            height: contentBtn.height*5
            anchors.centerIn: contentBtn

            pathItemCount: 5
            preferredHighlightBegin: 0.5
            preferredHighlightEnd: 0.5
            highlightRangeMode: PathView.StrictlyEnforceRange
            snapMode: PathView.SnapToItem
            flickDeceleration: 100
            clip: true
            opacity: 0
            enabled: false

            function setCurrentIndex(index) {
                positionViewAtIndex(index, PathView.Center)
            }

            states: State {
                name: "visible"
                PropertyChanges { target: popView; opacity: 1 }
                PropertyChanges { target: contentTxt; opacity: 0 }
                PropertyChanges { target: popView; enabled: true }
            }

            transitions: Transition {
                from: "visible"; to: ""; reversible: true
                NumberAnimation { properties: "opacity"; duration: 400 }
            }

            path: Path{
                startX: popView.width/2; startY: 0;
                PathLine { x: popView.width/2; y: popView.height }
            }

            BorderImage {
                source: "qrc:/res/00.png"
                anchors.fill: parent
                border.left: 5; border.top: 5
                border.right: 5; border.bottom: 5
                horizontalTileMode: BorderImage.Repeat
                verticalTileMode: BorderImage.Repeat
            }

            Rectangle{
                anchors.fill: parent
                color: "transparent"
                border.width: Config.lineWidth
                border.color: Config.lineColor
            }
        }

        KButton{
            id: rightBtn
            height: parent.height
            width: height
            anchors.right: parent.right
            onClicked: {
                clearOtherPop()
                popView.state = ""
                if(popView.count - 1 === currentIndex){
                    return comboBox.clicked(0)
                }
                comboBox.clicked(Math.min(currentIndex + 1, popView.count - 1))
            }

            Rectangle{
                anchors{
                    right: parent.left
                    verticalCenter: parent.verticalCenter
                }
                color: Config.lineColor
                implicitHeight: 20
                implicitWidth: Config.lineWidth
                visible: dividerVisible
            }

            Text {
                anchors.centerIn: parent
                text: ">"
                color: mainColor
            }
        }
    }

    onClicked: {
        currentIndex = index
        popView.state = ""
    }

}
