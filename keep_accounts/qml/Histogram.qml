import QtQuick 2.0

Item {
    id: histogram

    width: parent.width
    height: parent.height/4.0

    property alias count: repeaterObj.count
    property alias currentIndex: mouseAreaObj.currentIndex
    property alias dayInfo: dayInfoObj.text
    property alias model: repeaterObj.model

    Row {
        anchors.fill: parent
        anchors.leftMargin: mouseAreaObj.itemWidth
        anchors.rightMargin: mouseAreaObj.itemWidth
        anchors.topMargin: mouseAreaObj.dayInfoHeight
        spacing: 0

        Repeater {
            id: repeaterObj
            Item {
                width: mouseAreaObj.itemWidth
                height: histogram.height - mouseAreaObj.dayInfoHeight

                Rectangle {
                    id: ballRect
                    width: mouseAreaObj.ballRectWidth
                    height: width
                    color: index === currentIndex ? "black" : "transparent"
                    radius: width/2.0
                    antialiasing: true
                    anchors.top: parent.top
                    anchors.topMargin: width
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                Rectangle {
                    width: 1
                    height: parent.height - ballRect.height * 2
                    anchors.bottom: parent.bottom
                    color: index === currentIndex ? "black" : "#EEEEEE"
                    anchors.horizontalCenter: parent.horizontalCenter
                    antialiasing: true
                }

            }
        }
    }

    MouseArea {
        id: mouseAreaObj
        anchors.fill: parent
        anchors.leftMargin: mouseAreaObj.itemWidth
        anchors.rightMargin: mouseAreaObj.itemWidth

        property int currentIndex: -1
        readonly property real itemWidth: histogram.width*1.0/(count + 2)
        readonly property real ballRectWidth: itemWidth*0.8
        readonly property real dayInfoHeight: 30

        function positionIndex(mouseX, mouseY) {
            currentIndex = utils.bound(0, mouseX / itemWidth, count - 1)
        }

        function currentItemX() {
            var p = repeaterObj.itemAt(utils.bound(3, currentIndex, count - 4)).mapToItem(histogram, 0, 0)
            return p.x - dayInfoObj.width/2 + itemWidth/2
        }

        onPositionChanged: {
            positionIndex(mouseX, mouseY)
        }

        onClicked: {
            positionIndex(mouseX, mouseY)
        }
    }

    Text {
        id: dayInfoObj
        width: 80
        height: 12
        x: mouseAreaObj.currentItemX()
        font.pointSize: 9
        text: "07/01"
        anchors.top: mouseAreaObj.top
        anchors.topMargin: mouseAreaObj.dayInfoHeight/2.0
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "#EEEEEE"
        }
    }

    Component.onCompleted: {
        delayUpdateTimer.start()
    }

    Timer {
        id: delayUpdateTimer
        interval: 100
        onTriggered: {
            dayInfoObj.x = Qt.binding(function() {
                return mouseAreaObj.currentItemX() });
        }
    }

}
