import QtQuick 2.6
import QtQuick.Window 2.2
import "qml"
import "qml/Tools"

Window {
    id: rootWindow
    visible: true
    width: 480
    height: 720
    title: qsTr("Keep Accounts")

    MainPage {
        anchors.fill: parent
    }
}
