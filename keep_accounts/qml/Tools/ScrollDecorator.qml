import QtQuick 2.3

Item {
    property Flickable flickableItem: null
    property alias indicatorWidth: indicator.width

    readonly property bool show: flickableItem.contentHeight > flickableItem.height

    // Private
    anchors.fill: parent
    anchors.rightMargin: 2

    Rectangle{
        id: indicator
        y: show ? sizer.position() : 0
        width: 6
        radius: width/2
        height: show ? Math.max((sizer.size() - parent.anchors.topMargin - parent.anchors.bottomMargin), sizer.min) : 0
        anchors.right: parent.right
        color: "black"
        opacity: 0

        states: State {
            name: "visible"
            when: show && flickableItem.moving
            PropertyChanges { target: indicator; opacity: 0.8 }
        }

        transitions: Transition {
            from: "visible"; to: ""
            NumberAnimation { properties: "opacity"; duration: 1500 }
        }
    }

    QtObject {
        id: sizer
        property real pos: flickableItem.visibleArea.yPosition
        property real ratio: flickableItem.visibleArea.heightRatio
        property real max: flickableItem.height
        property real min: 20
        property real over: Math.max(min - (ratio * max), 0)


        function position(){
            return 0.5 + Math.min(max - min,
                                  pos * (max - over)
                                  + Math.max(pos * (over - max), 0))
        }

        function size(){
            var rawEndPos = (pos + ratio) * (max - over) + over

            return 0.5 + Math.max(rawEndPos - Math.max(pos * (over - max), 0)
                                  - Math.max(rawEndPos - max, 0) - position(), min)
        }
    }

    Timer{
        interval: 100

        property real height: flickableItem.height
        onHeightChanged: restart()

        onTriggered: {
            if(show){
                indicator.state = "visible"
                indicator.state = ""
            }
        }
    }

}
