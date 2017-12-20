import QtQuick 2.3

Item {
    anchors.fill: parent

    property int animationDuration: 500
    property alias backgroundColor: background.color
    property bool canceledOnTouchOutside: true
    property alias contextActive: context.active
    property alias contextAsynchronous: context.asynchronous
    property alias contextItem: context.item
    property alias contextSource: context.source
    property alias contextSourceComponent: context.sourceComponent
    property alias contextStatus: context.status

    signal canceled()
    signal closed()
    signal contextLoaded()
    signal opened()

    function open() {
        private_data.opened = true
        opened()
    }

    function close() {
        private_data.opened = false
        closed()
    }

    function setContextSource(/*url*/ source, /*object*/ properties) {
        context.setSource(source, properties)
    }

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#66666666"
        enabled: private_data.opened
        opacity: private_data.opened

        Behavior on opacity {
            NumberAnimation { duration: animationDuration }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (canceledOnTouchOutside) {
                    private_data.cancel()
                }
            }
        }
    }

    Loader {
        id: context
        anchors.centerIn: parent
        enabled: private_data.opened
        anchors.verticalCenterOffset:  private_data.opened ? 0 : - parent.height
        rotation: private_data.opened ? 0 : 60

        Behavior on anchors.verticalCenterOffset {
            NumberAnimation { duration: animationDuration; easing.type: Easing.InOutBack }
        }

        Behavior on rotation {
            NumberAnimation { duration: animationDuration; easing.type: Easing.InOutBack }
        }

        onLoaded: {
            contextLoaded()
        }
    }

    QtObject {
        id: private_data

        property bool opened: false

        function cancel() {
            opened = false;
            canceled();
            closed();
        }
    }

}
