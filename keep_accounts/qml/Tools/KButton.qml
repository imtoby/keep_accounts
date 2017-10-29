import QtQuick 2.3
import "../Config"

MouseArea {

    property alias background: backgroundObj

    Rectangle {
        id: backgroundObj
        anchors.fill: parent
        anchors.margins: Config.margin
        color: Config.backgroundColor
        opacity: 0

        states: State {
            name: "visible"
            PropertyChanges { target: backgroundObj; opacity: 0.8 }
        }

        transitions: Transition {
            from: "visible"; to: ""; reversible: true
            NumberAnimation { properties: "opacity"; duration: 400 }
        }
    }

    onPressed: backgroundObj.state  = "visible"
    onReleased: backgroundObj.state = ""
    onCanceled: backgroundObj.state = ""
}
